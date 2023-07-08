#include "../../include/Exceptions/Exception.h"

Exception::Exception(const std::string& message) : message(message) {}

Exception::~Exception() {}

const std::string Exception::GetMessage() const {
    return message;
}
