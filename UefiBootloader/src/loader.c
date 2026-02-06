#include <Bootloader/loader.h>

#include <Bootloader/acpi.h>
#include <Bootloader/bootinfo.h>
#include <Bootloader/console.h>
#include <Bootloader/crc32.h>
#include <Bootloader/filesystem.h>
#include <Bootloader/graphics.h>
#include <Bootloader/memory.h>
#include <Bootloader/memory_map.h>
#include <Bootloader/runtime.h>
#include <Bootloader/types.h>

#include <cal_annotate.h>

#define UEFI_MIN(x, y) (((x) < (y)) ? (x) : (y))

#pragma pack(push, 1)

typedef struct
{
    UINT64 Null;
    UINT64 Code;
    UINT64 Data;
} _GDT;

typedef struct
{
    UINT16 Limit;
    UINT32 Address;
} _GDT_DESCRIPTOR;

typedef struct
{
    UINT16 Limit;
    UINT64 Address;
} _GDT_DESCRIPTOR_64;

typedef struct
{
    UINT64 JumpAddress;
    UINT32 JumpAddressPm;
    _GDT_DESCRIPTOR *GdtDescriptor;
    _GDT_DESCRIPTOR_64 *GdtDescriptor64;
} _TRANSITION;

#pragma pack(pop)

typedef void MS_ABI (*StartOS) (UINT64 EntryAddress, UINT64 BootInfo, UINT64 Transition);

typedef struct
{
    EFI_SYSTEM_TABLE *ST;
    EFI_HANDLE ImageHandle;
    EFI_FILE_HANDLE RootDirectory; 
    EFI_FILE_HANDLE OsBinary; 
    UINT64 LoadAddress;
    UINT32 BootHeaderOffset;
    HAL_BOOT_HEADER BootHeader;
    UINT64 OsPageCount;
    UINT32 BootModuleCount;
    UINT32 BootModules;
    StartOS StartOsRoutine;
    UINT64 AcpiRsdp;
    EFI_GRAPHICS_OUTPUT_PROTOCOL *GOP;
    UINT32 GopModeIndex;
    HAL_FRAMEBUFFER Framebuffer;
    UINTN MemoryMapKey;
    HAL_BOOT_INFORMATION *BootInformation;
    UINT32 DescriptorVersion;
    BOOLEAN SwitchedToVirtualRuntime;
} _LOADER;

static _LOADER gLoader;

extern char __start_os[];
extern char __start_os_bits32[];
extern char __start_os_pm[];

ALIGN(8) static _GDT gdt = { .Null = 0x0, .Code = 0x00CF9A000000FFFF, .Data = 0x00CF92000000FFFF }; 
ALIGN(8) static _GDT_DESCRIPTOR gdtDescriptor;
ALIGN(8) static _GDT_DESCRIPTOR_64 gdtDescriptor64;
static _TRANSITION transition;

static
void
_Die(
    CHAR16 *Message
    )
{
    PrintString(gLoader.ST, EFI_RED, Message);
    WaitForKeyPress(gLoader.ST);
    gLoader.ST->BootServices->Exit(gLoader.ImageHandle, EFI_LOAD_ERROR, 0, NULL);
}

void
LoaderPreInit(
    EFI_SYSTEM_TABLE *SystemTable,
    EFI_HANDLE ImageHandle
    )
{
    gLoader.ST = SystemTable;
    gLoader.ImageHandle = ImageHandle; 

    gLoader.ST->BootServices->SetWatchdogTimer(0, 0, 0, 0);
    PrepareConsole(gLoader.ST);
    ClearScreen(gLoader.ST);

    PrintString(gLoader.ST, EFI_GREEN, L"Loader preinitialized\r\n");
}

void
LoaderInit(
    void
    )
{
    gLoader.RootDirectory = GetRootDirectory(gLoader.ST, gLoader.ImageHandle);
    if (!gLoader.RootDirectory)
        _Die(L"Failed to open root directory\r\n");
    PrintString(gLoader.ST, EFI_GREEN, L"Loader initialized\r\n");
}

