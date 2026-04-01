module;
#include <exception.hpp>
#include <expected>
#include <map/w3m.hpp>
#include <platform.hpp>
#include <spanstream>

module assmpq.w3m;

import assmpq;

namespace assmpq::w3m {

using format_getter_t = const char *(*)(const wc3lib::map::W3m &map);

static auto extract_file(const FileData &w3m_file, format_getter_t fmt_getter) -> std::expected<FileData, ErrorMessage>
try {
    wc3lib::map::W3m map;
    std::ispanstream input(w3m_file);
    map.read(input);

    const wc3lib::mpq::File file = map.findFile(fmt_getter(map));

    std::vector<char> output_buffer(file.size());
    std::ospanstream output_stream(output_buffer, std::ios::out | std::ios::binary);
    file.decompress(input, output_stream);
    return output_buffer;

} catch (const wc3lib::Exception &exception) {
    return std::unexpected(exception.what());
}

auto extract_w3e_file(const FileData &w3m_file) -> std::expected<FileData, ErrorMessage>
{
    return extract_file(w3m_file, [](const auto &map) -> const char * { return map.environment().get()->fileName(); });
}

auto extract_shd_file(const FileData &w3m_file) -> std::expected<FileData, ErrorMessage>
{
    return extract_file(w3m_file, [](const auto &map) -> const char * { return map.shadow().get()->fileName(); });
}

auto extract_wpm_file(const FileData &w3m_file) -> std::expected<FileData, ErrorMessage>
{
    return extract_file(w3m_file, [](const auto &map) -> const char * { return map.pathmap().get()->fileName(); });
}

auto extract_doo_file(const FileData &w3m_file) -> std::expected<FileData, ErrorMessage>
{
    return extract_file(w3m_file, [](const auto &map) -> const char * { return map.trees().get()->fileName(); });
}

}// namespace assmpq::w3m