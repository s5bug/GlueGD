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

#ifndef ZTD_RANGES_COUNTED_ITERATOR_HPP
#define ZTD_RANGES_COUNTED_ITERATOR_HPP

#include <ztd/ranges/version.hpp>

#include <ztd/ranges/adl.hpp>
#include <ztd/ranges/default_sentinel.hpp>
#include <ztd/ranges/iterator.hpp>

#include <ztd/idk/type_traits.hpp>

#include <iterator>
#include <utility>

#if ZTD_IS_ON(ZTD_STD_LIBRARY_RANGES_I_)
#include <ranges>
#endif

#include <ztd/prologue.hpp>

namespace ztd { namespace ranges {
	ZTD_RANGES_INLINE_ABI_NAMESPACE_OPEN_I_

	//////
	/// @brief A sentinel for the counted_iterator to be paired with; lets a ztd::text::counted_iterator compare
	/// against this type to test if it is at the end.
	//////
	using counted_sentinel_t = default_sentinel_t;

	namespace __rng_detail {

		template <typename _It>
		class __counted_iterator {
		private:
			using _ItDiff = ranges::iterator_difference_type_t<_It>;

			static constexpr bool _S_operator_plusplus_noexcept() noexcept {
				return noexcept(++::std::declval<_It&>())&& noexcept(--::std::declval<_ItDiff&>());
			}

			static constexpr bool _S_operator_minusminus_noexcept() noexcept {
				return noexcept(--::std::declval<_It&>())&& noexcept(++::std::declval<_ItDiff&>());
			}

		public:
			using iterator_type   = _It;
			using difference_type = _ItDiff;

			constexpr __counted_iterator() = default;
			constexpr __counted_iterator(_It __it, _ItDiff __count) noexcept(
				::std::is_nothrow_move_constructible_v<iterator_type>)
			: _M_it(::std::move(__it)), _M_count(::std::move(__count)) {
			}

			template <typename _It2, ::std::enable_if_t<::std::is_convertible_v<_It2, iterator_type>>* = nullptr>
			constexpr __counted_iterator(const __counted_iterator<_It2>& __from) noexcept(
#if ZTD_IS_ON(ZTD_STD_LIBRARY_IS_NOTHROW_CONVERTIBLE_I_)
				::std::is_nothrow_convertible_v<const _It2&, iterator_type>
#else
				noexcept(static_cast<iterator_type>(::std::declval<const _It2&>()))
#endif
				)
			: _M_count(__from._M_count), _M_it(__from._M_it) {
			}

			template <typename _It2, ::std::enable_if_t<::std::is_convertible_v<_It2, iterator_type>>* = nullptr>
			constexpr __counted_iterator(__counted_iterator<_It2>&& __from) noexcept(
#if ZTD_IS_ON(ZTD_STD_LIBRARY_IS_NOTHROW_CONVERTIBLE_I_)
				::std::is_nothrow_convertible_v<_It2&&, iterator_type>
#else
				noexcept(static_cast<iterator_type>(::std::declval<_It2&&>()))
#endif
				)
			: _M_count(::std::move(__from._M_count)), _M_it(::std::move(__from._M_it)) {
			}

			template <typename _It2, ::std::enable_if_t<::std::is_assignable_v<_It2, iterator_type>>* = nullptr>
			constexpr __counted_iterator& operator=(const __counted_iterator<_It2>& __right) noexcept(
				::std::is_nothrow_assignable_v<const _It2&, iterator_type>) {
				this->_M_count = __right._M_count;
				this->_M_it    = __right._M_it;
				return *this;
			}

			template <typename _It2, ::std::enable_if_t<::std::is_assignable_v<_It2, iterator_type>>* = nullptr>
			constexpr __counted_iterator& operator=(__counted_iterator<_It2>&& __right) noexcept(
				::std::is_nothrow_assignable_v<_It2&&, iterator_type>) {
				this->_M_count = ::std::move(__right._M_count);
				this->_M_it    = ::std::move(__right._M_it);
				return *this;
			}

			constexpr _It base() const& noexcept(::std::is_nothrow_copy_constructible_v<_It>) {
				return this->_M_it;
			}

			constexpr _It base() & noexcept(::std::is_nothrow_copy_constructible_v<_It>) {
				return this->_M_it;
			}

			constexpr _It&& base() && noexcept {
				return ::std::move(this->_M_it);
			}

			constexpr _ItDiff count() const noexcept {
				return this->_M_count;
			}

			constexpr decltype(auto) operator*() noexcept(noexcept(*this->_M_it)) {
				return *this->_M_it;
			}

