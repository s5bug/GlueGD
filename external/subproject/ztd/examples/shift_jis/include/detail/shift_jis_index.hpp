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

#ifndef ZTD_TEXT_EXAMPLES_SHIFT_JIS_SHIFT_JIS_INDEX_HPP
#define ZTD_TEXT_EXAMPLES_SHIFT_JIS_SHIFT_JIS_INDEX_HPP

#include <cstddef>
#include <optional>

namespace detail {
	std::optional<char32_t> shift_jis_index_to_code_point(std::size_t lookup_index_pointer) noexcept;
	std::optional<std::size_t> shift_jis_code_point_to_index(char32_t code) noexcept;
} // namespace detail

#endif // ZTD_TEXT_EXAMPLES_SHIFT_JIS_SHIFT_JIS_INDEX_HPP