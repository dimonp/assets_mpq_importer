module;
#include <cstdint>
#include <string>
#include <vector>

export module assmpq.merger;

export namespace assmpq::merger {

struct vector2 {
    float x, y;
};

struct vector3 {
    float x, y, z;
};

struct aabb3 {
    vector3 min, max;
};

struct TriFace
{
    uint32_t i0, i1, i2;
};

struct MeshData
{
    std::string name;
    aabb3 aabb;
    std::vector<vector3> vertices;
    std::vector<vector3> normals;
    std::vector<vector2> uvs;// Assuming only one set of UVs
    std::vector<TriFace> faces;
};

using MeshGroups = std::vector<MeshData>;

auto load_model(const std::string &path, assmpq::merger::MeshGroups &mesh_groups) -> bool;
auto save_model(const std::string &path, const MeshGroups &mesh_groups) -> bool;

void transform_mesh_to_base_xz(assmpq::merger::MeshData &mesh);
void scale_mesh(assmpq::merger::MeshData &mesh, float scale_factor);
auto split_ramp_mesh(const assmpq::merger::MeshData &mesh,
    assmpq::merger::MeshData &out_mesh_0,
    assmpq::merger::MeshData &out_mesh_1) -> bool;

auto get_cliff_key_from_geo_name(const std::string &str_key) -> uint32_t;
auto get_ramp_key_from_geo_name(const std::string &str_key, uint8_t part) -> uint32_t;

auto generate_fake(const assmpq::merger::MeshData &mesh) -> assmpq::merger::MeshData;

} // namespace assmpq::merger
