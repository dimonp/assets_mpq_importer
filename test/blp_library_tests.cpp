#include <vector>
#include <tuple>
#include <expected>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <nvimage/DirectDrawSurface.h>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include "test_utils.hpp"

import assmpq;
import assmpq.blp;

namespace assmpq::test {

    inline static auto get_png_info(const std::vector<char>& data)
        -> std::expected<std::tuple<int, int, int>, std::string>
    {
        int width = 0;
        int height = 0;
        int channels = 0;

        auto* image_data = stbi_load_from_memory(
            reinterpret_cast<const stbi_uc*>(data.data()), // NOLINT
            static_cast<int>(data.size()),
            &width,
            &height,
            &channels,
            0);

        if (image_data == nullptr) {
            return std::unexpected("Failed to load PNG image");
        }

        stbi_image_free(image_data);

        return std::make_tuple(width, height, channels);
    }

    struct DDS_Info {
        unsigned int width;
        unsigned int height;
        unsigned int color_bits;
        unsigned int mipmap_count;
        unsigned int format;
        unsigned int flags;
        unsigned int pitch;
    };

    static constexpr unsigned int DDSD_LINEARSIZE = 0x00080000;

    inline static auto get_dds_info(const std::vector<char>& data)
        -> std::expected<DDS_Info, std::string>
    {
        static constexpr std::uint32_t kDDSMagic = 0x20534444; // "DDS "
        static constexpr std::uint32_t k32BitsColor = 32;

        if (data.size() < sizeof(nv::DDSHeader)) {
            return std::unexpected("Invalid DDS data");
        }

        nv::DDSHeader header;
        std::memcpy(&header, data.data(), sizeof(nv::DDSHeader));
        if (header.fourcc == kDDSMagic) {
            return DDS_Info {
                .width = header.width,
                .height = header.height,
                .color_bits = ((header.pf.flags & static_cast<unsigned>(nv::DDPF_FOURCC)) != 0U) ? k32BitsColor : header.pf.bitcount,
                .mipmap_count = header.mipmapcount,
                .format = header.header10.dxgiFormat,
                .flags = header.flags,
                .pitch = header.pitch,
            };
        }

        return std::unexpected("Invalid DDS header");
    }
}

TEST_CASE("Convert_BLP_to_PNG_with_invalid_data_failed", "[blp]")
{
    const std::vector<char> invalid_data = { 'I', 'N', 'V', 'A', 'L', 'I', 'D' };
    const auto result = assmpq::blp::convert_blp_to_png_image(invalid_data);

    // This should fail since the data is not a valid BLP file
    REQUIRE_FALSE(result.has_value());
}

TEST_CASE("Convert_BLP_to_PNG_success", "[blp]")
{
    const auto blp_data = assmpq::test::load_file("testdata/test_jpeg_1x1.blp");
    const auto result = assmpq::blp::convert_blp_to_png_image(blp_data);

    REQUIRE(result.has_value());

    const auto png_info = assmpq::test::get_png_info(result.value());
    REQUIRE(png_info.has_value());

    auto [width, height, channels] = png_info.value();
    REQUIRE(width == 1);
    REQUIRE(height == 1);
    REQUIRE(channels == 4);
}

TEST_CASE("Convert_BLP_to_PNG_with_mipmap_index_success", "[blp]")
{
    const auto blp_data = assmpq::test::load_file("testdata/test_jpeg_32x32.blp");
    const auto result = assmpq::blp::convert_blp_to_png_image(blp_data, 2);

    REQUIRE(result.has_value());

    const auto png_info = assmpq::test::get_png_info(result.value());
    REQUIRE(png_info.has_value());

    auto [width, height, channels] = png_info.value();
    REQUIRE(width == 8);
    REQUIRE(height == 8);
    REQUIRE(channels == 4);
}

TEST_CASE("Convert_BLP_to_PNG_paletted_success", "[blp]")
{
    const auto blp_data = assmpq::test::load_file("testdata/test_raw_32x32_paletted.blp");
    const auto result = assmpq::blp::convert_blp_to_png_image(blp_data);

    REQUIRE(result.has_value());

    const auto png_info = assmpq::test::get_png_info(result.value());
    REQUIRE(png_info.has_value());

    auto [width, height, channels] = png_info.value();
    REQUIRE(width == 32);
    REQUIRE(height == 32);
    REQUIRE(channels == 4);
}

TEST_CASE("Convert_BLP_to_PNG_with_mipmap_index_out_of_range_failed", "[blp]")
{
    const auto blp_data = assmpq::test::load_file("testdata/test_jpeg_32x32.blp");
    const auto result = assmpq::blp::convert_blp_to_png_image(blp_data, 100500);

    REQUIRE_FALSE(result.has_value());
    REQUIRE(result.error() == "Mipmap index 100500 is out of range.");
}

