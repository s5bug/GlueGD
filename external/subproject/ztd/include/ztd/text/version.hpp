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

#ifndef ZTD_TEXT_VERSION_HPP
#define ZTD_TEXT_VERSION_HPP

#include <ztd/version.hpp>

#define ZTD_TEXT_VERSION_MAJOR 0
#define ZTD_TEXT_VERSION_MINOR 0
#define ZTD_TEXT_VERSION_PATCH 0
#define ZTD_TEXT_VERSION_STRING "0.0.0"
#define ZTD_TEXT_VERSION \
	((ZTD_TEXT_VERSION_MAJOR * 100'000) + (ZTD_TEXT_VERSION_MINOR * 100) + (ZTD_TEXT_VERSION_PATCH))

#if ZTD_IS_ON(ZTD_STD_LIBRARY_TEXT_ENCODING_ID_I_)
#include <text_encoding_id>
#endif

// clang-format off

#if defined(ZTD_TEXT_ICONV)
	#if (ZTD_TEXT_ICONV != 0)
		#define ZTD_TEXT_ICONV_I_ ZTD_ON
	#else
		#define ZTD_TEXT_ICONV_I_ ZTD_OFF
	#endif
#else
	#define ZTD_TEXT_ICONV_I_ ZTD_DEFAULT_OFF
#endif

#if defined(ZTD_TEXT_CUNEICODE)
	#if (ZTD_TEXT_CUNEICODE != 0)
		#define ZTD_TEXT_CUNEICODE_I_ ZTD_ON
	#else
		#define ZTD_TEXT_CUNEICODE_I_ ZTD_OFF
	#endif
#else
	#define ZTD_TEXT_CUNEICODE_I_ ZTD_DEFAULT_OFF
#endif

#if defined(ZTD_TEXT_DEFAULT_HANDLER_THROWS)
	#if (ZTD_TEXT_DEFAULT_HANDLER_THROWS != 0)
		#define ZTD_TEXT_DEFAULT_HANDLER_THROWS_I_ ZTD_ON
	#else
		#define ZTD_TEXT_DEFAULT_HANDLER_THROWS_I_ ZTD_OFF
	#endif
#else
	#define ZTD_TEXT_DEFAULT_HANDLER_THROWS_I_ ZTD_DEFAULT_OFF
#endif

#if defined(ZTD_TEXT_ASSUME_VALID_HANDLER_TRAPS_ON_INVOCATION)
	#if (ZTD_TEXT_ASSUME_VALID_HANDLER_TRAPS_ON_INVOCATION != 0)
		#define ZTD_TEXT_ASSUME_VALID_HANDLER_TRAPS_ON_INVOCATION_I_ ZTD_ON
	#else
		#define ZTD_TEXT_ASSUME_VALID_HANDLER_TRAPS_ON_INVOCATION_I_ ZTD_OFF
	#endif
#elif ZTD_IS_ON(ZTD_DEBUG_I_)
	#define ZTD_TEXT_ASSUME_VALID_HANDLER_TRAPS_ON_INVOCATION_I_ ZTD_DEFAULT_ON
#else
	#define ZTD_TEXT_ASSUME_VALID_HANDLER_TRAPS_ON_INVOCATION_I_ ZTD_DEFAULT_OFF
#endif

#if defined(ZTD_TEXT_INTERMEDIATE_DECODE_BUFFER_BYTE_SIZE)
	#define ZTD_TEXT_INTERMEDIATE_DECODE_BUFFER_BYTE_SIZE_I_ ZTD_TEXT_INTERMEDIATE_DECODE_BUFFER_BYTE_SIZE
#else
	#define ZTD_TEXT_INTERMEDIATE_DECODE_BUFFER_BYTE_SIZE_I_ ZTD_INTERMEDIATE_BUFFER_SUGGESTED_BYTE_SIZE_I_
#endif // Intermediate buffer sizing

#define ZTD_TEXT_INTERMEDIATE_DECODE_BUFFER_SIZE_I_(...) (ZTD_TEXT_INTERMEDIATE_DECODE_BUFFER_BYTE_SIZE_I_ / sizeof(__VA_ARGS__))

#if defined(ZTD_TEXT_INTERMEDIATE_ENCODE_BUFFER_BYTE_SIZE)
	#define ZTD_TEXT_INTERMEDIATE_ENCODE_BUFFER_BYTE_SIZE_I_ ZTD_TEXT_INTERMEDIATE_ENCODE_BUFFER_BYTE_SIZE
#else
	#define ZTD_TEXT_INTERMEDIATE_ENCODE_BUFFER_BYTE_SIZE_I_ ZTD_INTERMEDIATE_BUFFER_SUGGESTED_BYTE_SIZE_I_
#endif // Intermediate buffer sizing

#define ZTD_TEXT_INTERMEDIATE_ENCODE_BUFFER_SIZE_I_(...) (ZTD_TEXT_INTERMEDIATE_ENCODE_BUFFER_BYTE_SIZE_I_ / sizeof(__VA_ARGS__))

#if defined(ZTD_TEXT_INTERMEDIATE_TRANSCODE_BUFFER_BYTE_SIZE)
	#define ZTD_TEXT_INTERMEDIATE_TRANSCODE_BUFFER_BYTE_SIZE_I_ ZTD_TEXT_INTERMEDIATE_TRANSCODE_BUFFER_BYTE_SIZE
#else
	#define ZTD_TEXT_INTERMEDIATE_TRANSCODE_BUFFER_BYTE_SIZE_I_ ZTD_INTERMEDIATE_BUFFER_SUGGESTED_BYTE_SIZE_I_
#endif // Intermediate buffer sizing

#define ZTD_TEXT_INTERMEDIATE_TRANSCODE_BUFFER_SIZE_I_(...) (ZTD_TEXT_INTERMEDIATE_TRANSCODE_BUFFER_BYTE_SIZE_I_ / sizeof(__VA_ARGS__))

#if defined(ZTD_TEXT_YES_PLEASE_DESTROY_MY_LITERALS_UTTERLY_I_MEAN_IT)
	#if (ZTD_TEXT_YES_PLEASE_DESTROY_MY_LITERALS_UTTERLY_I_MEAN_IT != 0)
		#define ZTD_TEXT_YES_PLEASE_DESTROY_MY_LITERALS_UTTERLY_I_MEAN_IT_I_ ZTD_ON
	#else
		#define ZTD_TEXT_YES_PLEASE_DESTROY_MY_LITERALS_UTTERLY_I_MEAN_IT_I_ ZTD_OFF
	#endif
#else
	#define ZTD_TEXT_YES_PLEASE_DESTROY_MY_LITERALS_UTTERLY_I_MEAN_IT_I_ ZTD_DEFAULT_OFF
#endif

#if defined(ZTD_TEXT_YES_PLEASE_DESTROY_MY_WIDE_LITERALS_UTTERLY_I_MEAN_IT)
	#if (ZTD_TEXT_YES_PLEASE_DESTROY_MY_WIDE_LITERALS_UTTERLY_I_MEAN_IT != 0)
		#define ZTD_TEXT_YES_PLEASE_DESTROY_MY_WIDE_LITERALS_UTTERLY_I_MEAN_IT_I_ ZTD_ON
	#else
		#define ZTD_TEXT_YES_PLEASE_DESTROY_MY_WIDE_LITERALS_UTTERLY_I_MEAN_IT_I_ ZTD_OFF
	#endif
#else
	#define ZTD_TEXT_YES_PLEASE_DESTROY_MY_WIDE_LITERALS_UTTERLY_I_MEAN_IT_I_ ZTD_DEFAULT_OFF
#endif

#if defined (ZTD_TEXT_UNICODE_CODE_POINT_DISTINCT_TYPE)
	#if (ZTD_TEXT_UNICODE_CODE_POINT_DISTINCT_TYPE != 0)
		#define ZTD_TEXT_UNICODE_CODE_POINT_DISTINCT_TYPE_I_ ZTD_ON
	#else
		#define ZTD_TEXT_UNICODE_CODE_POINT_DISTINCT_TYPE_I_ ZTD_OFF
	#endif
#else
	#define ZTD_TEXT_UNICODE_CODE_POINT_DISTINCT_TYPE_I_ ZTD_DEFAULT_OFF
#endif

#if defined(ZTD_TEXT_UNICODE_CODE_POINT_INVARIANT_ABORT)
	#if (ZTD_TEXT_UNICODE_CODE_POINT_INVARIANT_ABORT != 0)
		#define ZTD_TEXT_UNICODE_CODE_POINT_INVARIANT_ABORT_I_ ZTD_ON
	#else
		#define ZTD_TEXT_UNICODE_CODE_POINT_INVARIANT_ABORT_I_ ZTD_OFF
	#endif
#else
	#define ZTD_TEXT_UNICODE_CODE_POINT_INVARIANT_ABORT_I_ ZTD_OFF
#endif

#if defined (ZTD_TEXT_UNICODE_SCALAR_VALUE_DISTINCT_TYPE)
	#if (ZTD_TEXT_UNICODE_CODE_POINT_DISTINCT_TYPE != 0)
		#define ZTD_TEXT_UNICODE_SCALAR_VALUE_DISTINCT_TYPE_I_ ZTD_ON
	#else
		#define ZTD_TEXT_UNICODE_SCALAR_VALUE_DISTINCT_TYPE_I_ ZTD_OFF
	#endif
#else
	#define ZTD_TEXT_UNICODE_SCALAR_VALUE_DISTINCT_TYPE_I_ ZTD_DEFAULT_ON
#endif

#if defined(ZTD_TEXT_UNICODE_SCALAR_VALUE_INVARIANT_ABORT)
	#if (ZTD_TEXT_UNICODE_SCALAR_VALUE_INVARIANT_ABORT != 0)
		#define ZTD_TEXT_UNICODE_SCALAR_VALUE_INVARIANT_ABORT_I_ ZTD_ON
	#else
		#define ZTD_TEXT_UNICODE_SCALAR_VALUE_INVARIANT_ABORT_I_ ZTD_OFF
	#endif
#else
	#define ZTD_TEXT_UNICODE_SCALAR_VALUE_INVARIANT_ABORT_I_ ZTD_OFF
#endif

#if defined(ZTD_TEXT_ABI_NAMESPACE)
	#define ZTD_TEXT_INLINE_ABI_NAMESPACE_OPEN_I_ inline namespace ZTD_TEXT_ABI_NAMESPACE {
	#define ZTD_TEXT_INLINE_ABI_NAMESPACE_CLOSE_I_ }
#else
	#define ZTD_TEXT_INLINE_ABI_NAMESPACE_OPEN_I_ inline namespace __v0 {
	#define ZTD_TEXT_INLINE_ABI_NAMESPACE_CLOSE_I_ }
#endif

// clang-format on

#define ZTD_TEXT_LOSSY_DECODE_MESSAGE_I_                                                                               \
	"This decode is a lossy, non-injective operation. This means you may lose data that you did not intend to lose; " \
	"specify a 'handler' error handler parameter to decode(in, encoding, handler, ...) or decode_into(in, encoding, " \
	"out, handler, ...) explicitly in order to bypass this."

#define ZTD_TEXT_LOSSY_ENCODE_MESSAGE_I_                                                                               \
	"This encode is a lossy, non-injective operation. This means you may lose data that you did not intend to lose; " \
	"specify a 'handler' error handler parameter to encode(in, encoding, handler, ...) or encode_into(in, encoding, " \
	"out, handler, ...) explicitly in order to bypass this."

#define ZTD_TEXT_LOSSY_TRANSCODE_DECODE_MESSAGE_I_                                                                     \
	"The decode (input) portion of this transcode is a lossy, non-injective operation. This means you may lose data " \
	"that you did not intend to lose; specify an 'in_handler' error handler parameter to transcode[_to](in, "         \
	"in_encoding, out_encoding, in_handler, ...) or transcode_into(in, in_encoding, out, out_encoding, in_handler, "  \
	"...) explicitly in order to bypass this."

#define ZTD_TEXT_LOSSY_TRANSCODE_ENCODE_MESSAGE_I_                                                                     \
	"The encode (output) portion of this transcode is a lossy, non-injective operation. This means you may lose "     \
	"data that you did not intend to lose; specify an 'out_handler' error handler parameter to transcode[_to](in, "   \
	"in_encoding, out_encoding, in_handler, out_handler, ...) or transcode_into(in, in_encoding, out, out_encoding, " \
	"in_handler, out_handler, ...) explicitly in order to bypass this."

#include <ztd/prologue.hpp>
#include <ztd/epilogue.hpp>

#endif // ZTD_TEXT_VERSION_HPP
