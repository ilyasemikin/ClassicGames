struct Point {
	bool isFilled;
	size_t texture;
	Point() : Point(false, 0) {}
	Point(bool _isFilled, size_t _texture) :
		isFilled(_isFilled), 
		texture(_texture)
	{}
};
