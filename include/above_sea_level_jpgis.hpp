/*#######################################################################################
	Copyright (c) 2020 Kasugaccho
	https://github.com/Kasugaccho/Atl-GIS
	wanotaitei@gmail.com
	Distributed under the Boost Software License, Version 1.0. (See accompanying
	file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#######################################################################################*/
#ifndef INCLUDED_ATL_GIS_ABOVE_SEA_LEVEL_JPGIS_HPP
#define INCLUDED_ATL_GIS_ABOVE_SEA_LEVEL_JPGIS_HPP

# include <using_name.hpp>

#include <fstream>
#include <iostream>
#include <string>

#include <cstddef>
#include <cstdint>

#include <vector>

agis::str high{ u8"gml:high>" };
agis::str tlist{ u8"gml:tupleList>" };

agis::str chi{ u8"地表面," };
agis::str kai{ u8"海水面," };
agis::str nai{ u8"内水面," };
agis::str de{ u8"データなし," };

//float文字列を1000倍してi32型にして返す
agis::i32 getValueStof(const agis::c8* const str_index_) noexcept {
	return static_cast<agis::i32>(std::stof(reinterpret_cast<const char* const>(str_index_)) * 1000.0f);
}

agis::i32 getValue(agis::str& str, agis::size& index) noexcept {
	const agis::c8* const str_start_index{ &str[index] };
	for (; index < str.size(); ++index) {
		if (str[index] >= u8'-' && str[index] != u8'/' && str[index] <= u8'9') continue;
		str[index] = u8'\0';
		return getValueStof(str_start_index);
	}
	return getValueStof(str_start_index);
}

bool getX(agis::str& str, agis::size& index, agis::size& x) {
	for (agis::size i{ index }; i < str.size(); ++i) {
		if (str[i] != u8' ') continue;
		str[i] = 0;
		x = agis::size(std::stoi(reinterpret_cast<const char*>(&str[index]))) + 1;
		index = i + 1;
		return true;
	}
	return false;
}

bool getY(agis::str& str, agis::size& index, agis::size& y) {
	for (agis::size i{ index }; i < str.size(); ++i) {
		if (str[i] != u8'<') continue;
		str[i] = 0;
		y = agis::size(std::stoi(reinterpret_cast<const char*>(&str[index]))) + 1;
		return true;
	}
	return false;
}

int getHigh(agis::str str, agis::size index, agis::size& x, agis::size& y) {
	getX(str, index, x);
	getY(str, index, y);
	return 0;
}

int ku(agis::str& str, agis::size index, bool& is_tlist, agis::size& x, agis::size& y) {
	switch (str[index]) {
	case 0:
	case u8'\n':
	case u8'/':
		return -1;
	default:
		int ii = (str.compare(index, high.size(), high) == 0) ? getHigh(str, index + high.size(), x, y) : 1;
		if (!is_tlist) is_tlist = (str.compare(index, tlist.size(), tlist) == 0);
		return ii;
	}
}

agis::size getValueIndex(agis::str& str, std::vector<agis::i32>& world, agis::size& value_index, bool& is_tlist) {
	agis::size start_index{};
	if (is_tlist && value_index < world.size()) {
		if (str.compare(0, chi.size(), chi) == 0) {
			world[value_index] = getValue(str, (start_index = chi.size()));
			++value_index;
		}
		else if (str.compare(0, kai.size(), kai) == 0) {
			world[value_index] = -999000;
			++value_index;
		}
		else if (str.compare(0, nai.size(), nai) == 0) {
			world[value_index] = -9999000;
			++value_index;
		}
		else if (str.compare(0, de.size(), de) == 0) {
			world[value_index] = -99999000;
			++value_index;
		}
		else is_tlist = false;
	}
	return start_index;
}

bool getLoop(const agis::str& name, std::vector<agis::i32>& world, agis::size& x, agis::size& y) {
	if (name.size() == 0) return false;
	agis::ifstream ifs(name);
	if (ifs.fail()) return false;


	agis::size value_index{};
	agis::str str{};
	bool is_tlist{ false };
	while (std::getline(ifs, str)) {
		for (agis::size i{ getValueIndex(str,world,value_index,is_tlist) }; i < str.size(); ++i) {
			if (str[i] != u8'<') continue;
			is_tlist = false;
			if (ku(str, i + 1, is_tlist, x, y) == 0) world.resize(x * y);
		}
	}
	return true;
}

void outputSpace(agis::i32 world_i) {
	std::cout <<
		((world_i > 50'000) ?
			"\x1b[47m" :
			((world_i > 30'000) ?
				"\x1b[42m" :
				((world_i > 0) ?
					"\x1b[43m" :
					((world_i > -1'000'000) ?
						"\x1b[44m" :
						"\x1b[46m"
						))));
}

void output(agis::i32 world_i) {
	outputSpace(world_i);
	std::cout <<
		((world_i > 70'000) ?
			" " :
			((world_i > 60'000) ?
				"\x1b[32m//" :
				((world_i > 50'000) ?
					"\x1b[32m##" :
					((world_i > 30'000) ?
						"\x1b[42m  " :
						((world_i > 20'000) ?
							"\x1b[32m##" :
							((world_i > 10'000) ?
								"\x1b[32m11" :
								((world_i > 0) ?
									"  " :
									((world_i > -1'000'000) ?
										"  " :
										"  "
										))))))));
	std::cout << "\x1b[49m" << "\x1b[39m";
}

//
//name: 読み込むファイルの文字列
void input(const agis::str& name) {
	std::vector<agis::i32> world{};
	agis::size x{}, y{};
	getLoop(name, world, x, y);

	for (agis::size i{}, j{}; i < world.size(); ++i) {
		output(world[i]);
		if (j >= x - 1) {
			j = 0;
			std::cout << '\n';
		}
		else ++j;
	}
}

#endif //Included Atl Library
