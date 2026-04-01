module;
#include <array>
#include <algorithm>
#include <ranges>
#include <limits>
#include <spdlog/spdlog.h>

module assmpq.merger;

namespace assmpq::merger {

inline constexpr float kHighestValue = std::numeric_limits<float>::max();
inline constexpr float kLowestValue = std::numeric_limits<float>::lowest();
inline constexpr float kEpsilonValue = 0.1F;

inline constexpr float kW3MapCellSize = 128.0F;

void recalculate_aabb(assmpq::merger::MeshData &mesh)
{
    vector3 min(assmpq::merger::kHighestValue, assmpq::merger::kHighestValue, assmpq::merger::kHighestValue);
    vector3 max(assmpq::merger::kLowestValue, assmpq::merger::kLowestValue, assmpq::merger::kLowestValue);

    for (const auto &vertex : mesh.vertices) {
        min.x = std::min(vertex.x, min.x);
        min.y = std::min(vertex.y, min.y);
        min.z = std::min(vertex.z, min.z);

        max.x = std::max(vertex.x, max.x);
        max.y = std::max(vertex.y, max.y);
        max.z = std::max(vertex.z, max.z);
    }

    mesh.aabb.min = { min.x, min.y, min.z };
    mesh.aabb.max = { max.x, max.y, max.z };
}

void transform_mesh_to_base_xz(assmpq::merger::MeshData &mesh)
{
    for (size_t i = 0; i < mesh.vertices.size(); i++) {
        // coord
        vector3 &vertex = mesh.vertices[i];
        vertex.x = -vertex.x;
        std::swap(vertex.y, vertex.x);
        std::swap(vertex.y, vertex.z);
        vertex.z = -vertex.z;

        // normal
        vector3 &normal = mesh.normals[i];
        normal.x = -normal.x;
        std::swap(normal.y, normal.x);
        std::swap(normal.y, normal.z);
        normal.z = -normal.z;
    }

    recalculate_aabb(mesh);
}

void scale_mesh(assmpq::merger::MeshData &mesh, float scale_factor)
{
    for (auto &vertex : mesh.vertices) {
        vertex.x *= scale_factor;
        vertex.y *= scale_factor;
        vertex.z *= scale_factor;
    }

    recalculate_aabb(mesh);
}

auto split_ramp_mesh(const assmpq::merger::MeshData &mesh,
  assmpq::merger::MeshData &out_mesh_0,
  assmpq::merger::MeshData &out_mesh_1) -> bool
{
    const aabb3 &aabb = mesh.aabb;
    if (std::abs(std::abs(aabb.max.x) - (kW3MapCellSize + kW3MapCellSize)) < assmpq::merger::kEpsilonValue) {
        // Spilt by x axis
        for (const auto &face : mesh.faces) {
            vector3 vertex0 = mesh.vertices[face.i0];
            vector3 vertex1 = mesh.vertices[face.i1];
            vector3 vertex2 = mesh.vertices[face.i2];

            if (std::abs(vertex0.x) < (kW3MapCellSize + kEpsilonValue)
                && std::abs(vertex1.x) < (kW3MapCellSize + kEpsilonValue)
                && std::abs(vertex2.x) < (kW3MapCellSize + kEpsilonValue)) {

                auto from_idx = static_cast<uint32_t>(out_mesh_0.vertices.size());

                out_mesh_0.vertices.push_back(vertex0);
                out_mesh_0.vertices.push_back(vertex1);
                out_mesh_0.vertices.push_back(vertex2);

                out_mesh_0.normals.push_back(mesh.normals[face.i0]);
                out_mesh_0.normals.push_back(mesh.normals[face.i1]);
                out_mesh_0.normals.push_back(mesh.normals[face.i2]);

                out_mesh_0.uvs.push_back(mesh.uvs[face.i0]);
                out_mesh_0.uvs.push_back(mesh.uvs[face.i1]);
                out_mesh_0.uvs.push_back(mesh.uvs[face.i2]);

                out_mesh_0.faces.push_back(
                  assmpq::merger::TriFace{ .i0 = from_idx, .i1 = from_idx + 1, .i2 = from_idx + 2 });
            }

            if (std::abs(vertex0.x) > (kW3MapCellSize - kEpsilonValue)
                && std::abs(vertex1.x) > (kW3MapCellSize - kEpsilonValue)
                && std::abs(vertex2.x) > (kW3MapCellSize - kEpsilonValue)) {

                auto from_idx = static_cast<uint32_t>(out_mesh_1.vertices.size());

                vertex0.x -= kW3MapCellSize;
                vertex1.x -= kW3MapCellSize;
                vertex2.x -= kW3MapCellSize;

                out_mesh_1.vertices.push_back(vertex0);
                out_mesh_1.vertices.push_back(vertex1);
                out_mesh_1.vertices.push_back(vertex2);

                out_mesh_1.normals.push_back(mesh.normals[face.i0]);
                out_mesh_1.normals.push_back(mesh.normals[face.i1]);
                out_mesh_1.normals.push_back(mesh.normals[face.i2]);

                out_mesh_1.uvs.push_back(mesh.uvs[face.i0]);
                out_mesh_1.uvs.push_back(mesh.uvs[face.i1]);
                out_mesh_1.uvs.push_back(mesh.uvs[face.i2]);

                out_mesh_1.faces.push_back(
                  assmpq::merger::TriFace{ .i0 = from_idx, .i1 = from_idx + 1, .i2 = from_idx + 2 });
            }
        }
    } else if (std::abs(std::abs(aabb.min.z) - (kW3MapCellSize + kW3MapCellSize)) < assmpq::merger::kEpsilonValue) {
        // Spilt by z axis
        for (const auto &face : mesh.faces) {
            vector3 vertex0 = mesh.vertices[face.i0];
            vector3 vertex1 = mesh.vertices[face.i1];
            vector3 vertex2 = mesh.vertices[face.i2];

            if (std::abs(vertex0.z) < (kW3MapCellSize + kEpsilonValue)
                && std::abs(vertex1.z) < (kW3MapCellSize + kEpsilonValue)
                && std::abs(vertex2.z) < (kW3MapCellSize + kEpsilonValue)) {

                auto from_idx = static_cast<uint32_t>(out_mesh_0.vertices.size());

                out_mesh_0.vertices.push_back(vertex0);
                out_mesh_0.vertices.push_back(vertex1);
                out_mesh_0.vertices.push_back(vertex2);

                out_mesh_0.normals.push_back(mesh.normals[face.i0]);
                out_mesh_0.normals.push_back(mesh.normals[face.i1]);
                out_mesh_0.normals.push_back(mesh.normals[face.i2]);

                out_mesh_0.uvs.push_back(mesh.uvs[face.i0]);
                out_mesh_0.uvs.push_back(mesh.uvs[face.i1]);
                out_mesh_0.uvs.push_back(mesh.uvs[face.i2]);

                out_mesh_0.faces.push_back(
                  assmpq::merger::TriFace{ .i0 = from_idx, .i1 = from_idx + 1, .i2 = from_idx + 2 });
            }

            if (std::abs(vertex0.z) > (kW3MapCellSize - kEpsilonValue)
                && std::abs(vertex1.z) > (kW3MapCellSize - kEpsilonValue)
                && std::abs(vertex2.z) > (kW3MapCellSize - kEpsilonValue)) {

                auto from_idx = static_cast<uint32_t>(out_mesh_1.vertices.size());

                vertex0.z += kW3MapCellSize;
                vertex1.z += kW3MapCellSize;
                vertex2.z += kW3MapCellSize;

                out_mesh_1.vertices.push_back(vertex0);
                out_mesh_1.vertices.push_back(vertex1);
                out_mesh_1.vertices.push_back(vertex2);

                out_mesh_1.normals.push_back(mesh.normals[face.i0]);
                out_mesh_1.normals.push_back(mesh.normals[face.i1]);
                out_mesh_1.normals.push_back(mesh.normals[face.i2]);

                out_mesh_1.uvs.push_back(mesh.uvs[face.i0]);
                out_mesh_1.uvs.push_back(mesh.uvs[face.i1]);
                out_mesh_1.uvs.push_back(mesh.uvs[face.i2]);

                out_mesh_1.faces.push_back(
                  assmpq::merger::TriFace{ .i0 = from_idx, .i1 = from_idx + 1, .i2 = from_idx + 2 });
            }
        }
    } else {
        return false;
    }

    out_mesh_0.name = mesh.name + "0";
    out_mesh_1.name = mesh.name + "1";

    recalculate_aabb(out_mesh_0);
    recalculate_aabb(out_mesh_1);
    return true;
}

auto generate_fake(const assmpq::merger::MeshData &mesh) -> assmpq::merger::MeshData
{
    static constexpr auto kCornerXZ = std::to_array<std::pair<float, float>>({
      { 0.0F, -128.0F },
      { 128.0F, -128.0F },
      { 128.0F, 0.0F },
      { 0.0F, 0.0F },
    });

    static constexpr auto kCornerUV = std::to_array<std::pair<float, float>>({
      { 0.75F, 0.0F },
      { 1.0F, 0.0F },
      { 1.0F, 0.25F },
      { 0.75F, 0.25F },
    });

    assmpq::merger::MeshData mesh_result;
    mesh_result.name = mesh.name;

    for (auto const [idx, xz] : std::views::enumerate(kCornerXZ)) {
        const auto &uvs = kCornerUV[static_cast<size_t>(idx)];

        auto found = std::ranges::find_if(mesh.vertices, [&xz](const auto &vertex) {
            return std::abs(xz.first - vertex.x) < assmpq::merger::kEpsilonValue
                   && std::abs(xz.second - vertex.z) < assmpq::merger::kEpsilonValue;
        });

        if (found != std::ranges::end(mesh.vertices)) {
            auto index = static_cast<size_t>(std::distance(mesh.vertices.begin(), found));
            mesh_result.vertices.emplace_back(xz.first, found->y, xz.second);
            mesh_result.uvs.emplace_back(uvs.first, uvs.second);
            mesh_result.normals.push_back(mesh.normals[index]);
        }
    }

    if (mesh_result.vertices.size() != 4) {
        spdlog::error("Fake cliff mesh {} size should be 4. Got {}", mesh_result.name, mesh_result.vertices.size());
        return mesh_result;
    }

    mesh_result.faces.push_back(assmpq::merger::TriFace{ .i0 = 0, .i1 = 3, .i2 = 2 });
    mesh_result.faces.push_back(assmpq::merger::TriFace{ .i0 = 2, .i1 = 1, .i2 = 0 });

    return mesh_result;
}

}// namespace assmpq::merger
