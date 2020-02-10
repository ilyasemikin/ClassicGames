#pragma once

#include <vector>

template <typename T>
class Array2d {
private:
	std::vector<T> _array;

	size_t _m;
	size_t _n;
public:
	Array2d(size_t m, size_t n);
	Array2d(size_t m, size_t n, T value);

	inline size_t getM();
	inline size_t getN();

	T &operator()(size_t i, size_t j);
};

template <typename T>
Array2d<T>::Array2d(size_t m, size_t n) : _array(m * n), _m(m), _n(n) {
	
}

template <typename T>
Array2d<T>::Array2d(size_t m, size_t n, T value) : _array(m * n), _m(m), _n(n) {
	for (size_t i = 0; i < m * n; i++)
		_array[i] = value;
}

template <typename T>
size_t Array2d<T>::getM() {
	return _m;
}

template <typename T>
size_t Array2d<T>::getN() {
	return _n;
}

template <typename T>
T &Array2d<T>::operator()(size_t i, size_t j) {
	return _array[_m * i + j];
}
