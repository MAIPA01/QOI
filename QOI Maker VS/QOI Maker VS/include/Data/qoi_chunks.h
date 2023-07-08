#ifndef QOI_CHUNKS_H_INCLUDED
#define QOI_CHUNKS_H_INCLUDED

#include <stdint.h>
#include <stddef.h>

struct QOI_HEADER {
    char magic[4]; // magic bytes "qoif"
    uint32_t width; // image width in pixels (BE)
    uint32_t height; // image height in pixels (BE)
    uint8_t channels; // 3 = RGB, 4 = RGBA
    uint8_t colorspace; // 0 = sRGB with linear alpha, 1 = all channels linear
};

struct QOI_END_MARK {
    uint64_t end_tag = 0b01;
};

struct QOI_OP_RGB {
    uint8_t tag = 0b11111110;
    uint8_t red; // red channel
    uint8_t green; // green channel
    uint8_t blue; // blue channel
};

struct QOI_OP_RGBA {
    uint8_t tag = 0b11111111;
    uint8_t red; // red channel
    uint8_t green; // green channel
    uint8_t blue; // blue channel
    uint8_t alpha; // alpha channel
};

struct QOI_OP_INDEX {
    uint8_t tag : 2; // tag 0b00
    uint8_t index : 6; // index in color array 0..63
};

struct QOI_OP_DIFF {
    uint8_t tag : 2; // tag 0b01
    int8_t dr : 2; // difference from previous pixel -2..1 (red) (saved with bias -2)
    int8_t dg : 2; // difference from previous pixel -2..1 (green) (saved with bias -2)
    int8_t db : 2; // difference from previous pixel -2..1 (blue) (saved with bias -2)
    // alpha remains unchanged from the previous pixel
};

struct QOI_OP_LUMA {
    uint8_t tag : 2; // tag 0b10
    int8_t dg : 6; // green channel difference from the previous pixel -32..31 (saved with bias -32)
    int8_t dr_dg : 4; // red channel difference minus green channel difference -8..7 (saved with bias -8)
    int8_t db_dg : 4; // blue channel difference minus green channel difference -8..7 (saved with bias -8)
    // alpha remains unchanged from the previous pixel
};

struct QOI_OP_RUN {
    uint8_t tag : 2; // tag 0b11
    uint8_t run : 6; // run-length repeating the previous pixel 1..62 (saved with bias -1)
};

#endif // QOI_CHUNKS_H_INCLUDED
