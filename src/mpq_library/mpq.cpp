#include <string>
#include <algorithm>
#include <functional>
#include <ranges>
#include <filesystem>
#include <expected>
#include <regex>
#include <spanstream>

#include <platform.hpp>
#include <exception.hpp>
#include <mpq/archive.hpp>
#include <mpq/listfile.hpp>

#include "assets_mpq_importer/mpq.hpp"


namespace assmpq::mpq {

using filelist_filter_t = std::function<bool(const std::string&)>;

static auto wildcard_to_regex(std::string mask)-> std::regex {
    // Convert wildcard to regex pattern
    // Simple replacements; for production, escape other regex special chars
    std::size_t pos = 0;
    while ((pos = mask.find('.', pos)) != std::string::npos) { mask.replace(pos, 1, "\\."); pos += 2; }
    pos = 0;
    while ((pos = mask.find('?', pos)) != std::string::npos) { mask.replace(pos, 1, "."); pos += 1; }
    pos = 0;
    while ((pos = mask.find('*', pos)) != std::string::npos) { mask.replace(pos, 1, ".*"); pos += 2; }

    return std::regex(mask, std::regex_constants::icase); // icase for case-insensitive
}

auto list_mpq_files(const std::filesystem::path& archive_path, const std::string& mask)
    -> std::expected<ArchiveEntries, ErrorMessage>
{
    wc3lib::mpq::Archive archive;

    try {
        archive.open(archive_path.c_str());
    } catch (const wc3lib::Exception &exception) {
        return std::unexpected(exception.what());
    }

    if (archive.containsListfileFile()) {
        const wc3lib::mpq::Listfile filelist = archive.listfileFile();

        if (filelist.isValid()) {
            static auto mask_regex = wildcard_to_regex(mask);

            auto filter = mask.empty() ?
                filelist_filter_t([](const std::string &entry)-> bool { return !entry.empty(); }) :
                filelist_filter_t([](const std::string &entry)-> bool {
                    return std::regex_match(entry, mask_regex);
                });

            wc3lib::mpq::Listfile::Entries entries = filelist.entries();
            std::ranges::sort(entries);

            return entries |
                std::views::filter(filter) |
                std::views::transform([&archive](const auto &entry)-> FileEntry {
                    const wc3lib::mpq::File file = archive.findFile(entry);
                    return FileEntry{ .filename = entry, .size = file.size() };
                }) |
                std::ranges::to<ArchiveEntries>();
        }
    }

    return std::unexpected("List file not found.");
}

auto extract_mpq_file(const std::filesystem::path& archive_path, const std::string& filename)
    -> std::expected<FileData, ErrorMessage>
{
    wc3lib::mpq::Archive archive;

    try {
        archive.open(archive_path.c_str());

        const wc3lib::mpq::File file = archive.findFile(filename);
        if (!file.isValid()) {
            return std::unexpected("File not found.");
        }

        std::vector<char> buffer(file.size());
        std::ospanstream output(buffer, std::ios::out | std::ios::binary);
        file.decompress(output);

        return buffer;
    } catch (const wc3lib::Exception &exception) {
        return std::unexpected(exception.what());
    }
}


}  // namespace assmpq::mpq