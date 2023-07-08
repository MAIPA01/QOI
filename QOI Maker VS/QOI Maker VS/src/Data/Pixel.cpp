#include "../../include/Data/Pixel.h"

Pixel::Pixel() : r(0), g(0), b(0), a(0) {}

Pixel::~Pixel(){}

bool Pixel::operator==(const Pixel& other){
    return this->r == other.r && this->g == other.g && this->b == other.b && this->a == other.a;
}

bool Pixel::operator!=(const Pixel& other){
    return !(*this == other);
}

void Pixel::operator=(const Pixel& other) {
    this->r = other.r;
    this->g = other.g;
    this->b = other.b;
    this->a = other.a;
}