static
void
_SearchForOperatingSystem(
    void
    )
{
    static CHAR16 osFilename[512] = L"\\EFI\\OS\\";

    EFI_STATUS status;
    EFI_FILE_INFO **dirEntries;
    UINT64 dirCount;
    
    status = ListDirectory(gLoader.ST,
                           gLoader.RootDirectory,
                           osFilename,
                           &dirEntries,
                           &dirCount);

    if (EFI_ERROR(status))
        _Die(L"Failed to list \\EFI\\OS\\\r\n");

    if (dirCount < 3 || (dirEntries[2]->Attribute & EFI_FILE_DIRECTORY))
        _Die(L"No Operating System found\r\n");

    // First two entries are . and .. in a directory
    EFI_FILE_INFO *osFileInfo = dirEntries[2];
    // See EFI_FILE_INFO struct
    UINT64 filenameLength = osFileInfo->Size - (4 * sizeof(UINT64) + 3 * sizeof(EFI_TIME));
    CopyMemory(&osFilename[8], &(osFileInfo->FileName), filenameLength);

    status = OpenFileForRead(gLoader.RootDirectory, osFilename, &(gLoader.OsBinary));
    if (EFI_ERROR(status))
        _Die(L"Could not open Operating System image\r\n");

    for (UINT64 i = 0;i < dirCount; i++)
        FreeFromPool(gLoader.ST, dirEntries[i]);
    FreeFromPool(gLoader.ST, dirEntries);

    PrintString(gLoader.ST, EFI_LIGHTGRAY, L"Found Operating System Image at ");
    PrintString(gLoader.ST, EFI_LIGHTGRAY, osFilename);
    PrintString(gLoader.ST, EFI_LIGHTGRAY, L"\r\n");
}

static
void
_DumpHalBootHeader(
    void)
{
    PrintIntegerWithName(gLoader.ST, EFI_LIGHTGRAY,
                         L"Offset", gLoader.BootHeaderOffset);
    PrintIntegerWithName(gLoader.ST, EFI_LIGHTGRAY,
                         L"Magic", gLoader.BootHeader.Magic);
    PrintIntegerWithName(gLoader.ST, EFI_LIGHTGRAY,
                         L"Crc32", gLoader.BootHeader.Crc32);
    PrintIntegerWithName(gLoader.ST, EFI_LIGHTGRAY,
                         L"PreferedLoadAddress", gLoader.BootHeader.PreferredLoadAddress);
    PrintIntegerWithName(gLoader.ST, EFI_LIGHTGRAY,
                         L"EntryAddress", gLoader.BootHeader.EntryAddress);
    PrintIntegerWithName(gLoader.ST, EFI_LIGHTGRAY,
                         L"FrameBufferWidth", gLoader.BootHeader.FramebufferWidth);
    PrintIntegerWithName(gLoader.ST, EFI_LIGHTGRAY,
                         L"FrameBufferHeight", gLoader.BootHeader.FramebufferHeight);
}

static
void
_SearchForHalBootHeader(
    void
    )
{
    EFI_STATUS status;

    // Search for HAL Boot Header in the first 8 KB
    UINT8 *buffer = AllocateFromPool(gLoader.ST, SIZE_8KB);
    UINT64 bufferSize = SIZE_8KB;
    SetMemory(buffer, 0, SIZE_8KB);
    status = gLoader.OsBinary->Read(gLoader.OsBinary, &bufferSize, buffer); 
    if (EFI_ERROR(status))
        _Die(L"Failed to read from the image\r\n");

    HAL_BOOT_HEADER *header;
    UINT8 *pos = (UINT8 *) buffer;
    UINT32 magic = HAL_BOOT_HEADER_MAGIC;
    UINT8 found = 0;
    while (pos < buffer + SIZE_8KB)
    {
        header = (HAL_BOOT_HEADER *) pos;
        if (MemoryEquals(&(header->Magic), &magic, sizeof(UINT32)))
        {
            found = 1;
            break;
        }
        pos++;
    }
    if (!found)
        _Die(L"Boot header not found\r\n");

    gLoader.BootHeaderOffset = ((UINT8 *) (header)) - buffer;
    CopyMemory(&(gLoader.BootHeader), header, sizeof(HAL_BOOT_HEADER));
    FreeFromPool(gLoader.ST, buffer);
}