TEST_CASE("Convert_BLP_to_DDS_NVTT_with_invalid_data_failed", "[blp]")
{
    const std::vector<char> invalid_data = { 'I', 'N', 'V', 'A', 'L', 'I', 'D' };
    const auto result = assmpq::blp::convert_blp_to_dds_texture_nvtt(invalid_data);

    // This should fail since the data is not a valid BLP file
    REQUIRE_FALSE(result.has_value());
}

TEST_CASE("Convert_BLP_to_DDS_NVTT_success", "[blp]")
{
    const auto blp_data = assmpq::test::load_file("testdata/test_jpeg_1x1.blp");
    const auto result = assmpq::blp::convert_blp_to_dds_texture_nvtt(blp_data);

    REQUIRE(result.has_value());

    const auto dds_info = assmpq::test::get_dds_info(result.value());
    REQUIRE(dds_info.has_value());

    const auto info = dds_info.value();
    REQUIRE(info.width == 1);
    REQUIRE(info.height == 1);
    REQUIRE(info.color_bits == 32);
    REQUIRE(info.mipmap_count == 1);
}

TEST_CASE("Convert_BLP_to_DDS_NVTT_with_compression_BC1_success", "[blp]")
{
    const auto blp_data = assmpq::test::load_file("testdata/test_jpeg_32x32.blp");
    const auto result_bc1 = assmpq::blp::convert_blp_to_dds_texture_nvtt(
        blp_data,
        assmpq::Compression::DDS_BC1
    );

    REQUIRE(result_bc1.has_value());

    const auto dds_info_bc1 = assmpq::test::get_dds_info(result_bc1.value());
    REQUIRE(dds_info_bc1.has_value());

    const auto info = dds_info_bc1.value();
    REQUIRE(info.width == 32);
    REQUIRE(info.height == 32);
    REQUIRE(info.color_bits == 32);
    REQUIRE(info.mipmap_count == 6);
    REQUIRE(info.format == nv::DXGI_FORMAT_BC1_UNORM);
}

TEST_CASE("Convert_BLP_to_DDS_NVTT_with_compression_BC3_succeess", "[blp]")
{
    const auto blp_data = assmpq::test::load_file("testdata/test_jpeg_32x32.blp");
    const auto result_bc3 = assmpq::blp::convert_blp_to_dds_texture_nvtt(
        blp_data,
        assmpq::Compression::DDS_BC3
    );

    REQUIRE(result_bc3.has_value());

    const auto dds_info_bc3 = assmpq::test::get_dds_info(result_bc3.value());
    REQUIRE(dds_info_bc3.has_value());

    const auto info = dds_info_bc3.value();
    REQUIRE(info.width == 32);
    REQUIRE(info.height == 32);
    REQUIRE(info.color_bits == 32);
    REQUIRE(info.mipmap_count == 6);
    REQUIRE(info.format == nv::DXGI_FORMAT_BC3_UNORM);
}

TEST_CASE("Convert_BLP_to_DDS_NVTT_with_compression_BC7_success", "[blp]")
{
    const auto blp_data = assmpq::test::load_file("testdata/test_jpeg_32x32.blp");
    const auto result_bc7 = assmpq::blp::convert_blp_to_dds_texture_nvtt(
        blp_data,
        assmpq::Compression::DDS_BC7
    );

    REQUIRE(result_bc7.has_value());
    REQUIRE_FALSE(result_bc7->empty());

    const auto dds_info_bc7 = assmpq::test::get_dds_info(result_bc7.value());
    REQUIRE(dds_info_bc7.has_value());

    const auto info = dds_info_bc7.value();
    REQUIRE(info.width == 32);
    REQUIRE(info.height == 32);
    REQUIRE(info.color_bits == 32);
    REQUIRE(info.mipmap_count == 6);
    REQUIRE(info.format == nv::DXGI_FORMAT_BC7_UNORM);
}

TEST_CASE("Convert_BLP_to_DDS_NVTT_dont_generate_mipmaps_succeess", "[blp]")
{
    const auto blp_data = assmpq::test::load_file("testdata/test_jpeg_32x32-no-mipmaps.blp");
    const auto result_bc3 = assmpq::blp::convert_blp_to_dds_texture_nvtt(
        blp_data,
        assmpq::Compression::DDS_BC3,
        false
    );

    REQUIRE(result_bc3.has_value());

    const auto dds_info_bc3 = assmpq::test::get_dds_info(result_bc3.value());
    REQUIRE(dds_info_bc3.has_value());

    const auto info = dds_info_bc3.value();
    REQUIRE(info.width == 32);
    REQUIRE(info.height == 32);
    REQUIRE(info.color_bits == 32);
    REQUIRE(info.mipmap_count == 1);
    REQUIRE(info.format == nv::DXGI_FORMAT_BC3_UNORM);
}

