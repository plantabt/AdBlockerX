#pragma once
#include <iostream>
#include <cstdlib>  // °üº¬ malloc ºÍ free º¯Êý
#include <windows.h>
using namespace std;
class CDynamicBuffer {
public:
    CDynamicBuffer();

    ~CDynamicBuffer();

    void Resize(size_t newSize);

    size_t Size();
    void Append(const void* src, size_t len);
    int& operator[](size_t index);
    CDynamicBuffer& operator+=(BYTE value);
    BYTE* Buffer();
private:
    void* buffer;
    size_t bufferSize;
};