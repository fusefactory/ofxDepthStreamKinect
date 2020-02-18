#pragma once

#include <zlib.h>

class ZlibUtils{

public:
	static int compress(char* uncompressedBytes, unsigned int lengthUncompressed, char* compressedByte, int lenghtCompressed);
	static int decompress(char* uncompressedBytes, unsigned int lengthUncompressed, char* compressedByte, int lenghtCompressed);
};

