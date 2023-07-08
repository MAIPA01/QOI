#include "../../include/Model/qoi.h"
#include "../../include/Data/qoi_chunks.h"
#include "../../include/Operations/qoi_io.h"
#include "../../include/Exceptions/qoi_exceptions.h"

std::string CharToBytes(unsigned char character) {
    std::string value = "";
    std::string zeros = "";
    std::string reverse = "";
    while (character != 0) {
        reverse += ((character % 2) + '0');
        character >>= 1;
    }
    for (int i = 0; i < reverse.length(); i++) {
        value += reverse[reverse.length() - i - 1];
    }

    if (value.length() != 8) {
        for (int i = 0; i < 8 - value.length(); i++) {
            zeros += '0';
        }
    }
    return zeros + value;
}

std::string FileBytes(std::string path) {
    std::fstream file(path, std::ios_base::in | std::ios_base::binary);
    std::string value = "";
    char byte = 0;

    while (file.read(&byte, sizeof(char))) {
        value += CharToBytes(byte) + ' ';
    }

    file.close();

    return value;
}

bool check_pixel_diff(const Pixel& prev, const Pixel& curr) {
    if (prev.a != curr.a)
        return false;

    signed int dr = curr.r - prev.r;
    signed int dg = curr.g - prev.g;
    signed int db = curr.b - prev.b;

    return (dr <= 1 && dr >= -2) && (dg <= 1 && dg >= -2) && (db <= 1 && db >= -2);
}

bool check_pixel_luma(const Pixel& prev, const Pixel& curr) {
    if (prev.a != curr.a)
        return false;

    signed int dg = curr.g - prev.g;
    signed int dr_dg = (curr.r - prev.r) - dg;
    signed int db_dg = (curr.b - prev.b) - dg;

    return (dg <= 31 && dg >= -32) && (dr_dg <= 7 && dr_dg >= -8) && (db_dg <= 7 && db_dg >= -8);
}

QOI::QOI() : pixels(std::vector<PixelPtr>()), width(0), height(0), hasAlpha(false) {}

QOI::QOI(const std::vector<PixelPtr>& pixels, uint32_t width, uint32_t height, bool hasAlpha) : pixels(pixels), width(width), height(height), hasAlpha(hasAlpha) {}

QOI::~QOI() {}

void QOI::setPixels(const std::vector<PixelPtr>& pixels, uint32_t width, uint32_t height, bool hasAlpha, char colorspace){
    QOI::pixels.clear();
    QOI::pixels = pixels;
    QOI::width = width;
    QOI::height = height;
    QOI::hasAlpha = hasAlpha;
    QOI::colorspace = colorspace;
}

std::vector<PixelPtr> QOI::getPixels() const
{
    return pixels;
}

uint32_t QOI::getWidth() const
{
    return width;
}

uint32_t QOI::getHeight() const
{
    return height;
}

