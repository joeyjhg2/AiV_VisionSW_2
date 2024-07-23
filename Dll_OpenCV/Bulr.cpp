#include "pch.h"
#include "Bulr.h"
#include "ImageObject.h"
#include "Logging.h"

#define EXPORTDLL extern "C" __declspec(dllexport)

namespace OpenCV {
    // Bulr 처리 OpenCV 함수
    bool ImageBlur(const ImageObject* src, ImageObject* dst, const int kernelSize)
    {
        writeFile("image_blur.log", "OpenCV bulr() Start");
        // 커널이 짝수거나 1보다 작다면
        if (kernelSize % 2 == 0 || kernelSize < 1)
            return false;
        // 이미지 데이터 받아오기
        Mat imgMat(src->getHeight(), src->getWidth(), CV_8UC1, src->getBuffer());

        if (imgMat.empty())
            return false;

        Mat blurred;
        // OpenCV blur함수 이용
        blur(imgMat, blurred, Size(kernelSize, kernelSize));

        // dst 객체에 데이터 설정
        vector<uchar> blurredData(blurred.datastart, blurred.dataend);
        dynamic_cast<imageobject_dst*>(dst)->set_image_object(blurredData, blurred.cols, blurred.rows);

        writeFile("image_blur.log", "OpenCV bulr() Complete");

        return true;
    }
    bool return_ImageBlur_OpenCV(const ImageObject* src, ImageObject* dst, const int kernelSize)
    {
        return OpenCV::ImageBlur(src, dst, kernelSize);
    }
}

