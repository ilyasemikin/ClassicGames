#include <fstream>

#ifdef __linux__
#include <arpa/inet.h>
#endif

#include "png_image.h"

ImageSize PNGImage::getSize(const std::string &filename) {
	ImageSize ret;
	std::ifstream input(filename);
	
	input.seekg(16);
	std::uint32_t width, height;
	input.read((char *)&width, 4);
	input.read((char *)&height, 4);

#ifdef __linux__
	ret.width = ntohl(width);
	ret.height = ntohl(height);
#endif

	return ret;
}