			constexpr decltype(auto) operator*() const noexcept(noexcept(*this->_M_it)) {
				return *this->_M_it;
			}

			constexpr __counted_iterator& operator++() noexcept(_S_operator_plusplus_noexcept()) {
				++this->_M_it;
				--this->_M_count;
				return *this;
			}

			constexpr __counted_iterator operator++(int) noexcept(_S_operator_plusplus_noexcept()) {
				++(*this);
				return *this;
			}

			constexpr __counted_iterator& operator--() noexcept(_S_operator_minusminus_noexcept()) {
				--this->_M_it;
				++this->_M_count;
				return *this;
			}

			constexpr __counted_iterator operator--(int) noexcept(_S_operator_minusminus_noexcept()) {
				--(*this);
				return *this;
			}

#if 0
			constexpr __counted_iterator operator+(_ItDiff n) const requires random_­access_­iterator<_It>;
			friend constexpr __counted_iterator operator+(
				_ItDiff n, const __counted_iterator& x) requires random_­access_­iterator<_It>;
			constexpr __counted_iterator& operator+=(_ItDiff n) requires random_­access_­iterator<_It>;

			constexpr __counted_iterator operator-(_ItDiff n) const requires random_­access_­iterator<_It>;

			template <typename _ItRight>
			friend constexpr iter_difference_t<_ItRight> operator-(const __counted_iterator& x, const __counted_iterator<_ItRight>& y);
			constexpr __counted_iterator& operator-=(_ItDiff n) requires random_­access_­iterator<_It>;

			constexpr decltype(auto) operator[](_ItDiff n) const requires random_­access_­iterator<_It>;
#endif

			friend constexpr _ItDiff operator-(const __counted_iterator& __left, default_sentinel_t) noexcept {
				return __left._M_count;
			}

			friend constexpr _ItDiff operator-(default_sentinel_t, const __counted_iterator& __right) noexcept {
				return -__right._M_count;
			}

			template <typename _ItRight>
			friend constexpr bool operator==(
				const __counted_iterator& __left, const __counted_iterator<_ItRight>& __right) noexcept {
				return __left._M_it == __right._M_it && __left._M_count == __right._M_count;
			}

			friend constexpr bool operator==(const __counted_iterator& __left, default_sentinel_t) noexcept {
				return __left._M_count == static_cast<_ItDiff>(0);
			}

			template <typename _ItRight>
			friend constexpr bool operator!=(
				const __counted_iterator& __left, const __counted_iterator<_ItRight>& __right) noexcept {
				return __left._M_it != __right._M_it || __left._M_count != __right._M_count;
			}

			friend constexpr bool operator!=(const __counted_iterator& __left, default_sentinel_t) noexcept {
				return __left._M_count != static_cast<_ItDiff>(0);
			}

			friend constexpr ranges::iterator_rvalue_reference_t<_It> iter_move(
				const __counted_iterator& __it) noexcept(noexcept(ranges_adl::adl_iter_move(__it._M_it))) {
				return ranges_adl::adl_iter_move(__it._M_it);
			}

			template <typename _ItRight>
			friend constexpr void
			iter_swap(const __counted_iterator& x, const __counted_iterator<_ItRight>& y) noexcept(
				noexcept(ranges_adl::adl_iter_swap(x._M_it, y._M_it))) {
				ranges_adl::adl_iter_swap(x._M_it, y._M_it);
			}

		private:
			_It _M_it        = _It();
			_ItDiff _M_count = 0;
		};
	} // namespace __rng_detail

	//////
	/// @brief A counted iterator that stores an iterator plus a count, which is used to iterator over the
	/// specified count of elements. Useful for algorithms wherein the iterator is not random access but still
	/// works on a given iterator and a size (e.g., the `std::ranges::copy` algorithm).
	///
	/// @tparam _It The Iterator to wrap. The count is a `difference_type` that is associated with the Iterator.
	/// (The `difference_type` is usually a signed type such as the `ptrdiff_t` type.)
	//////
	template <typename _It>
	using counted_iterator =
#if ZTD_IS_ON(ZTD_STD_LIBRARY_RANGES_I_)
		::std::counted_iterator<_It>;
#else
		__rng_detail::__counted_iterator<_It>;
#endif

	ZTD_RANGES_INLINE_ABI_NAMESPACE_CLOSE_I_
}} // namespace ztd::ranges

namespace std {
	template <typename _It>
	struct iterator_traits<::ztd::ranges::__rng_detail::__counted_iterator<_It>> : iterator_traits<_It> {
		using pointer = void;
	};
} // namespace std

#include <ztd/epilogue.hpp>

#endif // ZTD_RANGES_COUNTED_ITERATOR_HPP
