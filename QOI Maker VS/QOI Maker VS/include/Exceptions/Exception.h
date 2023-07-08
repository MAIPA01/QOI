#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <string>

class Exception
{
private:
    std::string message;
public:
    Exception(const std::string& message);
    virtual ~Exception();

    virtual const std::string GetMessage() const;
};

#endif // EXCEPTION_H
