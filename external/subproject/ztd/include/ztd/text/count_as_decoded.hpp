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

#pragma once

#ifndef ZTD_TEXT_count_as_encoded_HPP
#define ZTD_TEXT_count_as_encoded_HPP

#include <ztd/text/version.hpp>

#include <ztd/text/code_point.hpp>
#include <ztd/text/default_encoding.hpp>
#include <ztd/text/count_result.hpp>
#include <ztd/text/error_handler.hpp>
#include <ztd/text/state.hpp>
#include <ztd/text/text_tag.hpp>
#include <ztd/text/type_traits.hpp>

#include <ztd/text/detail/is_lossless.hpp>
#include <ztd/text/detail/transcode_one.hpp>
#include <ztd/text/detail/encoding_range.hpp>

#include <ztd/ranges/subrange.hpp>
#include <ztd/ranges/unbounded.hpp>
#include <ztd/idk/span.hpp>

#include <string_view>

#include <ztd/prologue.hpp>

namespace ztd { namespace text {
	ZTD_TEXT_INLINE_ABI_NAMESPACE_OPEN_I_

	//////
	/// @addtogroup ztd_text_count_as_encoded ztd::text::count_as_encoded
	/// @brief These functions use a variety of means to count the number of code points that will result from the
	/// input code units.
	/// @{
	//////

	//////
	/// @brief Counts the number of code units that will result from attempting an encode operation on the input code
	/// points.
	///
	/// @param[in] __input The input range (of code points) to find out how many code units there are.
	/// @param[in] __encoding The encoding to count the input with.
	/// @param[in] __error_handler The error handler to invoke when an encode operation fails.
	/// @param[in,out] __state The state that will be used to count code units.
	///
	/// @returns A ztd::text::count_result that includes information about how many code units are present,
	/// taking into account any invoked errors (like replacement from ztd::text::replacement_handler_t) and a reference
	/// to the provided `__state`.
	///
	/// @remarks This method will not check any ADL extension points. A combination of implementation techniques will
	/// be used to count code units, with a loop over the `.encode` call into an intermediate, unseen buffer being the
	/// most basic choice.
	//////
	template <typename _Input, typename _Encoding, typename _ErrorHandler, typename _State>
	constexpr auto basic_count_as_encoded(
		_Input&& __input, _Encoding&& __encoding, _ErrorHandler&& __error_handler, _State& __state) {
		using _WorkingInput = __txt_detail::__string_view_or_span_or_reconstruct_t<_Input>;
		using _UEncoding    = remove_cvref_t<_Encoding>;
		using _Result       = count_result<_WorkingInput, _State>;

		_WorkingInput __working_input(
			ranges::reconstruct(::std::in_place_type<_WorkingInput>, ::std::forward<_Input>(__input)));

		::std::size_t __code_unit_count = 0;

		if constexpr (is_detected_v<__txt_detail::__detect_adl_text_count_as_encoded_one, _Encoding, _WorkingInput,
			              _ErrorHandler, _State>) {
			for (;;) {
				auto __result = text_count_as_encoded_one(
					text_tag<_UEncoding> {}, ::std::move(__working_input), __encoding, __error_handler, __state);
				if (__result.error_code != encoding_error::ok) {
					return _Result(
						::std::move(__result.input), __code_unit_count, __state, __result.error_code, false);
				}
				__code_unit_count += __result.count;
				__working_input = ::std::move(__result.input);
				if (ranges::ranges_adl::adl_empty(__working_input)) {
					break;
				}
			}
		}
		else if constexpr (is_detected_v<__txt_detail::__detect_adl_internal_text_count_as_encoded_one, _Encoding,
			                   _WorkingInput, _ErrorHandler, _State>) {
			for (;;) {
				auto __result = __text_count_as_encoded_one(
					text_tag<_UEncoding> {}, ::std::move(__working_input), __encoding, __error_handler, __state);
				if (__result.error_code != encoding_error::ok) {
					return _Result(
						::std::move(__result.input), __code_unit_count, __state, __result.error_code, false);
				}
				__code_unit_count += __result.count;
				__working_input = ::std::move(__result.input);
				if (ranges::ranges_adl::adl_empty(__working_input)) {
					break;
				}
			}
		}
		else {
			using _CodeUnit = code_unit_t<_UEncoding>;

			_CodeUnit __intermediate_storage[max_code_units_v<_UEncoding>] {};
			::ztd::span<_CodeUnit, max_code_units_v<_UEncoding>> __intermediate(__intermediate_storage);

			for (;;) {
				auto __result = __txt_detail::__basic_count_as_encoded_one(
					::std::move(__working_input), __encoding, __error_handler, __state, __intermediate);
				if (__result.error_code != encoding_error::ok) {
					return _Result(
						::std::move(__result.input), __code_unit_count, __state, __result.error_code, false);
				}
				__code_unit_count += __result.count;
				__working_input = ::std::move(__result.input);
				if (ranges::ranges_adl::adl_empty(__working_input)) {
					break;
				}
			}
		}
		return _Result(::std::move(__working_input), __code_unit_count, __state, encoding_error::ok, false);
	}

