#ifndef QOI_EXCEPTIONS_H
#define QOI_EXCEPTIONS_H

#include "Exception.h"

class QOIHeaderException : public Exception
{
    public:
        QOIHeaderException(const std::string& message);
        virtual ~QOIHeaderException();
        const std::string GetMessage() const override;
};

class QOIEndException : public Exception
{
    public:
        QOIEndException(const std::string& message);
        virtual ~QOIEndException();
        const std::string GetMessage() const override;
};

#endif // QOI_EXCEPTIONS_H
