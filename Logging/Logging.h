#pragma once
#include <string>
using namespace std;

__declspec(dllexport) string readFile(const string & filePath, int i);				// i번째의 줄을 읽어들임
__declspec(dllexport) string returnTime();											// 현재 날짜와 시간을 문자열로 변환
__declspec(dllexport) void writeFile(const string & filePath, const string & log);  // 파일 명과 로그 내용에 대한 정보를 주면 로깅 가능