static
void
_VerifyHalBootHeader(
    void
    )
{
    HAL_BOOT_HEADER CrcHeader;

    CopyMemory(&CrcHeader, &(gLoader.BootHeader), sizeof(HAL_BOOT_HEADER));
    CrcHeader.Crc32 = 0;
    UINT32 Crc32 = ComputeCrc32((UINT8 *) &CrcHeader, sizeof(HAL_BOOT_HEADER));

    if (Crc32 != gLoader.BootHeader.Crc32)
    {
        PrintIntegerWithName(gLoader.ST, EFI_LIGHTGRAY, L"Correct CRC", Crc32);
        PrintIntegerWithName(gLoader.ST, EFI_LIGHTGRAY, L"Received CRC", gLoader.BootHeader.Crc32);
        _Die(L"Invalid boot header CRC\r\n");
    }
    
    PrintString(gLoader.ST, EFI_GREEN, L"Boot header CRC32 verified\r\n");
}

static
void
_CopyOperatingSystem(
    void
    )
{
    EFI_STATUS status;

    UINT64 pageCount = 0;
    UINT64 address = LoadFileToMemoryAt(gLoader.ST,
                                gLoader.OsBinary,
                                gLoader.BootHeader.LoadOffset,
                                gLoader.BootHeader.PreferredLoadAddress,
                                &pageCount);
    if (!address)
        _Die(L"Failed to load Operating System at specified address");
    
    gLoader.LoadAddress = address;
    gLoader.OsPageCount = pageCount;
    CloseFileHandle(gLoader.OsBinary);
}

static
void
_LoadBootModules(
    void
    )
{
    static CHAR16 osFilename[512] = L"\\EFI\\MODULES\\";

    EFI_STATUS status;
    EFI_FILE_INFO **dirEntries;
    UINT64 dirCount;

    // Allocate for max module count
    HAL_BOOT_MODULE *BootModules = AllocateZeroedPagesMaxAddress(
                                                                gLoader.ST,
                                                                EfiLoaderData,
                                                                (EFI_PHYSICAL_ADDRESS) (BASE_4GB - 1),
                                                                1);
    if (!BootModules)
        _Die(L"Failed to allocate boot module memory\r\n");

    status = ListDirectory(gLoader.ST,
                           gLoader.RootDirectory,
                           osFilename,
                           &dirEntries,
                           &dirCount);
    if (EFI_ERROR(status))
        return;

    // First two are . and ..
    UINT64 count = dirCount > 34 ? 34 : dirCount;
    for (UINT64 i = 2; i < count; i++)
    {
        PrintString(gLoader.ST, EFI_LIGHTGRAY, L"Loading module: ");
        PrintString(gLoader.ST, EFI_LIGHTGRAY, dirEntries[i]->FileName);
        PrintString(gLoader.ST, EFI_LIGHTGRAY, L"\r\n");

        UINT64 filenameLength = dirEntries[i]->Size - (4 * sizeof(UINT64) + 3 * sizeof(EFI_TIME));
        CopyMemory(&osFilename[13], dirEntries[i]->FileName, filenameLength);
        EFI_FILE_HANDLE fileHandle;
        if(EFI_ERROR(OpenFileForRead(gLoader.RootDirectory, osFilename, &fileHandle)))
            _Die(L"Could not open for read Boot Module\r\n");
        UINT64 pageCount;
        UINT64 address = LoadFileToMemory(gLoader.ST, fileHandle, 0, &pageCount);
        if (!address)
            _Die(L"Could not load Boot Module\r\n");
        CloseFileHandle(fileHandle);

        BootModules[i - 2].PhysicalAddress = address;
        BootModules[i - 2].Size = pageCount * PAGE_SIZE;
        filenameLength /= sizeof(CHAR16);
        CopyWcharAsChar(BootModules[i - 2].Name, dirEntries[i]->FileName, UEFI_MIN(33, filenameLength));
        
        PrintString(gLoader.ST, EFI_LIGHTGRAY, L"Loaded module ");
        PrintString(gLoader.ST, EFI_LIGHTGRAY, dirEntries[i]->FileName);
        PrintString(gLoader.ST, EFI_LIGHTGRAY, L" at address ");
        PrintIntegerInHexadecimal(gLoader.ST, EFI_LIGHTGRAY, address);
        PrintString(gLoader.ST, EFI_LIGHTGRAY, L"\r\n");
    }

    for (UINT64 i = 0;i < dirCount; i++)
        FreeFromPool(gLoader.ST, dirEntries[i]);
    FreeFromPool(gLoader.ST, dirEntries);

    if (count)
    {
        // Do not do this
        gLoader.BootModuleCount = (UINT32) (count - 2);
        gLoader.BootModules = (UINT32) ((UINT64) BootModules);
    }
}

