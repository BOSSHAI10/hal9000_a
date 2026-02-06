#include "HAL9000.h"
#include "display.h"
#include "display_internal.h"
#include "cal_annotate.h"
#include "cal_assert.h"
#include "cal_assembly.h"

#pragma pack(push,1)
typedef struct _SCREEN_CHARACTER
{
    BYTE        Character;
    BYTE        Color;
} SCREEN_CHARACTER,*PSCREEN_CHARACTER;
static_assert(sizeof(SCREEN_CHARACTER) == 2 * sizeof(BYTE), "This structure needs to be 2 bytes");
#pragma pack(pop)

typedef SCREEN_CHARACTER    SCREEN_MEMORY[LINES_PER_SCREEN][CHARS_PER_LINE];
typedef SCREEN_MEMORY*      PSCREEN_MEMORY;

typedef struct _DISPLAY_DATA
{
    BYTE                *MappedBufferAddress;
    BYTE                *StartOfUsableBuffer;
    PSCREEN_MEMORY      MappedScreenAddress;
    PSCREEN_MEMORY      StartOfUsableScreen; 
    DISPLAY_INFORMATION DisplayInformation;
    DWORD               TotalUsableLines;
    DWORD               TotalUsableCharacters;
    DWORD               TotalUsableBytes;
    BYTE                CurrentColumn;
    BYTE                CurrentLine;
    COLOR               CurrentColor;
    BYTE                IndexOfFirstValidLine;
    BYTE                IndexOfFirstInvalidLine;
} DISPLAY_DATA, *PDISPLAY_DATA;

static DISPLAY_DATA m_displayData;
static SCREEN_MEMORY m_screenChars;

static
void
_DispScrollScreen(
    void
    );


static
void
_DispPutChar(
    IN      char    Character,
    IN      BYTE    Line,
    IN      BYTE    Column
    );

void
DispPreinitScreen(
    IN      PDISPLAY_INFORMATION    DisplayInformation,
    IN      BYTE                    IndexOfFirstUsableRow,
    IN      BYTE                    IndexOfFirstUnusableRow
    )
{
    if (DisplayInformation->FrameBufferBpp != 32)
    {
        // We cannot write to other types of framebuffers
        AsmHalt();
    }

    memzero(&m_displayData, sizeof(DISPLAY_DATA));
    memcpy(&(m_displayData.DisplayInformation), DisplayInformation, sizeof(DISPLAY_INFORMATION));

    m_displayData.MappedBufferAddress = (BYTE *) DisplayInformation->FrameBufferAddress;
    m_displayData.StartOfUsableBuffer = 
        (BYTE *) &(m_displayData.MappedBufferAddress[GLYPH_HEIGHT * IndexOfFirstUsableRow * DisplayInformation->FrameBufferPitch]);
    m_displayData.MappedScreenAddress = &m_screenChars;
    m_displayData.StartOfUsableScreen = (PSCREEN_MEMORY) &((*m_displayData.MappedScreenAddress)[IndexOfFirstUsableRow]);
    m_displayData.IndexOfFirstValidLine = IndexOfFirstUsableRow;
    m_displayData.IndexOfFirstInvalidLine = IndexOfFirstUnusableRow;
    m_displayData.TotalUsableLines = IndexOfFirstUnusableRow - IndexOfFirstUsableRow;
    m_displayData.TotalUsableCharacters = (IndexOfFirstUnusableRow - IndexOfFirstUsableRow) * CHARS_PER_LINE;
    m_displayData.TotalUsableBytes = BYTES_PER_CHAR * m_displayData.TotalUsableCharacters;
    m_displayData.CurrentLine = m_displayData.IndexOfFirstValidLine;
    DispSetColor(WHITE_COLOR);

    DispClearLine(0);
    DispClearScreen();
}

void
DispSetColor(
    IN      COLOR           Color
    )
{
    m_displayData.CurrentColor = Color;
}

COLOR
DispGetColor(
    void
    )
{
    return m_displayData.CurrentColor;
}

