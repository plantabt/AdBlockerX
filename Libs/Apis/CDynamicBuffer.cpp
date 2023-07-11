#include "CDynamicBuffer.h"
CDynamicBuffer::~CDynamicBuffer() {
    if (buffer) {
        free(buffer);
        buffer = nullptr;
        bufferSize = 0;
    }
}
CDynamicBuffer::CDynamicBuffer() : buffer(nullptr), bufferSize(0) {
   
}
void CDynamicBuffer::Append(const void* src, size_t len) {
    size_t newBufferSize = bufferSize + len;
    void* newBuffer = realloc(buffer, newBufferSize);
    if (newBuffer) {
        buffer = newBuffer;
        bufferSize = newBufferSize;
        memcpy(static_cast<BYTE*>(buffer) + bufferSize - len, src, len);
    }
    else {
        throw std::bad_alloc();  // 重新分配内存失败
    }
}
BYTE* CDynamicBuffer::Buffer() {
    return (BYTE*)buffer;
}
CDynamicBuffer& CDynamicBuffer::operator +=(BYTE value) {
    size_t newBufferSize = bufferSize + sizeof(BYTE);
    void* newBuffer = realloc(buffer, newBufferSize);
    if (newBuffer) {
        buffer = newBuffer;
        bufferSize = newBufferSize;
        static_cast<BYTE*>(buffer)[bufferSize / sizeof(BYTE) - 1] = value;
    }
    else {
        throw std::bad_alloc();  // 重新分配内存失败
    }
    return *this;
}
void CDynamicBuffer::Resize(size_t newSize) {
    if (newSize == bufferSize) {
        return;  // 新大小与当前大小相同，无需调整
    }

    void* newBuffer = realloc(buffer, newSize);
    if (newBuffer) {
        buffer = newBuffer;
        bufferSize = newSize;
    }
    else {
        throw std::bad_alloc();  // 重新分配内存失败
    }
}

size_t CDynamicBuffer::Size()  {
    return bufferSize;
}

int& CDynamicBuffer::operator[](size_t index) {
    if (index >= bufferSize) {
        throw std::out_of_range("Index out of range");
    }
    return static_cast<int*>(buffer)[index];
}