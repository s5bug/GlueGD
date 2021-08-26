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

#include <ztd/text/validate_encodable_as.hpp>
#include <ztd/text/encoding.hpp>

#include <catch2/catch.hpp>

#include <ztd/text/tests/basic_unicode_strings.hpp>

TEST_CASE("text/validate_encodable_as/basic", "basic usages of validate_encodable_as function do not explode") {
	SECTION("execution") {
		ztd::text::execution_t encoding {};
		auto result0 = ztd::text::validate_encodable_as(ztd::tests::u32_basic_source_character_set, encoding);
		REQUIRE(result0);
		if (ztd::text::contains_unicode_encoding(encoding)) {
			auto result1
			     = ztd::text::validate_encodable_as(ztd::tests::u32_unicode_sequence_truth_native_endian, encoding);
			REQUIRE(result1);
		}
	}
	SECTION("wide_execution") {
		ztd::text::wide_execution_t encoding {};
		auto result0 = ztd::text::validate_encodable_as(ztd::tests::u32_basic_source_character_set, encoding);
		REQUIRE(result0);
		if (ztd::text::contains_unicode_encoding(encoding)) {
			auto result1
			     = ztd::text::validate_encodable_as(ztd::tests::u32_unicode_sequence_truth_native_endian, encoding);
			REQUIRE(result1);
		}
	}
	SECTION("literal") {
		ztd::text::literal_t encoding {};
		auto result0 = ztd::text::validate_encodable_as(ztd::tests::u32_basic_source_character_set, encoding);
		REQUIRE(result0);
		if (ztd::text::contains_unicode_encoding(encoding)) {
			auto result1
			     = ztd::text::validate_encodable_as(ztd::tests::u32_unicode_sequence_truth_native_endian, encoding);
			REQUIRE(result1);
		}
	}
	SECTION("wide_literal") {
		ztd::text::wide_literal_t encoding {};
		auto result0 = ztd::text::validate_encodable_as(ztd::tests::u32_basic_source_character_set, encoding);
		REQUIRE(result0);
		if (ztd::text::contains_unicode_encoding(encoding)) {
			auto result1
			     = ztd::text::validate_encodable_as(ztd::tests::u32_unicode_sequence_truth_native_endian, encoding);
			REQUIRE(result1);
		}
	}
	SECTION("utf8") {
		ztd::text::utf8_t encoding {};
		auto result0 = ztd::text::validate_encodable_as(ztd::tests::u32_basic_source_character_set, encoding);
		REQUIRE(result0);
		auto result1
		     = ztd::text::validate_encodable_as(ztd::tests::u32_unicode_sequence_truth_native_endian, encoding);
		REQUIRE(result1);
	}
	SECTION("utf16") {
		ztd::text::utf16_t encoding {};
		auto result0 = ztd::text::validate_encodable_as(ztd::tests::u32_basic_source_character_set, encoding);
		REQUIRE(result0);
		auto result1
		     = ztd::text::validate_encodable_as(ztd::tests::u32_unicode_sequence_truth_native_endian, encoding);
		REQUIRE(result1);
	}
	SECTION("utf32") {
		ztd::text::utf32_t encoding {};
		auto result0 = ztd::text::validate_encodable_as(ztd::tests::u32_basic_source_character_set, encoding);
		REQUIRE(result0);
		auto result1
		     = ztd::text::validate_encodable_as(ztd::tests::u32_unicode_sequence_truth_native_endian, encoding);
		REQUIRE(result1);
	}
}
