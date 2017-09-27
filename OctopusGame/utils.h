#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

class Utils {
public:
	// Splits  a string <txt> everywhere a certain character <ch> is found
	// Stores the resulting substrings in a vector <strs>
	// Returns the size of the vector
	static unsigned int split(const std::string &txt, std::vector<std::string> &strs, char ch);

private:
};

#endif // !UTILS_H
