#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>

#include "passenger.h"

std::vector<std::string> split(
	const std::string& str,
	char delimiter);

int parse_time(
	const std::string& timeStr);

std::vector<int> parse_config(
	const std::string& filename);

std::vector<passenger*> parse_passengers(
	const std::string& filename);

#endif // UTILS_H