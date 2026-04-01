module;
#include <filesystem>

export module assmpq.importer;

import assmpq;

export namespace assmpq::importer {

/// @brief Structure to hold command line options for the MPQ importer
/// @details Contains all configuration parameters that can be set by the user
/// through command line arguments
struct ProgramOptions
{
    std::filesystem::path input_mpq_file;   ///< Path to the input MPQ archive file
    std::filesystem::path output_folder;    ///< Path to the output folder for extracted files
    std::string pattern;                    ///< File filter pattern for extraction
    assmpq::Compression compression = assmpq::Compression::DDS_BC3;///< DDS compression format
    bool is_nvtt = false;                   ///< Flag to use Nvidia Texture Tools compressor
    bool is_dds = false;                    ///< Flag to convert BLP textures to DDS format
    bool is_regen_mipmaps = true;           ///< Flag to regenerate mipmaps from first level
    bool is_extract = false;                ///< Flag to extract files without conversion
    bool is_w3e_only = true;                ///< Flag to extract files without conversion
    bool is_verbose = false;                ///< Flag to enable verbose output
};

using import_func_t = bool (*)(const assmpq::FileData &, const std::filesystem::path &, const ProgramOptions &);

auto import_save(const assmpq::FileData &file_data,
    const std::filesystem::path &archived_file_path,
    const ProgramOptions &popt) -> bool;

auto import_blp(const assmpq::FileData &file_data,
    const std::filesystem::path &archived_file_path,
    const ProgramOptions &popt) -> bool;

auto import_mdx(const assmpq::FileData &file_data,
    const std::filesystem::path &archived_file_path,
    const ProgramOptions &popt) -> bool;

auto import_w3e(const assmpq::FileData &file_data,
    const std::filesystem::path &archived_file_path,
    const ProgramOptions &popt) -> bool;

auto import_shd(const assmpq::FileData &file_data,
    const std::filesystem::path &archived_file_path,
    const ProgramOptions &popt) -> bool;

auto import_wpm(const assmpq::FileData &file_data,
    const std::filesystem::path &archived_file_path,
    const ProgramOptions &popt) -> bool;

auto import_doo(const assmpq::FileData &file_data,
    const std::filesystem::path &archived_file_path,
    const ProgramOptions &popt) -> bool;

} // namespace assmpq::importer
