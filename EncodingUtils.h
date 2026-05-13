#pragma once
#include <string>
#include <windows.h>

inline std::string ConvertToUTF8(const std::string& input) {
    if (input.empty()) return "";

    int wideLen = MultiByteToWideChar(1251, 0, input.c_str(), -1, NULL, 0);
    if (wideLen == 0) {
        return input;
    }

    std::wstring wideStr(wideLen, 0);
    MultiByteToWideChar(1251, 0, input.c_str(), -1, &wideStr[0], wideLen);

    int utf8Len = WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, NULL, 0, NULL, NULL);
    if (utf8Len == 0) return input;

    std::string utf8Str(utf8Len, 0);
    WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, &utf8Str[0], utf8Len, NULL, NULL);

    if (!utf8Str.empty() && utf8Str.back() == '\0') {
        utf8Str.pop_back();
    }

    return utf8Str;
}

inline std::string ConvertFromUTF8(const std::string& input) {
    if (input.empty()) return "";

    int wideLen = MultiByteToWideChar(CP_UTF8, 0, input.c_str(), -1, NULL, 0);
    if (wideLen == 0) return input;

    std::wstring wideStr(wideLen, 0);
    MultiByteToWideChar(CP_UTF8, 0, input.c_str(), -1, &wideStr[0], wideLen);

    int ansiLen = WideCharToMultiByte(1251, 0, wideStr.c_str(), -1, NULL, 0, NULL, NULL);
    if (ansiLen == 0) {
        return input;
    }

    std::string ansiStr(ansiLen, 0);
    WideCharToMultiByte(1251, 0, wideStr.c_str(), -1, &ansiStr[0], ansiLen, NULL, NULL);

    if (!ansiStr.empty() && ansiStr.back() == '\0') {
        ansiStr.pop_back();
    }

    return ansiStr;
}