static
void
_PrepareAssembly(
    void
    )
{
    gdtDescriptor.Limit = 23;
    gdtDescriptor.Address = (UINT32) ((UINT64) &gdt);
    
    gdtDescriptor64.Limit = 23;
    gdtDescriptor64.Address = (UINT64) &gdt;

    transition.JumpAddress = (UINT64) &(__start_os_bits32[0]);
    transition.JumpAddressPm = (UINT32) ((UINT64) &(__start_os_pm[0]));
    transition.GdtDescriptor = &gdtDescriptor;
    transition.GdtDescriptor64 = &gdtDescriptor64;

    gLoader.StartOsRoutine = (StartOS) &(__start_os[0]);
}

static
void
_DumpFramebuffer(
    void
    )
{
    HAL_FRAMEBUFFER *buff = (HAL_FRAMEBUFFER *) &(gLoader.Framebuffer);
    PrintIntegerWithName(gLoader.ST, EFI_LIGHTGRAY, L"Address", buff->Address);
    PrintIntegerWithName(gLoader.ST, EFI_LIGHTGRAY, L"Pitch", buff->Pitch);
    PrintIntegerWithName(gLoader.ST, EFI_LIGHTGRAY, L"Width", buff->Width);
    PrintIntegerWithName(gLoader.ST, EFI_LIGHTGRAY, L"Height", buff->Height);
    PrintIntegerWithName(gLoader.ST, EFI_LIGHTGRAY, L"BitsPerPixel", buff->BitsPerPixel);
    PrintIntegerWithName(gLoader.ST, EFI_LIGHTGRAY, L"RedFieldPosition", buff->RedFieldPosition);
    PrintIntegerWithName(gLoader.ST, EFI_LIGHTGRAY, L"RedMaskSize", buff->RedMaskSize);
    PrintIntegerWithName(gLoader.ST, EFI_LIGHTGRAY, L"GreenFieldPosition", buff->GreenFieldPosition);
    PrintIntegerWithName(gLoader.ST, EFI_LIGHTGRAY, L"GreenMaskSize", buff->GreenMaskSize);
    PrintIntegerWithName(gLoader.ST, EFI_LIGHTGRAY, L"BlueFieldPosition", buff->BlueFieldPosition);
    PrintIntegerWithName(gLoader.ST, EFI_LIGHTGRAY, L"BlueMaskSize", buff->BlueMaskSize);
}

