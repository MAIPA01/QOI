#include "Pixel_Exceptions.h"

PixelViewerException::PixelViewerException(const std::string& message) : Exception(message) {}

PixelViewerException::~PixelViewerException() {}

const std::string PixelViewerException::GetMessage() const
{
	return "PIXEL_VIWER_EXCEPTION: " + Exception::GetMessage();
}
