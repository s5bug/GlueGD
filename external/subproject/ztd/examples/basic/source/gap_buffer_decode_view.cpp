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

#include <ztd/text/decode_view.hpp>
#include <ztd/text/encoding.hpp>

#include "gap_buffer.hpp"

#include <algorithm>

int main(int, char*[]) {
	using u16_gap_buffer  = gap::gap_vector<char16_t>;
	using iterator        = typename u16_gap_buffer::iterator;
	using buffer_subrange = ztd::ranges::subrange<iterator, iterator>;

	u16_gap_buffer buffer;
	std::u16string_view data = u"⛲ Très beau !";
	buffer.insert(buffer.begin(), data.begin(), data.end());

	buffer_subrange u16_buffer_view(buffer.begin(), buffer.end());

	ztd::text::decode_view<ztd::text::utf16_t, buffer_subrange> decoded_buffer_view(u16_buffer_view);

	std::u32string_view expected_data = U"⛲ Très beau !";
	ZTD_TEXT_ASSERT(std::equal(expected_data.cbegin(), expected_data.cend(), decoded_buffer_view.begin()));

	return 0;
}
