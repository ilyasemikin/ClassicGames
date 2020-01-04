#pragma once

#include <string>
#include "image_size.h"

class PNGImage {
public:
	static ImageSize getSize(const std::string &);
};