	//////
	/// @brief Counts the number of code units that will result from attempting an encode operation on the input code
	/// points.
	///
	/// @param[in] __input The input range (of code points) to find out how many code units there are.
	/// @param[in] __encoding The encoding to count the input with.
	/// @param[in] __error_handler The error handler to invoke when an encode operation fails.
	/// @param[in,out] __state The state that will be used to count code units.
	///
	/// @returns A ztd::text::count_result that includes information about how many code units are present,
	/// taking into account any invoked errors (like replacement from ztd::text::replacement_handler_t) and a reference
	/// to the provided `__state`.
	///
	/// @remarks This method will first check if an ADL Extension Point `text_count_as_encoded` is callable with
	/// the given arguments. If it is, then that method will be used to do the work after forwarding all four arguments
	/// to that function call. Otherwise, this defers to ztd::text::basic_count_as_encoded.
	//////
	template <typename _Input, typename _Encoding, typename _ErrorHandler, typename _State>
	constexpr auto count_as_encoded(
		_Input&& __input, _Encoding&& __encoding, _ErrorHandler&& __error_handler, _State& __state) {
		if constexpr (is_detected_v<__txt_detail::__detect_adl_text_count_as_encoded, _Input, _Encoding,
			              _ErrorHandler, _State>) {
			return text_count_as_encoded(text_tag<remove_cvref_t<_Encoding>> {}, ::std::forward<_Input>(__input),
				::std::forward<_Encoding>(__encoding), ::std::forward<_ErrorHandler>(__error_handler), __state);
		}
		else if constexpr (is_detected_v<__txt_detail::__detect_adl_internal_text_count_as_encoded, _Input, _Encoding,
			                   _ErrorHandler, _State>) {
			return __text_count_as_encoded(text_tag<remove_cvref_t<_Encoding>> {}, ::std::forward<_Input>(__input),
				::std::forward<_Encoding>(__encoding), ::std::forward<_ErrorHandler>(__error_handler), __state);
		}
		else {
			return basic_count_as_encoded(::std::forward<_Input>(__input), ::std::forward<_Encoding>(__encoding),
				::std::forward<_ErrorHandler>(__error_handler), __state);
		}
	}

	//////
	/// @brief Counts the number of code units that will result from attempting an encode operation on the input code
	/// points.
	///
	/// @param[in] __input The input range (of code points) to find out how many code units there are.
	/// @param[in] __encoding The encoding to count the input with.
	/// @param[in] __error_handler The error handler to invoke when an encode operation fails.
	///
	/// @returns A ztd::text::stateless_count_result that includes information about how many code units are present,
	/// taking into account any invoked errors (like replacement from ztd::text::replacement_handler_t).
	///
	/// @remarks This method will call ztd::text::count_as_encoded(Input, Encoding, ErrorHandler, State) with an
	/// `state` created by ztd::text::make_encode_state(Encoding).
	//////
	template <typename _Input, typename _Encoding, typename _ErrorHandler>
	constexpr auto count_as_encoded(_Input&& __input, _Encoding&& __encoding, _ErrorHandler&& __error_handler) {
		using _UEncoding = remove_cvref_t<_Encoding>;
		using _State     = encode_state_t<_UEncoding>;

		_State __state = make_encode_state(__encoding);

		return count_as_encoded(::std::forward<_Input>(__input), ::std::forward<_Encoding>(__encoding),
			::std::forward<_ErrorHandler>(__error_handler), __state);
	}

	//////
	/// @brief Counts the number of code units that will result from attempting an encode operation.
	///
	/// @param[in] __input The input range (of code points) to find out how many code units there are.
	/// @param[in] __encoding The encoding to count the input with.
	///
	/// @returns A ztd::text::stateless_count_result that includes information about how many code units are present,
	/// taking into account any invoked errors (like replacement from ztd::text::replacement_handler_t).
	///
	/// @remarks This method will call ztd::text::count_as_encoded(Input, Encoding, ErrorHandler) by creating an
	/// `error_handler` similar to ztd::text::default_handler_t.
	//////
	template <typename _Input, typename _Encoding>
	constexpr auto count_as_encoded(_Input&& __input, _Encoding&& __encoding) {
		default_handler_t __handler {};
		return count_as_encoded(::std::forward<_Input>(__input), ::std::forward<_Encoding>(__encoding), __handler);
	}

	//////
	/// @brief Counts the number of code units that will result from attempting an encode operation on the input code
	/// points.
	///
	/// @param[in] __input The input range (of code points) to find out how many code units there are.
	///
	/// @returns A ztd::text::stateless_count_result that includes information about how many code units are present,
	/// taking into account any invoked errors (like replacement from ztd::text::replacement_handler_t).
	///
	/// @remarks Calls ztd::text::count_as_encoded(Input, Encoding) with an `encoding` that is derived from
	/// ztd::text::default_code_unit_encoding.
	//////
	template <typename _Input>
	constexpr auto count_as_encoded(_Input&& __input) {
		using _UInput    = remove_cvref_t<_Input>;
		using _CodePoint = remove_cvref_t<ranges::range_value_type_t<_UInput>>;
#if ZTD_IS_ON(ZTD_STD_LIBRARY_IS_CONSTANT_EVALUATED_I_)
		if (::std::is_constant_evaluated()) {
			// Use literal encoding instead, if we meet the right criteria
			using _Encoding = default_consteval_code_point_encoding_t<_CodePoint>;
			_Encoding __encoding {};
			return count_as_encoded(::std::forward<_Input>(__input), __encoding);
		}
		else
#endif
		{
			using _Encoding = default_consteval_code_point_encoding_t<_CodePoint>;
			_Encoding __encoding {};
			return count_as_encoded(::std::forward<_Input>(__input), __encoding);
		}
	}

	//////
	/// @}

	ZTD_TEXT_INLINE_ABI_NAMESPACE_CLOSE_I_
}} // namespace ztd::text

#include <ztd/epilogue.hpp>

#endif // ZTD_TEXT_count_as_encoded_HPP
