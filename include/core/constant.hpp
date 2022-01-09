#pragma once
#include <iostream>
#include <string>
constexpr float PI = 3.14159265359f;
constexpr float PI2 = 2.0f * PI;
constexpr float PI4 = 4.0f * PI;
constexpr float PI_DIV2 = 0.5f * PI;
constexpr float PI_DIV4 = 0.25f * PI;
constexpr float PI_INV = 1.0f / PI;
constexpr float PI_INV2 = 1.0f / PI2;
constexpr float PI_INV4 = 1.0f / PI4;

template <class T>
inline void DebugLog(const std::string& s, const T& a) {
    // std::cout << s << " : " << a << std::endl;
}

inline void DebugLog(const std::string& s) {
    // std::cout << s << std::endl;
}