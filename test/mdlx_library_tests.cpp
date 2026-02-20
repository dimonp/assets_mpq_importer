#include <spanstream>
#include <string>
#include <catch2/catch_test_macros.hpp>

#include <assets_mpq_importer/mdlx.hpp>
#include "test_utils.hpp"

namespace assmpq::test {

    inline static auto get_obj_info(const std::vector<char>& data)
        -> std::expected<std::tuple<int, int, int, int>, std::string>
    {
        std::ispanstream input(data);
        std::string line;

        int vertices = 0;
        int tvertices = 0;
        int normales = 0;
        int faces = 0;
        while (std::getline(input, line)) {
            if (line.starts_with("v ")) {
                ++vertices;
            } if (line.starts_with("vt ")) {
                ++tvertices;
            } if (line.starts_with("vn ")) {
                ++normales;
            } if (line.starts_with("f ")) {
                ++faces;
            }
        }

        return std::make_tuple(vertices, tvertices, normales, faces);
    }

}

TEST_CASE("Convert_MDX_to_OBJ", "[mdlx]")
{
    const auto mdlx_data = assmpq::test::load_file("testdata/test_4v_4n_4t_2f.mdx");
    const auto obj_result = assmpq::mdlx::convert_mdlx_to_obj_mesh("test_mesh", mdlx_data);

    REQUIRE(obj_result.has_value());
    const auto obj_info = assmpq::test::get_obj_info(obj_result.value());

    auto [num_vertices, num_uv_components, num_normales, num_faces] = obj_info.value();
    REQUIRE(num_vertices == 4);
    REQUIRE(num_uv_components == 4);
    REQUIRE(num_normales == 4);
    REQUIRE(num_faces == 2);
}