static
void 
_GetDisplayMode(
    void
    )
{
    EFI_GRAPHICS_OUTPUT_PROTOCOL *gop = GetGraphicsProtocol(gLoader.ST);
    if (!gop)
        _Die(L"Could not obtain GOP\n\r");
    gLoader.GOP = gop;

    UINT32 mode = ObtainClosestGraphicsMode(gop,
                                            gLoader.BootHeader.FramebufferWidth,
                                            gLoader.BootHeader.FramebufferHeight,
                                            &(gLoader.Framebuffer));
    if (mode == UINT32_MAX)
        _Die(L"Could not obtain GOP mode\r\n");
    
    gLoader.GopModeIndex = mode;

    PrintString(gLoader.ST, EFI_LIGHTGRAY, L"Chosen graphics mode:\r\n");
    _DumpFramebuffer();
}

static
void
_FillHalBootInformation(
    void
    )
{
    HAL_BOOT_INFORMATION *bootInfo = 
        AllocateZeroedPagesMaxAddress(
            gLoader.ST,
            EfiLoaderData,
            (EFI_PHYSICAL_ADDRESS) (SIZE_4GB - 1),
            1
        );
    if (!bootInfo)
        _Die(L"Failed to allocate boot information\n");

    SetMemory(bootInfo, 0, sizeof(HAL_BOOT_INFORMATION));

    bootInfo->Magic = HAL_BOOT_MAGIC;

    bootInfo->KernelBaseAddress = gLoader.LoadAddress; 
    bootInfo->KernelSize = gLoader.OsPageCount * PAGE_SIZE;

    bootInfo->AcpiRsdp = gLoader.AcpiRsdp;

    bootInfo->BootModuleCount = gLoader.BootModuleCount;
    bootInfo->BootModules = gLoader.BootModules;

    CopyMemory(&(bootInfo->Framebuffer), &(gLoader.Framebuffer), sizeof(HAL_FRAMEBUFFER));

    // Known serial port numbers
    bootInfo->SerialPorts[0] = 0x3F8;
    bootInfo->SerialPorts[1] = 0x2F8; 

    gLoader.BootInformation = bootInfo;
}

void 
LoadOperatingSystem(
    void
    )
{
    PrintString(gLoader.ST, EFI_GREEN, L"Loading Operating System\r\n");
    
    PrintString(gLoader.ST, EFI_LIGHTGRAY, L"Searching for an Operating System\r\n");
    _SearchForOperatingSystem();
    _SearchForHalBootHeader();

    PrintString(gLoader.ST, EFI_LIGHTGRAY, L"Found boot header:\r\n");
    _DumpHalBootHeader();

    _VerifyHalBootHeader();

    PrintString(gLoader.ST, EFI_LIGHTGRAY, L"Loading Operating System to ");
    PrintIntegerInHexadecimal(gLoader.ST, EFI_LIGHTGRAY, gLoader.BootHeader.PreferredLoadAddress);
    PrintString(gLoader.ST, EFI_LIGHTGRAY, L" starting from offset ");
    PrintIntegerInHexadecimal(gLoader.ST, EFI_LIGHTGRAY, gLoader.BootHeader.LoadOffset);
    PrintString(gLoader.ST, EFI_LIGHTGRAY, L"\r\n");
    _CopyOperatingSystem();
    PrintString(gLoader.ST, EFI_LIGHTGRAY, L"Operating System Loaded\r\n");

    PrintString(gLoader.ST, EFI_LIGHTGRAY, L"Loading Boot Modules\r\n");
    _LoadBootModules();
    PrintString(gLoader.ST, EFI_LIGHTGRAY, L"Loaded Boot Modules\r\n");

    _PrepareAssembly();

    PrintString(gLoader.ST, EFI_LIGHTGRAY, L"Searching for ACPI RSDP\r\n");
    UINT64 rsdp = GetAcpiRsdp(gLoader.ST);    
    if (!rsdp)
        _Die(L"Could not find ACPI RSDP\r\n");
    gLoader.AcpiRsdp = rsdp;
    PrintIntegerWithName(gLoader.ST, EFI_LIGHTGRAY, L"ACPI RSDP", rsdp);

    PrintString(gLoader.ST, EFI_LIGHTGRAY, L"Obtaining a framebuffer\r\n");
    _GetDisplayMode();

    PrintString(gLoader.ST, EFI_LIGHTGRAY, L"Filling HAL Boot Information\r\n");
    _FillHalBootInformation();
    PrintString(gLoader.ST, EFI_LIGHTGRAY, L"Filled HAL Boot Information\r\n");

    PrintString(gLoader.ST, EFI_GREEN, L"Loaded Operating System\r\n");
}

