#include "JpegEncoder.hpp"

JpegEncoder::JpegEncoder() {
    MagickWandGenesis();
    mWand = NewMagickWand();
}

JpegEncoder::~JpegEncoder() {
    DestroyMagickWand(mWand);
}

void JpegEncoder::encode(Frame* frame) {
    const auto pixelFormat = "BGRA";
    const auto imageFormat = "JPEG";
    const auto compressionQuality = 80;
    const auto width = frame->width;
    const auto height = frame->height;
    const auto data = frame->data;

    MagickConstituteImage(mWand, width, height, pixelFormat, CharPixel, data);

    MagickSetSize(mWand, width, height);
    MagickSetImageFormat(mWand, imageFormat);
    MagickSetCompressionQuality(mWand, compressionQuality);

    mEncodedData = MagickGetImageBlob(mWand, &mEncodedSize);
}

unsigned char* JpegEncoder::getEncodedData() const {
    return mEncodedData;
}

size_t JpegEncoder::getEncodedSize() const {
    return mEncodedSize;
}

void JpegEncoder::cleanUp() {
    ClearMagickWand(mWand);
    MagickRelinquishMemory(mEncodedData);
}
