module;
#include <expected>

export module assmpq.w3m;

import assmpq;
#include "assets_mpq_importer/w3m_library_export.hpp"

export namespace assmpq::w3m {

/**
 * @brief Extracts the W3E (environment) file from a W3M/W3X (Warcraft III map) file.
 *
 * @param w3m_file The raw data of the W3M file to extract the environment from.
 * @return std::expected<FileData, ErrorMessage> containing the extracted file data
 *         on success, or an error message on failure.
 */
[[nodiscard]] W3M_LIBRARY_EXPORT
auto extract_w3e_file(const FileData &w3m_file) -> std::expected<FileData, ErrorMessage>;

/**
 * @brief Extracts the SHD (Shadow Map File) file from a W3M/W3X (Warcraft III map) file.
 *
 * @param w3m_file The raw data of the W3M file to extract the environment from.
 * @return std::expected<FileData, ErrorMessage> containing the extracted file data
 *         on success, or an error message on failure.
 */
[[nodiscard]] W3M_LIBRARY_EXPORT
auto extract_shd_file(const FileData &w3m_file) -> std::expected<FileData, ErrorMessage>;

/**
 * @brief Extracts the WPM (The Path Map File) file from a W3M/W3X (Warcraft III map) file.
 *
 * @param w3m_file The raw data of the W3M file to extract the environment from.
 * @return std::expected<FileData, ErrorMessage> containing the extracted file data
 *         on success, or an error message on failure.
 */
[[nodiscard]] W3M_LIBRARY_EXPORT
auto extract_wpm_file(const FileData &w3m_file) -> std::expected<FileData, ErrorMessage>;

/**
 * @brief Extracts the DOO (The doodad file for trees) file from a W3M/W3X (Warcraft III map) file.
 *
 * @param w3m_file The raw data of the W3M file to extract the environment from.
 * @return std::expected<FileData, ErrorMessage> containing the extracted file data
 *         on success, or an error message on failure.
 */
[[nodiscard]] W3M_LIBRARY_EXPORT
auto extract_doo_file(const FileData &w3m_file) -> std::expected<FileData, ErrorMessage>;

} // namespace assmpq::w3m