static
void 
_ExitBootServices(
    void
    )
{
    CloseFileHandle(gLoader.RootDirectory);
    UINTN mapKey = gLoader.MemoryMapKey;
    while(gLoader.ST->BootServices->ExitBootServices(gLoader.ImageHandle, mapKey) != EFI_SUCCESS)
        mapKey = GetMemoryMap(gLoader.ST, &(gLoader.BootInformation->MemoryMap), &(gLoader.DescriptorVersion));
}

static
void
_SwitchUefiToVirtual(
    void
    )
{
    // Read UEFI spec about ExitBootServices
    // Clear EFI System Table
    gLoader.ST->ConsoleInHandle = NULL;
    gLoader.ST->ConIn = NULL;
    gLoader.ST->ConsoleOutHandle = NULL;
    gLoader.ST->ConOut = NULL;
    gLoader.ST->StandardErrorHandle = NULL;
    gLoader.ST->StdErr = NULL;
    gLoader.ST->BootServices = NULL;
    
    // Recompute CRC32
    gLoader.ST->Hdr.CRC32 = 0;
    gLoader.ST->Hdr.CRC32 = ComputeCrc32((UINT8 *) gLoader.ST,
                                         gLoader.ST->Hdr.HeaderSize);


    EFI_STATUS status = SwitchToVirtualAdressingMode(gLoader.ST,
                                                     &(gLoader.BootInformation->MemoryMap),
                                                     gLoader.DescriptorVersion,
                                                     &(gLoader.BootInformation->EfiRuntimeVirtualAddress),
                                                     &(gLoader.BootInformation->EfiRuntimeDescriptorCount));
    if (status == EFI_SUCCESS)
    {
        gLoader.SwitchedToVirtualRuntime = TRUE;
        gLoader.BootInformation->EfiRuntimeServices = (UINT64) (gLoader.ST->RuntimeServices);
    }
    gLoader.BootInformation->Crc32 = 0;
    gLoader.BootInformation->Crc32 = ComputeCrc32((UINT8 *) gLoader.BootInformation,
                                                  sizeof(HAL_BOOT_INFORMATION) - 16);
}

void
StartOperatingSystem(
    void
    )
{
    // PrintString(gLoader.ST, EFI_GREEN, L"To start the Operating System, press any key\r\n");
    // PrintString(gLoader.ST, EFI_LIGHTGRAY, L"...\r\n");
    // WaitForKeyPress(gLoader.ST);

    SetGraphicsMode(gLoader.GOP, gLoader.GopModeIndex);
    _ExitBootServices();
    // Now the machine is ours
    _SwitchUefiToVirtual();

    // Signal that we are done
    UINT32 lineColor = gLoader.SwitchedToVirtualRuntime ? UINT32_MAX : 0xFF0000;
    UINT32 *buff = (UINT32 *) gLoader.Framebuffer.Address;
    for (int i = 0; i < 100; i++) 
        buff[i] = lineColor;

    (gLoader.StartOsRoutine)(gLoader.BootHeader.EntryAddress,
                             (UINT64) gLoader.BootInformation,
                             (UINT64) &transition);
}
