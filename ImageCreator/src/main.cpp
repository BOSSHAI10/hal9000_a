#include <iostream>
#include <optional>
#include <filesystem>

#define UTF_CPP_CPLUSPLUS 202002L

#include <cal_types.h>
#include <gpt.hpp>
#include <fat.hpp>
#include <json.hpp>
#include <utf8.h>

using json = nlohmann::json;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <config_path>\n", argv[0]);
        return 1;
    }

    std::ifstream in(argv[1]);
    json jsonConfig = json::parse(in);
    in.close();

    std::string outputImagePath = jsonConfig["output"];
    std::string diskId = jsonConfig["disk"];

    // Create the partition config
    std::vector<ConfigurationParitition> partitionConfig;
    partitionConfig.reserve(8);
    for (auto const &jsonPartition : jsonConfig["partitions"])
    {
        ConfigurationParitition partition;

        if (jsonPartition["type"] == "EFI")
            partition.Type = EFI_PART_TYPE_EFI_SYSTEM_PART_GUID;
        else if (jsonPartition["type"] == "BDP")
            partition.Type = EFI_PART_TYPE_MICROSOFT_BASIC_DATA_GUID;
        else if (jsonPartition["type"] == "LINUX_SWAP")
            partition.Type = EFI_PART_TYPE_LINUX_SWAP_GUID;
        else
        {
            std::cerr << "Invalid partition type: " << jsonPartition["type"].get<std::string>() << std::endl;
            return 2;
        }

        partition.LBACount = jsonPartition["size"].get<QWORD>() * 2;
        std::string partName = jsonPartition["name"].get<std::string>();
        partition.PartitionName = utf8::utf8to16(partName);
        partition.PartitionId = jsonPartition["id"].get<std::string>();
        partitionConfig.push_back(partition);
    }

    std::ofstream f(outputImagePath);
    f.close();

    GptDisk gptDisk(outputImagePath);
    gptDisk.configureDisk(diskId, partitionConfig);
    gptDisk.createDisk();

    for (auto const &jsonFilesystem : jsonConfig["filesystems"])
    {
        const auto partitionName = jsonFilesystem["partition"].get<std::string>();
        std::optional<GptPartition>
            diskPartition = gptDisk.getPartition(utf8::utf8to16(partitionName));
        if (!diskPartition.has_value())
        {
            std::cerr << "Failed to create partition " << partitionName << std::endl;
            return 3;
        }

        Fat fat(outputImagePath, diskPartition.value());
        fat.createFilesystem();
        fat.openFilesystem();

        for (auto const &jsonDirectory : jsonFilesystem["directories"])
        {
            const auto &directory = jsonDirectory.get<std::string>();
            if (!fat.createDirectory(directory))
            {
                std::cerr << "Failed to create directory " << directory << std::endl;
                return 4;
            }
        }

        for (auto const &jsonFile : jsonFilesystem["files"])
        {
            const auto &source = jsonFile["source"].get<std::string>();
            const auto &destination = jsonFile["destination"].get<std::string>();
            if (!std::filesystem::is_regular_file(source))
            {
                std::cerr << "Invalid source path: " << source << std::endl;
                return 5;
            }
            if (!fat.createFile(destination, source))
            {
                std::cerr << "Failed to create file: " << destination << std::endl;
                return 6;
            }
        }

        fat.closeFilesystem();
    }

    return 0;
}
