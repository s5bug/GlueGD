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

#ifndef ZTD_RANGES_DEFAULT_SENTINEL_HPP
#define ZTD_RANGES_DEFAULT_SENTINEL_HPP

#include <ztd/ranges/version.hpp>

#if ZTD_IS_ON(ZTD_STD_LIBRARY_RANGES_I_)
#include <ranges>
#endif

#include <ztd/prologue.hpp>

namespace ztd { namespace ranges {
	ZTD_RANGES_INLINE_ABI_NAMESPACE_OPEN_I_

#if ZTD_IS_ON(ZTD_STD_LIBRARY_RANGES_I_)
	//////
	/// @brief A sentinel that cannot compare equal to any other iterator and thus results in infinitely long ranges.
	using default_sentinel_t = ::std::default_sentinel_t;
#else
	//////
	/// @brief A sentinel that cannot compare equal to any other iterator and thus results in infinitely long ranges.
	struct default_sentinel_t { };
#endif


	//////
	/// @brief An available and usable ztd::text::default_sentinel for ease of use.
	inline constexpr default_sentinel_t default_sentinel = {};

	ZTD_RANGES_INLINE_ABI_NAMESPACE_CLOSE_I_
}} // namespace ztd::ranges

#include <ztd/epilogue.hpp>

#endif // ZTD_RANGES_DEFAULT_SENTINEL_HPP
