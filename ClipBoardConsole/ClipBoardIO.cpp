#include "stdafx.h"
#include "ClipBoardIO.h"

size_t GetDataFromClipboard(int _dataFormat, void** _pData, size_t* _size)
{
	HGLOBAL hMemory = NULL;
	void* pMemory = NULL;
	size_t size = 0;

	if (_pData == NULL || *_pData == NULL || _size == NULL)
	{
		_tprintf_s(TEXT("输入参数无效！"));
		return 0;
	}

	if (!OpenClipboard(NULL))    // 打开剪切板，打开后，其他进程无法访问
	{
		_tprintf_s(TEXT("剪切板打开失败"));
		return 0;
	}

	hMemory = GetClipboardData(_dataFormat);
	if (hMemory == NULL)
	{
		CloseClipboard();
		return 0;
	}

	pMemory = (TCHAR*)GlobalLock(hMemory);
	if (pMemory == NULL)             // 将内存区域锁定
	{
		_tprintf_s(TEXT("锁定内存错误!!!"));
		CloseClipboard();
		return 0;
	}

	size = GlobalSize(hMemory);
	if (size == 0)
	{
		GlobalUnlock(hMemory);
		CloseClipboard();
		return 0;
	}

	if (*_size < size)
	{
		free(*_pData);
		*_pData = (TCHAR*)malloc(size);
		if (!(*_pData))
		{
			_tprintf_s(TEXT("内存分配错误!!!"));
			GlobalUnlock(hMemory);
			CloseClipboard();
			return 0;
		}
		*_size = size;
	}

	memcpy_s(*_pData, *_size, pMemory, size);

	GlobalUnlock(hMemory);
	CloseClipboard();

	return size;
}

bool SetDataToClipboard(void* _pData, int _dataFormat, size_t _length, size_t _elementSize)
{
	HGLOBAL hMemory = NULL;
	void* pMemory = NULL;

	if (!OpenClipboard(NULL))    // 打开剪切板，打开后，其他进程无法访问
	{
		_tprintf_s(TEXT("剪切板打开失败"));
		return false;
	}

	if (!EmptyClipboard())       // 清空剪切板，写入之前，必须先清空剪切板
	{
		_tprintf_s(TEXT("清空剪切板失败"));
		CloseClipboard();
		return false;
	}

	SIZE_T writeSize = _length * _elementSize;
	if ((hMemory = GlobalAlloc(GMEM_MOVEABLE, writeSize)) == NULL)    // 对剪切板分配内存
	{
		_tprintf_s(TEXT("内存申请错误!!!"));
		CloseClipboard();
		return false;
	}

	if (((pMemory = (LPTSTR)GlobalLock(hMemory))) == NULL)             // 将内存区域锁定
	{
		_tprintf_s(TEXT("锁定内存错误!!!"));
		CloseClipboard();
		return false;
	}

	memcpy_s(pMemory, writeSize, _pData, writeSize);   // 将数据复制进入内存区域

	GlobalUnlock(pMemory);                   // 解除内存锁定

	if (SetClipboardData(_dataFormat, pMemory) == NULL)
	{
		_tprintf_s(TEXT("设置剪切板数据失败!!!"));
		CloseClipboard();
		return false;
	}

	CloseClipboard();

	return true;
}
