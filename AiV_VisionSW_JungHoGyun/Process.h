#pragma once
#include <string>
#include <vector>
#include <memory> // 스마트 포인터 사용을 위해 필요

class imageobject_src;
class imageobject_dst;
namespace std 
{
	class mutex;
}

class Process
{
public:
	Process();	// 생성자, 프로세스 시작
	~Process();	// 소멸자, 스레드,메모리 해제

private:
	std::string img_path;				// 이미지 경로
	int kernel;							// 커널 수
	int num_threads;					// 스레드 수
	std::string resultimg_path;			// 결과 이미지 저장 경로
	std::vector<imageobject_src*> src;	// 원본 이미지 객체
	std::unique_ptr<std::mutex> mtx;
	bool initialize();	// 초기화. 프로그램 실행 시 설정 파일 정보 읽기
	void thread_Start();// 스레드 시작

	void Start(int i);																// New_processImage 실행
	bool New_processImage(imageobject_src* src);									// 이미지 셋업
	bool CompareImage(const imageobject_dst& dst1, const imageobject_dst& dst2);	// 이미지 비교
	void SaveImage(const imageobject_dst& dst1, const imageobject_dst& dst2);		// 이미지 저장
};

