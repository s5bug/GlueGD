// =============================================================================
//
// ztd.text
// Copyright © 2021 JeanHeyd "ThePhD" Meneide and Shepherd's Oasis, LLC
// Contact: opensource@soasis.org
//
// Commercial License Usage
// Licensees holding valid commercial ztd.text licenses may use this file in
// accordance with the commercial license agreement provided with the
// Software or, alternatively, in accordance with the terms contained in
// a written agreement between you and Shepherd's Oasis, LLC.
// For licensing terms and conditions see your agreement. For
// further information contact opensource@soasis.org.
//
// Apache License Version 2 Usage
// Alternatively, this file may be used under the terms of Apache License
// Version 2.0 (the "License") for non-commercial use; you may not use this
// file except in compliance with the License. You may obtain a copy of the
// License at
//
//		http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// ============================================================================>

#include <ztd/text/validate_decodable_as.hpp>
#include <ztd/text/encoding.hpp>

#include <catch2/catch.hpp>

#include <ztd/text/tests/basic_unicode_strings.hpp>

inline namespace ztd_text_tests_basic_run_time_validate_decodable_as {
	template <typename Input, typename Encoding>
	void validate_check(Input& input, Encoding& encoding) {
		auto result0 = ztd::text::validate_decodable_as(input);
		REQUIRE(result0);
		auto result1 = ztd::text::validate_decodable_as(input, encoding);
		REQUIRE(result1);
	}
} // namespace ztd_text_tests_basic_run_time_validate_decodable_as

TEST_CASE("text/validate_decodable_as/basic", "basic usages of validate_decodable_as function do not explode") {
	SECTION("execution") {
		ztd::text::execution_t encoding {};
		validate_check(ztd::tests::basic_source_character_set, encoding);
		if (ztd::text::contains_unicode_encoding(encoding)) {
			validate_check(ztd::tests::unicode_sequence_truth_native_endian, encoding);
		}
	}
	SECTION("wide_execution") {
		ztd::text::wide_execution_t encoding {};
		validate_check(ztd::tests::w_basic_source_character_set, encoding);
		if (ztd::text::contains_unicode_encoding(encoding)) {
			validate_check(ztd::tests::w_unicode_sequence_truth_native_endian, encoding);
		}
	}
	SECTION("literal") {
		ztd::text::literal_t encoding {};
		auto result0 = ztd::text::validate_decodable_as(ztd::tests::basic_source_character_set, encoding);
		REQUIRE(result0);
		if (ztd::text::contains_unicode_encoding(encoding)) {
			auto result1
			     = ztd::text::validate_decodable_as(ztd::tests::unicode_sequence_truth_native_endian, encoding);
			REQUIRE(result1);
		}
	}
	SECTION("wide_literal") {
		ztd::text::wide_literal_t encoding {};
		auto result0 = ztd::text::validate_decodable_as(ztd::tests::w_basic_source_character_set, encoding);
		REQUIRE(result0);
		if (ztd::text::contains_unicode_encoding(encoding)) {
			auto result1
			     = ztd::text::validate_decodable_as(ztd::tests::w_unicode_sequence_truth_native_endian, encoding);
			REQUIRE(result1);
		}
	}
	SECTION("utf8") {
		ztd::text::utf8_t encoding {};
		validate_check(ztd::tests::u8_basic_source_character_set, encoding);
		validate_check(ztd::tests::u8_unicode_sequence_truth_native_endian, encoding);
	}
	SECTION("utf16") {
		ztd::text::utf16_t encoding {};
		validate_check(ztd::tests::u16_basic_source_character_set, encoding);
		validate_check(ztd::tests::u16_unicode_sequence_truth_native_endian, encoding);
	}
	SECTION("utf32") {
		ztd::text::utf32_t encoding {};
		validate_check(ztd::tests::u32_basic_source_character_set, encoding);
		validate_check(ztd::tests::u32_unicode_sequence_truth_native_endian, encoding);
	}
}