TEST_CASE("Convert_BLP_to_DDS_NVTT_generate_mipmaps_succeess", "[blp]")
{
    const auto blp_data = assmpq::test::load_file("testdata/test_jpeg_32x32-no-mipmaps.blp");
    const auto result_bc3 = assmpq::blp::convert_blp_to_dds_texture_nvtt(
        blp_data,
        assmpq::Compression::DDS_BC3,
        true
    );

    REQUIRE(result_bc3.has_value());

    const auto dds_info_bc3 = assmpq::test::get_dds_info(result_bc3.value());
    REQUIRE(dds_info_bc3.has_value());

    const auto info = dds_info_bc3.value();
    REQUIRE(info.width == 32);
    REQUIRE(info.height == 32);
    REQUIRE(info.color_bits == 32);
    REQUIRE(info.mipmap_count == 6);
    REQUIRE(info.format == nv::DXGI_FORMAT_BC3_UNORM);
}

TEST_CASE("Convert_BLP_to_DDS_NVTT_paletted_generate_mipmaps_succeess", "[blp]")
{
    const auto blp_data = assmpq::test::load_file("testdata/test_raw_32x32_paletted.blp");
    const auto result_bc3 = assmpq::blp::convert_blp_to_dds_texture_nvtt(
        blp_data,
        assmpq::Compression::DDS_BC3,
        true
    );

    REQUIRE(result_bc3.has_value());

    const auto dds_info_bc3 = assmpq::test::get_dds_info(result_bc3.value());
    REQUIRE(dds_info_bc3.has_value());

    const auto info = dds_info_bc3.value();
    REQUIRE(info.width == 32);
    REQUIRE(info.height == 32);
    REQUIRE(info.color_bits == 32);
    REQUIRE(info.mipmap_count == 6);
    REQUIRE(info.format == nv::DXGI_FORMAT_BC3_UNORM);
}

TEST_CASE("Convert_BLP_to_DDS_AMDC_with_invalid_data_failed", "[blp]")
{
    const std::vector<char> invalid_data = { 'I', 'N', 'V', 'A', 'L', 'I', 'D' };
    const auto result = assmpq::blp::convert_blp_to_dds_texture_amdc(invalid_data);

    // This should fail since the data is not a valid BLP file
    REQUIRE_FALSE(result.has_value());
}

TEST_CASE("Convert_BLP_to_DDS_AMDC_success", "[blp]")
{
    const auto blp_data = assmpq::test::load_file("testdata/test_jpeg_1x1.blp");
    const auto result = assmpq::blp::convert_blp_to_dds_texture_amdc(blp_data);

    REQUIRE(result.has_value());

    const auto dds_info = assmpq::test::get_dds_info(result.value());
    REQUIRE(dds_info.has_value());

    const auto info = dds_info.value();
    REQUIRE(info.width == 1);
    REQUIRE(info.height == 1);
    REQUIRE(info.color_bits == 32);
    REQUIRE(info.mipmap_count == 1);
    REQUIRE(info.pitch == 16);
}

TEST_CASE("Convert_BLP_to_DDS_AMDC_with_compression_BC1_success", "[blp]")
{
    const auto blp_data = assmpq::test::load_file("testdata/test_jpeg_32x32.blp");
    const auto result_bc1 = assmpq::blp::convert_blp_to_dds_texture_amdc(
        blp_data,
        assmpq::Compression::DDS_BC1
    );

    REQUIRE(result_bc1.has_value());

    const auto dds_info_bc1 = assmpq::test::get_dds_info(result_bc1.value());
    REQUIRE(dds_info_bc1.has_value());

    const auto info = dds_info_bc1.value();
    REQUIRE(info.width == 32);
    REQUIRE(info.height == 32);
    REQUIRE(info.color_bits == 32);
    REQUIRE(info.mipmap_count == 6);
    REQUIRE(info.format == nv::DXGI_FORMAT_BC1_UNORM);
    REQUIRE(info.flags | assmpq::test::DDSD_LINEARSIZE);
    REQUIRE(info.pitch == 512);
}

