#ifndef QOI_H_INCLUDED
#define QOI_H_INCLUDED

#include <fstream>
#include <stdint.h>
#include <stddef.h>
#include <string>
#include <vector>
#include "../../include/Data/Pixel.h"
#include "../../typedefs.h"

std::string FileBytes(std::string path);

class QOI{
private:
    std::vector<PixelPtr> pixels = std::vector<PixelPtr>();
    uint32_t width;
    uint32_t height;
    bool hasAlpha;
    char colorspace = 1;

    bool makeLog = false;
public:
    QOI();
    QOI(const std::vector<PixelPtr>& pixels, uint32_t width, uint32_t height, bool hasAlpha);
    virtual ~QOI();
    void setPixels(const std::vector<PixelPtr>& pixels, uint32_t width, uint32_t height, bool hasAlpha, char colorspace);
    std::vector<PixelPtr> getPixels() const;
    uint32_t getWidth() const;
    uint32_t getHeight() const;
    void encode(const std::string& pathToFile) const;
    void decode(const std::string& pathToFile);
    friend std::fstream& operator<<(std::fstream& file, const QOI& qoi);
    friend std::fstream& operator>>(std::fstream& file, QOI& qoi);
};

#endif // QOI_H_INCLUDED
