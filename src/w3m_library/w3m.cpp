#include <expected>
#include <spanstream>

#include <platform.hpp>
#include <exception.hpp>
#include <map/w3m.hpp>

#include "assets_mpq_importer/w3m.hpp"


namespace assmpq::w3m {

using format_getter_t = const char*(*)(const wc3lib::map::W3m& map);

static auto extract_file(const FileData& w3m_file, format_getter_t fmt_getter)
    -> std::expected<FileData, ErrorMessage>
{
	wc3lib::map::W3m map;
    try {
        std::ispanstream input(w3m_file);
        map.read(input);

        // const auto* format = map.environment().get();
        const wc3lib::mpq::File file = map.findFile(fmt_getter(map));

        std::vector<char> buffer(file.size());
        std::ospanstream output(buffer, std::ios::out | std::ios::binary);
        file.decompress(input, output);
        return buffer;
    } catch (const wc3lib::Exception &exception) {
        return std::unexpected(exception.what());
    }
}

auto extract_w3e_file(const FileData& w3m_file)
    -> std::expected<FileData, ErrorMessage>
{
    return extract_file(w3m_file, [](const auto& map) -> const char* {
        return map.environment().get()->fileName();
    });
}

auto extract_shd_file(const FileData& w3m_file)
    -> std::expected<FileData, ErrorMessage>
{
    return extract_file(w3m_file, [](const auto& map) -> const char* {
        return map.shadow().get()->fileName();
    });
}

auto extract_wpm_file(const FileData& w3m_file)
    -> std::expected<FileData, ErrorMessage>
{
    return extract_file(w3m_file, [](const auto& map) -> const char* {
        return map.pathmap().get()->fileName();
    });
}

auto extract_doo_file(const FileData& w3m_file)
    -> std::expected<FileData, ErrorMessage>
{
    return extract_file(w3m_file, [](const auto& map) -> const char* {
        return map.trees().get()->fileName();
    });
}

}  // namespace assmpq::w3m