#ifndef IOS_MINICAP_JPEGENCODER_HPP
#define IOS_MINICAP_JPEGENCODER_HPP


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

private:
    MagickWand* mWand = nullptr;

    unsigned char* mEncodedData;
    size_t mEncodedSize;
};


#endif //IOS_MINICAP_JPEGENCODER_HPP
