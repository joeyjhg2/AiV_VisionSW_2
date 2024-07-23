#include "pch.h"
#include "CustomBlur.h"
#include "ImageObject.h"
#include "Logging.h"

#define EXPORTDLL extern "C" __declspec(dllexport)
namespace Custom {
	// 적분 영상 넒이 구하기
	int Integral_Sum(const vector<vector<int>>& integralImage, int x1, int y1, int x2, int y2) {
		int A = (x1 > 0 && y1 > 0) ? integralImage[x1 - 1][y1 - 1] : 0;
		int B = (x1 > 0) ? integralImage[x1 - 1][y2] : 0;
		int C = (y1 > 0) ? integralImage[x2][y1 - 1] : 0;
		int D = integralImage[x2][y2];
		return D - B - C + A;
	}
	// Bulr 처리 Custom 함수
	bool ImageBlur(const ImageObject* src, ImageObject* dst, const int kernelSize) {

		writeFile("image_blur.log", "Custom bulr() Start");
		// 커널이 짝수거나 1보다 작다면
		if (kernelSize % 2 == 0 || kernelSize < 1)
			return false;

		int height = src->getHeight();
		int width = src->getWidth();
		const unsigned char* srcBuffer = src->getBuffer();

		// 이미지 크기 계산 - default 값인 BORDER_REFLECT_101을 계산하기 위해 (kernel/2)*2 만큼 가로, 세로길이 늘림
		int halfKernel = kernelSize / 2;
		int paddedWidth = width + 2 * halfKernel;
		int paddedHeight = height + 2 * halfKernel;

		// 패딩된 데이터 할당
		vector<vector<unsigned char>> paddedData(paddedHeight, vector<unsigned char>(paddedWidth, 0));


		// 원본 데이터를 중앙에 복사
		for (int i = 0; i < height; ++i)
			for (int j = 0; j < width; ++j)
				paddedData[i + halfKernel][j + halfKernel] = srcBuffer[i * width + j];


		// 경계 처리 - BORDER_REFLECT_101은 경계값이 안쪽에 있는 값과 대칭되도록 해야함
		for (int i = 0; i < halfKernel; ++i)
			for (int j = 0; j < width; ++j)
			{
				paddedData[i][j + halfKernel] = srcBuffer[(halfKernel - i) * width + j];						// 상단 패딩
				paddedData[height + halfKernel + i][j + halfKernel] = srcBuffer[(height - 2 - i) * width + j];  // 하단 패딩
			}


		for (int i = 0; i < height; ++i)
			for (int j = 0; j < halfKernel; ++j)
			{
				paddedData[i + halfKernel][j] = srcBuffer[i * width + (halfKernel - j)];						// 좌측 패딩
				paddedData[i + halfKernel][width + halfKernel + j] = srcBuffer[i * width + (width - 2 - j)];	// 우측 패딩
			}

		for (int i = 0; i < halfKernel; ++i)
			for (int j = 0; j < halfKernel; ++j)
			{
				paddedData[i][j] = srcBuffer[(halfKernel - i) * width + (halfKernel - j)];											// 좌상단 패딩
				paddedData[i][width + halfKernel + j] = srcBuffer[(halfKernel - i) * width + (width - 2 - j)];						// 우상단 패딩
				paddedData[height + halfKernel + i][j] = srcBuffer[(height - 2 - i) * width + (halfKernel - j)];					// 좌하단 패딩
				paddedData[height + halfKernel + i][width + halfKernel + j] = srcBuffer[(height - 2 - i) * width + (width - 2 - j)];// 우하단 패딩
			}


		// 적분 이미지 계산
		vector<vector<int>> integralImage(paddedHeight, vector<int>(paddedWidth, 0));

		for (int i = 0; i < paddedHeight; ++i) 
		{
			int rowSum = 0;
			for (int j = 0; j < paddedWidth; ++j) 
			{
				rowSum += paddedData[i][j];
				integralImage[i][j] = rowSum + (i > 0 ? integralImage[i - 1][j] : 0);
			}
		}
		// 블러 처리된 데이터를 위한 메모리 할당
		unsigned char* dstBuffer = new unsigned char[width * height];

		// 블러 처리
		for (int i = 0; i < height; ++i)
			for (int j = 0; j < width; ++j) 
			{
				int x1 = i;
				int y1 = j;
				int x2 = i + 2 * halfKernel;
				int y2 = j + 2 * halfKernel;

				int sum = Integral_Sum(integralImage, x1, y1, x2, y2);
				int area = (x2 - x1 + 1) * (y2 - y1 + 1);
				dstBuffer[i * width + j] = static_cast<unsigned char>(round(sum / float(area)));
			}
		

		// dst에 데이터 복사
		dst->set_image_object(vector<unsigned char>(dstBuffer, dstBuffer + width * height), width, height);
		delete[] dstBuffer;

		writeFile("image_blur.log", "Custom bulr() Complete");

		return true;
	}

	bool return_ImageBlur_Custom(const ImageObject* src, ImageObject* dst, const int kernelSize)
	{
		return Custom::ImageBlur(src, dst, kernelSize);
	}

}
