#include "pch.h"
#include "Logging.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <iomanip>

#define EXPORTDLL extern "C" __declspec(dllexport)

// 현재 날짜와 시간을 문자열로 변환
string returnTime() {
    time_t timer = time(nullptr);
    struct tm t;
    localtime_s(&t, &timer);
    ostringstream oss;
    oss << put_time(&t, "%Y%m%d_%H%M%S");
    return oss.str();
}

// 파일 명과 로그 내용에 대한 정보를 주면 로깅 가능
void writeFile(const string& filePath, const string& log)
{
    string line = returnTime();
    ofstream file(filePath, ios::app);
    if (file.is_open()) {
        file << line << "_" << log + "\n";
        file.close();
    }
    else
        return;
}

// i번째의 줄을 읽어들임
string readFile(const string& filePath, int i) {
    ifstream file(filePath);
    if (!file.is_open())
        return "";

    string line;
    int currentLine = 0;
    while (getline(file, line)) {
        if (currentLine == i) {
            size_t pos = line.find('=');
            if (pos != string::npos)
                return line.substr(pos + 1);
            else
                return "";
        }
        currentLine++;
    }
    return "";
}


