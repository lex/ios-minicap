#ifndef IOS_MINICAP_JPEGENCODER_HPP
#define IOS_MINICAP_JPEGENCODER_HPP


#include <MagickWand/MagickWand.h>
#include "Frame.hpp"

class JpegEncoder {
public:
    JpegEncoder(Frame *frame);
    ~JpegEncoder();

    void encode(Frame *frame);
    unsigned char* getEncodedData();
    size_t getEncodedSize();
    unsigned long getBufferSize();

private:
    MagickWand* wand = nullptr;

    unsigned char* mEncodedData;
    size_t mEncodedSize;
    unsigned long mBufferSize;
};


#endif //IOS_MINICAP_JPEGENCODER_HPP
