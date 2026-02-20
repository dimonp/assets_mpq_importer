#include <span>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_range_equals.hpp>

#include <assets_mpq_importer/w3m.hpp>
#include "test_utils.hpp"

TEST_CASE("Extract_W3E_success", "[w3m]")
{
    const auto w3m_data = assmpq::test::load_file("testdata/test.w3m");
    const auto result = assmpq::w3m::extract_w3e_file(w3m_data);
    const std::vector<char> expected = {'W', '3', 'E', '!'};

    REQUIRE(result.has_value());
    REQUIRE(result->size() == 7684);
    REQUIRE_THAT(std::span(result->data(), 4),  Catch::Matchers::RangeEquals(expected));
}
