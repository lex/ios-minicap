#include <iostream>
#include "JpegEncoder.hpp"

JpegEncoder::JpegEncoder(Frame *frame) {
    MagickWandGenesis();
    wand = NewMagickWand();
}

JpegEncoder::~JpegEncoder() {
  // TODO: clean up
}

void JpegEncoder::encode(Frame *frame) {
    const auto width = frame->width;
    const auto height = frame->height;
    const auto data = frame->data;

    MagickConstituteImage(wand, width, height, "BGRA", CharPixel, data);

    MagickSetSize(wand, width, height);
    MagickSetImageFormat(wand, "JPEG");
    MagickSetCompressionQuality(wand, 80);

    size_t length = 0;
    mEncodedData = MagickGetImageBlob(wand, &length);
    mEncodedSize = length;
    // do we need to free memory?
}

unsigned char *JpegEncoder::getEncodedData() {
    return mEncodedData;
}

size_t JpegEncoder::getEncodedSize() {
    return mEncodedSize;
}

unsigned long JpegEncoder::getBufferSize() {
    return mBufferSize;
}