void
DispPrintString
(
    IN_Z    char*    Buffer
    )
{
    DWORD index;
    BOOLEAN newline;

    // check parameters
    if (NULL == Buffer)
    {
        return;
    }

    // preinit variables
    index = 0;

    while ('\0' != Buffer[index])
    {
        newline = FALSE;

        // check if we need to scroll the screen before writing
        if (m_displayData.IndexOfFirstInvalidLine == m_displayData.CurrentLine)
        {
            _DispScrollScreen();
        }

        if ('\n' == Buffer[index])
        {
            // newline
            newline = TRUE;
            goto end_it;
        }

        if ('\t' == Buffer[index])
        {
            // lets place a tab
            DispPrintString("    ");

            // go to next character
            ++index;
            continue;
        }

        _DispPutChar(Buffer[index], m_displayData.CurrentLine, m_displayData.CurrentColumn);

        ++m_displayData.CurrentColumn;

        if (CHARS_PER_LINE == m_displayData.CurrentColumn)
        {
            // newline
            newline = TRUE;
            goto end_it;
        }

    end_it:
        if (newline)
        {
            m_displayData.CurrentLine++;
            m_displayData.CurrentColumn = 0;
        }

        ++index;
    }

}

void
DispPutBuffer(
    IN_Z    char*       Buffer,
    IN      BYTE        Line,
    IN      BYTE        Column
    )
{
    DispPutBufferColor(Buffer, Line, Column, DispGetColor());
}

void
DispPutBufferColor(
    IN_Z    char*       Buffer,
    IN      BYTE        Line,
    IN      BYTE        Column,
    IN      COLOR       Color
    )
{
    BYTE i;
    COLOR prevColor;

    prevColor = DispGetColor();

    DispSetColor(Color);
    for (i = 0;
    (i < CHARS_PER_LINE - Column) && (Buffer[i] != '\0');
        ++i)
    {
        _DispPutChar(Buffer[i], Line, Column + i);
    }

    DispSetColor(prevColor);
}

static
void
_DispClearFrameBuffer(
    void
    )
{
    BYTE *pLine = m_displayData.StartOfUsableBuffer;
    for (DWORD i = 0; i < m_displayData.TotalUsableLines * GLYPH_HEIGHT; i++)
    {
        memzero(pLine, CHARS_PER_LINE * GLYPH_WIDTH * sizeof(DWORD));
        pLine += m_displayData.DisplayInformation.FrameBufferPitch;
    }
}

ALWAYS_INLINE
static
void
_DispPutCharFrameBuffer(
    IN      char    Character,
    IN      BYTE    Line,
    IN      BYTE    Column
    )
{
    COLOR_INTERNAL bufferColor = BIOS_COLOR_MAP[m_displayData.CurrentColor];
    BYTE *bufferPtr =
        m_displayData.MappedBufferAddress +
        (Line * GLYPH_HEIGHT * m_displayData.DisplayInformation.FrameBufferPitch) +
        (Column * GLYPH_WIDTH * sizeof(DWORD));
    BYTE *glyph = GLYPHS[(WORD)Character];

    for (BYTE i = 0; i < GLYPH_HEIGHT; i++)
    {
        for (BYTE j = GLYPH_WIDTH; j >= 1; j--)
        {
            if (glyph[i] & (1 << (j - 1)))
                *((COLOR_INTERNAL *) bufferPtr) = bufferColor;
            else
                *((COLOR_INTERNAL *) bufferPtr) = 0x0;
            bufferPtr += sizeof(DWORD);
        }
        bufferPtr -= GLYPH_WIDTH * sizeof(DWORD);
        bufferPtr += m_displayData.DisplayInformation.FrameBufferPitch;
    }
}

