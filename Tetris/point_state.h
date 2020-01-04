struct PointState {
	bool isFilled;
	size_t texture;
	PointState(bool _isFilled = false, size_t _texture = 0) :
		isFilled(_isFilled), 
		texture(_texture)
	{}
};