std::fstream& operator<<(std::fstream& file, const QOI& qoi){
    std::vector<Pixel> savedPixels = std::vector<Pixel>(64);
    PixelPtr cur_px = std::make_shared<Pixel>();
    PixelPtr prev_px = std::make_shared<Pixel>();

    QOI_HEADER header;
    char magic_value[4] = { 'q', 'o', 'i', 'f' };
    for (int i = 0; i < 4; i++) {
        header.magic[i] = magic_value[i];
    }
    header.width = qoi.width;
    header.height = qoi.height;
    header.channels = (qoi.hasAlpha ? 4 : 3);
    header.colorspace = qoi.colorspace;

    file << header;

    unsigned int run_counter = 0;

    for (unsigned int row = 0; row < qoi.height; row++){
        for (unsigned int col = 0; col < qoi.width; col++){
            cur_px = qoi.pixels[col + row * qoi.width];

            // FOR INDEX
            unsigned int index_position = (cur_px->r * 3 + cur_px->g * 5 + cur_px->b * 7 + cur_px->a * 11) % 64;

            // QOI RUN END
            if (run_counter > 0){
                if(*prev_px != *cur_px || run_counter == 62){
                    QOI_OP_RUN qoi_run;
                    qoi_run.tag = 0b11;
                    qoi_run.run = run_counter;
                    file << qoi_run;
                    run_counter = 0;
                }
            }

            // QOI RUN START/CONTINUE
            if (prev_px == cur_px){
                if (run_counter == 0){
                    run_counter = 1;
                }
                else{
                    ++run_counter;
                }

                if (col + row * qoi.width == (qoi.width - 1) + (qoi.height - 1) * qoi.width) {
                    QOI_OP_RUN qoi_run;
                    qoi_run.tag = 0b11;
                    qoi_run.run = run_counter;
                    file << qoi_run;
                    run_counter = 0;
                }
            }

            // QOI INDEX
            else if (savedPixels[index_position] == *cur_px && run_counter == 0) {
                QOI_OP_INDEX qoi_index;
                qoi_index.tag = 0b00;
                qoi_index.index = index_position;
                file << qoi_index;
            }

            // QOI DIFF
            else if (check_pixel_diff(*prev_px, *cur_px) && run_counter == 0){
                QOI_OP_DIFF qoi_diff;
                qoi_diff.tag = 0b01;
                qoi_diff.dr = cur_px->r - prev_px->r;
                qoi_diff.dg = cur_px->g - prev_px->g;
                qoi_diff.db = cur_px->b - prev_px->b;
                file << qoi_diff;
            }

            // QOI LUMA
            else if (check_pixel_luma(*prev_px, *cur_px) && run_counter == 0){
                QOI_OP_LUMA qoi_luma;
                qoi_luma.tag = 0b10;
                qoi_luma.dg = cur_px->g - prev_px->g;
                qoi_luma.dr_dg = (cur_px->r - prev_px->r) - qoi_luma.dg;
                qoi_luma.db_dg = (cur_px->b - prev_px->b) - qoi_luma.dg;
                file << qoi_luma;
            }

            // QOI RGB and RGBA
            else if (run_counter == 0) {
                if (qoi.hasAlpha && cur_px->a != prev_px->a){
                    QOI_OP_RGBA qoi_rgba;
                    qoi_rgba.red = cur_px->r;
                    qoi_rgba.green = cur_px->g;
                    qoi_rgba.blue = cur_px->b;
                    qoi_rgba.alpha = cur_px->a;
                    file << qoi_rgba;
                }
                else{
                    QOI_OP_RGB qoi_rgb;
                    qoi_rgb.red = cur_px->r;
                    qoi_rgb.green = cur_px->g;
                    qoi_rgb.blue = cur_px->b;
                    file << qoi_rgb;
                }
            }

            savedPixels[index_position] = *cur_px;
            prev_px = cur_px;
        }
    }

    QOI_END_MARK qoi_end;
    file << qoi_end;
    return file;
}

void QOI::encode(const std::string& pathToFile) const {
    std::fstream file(pathToFile, std::ios_base::out | std::ios_base::trunc | std::ios_base::binary);
    file << *this;
    file.close();
}

