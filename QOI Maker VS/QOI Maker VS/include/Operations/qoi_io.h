#ifndef QOI_IO_H_INCLUDED
#define QOI_IO_H_INCLUDED

#include <fstream>
#include "../../include/Data/qoi_chunks.h"

// HEADER
std::fstream& operator<<(std::fstream& out, const QOI_HEADER& qoi_header);
std::fstream& operator>>(std::fstream& in, QOI_HEADER& qoi_header);

// END MARK
std::fstream& operator<<(std::fstream& out, const QOI_END_MARK& qoi_end);
std::fstream& operator>>(std::fstream& in, QOI_END_MARK& qoi_end);

// OP RGB
std::fstream& operator<<(std::fstream& out, const QOI_OP_RGB& qoi_rgb);

// OP RGBA
std::fstream& operator<<(std::fstream& out, const QOI_OP_RGBA& qoi_rgba);

// OP INDEX
std::fstream& operator<<(std::fstream& out, const QOI_OP_INDEX& qoi_index);

// OP DIFF
std::fstream& operator<<(std::fstream& out, const QOI_OP_DIFF& qoi_diff);

// OP LUMA
std::fstream& operator<<(std::fstream& out, const QOI_OP_LUMA& qoi_luma);

// OP RUN
std::fstream& operator<<(std::fstream& out, const QOI_OP_RUN& qoi_run);

#endif // QOI_IO_H_INCLUDED