static
void
_DispScreenToFrameBuffer(
    void
    )
{
    COLOR previousColor = m_displayData.CurrentColor;
    for (BYTE line = m_displayData.IndexOfFirstValidLine; line < m_displayData.IndexOfFirstInvalidLine; line++)
    {
        for (BYTE column = 0; column < CHARS_PER_LINE; column++)
        {
            m_displayData.CurrentColor = m_screenChars[line][column].Color;
            _DispPutCharFrameBuffer(m_screenChars[line][column].Character, line, column);
        }
    }
    m_displayData.CurrentColor = previousColor;
}

static
void
_DispFrameBufferClearLine(
    IN      BYTE    Line
    )
{
    BYTE *pLine = 
        m_displayData.MappedBufferAddress + (Line * GLYPH_HEIGHT * m_displayData.DisplayInformation.FrameBufferPitch);
    for (BYTE i = 0; i < GLYPH_HEIGHT; i++)
    {
        memzero(pLine, CHARS_PER_LINE * GLYPH_WIDTH * sizeof(DWORD));
        pLine += m_displayData.DisplayInformation.FrameBufferPitch;
    }
}

static
void
_DispScrollScreen(
    void
    )
{
    // we scroll the screen

    // warning C4312: 'type cast': conversion from 'DWORD' to 'PVOID' of greater size
MSVC_WARNING_SUPPRESS(4312)
    memmove(m_displayData.StartOfUsableScreen, 
           (*m_displayData.StartOfUsableScreen)[1], 
           m_displayData.TotalUsableBytes - BYTES_PER_LINE );
    _DispScreenToFrameBuffer();

    // clear last line
    m_displayData.CurrentLine--;
    DispClearLine(m_displayData.CurrentLine);
}

void
DispClearScreen(
    void
    )
{
    // clear the screen
    memzero(m_displayData.StartOfUsableScreen, m_displayData.TotalUsableBytes);
    _DispClearFrameBuffer();

    // update current line to start toping at top of the usable screen
    m_displayData.CurrentLine = m_displayData.IndexOfFirstValidLine;
}

static
void
_DispPutChar(
    IN      char    Character,
    IN      BYTE    Line,
    IN      BYTE    Column
    )
{
    PSCREEN_CHARACTER pScreenAddress;

// warning C4312: 'type cast': conversion from 'unsigned long' to 'WORD *' of greater size
MSVC_WARNING_SUPPRESS(4312)
    pScreenAddress = (PSCREEN_CHARACTER) &((*m_displayData.MappedScreenAddress)[Line][Column]);

    pScreenAddress->Color = m_displayData.CurrentColor;
    pScreenAddress->Character = Character;

    _DispPutCharFrameBuffer(Character, Line, Column);
}

void
DispClearLine(
    IN      BYTE        Line
    )
{
    memzero( (*m_displayData.MappedScreenAddress)[Line], BYTES_PER_LINE);
    _DispFrameBufferClearLine(Line);
}

void
DispSetCursor(
    IN      SCREEN_POSITION     CursorPosition,
    IN      COLOR               Color
    )
{
    DispPutBufferColor("_",
                       CursorPosition.Line,
                       CursorPosition.Column,
                       Color);
}

STATUS
DispStoreBuffer(
    OUT_WRITES_BYTES(Size)  PVOID               Buffer,
    IN                      DWORD               Size
    )
{
    if (Size < m_displayData.TotalUsableBytes)
    {
        return STATUS_BUFFER_TOO_SMALL;
    }

    memcpy(Buffer, m_displayData.StartOfUsableScreen, m_displayData.TotalUsableBytes );

    return STATUS_SUCCESS;
}

STATUS
DispRestoreBuffer(
    IN_READS_BYTES(Size)    PVOID               Buffer,
    IN                      DWORD               Size
    )
{
    if (Size < m_displayData.TotalUsableBytes)
    {
        return STATUS_BUFFER_TOO_SMALL;
    }

    memcpy(m_displayData.StartOfUsableScreen, Buffer, m_displayData.TotalUsableBytes);
    _DispScreenToFrameBuffer();

    m_displayData.CurrentLine = m_displayData.IndexOfFirstInvalidLine;

    return STATUS_SUCCESS;
}