#include "../../include/Exceptions/qoi_exceptions.h"

QOIHeaderException::QOIHeaderException(const std::string& message) : Exception(message) {}

QOIHeaderException::~QOIHeaderException() {}

const std::string QOIHeaderException::GetMessage() const {
    return "QOI_HEADER_FILE_ERROR: " + Exception::GetMessage();
}

QOIEndException::QOIEndException(const std::string& message) : Exception(message) {}

QOIEndException::~QOIEndException() {}

const std::string QOIEndException::GetMessage() const {
    return "QOI_END_FILE_ERROR: " + Exception::GetMessage();
}
