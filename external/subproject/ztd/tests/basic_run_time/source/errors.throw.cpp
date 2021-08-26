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

#include <ztd/text/encoding.hpp>

#include <catch2/catch.hpp>

#include <ztd/text/tests/basic_unicode_strings.hpp>

inline namespace ztd_text_tests_basic_run_time_errors_throw {
	template <typename Encoding, typename Input>
	void decode_one_throw_check(Encoding& encoding, Input& container) {
		constexpr std::size_t output_max = ztd::text::max_code_points_v<Encoding>;
		using code_point                 = ztd::text::code_point_t<Encoding>;
		using code_unit                  = ztd::text::code_unit_t<Encoding>;
		using state                      = ztd::text::decode_state_t<Encoding>;

		auto action = [&]() noexcept(false) {
			code_point output_buffer[ztd::text::max_code_points_v<Encoding>] {};
			ztd::span<const code_unit> input(container.data(), container.size());
			ztd::span<code_point> output(output_buffer, output_max);
			state s {};
			[[maybe_unused]] auto result = encoding.decode_one(input, output, ztd::text::throw_handler_t {}, s);
			REQUIRE(false); // should not make it here ever!
		};
		REQUIRE_THROWS_MATCHES(action(), std::system_error, Catch::Matchers::Message("invalid_sequence"));
	}

	template <typename Encoding, typename Input>
	void encode_one_throw_check(Encoding& encoding, Input& container) {
		constexpr std::size_t output_max = ztd::text::max_code_units_v<Encoding>;
		using code_point                 = ztd::text::code_point_t<Encoding>;
		using code_unit                  = ztd::text::code_unit_t<Encoding>;
		using state                      = ztd::text::encode_state_t<Encoding>;

		auto action = [&]() noexcept(false) {
			code_unit output_buffer[ztd::text::max_code_units_v<Encoding>] {};
			ztd::span<const code_point> input(container.data(), container.size());
			ztd::span<code_unit> output(output_buffer, output_max);
			state s {};
			[[maybe_unused]] auto result = encoding.encode_one(input, output, ztd::text::throw_handler_t {}, s);
			REQUIRE(false); // should not make it here ever!
		};
		REQUIRE_THROWS_MATCHES(action(), std::system_error, Catch::Matchers::Message("invalid_sequence"));
	}
} // namespace ztd_text_tests_basic_run_time_errors_throw

TEST_CASE("text/encoding/errors", "invalid characters are handled") {
	SECTION("decode") {
		SECTION("utf8") {
			ztd::text::utf8_t encoding;
			decode_one_throw_check(encoding, ztd::tests::u8_unicode_invalid_input);
		}
		SECTION("utf16") {
			ztd::text::utf16_t encoding;
			decode_one_throw_check(encoding, ztd::tests::u16_unicode_invalid_input);
		}
		SECTION("utf32") {
			ztd::text::utf32_t encoding;
			decode_one_throw_check(encoding, ztd::tests::u32_unicode_invalid_input);
		}
	}
	SECTION("encode") {
		SECTION("utf8") {
			ztd::text::utf8_t encoding;
			encode_one_throw_check(encoding, ztd::tests::u32_unicode_invalid_input);
		}
		SECTION("utf16") {
			ztd::text::utf16_t encoding;
			encode_one_throw_check(encoding, ztd::tests::u32_unicode_invalid_input);
		}
		SECTION("utf32") {
			ztd::text::utf32_t encoding;
			encode_one_throw_check(encoding, ztd::tests::u32_unicode_invalid_input);
		}
	}
}
