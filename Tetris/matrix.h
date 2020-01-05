#pragma once

#include <algorithm>
#include <iostream>
#include <vector>

class Matrix2 {
public:
	enum RotateDirection : int {
		CLOCKWISE,
		COUNTER_CLOCKWISE
	};
	
	template <typename T>
	static std::pair<size_t, size_t> getSize(const std::vector<std::vector<T>> &mx);
	template <typename T>
	static void transpose(std::vector<std::vector<T>> &mx);
	template <typename T>
	static void reverseLines(std::vector<std::vector<T>> &mx);
	template <typename T>
	static void reverseColumns(std::vector<std::vector<T>> &mx);
	template <typename T>
	static void rotate(std::vector<std::vector<T>> &mx, RotateDirection dir);
	template <typename T>
	static void print(const std::vector<std::vector<T>> &mx);
};

template <typename T>
std::pair<size_t, size_t> Matrix2::getSize(const std::vector<std::vector<T>> &mx) {
	std::pair<size_t, size_t> ret;
	ret.first = mx[0].size();
	ret.second = mx.size();
	return ret;
}

template <typename T>
void Matrix2::transpose(std::vector<std::vector<T>> &mx) {
	auto [width, height] { getSize(mx) };
	std::vector<std::vector<T>> ret(width, std::vector<T>(height));

	for (size_t i = 0; i < width; i++)
		for (size_t j = 0; j < height; j++)
			ret[i][j] = mx[j][i];

	mx = ret;
}

template <typename T>
void Matrix2::reverseLines(std::vector<std::vector<T>> &mx) {
	auto [width, height] { getSize(mx) };
	for (size_t i = 0; i < height; i++)
		for (size_t j = 0; j < width / 2; j++)
			std::swap(mx[i][j], mx[i][width - j - 1]);
}

template <typename T>
void Matrix2::reverseColumns(std::vector<std::vector<T>> &mx) {
	auto [width, height] { getSize(mx) }; 
	for (size_t j = 0; j < width; j++)
		for (size_t i = 0; i < height / 2; i++)
			std::swap(mx[i][j], mx[height - i - 1][j]);
}

template <typename T>
void Matrix2::rotate(std::vector<std::vector<T>> &mx, RotateDirection dir) {
	switch (dir) {
		case RotateDirection::CLOCKWISE:
			transpose(mx);
			reverseLines(mx);
			break;
		case RotateDirection::COUNTER_CLOCKWISE:
			transpose(mx);
			reverseColumns(mx);
			break;
	}
}

template <typename T>
void Matrix2::print(const std::vector<std::vector<T>> &mx) {
	auto [width, height] { getSize(mx) };
	for (size_t i = 0; i < height; i++) {
		for (size_t j = 0; j < width; j++)
			std::cout << mx[i][j] << ' ';
		std::cout << std::endl;
	}
}
