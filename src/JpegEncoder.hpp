#ifndef IOS_MINICAP_JPEGENCODER_HPP
#define IOS_MINICAP_JPEGENCODER_HPP


#include <cmath>
#include <MagickWand/MagickWand.h>
#include "Frame.hpp"

class JpegEncoder {
public:
    JpegEncoder();
    ~JpegEncoder();

    void encode(Frame* frame);
    unsigned char* getEncodedData() const;
    size_t getEncodedSize() const;
    void cleanUp();
    void setScale(double factor);

private:
    MagickWand* mWand = nullptr;

    unsigned char* mEncodedData;
    size_t mEncodedSize;
    double scale = 1.0f;
};


#endif //IOS_MINICAP_JPEGENCODER_HPP
