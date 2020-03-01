/*#######################################################################################
	Copyright (c) 2020 Kasugaccho
	https://github.com/Kasugaccho/Atl-GIS
	wanotaitei@gmail.com
	Distributed under the Boost Software License, Version 1.0. (See accompanying
	file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#######################################################################################*/
#ifndef INCLUDED_ATL_GIS_USING_NAME_HPP
#define INCLUDED_ATL_GIS_USING_NAME_HPP

#include <cstddef>
#include <cstdint>
#include <string>

namespace agis {

	using size = ::std::size_t;
#ifdef __cpp_char8_t
	using c8 = char8_t;
#else
	using c8 = char;
#endif
	using str = ::std::basic_string<::agis::c8>;
	using ifstream = std::basic_ifstream<::agis::c8>;
	using i8 = ::std::int8_t;
	using i16 = ::std::int16_t;
	using i32 = ::std::int32_t;
	using i64 = ::std::int64_t;
	using u8 = ::std::uint8_t;
	using u16 = ::std::uint16_t;
	using u32 = ::std::uint32_t;
	using u64 = ::std::uint64_t;

}

#endif //Included Atl Library
