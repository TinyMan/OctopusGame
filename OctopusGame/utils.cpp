// utils.cpp
// File handling all the utility functions of the program
// Example : splitting a string

#include "stdafx.h"
#include "utils.h"
#include <algorithm>

unsigned int Utils::split(const std::string &txt, std::vector<std::string> &strs, char ch) {
	int pos = txt.find(ch);
	int initialPos = 0;
	strs.clear();
	while (pos != std::string::npos) {
		strs.push_back(txt.substr(initialPos, pos - initialPos + 1));
		initialPos = pos + 1;

		pos = txt.find(ch, initialPos);
	}

	// Add the last one
	strs.push_back(txt.substr(initialPos, std::min<int>(pos, txt.size() - (initialPos + 1))));

	return strs.size();
}