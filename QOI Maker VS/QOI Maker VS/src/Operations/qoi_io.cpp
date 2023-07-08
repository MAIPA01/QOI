#include "../../include/Operations/qoi_io.h"

// HEADER
std::fstream& operator<<(std::fstream& out, const QOI_HEADER& qoi_header) {
    for(uint8_t i = 0; i < 4; i++){
        out << qoi_header.magic[i];
    }

    for (uint8_t i = 0; i < 4; i++) {
        uint32_t mask = 0b011111111;
        mask <<= (3 - i) * 8;
        uint8_t v = (qoi_header.width & mask) >> ((3-i)*8);
        out << v;
    }

    for (uint8_t i = 0; i < 4; i++) {
        uint32_t mask = 0b011111111;
        mask <<= (3 - i) * 8;
        uint8_t v = (qoi_header.height & mask) >> ((3 - i) * 8);
        out << v;
    }

    out << qoi_header.channels;
    out << qoi_header.colorspace;
    return out;
}

std::fstream& operator>>(std::fstream& in, QOI_HEADER& qoi_header) {

    // Magic
    for(uint8_t i = 0; i < 4; i++){
        in >> qoi_header.magic[i];
    }

    char bajt;

    // Width
    qoi_header.width = 0;
    for (uint8_t i = 0; i < 4; i++) {
        qoi_header.width <<= 8;
        in.read(&bajt, sizeof(char));
        qoi_header.width += (uint8_t)bajt;
    }

    // Height
    qoi_header.height = 0;
    for (uint8_t i = 0; i < 4; i++) {
        qoi_header.height <<= 8;
        in.read(&bajt, sizeof(char));
        qoi_header.height += (uint8_t)bajt;
    }

    // Channels
    in >> bajt;
    qoi_header.channels = bajt;

    // Colorspace
    in >> bajt;
    qoi_header.colorspace = bajt;

    return in;
}

// END MARK
std::fstream& operator<<(std::fstream& out, const QOI_END_MARK& qoi_end){

    for (uint8_t i = 0; i < 8; i++) {
        uint64_t mask = 0b011111111;
        mask <<= (7 - i) * 8;
        uint8_t v = (qoi_end.end_tag & mask) >> ((7 - i) * 8);
        out << v;
    }
    return out;
}

std::fstream& operator>>(std::fstream& in, QOI_END_MARK& qoi_end){
    char bajt;
    qoi_end.end_tag = 0;
    for (uint8_t i = 0; i < 8; i++) {
        qoi_end.end_tag <<= 8;
        in.read(&bajt, sizeof(char));
        qoi_end.end_tag += (uint8_t)bajt;
    }

    return in;
}

// OP RGB
std::fstream& operator<<(std::fstream& out, const QOI_OP_RGB& qoi_rgb){
    out << qoi_rgb.tag;
    out << qoi_rgb.red;
    out << qoi_rgb.green;
    out << qoi_rgb.blue;
    return out;
}

// OP RGBA
std::fstream& operator<<(std::fstream& out, const QOI_OP_RGBA& qoi_rgba){
    out << qoi_rgba.tag;
    out << qoi_rgba.red;
    out << qoi_rgba.green;
    out << qoi_rgba.blue;
    out << qoi_rgba.alpha;
    return out;
}

// OP INDEX
std::fstream& operator<<(std::fstream& out, const QOI_OP_INDEX& qoi_index){
    uint8_t v = ((qoi_index.tag & 0b00000011) << 6) | (qoi_index.index & 0b00111111);
    out << v;
    return out;
}

// OP DIFF
std::fstream& operator<<(std::fstream& out, const QOI_OP_DIFF& qoi_diff) {
    uint8_t v = 0;
    v |= (qoi_diff.tag & 0b00000011) << 6;
    v |= ((qoi_diff.dr + 2) & 0b00000011) << 4;
    v |= ((qoi_diff.dg + 2) & 0b00000011) << 2;
    v |= (qoi_diff.db + 2) & 0b00000011;
    out << v;
    return out;
}

// OP LUMA
std::fstream& operator<<(std::fstream& out, const QOI_OP_LUMA& qoi_luma){
    uint8_t v = 0;
    v |= (qoi_luma.tag & 0b00000011) << 6;
    v |= (qoi_luma.dg + 32) & 0b00111111;
    out << v;

    v = 0;
    v |= ((qoi_luma.dr_dg + 8) & 0b00001111) << 4;
    v |= (qoi_luma.db_dg + 8) & 0b00001111;
    out << v;
    return out;
}

// OP RUN
std::fstream& operator<<(std::fstream& out, const QOI_OP_RUN& qoi_run){
    uint8_t v = ((qoi_run.tag & 0b00000011) << 6) | ((qoi_run.run - 1) & 0b00111111);
    out << v;
    return out;
}
