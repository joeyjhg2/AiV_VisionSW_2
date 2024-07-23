#pragma once
#include "Common.h"
#include "Process.h"
#include "ImageObject.h"
#include "Logging.h"
#include "Bulr.h"
#include "CustomBlur.h"

Process::Process()
{
    if (!initialize())
        return;
    thread_Start();
}

Process::~Process()
{
    for (auto& s : src)
        delete s;
}



//i번째의 줄을 읽어들임
bool Process::initialize()
{
    kernel = 21;
    num_threads = 3;
    resultimg_path = readFile("setting.txt", 2);
    mtx = make_unique<mutex>(); // 생성자에서 mutex 객체 생성
    if (resultimg_path == "")
        return false;
    src.resize(num_threads);  // src 벡터 크기 초기화
    return true;
}

// 스레드 시작
void Process::thread_Start()
{
    vector<thread> threads;			// 스레드 벡터
    for (int i = 0; i < num_threads; i++)
        threads.push_back(thread(&Process::Start, this, i));    // i+1 번째 스레드 시작

    for (auto& thread : threads)
        if (thread.joinable())
            thread.join();   // 스레드 해제
}

// New_processImage 실행
void Process::Start(int i)
{
    {
        lock_guard<mutex> lock(*mtx);    // 스레드 동기화
        writeFile("image_blur.log", to_string(i + 1) + " Thread Start");    // log
        cout << i + 1 << "번째 스레드 시작" << endl;
    }

    src[i] = new imageobject_src(); // vector<imageobject_src*> src 초기화
    cout << i + 1 << "번째 랜덤 이미지 생성 완료" << endl;

    // New_processImage 실행
    if (!New_processImage(src[i])) 
    {
        lock_guard<mutex> lock(*mtx);
        cout << i + 1 << "번째 스레드 비정상 종료" << endl;
        delete src[i];
        return;
    }
    delete src[i];
    {
        lock_guard<mutex> lock(*mtx);
        writeFile("image_blur.log", to_string(i + 1) + " Thread Complete");
        cout << i + 1 << "번째 스레드 종료" << endl;
    }
}

// 이미지 셋업
bool Process::New_processImage(imageobject_src* src)
{
    // dst1,2 생성
    imageobject_dst dst1;
    imageobject_dst dst2;

    // OpenCV_bulr와 Custom_blur dst에 저장
    bool successOpenCV = OpenCV::return_ImageBlur_OpenCV(src, &dst1, kernel);
    bool successCustom = Custom::return_ImageBlur_Custom(src, &dst2, kernel);
    if (!successOpenCV || !successCustom) {
        return false;
    }
    if (!CompareImage(dst1, dst2))
        return false;

    return true;
}

// 이미지 비교
bool Process::CompareImage(const imageobject_dst& dst1, const imageobject_dst& dst2)
{
    cout << "비교 시작" << endl;
    // 각 픽셀값을 비교할 unsigned char*형 변수 선언후 할당
    const unsigned char* img_OpenCV = dst1.getBuffer();
    const unsigned char* img_Custom = dst2.getBuffer();

    int width = dst1.getWidth();
    int height = dst1.getHeight();

    // 이때 unsigned char*형은 1차원 배열이므로 [n][0]은 [n-1][0]보다 height만큼 떨어져있음.
    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            int idx = i * width + j;    // idx값을 i*height + j로 설정하여 차례로 비교
            if (img_OpenCV[idx] != img_Custom[idx]) 
            {
                lock_guard<mutex> lock(*mtx);
                writeFile("image_blur.log", "The gray values of the image pixels are different.");    // log
                cout << "이미지 다름" <<i<<" "<<j<< endl;
                return false;
            }
        }
    }

    {
        lock_guard<mutex> lock(*mtx);
        writeFile("image_blur.log", "The gray values of the image pixels are identical.");    // log
        cout << "이미지 같음" << endl;
    }
    // 이미지 저장
    SaveImage(dst1, dst2);
    delete img_OpenCV;
    delete img_Custom;
    return true;
}

// 이미지 저장
void Process::SaveImage(const imageobject_dst& dst1, const imageobject_dst& dst2)
{
    // imageobject에서 Mat과 호환되는 매개변수들 리턴
    Mat blurredImg_OpenCV(dst1.getHeight(), dst1.getWidth(), CV_8UC1, const_cast<uchar*>(dst1.getBuffer()));
    Mat blurredImg_Custom(dst2.getHeight(), dst2.getWidth(), CV_8UC1, const_cast<uchar*>(dst2.getBuffer()));

    string time_str = returnTime();
    bool saved = false;
    int i = 1;
    // 고유한 파일 이름 생성
    while (!saved) 
    {
        string resultimg_path_OpenCV = "." + resultimg_path + "/blurred_OpenCV_" + time_str + "_" + to_string(i) + ".png";
        string resultimg_path_Custom = "." + resultimg_path + "/blurred_Custom_" + time_str + "_" + to_string(i) + ".png";

        {
            // 파일 존재 여부 확인
            if (!ifstream(resultimg_path_OpenCV) && !ifstream(resultimg_path_Custom)) 
            {
                // 파일 저장 시 잠금 사용
                lock_guard<mutex> lock(*mtx);
                if (!ifstream(resultimg_path_OpenCV) && !ifstream(resultimg_path_Custom)) 
                {
                    if (imwrite(resultimg_path_OpenCV, blurredImg_OpenCV) && imwrite(resultimg_path_Custom, blurredImg_Custom)) 
                    {
                        cout << "Save OK" << endl;
                        writeFile("image_blur.log", "Save Image_" + resultimg_path_OpenCV); // log
                        writeFile("image_blur.log", "Save Image_" + resultimg_path_Custom); // log
                        saved = true;
                    }
                    else
                        cout << "Save Fail" << endl;
                }
            }
        }

        if (!saved)
            i++;
        
    }
}