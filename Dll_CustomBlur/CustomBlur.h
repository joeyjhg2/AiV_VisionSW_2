#pragma once
class ImageObject;
namespace Custom {
	extern "C"
	{
		// Bulr 贸府 Custom 窃荐
		__declspec(dllexport) bool ImageBlur(const ImageObject* src, ImageObject* dst, const int kernelSize);
		// Bulr 贸府 窃荐 府畔
		__declspec(dllexport) bool return_ImageBlur_Custom(const ImageObject* src, ImageObject* dst, const int kernelSize);
	}
}

