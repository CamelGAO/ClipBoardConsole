#pragma once
#include "stdafx.h"

size_t GetDataFromClipboard(int _dataFormat, void** _pData, size_t* _size); //_size�ǻ������ĳߴ磬����ֵ�ǻ�����ݵĳ��ȣ�������ݳ��ȴ��ڻ������ߴ磬���������ͷ�ԭ�ڴ沢����������ʵĳߴ�
bool SetDataToClipboard(void* _pData, int _dataFormat, size_t _length, size_t _elementSize);