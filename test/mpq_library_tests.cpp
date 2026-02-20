#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>

#include <assets_mpq_importer/mpq.hpp>

TEST_CASE("Get_MPQ_listfile_success", "[mpq]")
{
    const auto result = assmpq::mpq::list_mpq_files("testdata/test_with_three_files.mpq");
    const assmpq::mpq::ArchiveEntries expected_list = std::vector {
        assmpq::mpq::FileEntry { .filename = "testfile10.txt", .size = 10  },
        assmpq::mpq::FileEntry { .filename = "testfile20.txt", .size = 20  },
        assmpq::mpq::FileEntry { .filename = "testfile25.txt", .size = 25  },
    };

   REQUIRE(result.has_value());
   REQUIRE(result->size() == 3);
   REQUIRE_THAT(result.value(), Catch::Matchers::Equals(expected_list));
}

TEST_CASE("Get_MPQ_listfile_filtered_success", "[mpq]")
{
    const auto result = assmpq::mpq::list_mpq_files("testdata/test_with_three_files.mpq", "*20*");
    const assmpq::mpq::ArchiveEntries expected_list = std::vector {
        assmpq::mpq::FileEntry { .filename = "testfile20.txt", .size = 20  },
    };

    REQUIRE(result.has_value());
    REQUIRE(result->size() == 1);
    REQUIRE_THAT(result.value(), Catch::Matchers::Equals(expected_list));
}

TEST_CASE("Get_MPQ_listfile_failed", "[mpq]")
{
    const auto result = assmpq::mpq::list_mpq_files("testdata/test_with_no_listfile.mpq");

    REQUIRE_FALSE(result.has_value());
    REQUIRE(result.error() == "List file not found.");
}

TEST_CASE("Extract_MPQ_file_success", "[mpq]")
{
    const auto result = assmpq::mpq::extract_mpq_file("testdata/test_with_three_files.mpq", "testfile25.txt");

    REQUIRE(result.has_value());
    REQUIRE(result->size() == 25);
}

TEST_CASE("Extract_MPQ_file_failed", "[mpq]")
{
    const auto result = assmpq::mpq::extract_mpq_file("testdata/test_with_three_files.mpq", "testfile_not_exist.txt");

    REQUIRE_FALSE(result.has_value());
}
