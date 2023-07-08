#ifndef PIXEL_H
#define PIXEL_H

#include <stddef.h>
#include <stdint.h>

class Pixel
{
public:
    Pixel();
    virtual ~Pixel();

    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;

    bool operator==(const Pixel& other);
    bool operator!=(const Pixel& other);
    void operator=(const Pixel& other);
};

#endif // PIXEL_H
