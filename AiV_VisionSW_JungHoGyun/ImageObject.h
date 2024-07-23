#pragma once

#define interface class

interface ImageObject
{
public:
	virtual ~ImageObject() {};
	virtual int getWidth() const = 0;					// width 반환
	virtual int getHeight() const = 0;					// height 반환
	virtual unsigned char* getBuffer() const = 0;		// buffer 반환
	virtual void set_image_object(const vector<uchar>& data, int width, int height) = 0;	// 픽셀값, 영상 크기 정보를 받아 셋업
};

class imageobject_src : public ImageObject
{
public:
	imageobject_src();									// 5000x5000 랜덤 이미지 생성
	imageobject_src(const Mat& mat);					// 픽셀값, 영상 크기 정보를 받아 셋업				
	virtual int getWidth() const override;				// width 반환
	virtual int getHeight() const override;				// height 반환
	virtual unsigned char* getBuffer() const override;	// buffer 반환
	virtual void set_image_object(const vector<uchar>& data, int width, int height) override;	// 픽셀값, 영상 크기 정보를 받아 셋업
private:
	unsigned char* buffer;	// pixel 값이 포함된 buffer
	int width;				// 영상 크기(width)
	int height;				// 영상 크기(height)
};

class imageobject_dst : public ImageObject
{
public:
	imageobject_dst();
	virtual int getWidth() const override;				// width 반환
	virtual int getHeight() const override;				// height 반환
	virtual unsigned char* getBuffer() const override;	// buffer 반환
	virtual void set_image_object(const vector<uchar>& data, int width, int height) override;	// 픽셀값, 영상 크기 정보를 받아 셋업
private:
	unsigned char* buffer;	// pixel 값이 포함된 buffer
	int width;				// 영상 크기(width)
	int height;				// 영상 크기(height)
};

