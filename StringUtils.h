#pragma once

#include <algorithm> 
#include <cctype>
#include <locale>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// trim from start (in place)
inline void trimLeft(std::string& s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
		return !std::isspace(ch);
		}));
}

// trim from end (in place)
inline void trimRight(std::string& s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
		return !std::isspace(ch);
		}).base(), s.end());
}

std::vector<std::string> split(const std::string& str, char delimiter) {
	std::istringstream iss(str);
	std::vector<std::string> tokens;
	std::string token;
	while (std::getline(iss, token, delimiter)) {
		tokens.push_back(token);
	}

	return tokens;
}