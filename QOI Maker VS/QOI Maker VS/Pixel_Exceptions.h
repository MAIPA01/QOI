#ifndef PIXEL_EXCEPTIONS_H
#define PIXEL_EXCEPTIONS_H

#include "include/Exceptions/Exception.h"

class PixelViewerException : public Exception {
public:
	PixelViewerException(const std::string& message);
	virtual ~PixelViewerException();

	const std::string GetMessage() const override;
};

#endif // PIXEL_EXCEPTIONS_H

