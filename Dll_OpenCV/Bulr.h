#pragma once
class ImageObject;
namespace OpenCV {
	extern "C"
	{
		// Bulr 贸府 OpenCV 窃荐
		__declspec(dllexport) bool ImageBlur(const ImageObject* src, ImageObject* dst, const int kernelSize);
		// Bulr 贸府 窃荐 府畔
		__declspec(dllexport) bool return_ImageBlur_OpenCV(const ImageObject* src, ImageObject* dst, const int kernelSize);
	}
}