TEST_CASE("Convert_BLP_to_DDS_AMDC_with_compression_BC3_succeess", "[blp]")
{
    const auto blp_data = assmpq::test::load_file("testdata/test_jpeg_32x32.blp");
    const auto result_bc3 = assmpq::blp::convert_blp_to_dds_texture_amdc(
        blp_data,
        assmpq::Compression::DDS_BC3
    );

    REQUIRE(result_bc3.has_value());

    const auto dds_info_bc3 = assmpq::test::get_dds_info(result_bc3.value());
    REQUIRE(dds_info_bc3.has_value());

    const auto info = dds_info_bc3.value();
    REQUIRE(info.width == 32);
    REQUIRE(info.height == 32);
    REQUIRE(info.color_bits == 32);
    REQUIRE(info.mipmap_count == 6);
    REQUIRE(info.format == nv::DXGI_FORMAT_BC3_UNORM);
    REQUIRE(info.flags | assmpq::test::DDSD_LINEARSIZE);
    REQUIRE(info.pitch == 1024);
}

TEST_CASE("Convert_BLP_to_DDS_AMDC_with_compression_BC7_success", "[blp]")
{
    const auto blp_data = assmpq::test::load_file("testdata/test_jpeg_32x32.blp");
    const auto result_bc7 = assmpq::blp::convert_blp_to_dds_texture_amdc(
        blp_data,
        assmpq::Compression::DDS_BC7
    );

    REQUIRE(result_bc7.has_value());

    const auto dds_info_bc7 = assmpq::test::get_dds_info(result_bc7.value());
    REQUIRE(dds_info_bc7.has_value());
    const auto info = dds_info_bc7.value();
    REQUIRE(info.width == 32);
    REQUIRE(info.height == 32);
    REQUIRE(info.color_bits == 32);
    REQUIRE(info.mipmap_count == 6);
    REQUIRE(info.format == nv::DXGI_FORMAT_BC7_UNORM);
    REQUIRE(info.flags | assmpq::test::DDSD_LINEARSIZE);
    REQUIRE(info.pitch == 1024);
}

TEST_CASE("Convert_BLP_to_DDS_AMDC_generate_mipmaps_succeess", "[blp]")
{
    const auto blp_data = assmpq::test::load_file("testdata/test_jpeg_32x32-no-mipmaps.blp");
    const auto result_bc3 = assmpq::blp::convert_blp_to_dds_texture_amdc(
        blp_data,
        assmpq::Compression::DDS_BC3,
        true
    );

    REQUIRE(result_bc3.has_value());

    const auto dds_info_bc3 = assmpq::test::get_dds_info(result_bc3.value());
    REQUIRE(dds_info_bc3.has_value());

    const auto info = dds_info_bc3.value();
    REQUIRE(info.width == 32);
    REQUIRE(info.height == 32);
    REQUIRE(info.color_bits == 32);
    REQUIRE(info.mipmap_count == 6);
    REQUIRE(info.format == nv::DXGI_FORMAT_BC3_UNORM);
}

TEST_CASE("Convert_BLP_to_DDS_AMDC_paletted_generate_mipmaps_succeess", "[blp]")
{
    const auto blp_data = assmpq::test::load_file("testdata/test_raw_32x32_paletted.blp");
    const auto result_bc3 = assmpq::blp::convert_blp_to_dds_texture_amdc(
        blp_data,
        assmpq::Compression::DDS_BC3,
        true
    );

    REQUIRE(result_bc3.has_value());

    const auto dds_info_bc3 = assmpq::test::get_dds_info(result_bc3.value());
    REQUIRE(dds_info_bc3.has_value());

    const auto info = dds_info_bc3.value();
    REQUIRE(info.width == 32);
    REQUIRE(info.height == 32);
    REQUIRE(info.color_bits == 32);
    REQUIRE(info.mipmap_count == 6);
    REQUIRE(info.format == nv::DXGI_FORMAT_BC3_UNORM);
}

TEST_CASE("Convert_BLP_to_DDS_AMDC_dont_generate_mipmaps_succeess", "[blp]")
{
    const auto blp_data = assmpq::test::load_file("testdata/test_jpeg_32x32-no-mipmaps.blp");
    const auto result_bc3 = assmpq::blp::convert_blp_to_dds_texture_amdc(
        blp_data,
        assmpq::Compression::DDS_BC3,
        false
    );

    REQUIRE(result_bc3.has_value());

    const auto dds_info_bc3 = assmpq::test::get_dds_info(result_bc3.value());
    REQUIRE(dds_info_bc3.has_value());

    const auto info = dds_info_bc3.value();
    REQUIRE(info.width == 32);
    REQUIRE(info.height == 32);
    REQUIRE(info.color_bits == 32);
    REQUIRE(info.mipmap_count == 1);
    REQUIRE(info.format == nv::DXGI_FORMAT_BC3_UNORM);
}
