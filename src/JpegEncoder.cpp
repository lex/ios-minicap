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

    const auto columns = std::round(scale * width);
    const auto rows = std::round(scale * height);

    MagickConstituteImage(mWand, width, height, pixelFormat, CharPixel, data);

    MagickSetSize(mWand, width, height);
    MagickSetImageFormat(mWand, imageFormat);
    MagickSetCompressionQuality(mWand, compressionQuality);
    MagickScaleImage(mWand, columns, rows);

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

void JpegEncoder::setScale(double factor) {
    scale = std::fmax(0.01f, std::fmin(factor, 1.0f));
}
