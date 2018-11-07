#ifndef IOS_MINICAP_JPEGENCODER_HPP
#define IOS_MINICAP_JPEGENCODER_HPP


#include <cmath>
#include <numeric>
#include <MagickWand/MagickWand.h>
#include "Frame.hpp"

class JpegEncoder {
public:
    JpegEncoder(Frame* frame);
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
    int gcd = 0;
};


#endif //IOS_MINICAP_JPEGENCODER_HPP