std::fstream& operator>>(std::fstream& file, QOI& qoi){
    std::fstream logFile;

    if (qoi.makeLog)
        logFile.open("./DecodeLog.txt", std::ios_base::out);

    QOI_HEADER qoi_header;
    file >> qoi_header;

    if (qoi_header.magic[0] != 'q' || qoi_header.magic[1] != 'o' || qoi_header.magic[2] != 'i' || qoi_header.magic[3] != 'f'){
        throw QOIHeaderException("qoif not in file header\n");
    }        

    if (qoi_header.channels != 3 && qoi_header.channels != 4) {
        throw QOIHeaderException("not valid channels value which is " + std::to_string(qoi_header.channels) + "\n");
    }
    bool hasAlpha = qoi_header.channels != 3;        

    if (qoi_header.colorspace != 0 && qoi_header.colorspace != 1)
        throw QOIHeaderException("not valid colorspace value which is " + std::to_string(qoi_header.colorspace) + "\n");
    char colorspace = qoi_header.colorspace;

    std::vector<Pixel> savedPixels = std::vector<Pixel>(64);

    uint32_t width = qoi_header.width;
    uint32_t height = qoi_header.height;

    if (qoi.makeLog) {
        logFile << "Header: " << '\n';
        logFile << "Magic: " << qoi_header.magic[0] << qoi_header.magic[1] << qoi_header.magic[2] << qoi_header.magic[3] << '\n';
        logFile << "hasAlpha: " << (char)(qoi_header.channels + '0') << '\n';
        logFile << "colorspace: " << (char)(qoi_header.colorspace + '0') << '\n';
        logFile << "width: " << qoi_header.width << '\n';
        logFile << "height: " << qoi_header.height << "\n\n";
    }

    std::vector<PixelPtr> imagePixels = std::vector<PixelPtr>();
    unsigned int pixel_index = 0;

    PixelPtr prev_pix = std::make_shared<Pixel>();
    prev_pix->a = 255;
    unsigned char run_counter = 0;

    char bajt;
    while(file.read(&bajt, sizeof(char))) {
        unsigned char op_bajt = bajt;
        unsigned char tag = (op_bajt & 0b11000000) >> 6;

        if (qoi.makeLog) {
            logFile << "Readed Byte: " << CharToBytes(bajt) << '\n';
            logFile << "Op: " << CharToBytes(op_bajt) << '\n';
            logFile << "tag: " << CharToBytes(tag) << '\n';
        }

        // OP INDEX
        if (tag == 0){
            QOI_OP_INDEX op_index;
            op_index.index = (op_bajt & 0b00111111);
            imagePixels.push_back(std::make_shared<Pixel>());
            pixel_index = imagePixels.size() - 1;
            *imagePixels[pixel_index] = savedPixels[op_index.index];

            if (qoi.makeLog) {
                logFile << "-> Index (" << (unsigned int)op_index.index << " = " << CharToBytes(op_bajt & 0b00111111) << ")\n";
                logFile << "pixel: r" << (unsigned int)imagePixels[pixel_index]->r << " g" << (unsigned int)imagePixels[pixel_index]->g << " b" << (unsigned int)imagePixels[pixel_index]->b << " a" << (unsigned int)imagePixels[pixel_index]->a << "\n\n";
            }
        }

        // OP DIFF
        else if (tag == 1){
            QOI_OP_DIFF op_diff;
            op_diff.dr = ((op_bajt & 0b00110000) >> 4) - 2;
            op_diff.dg = ((op_bajt & 0b00001100) >> 2) - 2;
            op_diff.db = (op_bajt & 0b00000011) - 2;

            imagePixels.push_back(prev_pix);
            pixel_index = imagePixels.size() - 1;

            PixelPtr newPixel = std::make_shared<Pixel>();
            *newPixel = *imagePixels[pixel_index];

            newPixel->r += op_diff.dr;
            newPixel->g += op_diff.dg;
            newPixel->b += op_diff.db;

            imagePixels[pixel_index] = newPixel;

            unsigned int saved_index = ((unsigned int)newPixel->r * 3 + (unsigned int)newPixel->g * 5 + (unsigned int)newPixel->b * 7 + (unsigned int)newPixel->a * 11) % 64;
            savedPixels[saved_index] = *imagePixels[pixel_index];
            
            if (qoi.makeLog) {
                logFile << "-> Diff r(" << (signed int)op_diff.dr << " = " << CharToBytes((op_bajt & 0b00110000) >> 4) << ") ";
                logFile << "g(" << (signed int)op_diff.dg << " = " << CharToBytes((op_bajt & 0b00001100) >> 2) << ") ";
                logFile << "b(" << (signed int)op_diff.db << " = " << CharToBytes(op_bajt & 0b00000011) << ")\n";
                logFile << "newPixel: r" << (unsigned int)newPixel->r << " g" << (unsigned int)newPixel->g << " b" << (unsigned int)newPixel->b << " a" << (unsigned int)newPixel->a << "\n";
                logFile << "save Index: " << saved_index << " = " << CharToBytes(saved_index) << "\n\n";
            }
        }

        // OP LUMA
        else if (tag == 2){
            QOI_OP_LUMA op_luma;
            op_luma.dg = (op_bajt & 0b00111111) - 32;

            file.read(&bajt, sizeof(char));
            unsigned char bajt_chunk = bajt;

            op_luma.dr_dg = ((bajt_chunk & 0b11110000) >> 4) - 8;
            op_luma.db_dg = (bajt_chunk & 0b00001111) - 8;

            imagePixels.push_back(prev_pix);
            pixel_index = imagePixels.size() - 1;

            PixelPtr newPixel = std::make_shared<Pixel>();
            *newPixel = *imagePixels[pixel_index];

            newPixel->r += op_luma.dr_dg + op_luma.dg;
            newPixel->g += op_luma.dg;
            newPixel->b += op_luma.db_dg + op_luma.dg;

            imagePixels[pixel_index] = newPixel;

            unsigned int saved_index = ((unsigned int)newPixel->r * 3 + (unsigned int)newPixel->g * 5 + (unsigned int)newPixel->b * 7 + (unsigned int)newPixel->a * 11) % 64;
            savedPixels[saved_index] = *imagePixels[pixel_index];

            if (qoi.makeLog) {
                logFile << "-> Luma rg(" << (signed int)op_luma.dr_dg << " = " << CharToBytes((bajt_chunk & 0b11110000) >> 4) << ") ";
                logFile << "g(" << (signed int)op_luma.dg << " = " << CharToBytes(op_bajt & 0b00111111) << ") ";
                logFile << "bg(" << (signed int)op_luma.db_dg << " = " << CharToBytes(bajt_chunk & 0b00001111) << ")\n";
                logFile << "newPixel: r" << (unsigned int)newPixel->r << " g" << (unsigned int)newPixel->g << " b" << (unsigned int)newPixel->b << " a" << (unsigned int)newPixel->a << "\n";
                logFile << "save Index: " << saved_index << " = " << CharToBytes(saved_index) << "\n\n";
            }
        }

        // OP RUN || OP RGB || OP RGBA
        else if (tag == 3){
            run_counter = (op_bajt & 0b00111111);

            // OP RGBA
            if (run_counter == 63){
                uint8_t red;
                uint8_t green;
                uint8_t blue;
                uint8_t alpha;

                file.read(&bajt, sizeof(char));
                red = bajt;
                file.read(&bajt, sizeof(char));
                green = bajt;
                file.read(&bajt, sizeof(char));
                blue = bajt;
                file.read(&bajt, sizeof(char));
                alpha = bajt;

                imagePixels.push_back(std::make_shared<Pixel>());
                pixel_index = imagePixels.size() - 1;

                imagePixels[pixel_index]->r = red;
                imagePixels[pixel_index]->g = green;
                imagePixels[pixel_index]->b = blue;
                imagePixels[pixel_index]->a = alpha;

                unsigned int saved_index = ((unsigned int)red * 3 + (unsigned int)green * 5 + (unsigned int)blue * 7 + (unsigned int)alpha * 11) % 64;
                savedPixels[saved_index] = *imagePixels[pixel_index];

                if (qoi.makeLog) {
                    logFile << "-> RGBA r(" << (unsigned int)red << " = " << CharToBytes(red) << ") ";
                    logFile << "g(" << (unsigned int)green << " = " << CharToBytes(green) << ") ";
                    logFile << "b(" << (unsigned int)blue << " = " << CharToBytes(blue) << ") ";
                    logFile << "a(" << (unsigned int)alpha << " = " << CharToBytes(alpha) << ")\n";
                    logFile << "save Index: " << saved_index << " = " << CharToBytes(saved_index) << "\n\n";
                }
            }

            // OP RGB
            else if (run_counter == 62){
                uint8_t red;
                uint8_t green;
                uint8_t blue;

                file.read(&bajt, sizeof(char));
                red = bajt;
                file.read(&bajt, sizeof(char));
                green = bajt;
                file.read(&bajt, sizeof(char));
                blue = bajt;

                imagePixels.push_back(std::make_shared<Pixel>());
                pixel_index = imagePixels.size() - 1;

                imagePixels[pixel_index]->r = red;
                imagePixels[pixel_index]->g = green;
                imagePixels[pixel_index]->b = blue;
                imagePixels[pixel_index]->a = prev_pix->a;

                unsigned int saved_index = ((unsigned int)red * 3 + (unsigned int)green * 5 + (unsigned int)blue * 7 + (unsigned int)imagePixels[pixel_index]->a * 11) % 64;
                savedPixels[saved_index] = *imagePixels[pixel_index];

                if (qoi.makeLog) {
                    logFile << "-> RGB r(" << (unsigned int)red << " = " << CharToBytes(red) << ") ";
                    logFile << "g(" << (unsigned int)green << " = " << CharToBytes(green) << ") ";
                    logFile << "b(" << (unsigned int)blue << " = " << CharToBytes(blue) << ") ";
                    logFile << "a(" << (unsigned int)imagePixels[pixel_index]->a << ")\n";
                    logFile << "save Index: " << saved_index << " = " << CharToBytes(saved_index) << "\n\n";
                }
            }

            // OP RUN
            else {
                run_counter += 1;
                for(unsigned int i = 0; i < run_counter; i++){
                    imagePixels.push_back(prev_pix);
                    pixel_index = imagePixels.size() - 1;
                    if (imagePixels.size() >= width * height)
                        break;
                }

                if (qoi.makeLog)
                    logFile << "-> Run (" << (unsigned int)run_counter << " = " << CharToBytes(run_counter-1) << ")\n\n";
            }
        }

        if (imagePixels.size() >= width * height)
            break;
        else
            prev_pix = imagePixels[pixel_index];
    }

    QOI_END_MARK qoi_end;
    file >> qoi_end;

    if (qoi_end.end_tag != 1)
        throw QOIEndException("end mark is not valid\n");

    if (qoi.makeLog)
        logFile.close();

    qoi.setPixels(imagePixels, width, height, hasAlpha, colorspace);

    return file;
}

void QOI::decode(const std::string& pathToFile) {
    std::fstream file(pathToFile, std::ios_base::in | std::ios_base::binary);
    try {
        file >> *this;
        file.close();
    }
    catch (...){
        file.close();
        throw;
    }

    /*std::fstream bytesRecord("./DecodeBytesRecord.txt", std::ios_base::out);
    bytesRecord << FileBytes(pathToFile);
    bytesRecord.close();*/
}
