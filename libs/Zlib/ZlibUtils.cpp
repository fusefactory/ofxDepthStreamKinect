#include "ZlibUtils.h"

#include <stddef.h>
#include <stdlib.h>     /* malloc, calloc, realloc, free */
#include "ofMain.h"

int ZlibUtils::compress(char* uncompressedBytes, unsigned int lengthUncompressed, char* compressedByte, int lenghtCompressed) {
	if (uncompressedBytes == NULL) return -1;	//TODO: return NULL?

	z_stream defstream;
	defstream.zalloc = Z_NULL;
	defstream.zfree = Z_NULL;
	defstream.opaque = Z_NULL;
	defstream.avail_in = lengthUncompressed; // size of input
	defstream.avail_out = lenghtCompressed;
	defstream.next_in = (Bytef*)(uncompressedBytes);
	defstream.next_out = (Bytef*)compressedByte; // output char arra

	//source https://gist.github.com/arq5x/5315739#file-zlib-example-cpp-L35
	// the actual compression work.
	if (deflateInit(&defstream, Z_BEST_SPEED) != Z_OK) {

		ofLog(OF_LOG_ERROR) << "Compressor::deflateInit(&defstream, Z_BEST_SPEED) != Z_OK ";

		free(uncompressedBytes);
		return -1;
	}

	int deflateResult = deflate(&defstream, Z_FINISH);
	deflateEnd(&defstream);

	if (deflateResult != Z_STREAM_END) {
		ofLog(OF_LOG_ERROR) << "Compressor::couldn't finish deflation";

		return -1;
	}

	return defstream.total_out;
}

int ZlibUtils::decompress(char* uncompressedBytes, unsigned int lengthUncompressed, char* compressedByte, int lenghtCompressed) {

	z_stream strm;
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	strm.avail_in = lenghtCompressed;
	strm.next_in = (Bytef*)(compressedByte);
	strm.next_out = (Bytef*)(uncompressedBytes);
	strm.avail_out = lengthUncompressed;

	if (inflateInit(&strm) != Z_OK) {
		ofLog(OF_LOG_ERROR) << "Compressor::inflateInit(&defstream, Z_BEST_SPEED) != Z_OK ";

		return -1;
	}

	int inflateResult = inflate(&strm, Z_FINISH);
	if (inflateEnd(&strm) != Z_OK) {
		ofLog(OF_LOG_ERROR) << "Compressor::couldn't finish inflate";
		return -1;
	}

	return strm.total_out;
}