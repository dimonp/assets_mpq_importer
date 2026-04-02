module;
#include <string_view>
#include <expected>

export module assmpq.mdlx;

import assmpq;

export namespace assmpq::mdlx {

/**
 * @brief Converts MDLX mesh data to OBJ format
 *
 * Takes Blizzard MDLX file data and converts it to OBJ mesh format with the specified name.
 *
 * @param mesh_name The name to assign to the resulting OBJ mesh
 * @param mdx_file The MDLX file data to convert
 * @return std::expected<FileData, ErrorMessage> The resulting OBJ mesh data or an error message
 */
[[nodiscard]]
auto convert_mdlx_to_obj_mesh(std::string_view mesh_name, const FileData &mdx_file)
  -> std::expected<FileData, ErrorMessage>;

} // namespace assmpq::mdlx
