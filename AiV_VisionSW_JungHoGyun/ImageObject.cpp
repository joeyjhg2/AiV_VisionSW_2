#include "Common.h"
#include "ImageObject.h"

// 5000x5000 랜덤 이미지 생성
imageobject_src::imageobject_src()
{
    width = 5000;
    height = 5000;
    buffer = new unsigned char[width * height];

    // 각 객체마다 랜덤 시드값 생성
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 255);

    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            buffer[i * width + j] = static_cast<unsigned char>(dis(gen));
}

// 픽셀값, 영상 크기 정보를 받아 셋업
imageobject_src::imageobject_src(const Mat& mat)
{
    width = mat.cols;
    height = mat.rows;
    buffer = mat.data;
}

int imageobject_src::getWidth() const {
    return width;
}

int imageobject_src::getHeight() const {
    return height;
}

unsigned char* imageobject_src::getBuffer() const {
    return buffer;
}

void imageobject_src::set_image_object(const vector<uchar>& data, int width, int height)
{
    if (buffer)
        delete[] buffer;

    this->width = width;
    this->height = height;
    buffer = new unsigned char[width * height];
    memcpy(buffer, data.data(), width * height);
}

imageobject_dst::imageobject_dst()
{
    buffer = nullptr;
    width = 0;
    height = 0;
}

int imageobject_dst::getWidth() const {
    return width;
}

int imageobject_dst::getHeight() const {
    return height;
}

unsigned char* imageobject_dst::getBuffer() const {
    return buffer;
}

void imageobject_dst::set_image_object(const vector<uchar>& data, int width, int height)
{
    if (buffer)
        delete[] buffer;

    this->width = width;
    this->height = height;
    buffer = new unsigned char[width * height];
    memcpy(buffer, data.data(), width * height);
}
