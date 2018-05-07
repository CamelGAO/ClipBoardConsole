#pragma once
#include "stdafx.h"

size_t GetDataFromClipboard(int _dataFormat, void** _pData, size_t* _size); //_size是缓冲区的尺寸，返回值是获得数据的长度；如果数据长度大于缓冲区尺寸，缓冲区会释放原内存并重新申请合适的尺寸
bool SetDataToClipboard(void* _pData, int _dataFormat, size_t _length, size_t _elementSize);