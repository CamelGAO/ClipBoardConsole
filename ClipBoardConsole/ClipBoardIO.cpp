#include "stdafx.h"
#include "ClipBoardIO.h"

size_t GetDataFromClipboard(int _dataFormat, void** _pData, size_t* _size)
{
	HGLOBAL hMemory = NULL;
	void* pMemory = NULL;
	size_t size = 0;

	if (_pData == NULL || *_pData == NULL || _size == NULL)
	{
		_tprintf_s(TEXT("���������Ч��"));
		return 0;
	}

	if (!OpenClipboard(NULL))    // �򿪼��а壬�򿪺����������޷�����
	{
		_tprintf_s(TEXT("���а��ʧ��"));
		return 0;
	}

	hMemory = GetClipboardData(_dataFormat);
	if (hMemory == NULL)
	{
		CloseClipboard();
		return 0;
	}

	pMemory = (TCHAR*)GlobalLock(hMemory);
	if (pMemory == NULL)             // ���ڴ���������
	{
		_tprintf_s(TEXT("�����ڴ����!!!"));
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
			_tprintf_s(TEXT("�ڴ�������!!!"));
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

	if (!OpenClipboard(NULL))    // �򿪼��а壬�򿪺����������޷�����
	{
		_tprintf_s(TEXT("���а��ʧ��"));
		return false;
	}

	if (!EmptyClipboard())       // ��ռ��а壬д��֮ǰ����������ռ��а�
	{
		_tprintf_s(TEXT("��ռ��а�ʧ��"));
		CloseClipboard();
		return false;
	}

	SIZE_T writeSize = _length * _elementSize;
	if ((hMemory = GlobalAlloc(GMEM_MOVEABLE, writeSize)) == NULL)    // �Լ��а�����ڴ�
	{
		_tprintf_s(TEXT("�ڴ��������!!!"));
		CloseClipboard();
		return false;
	}

	if (((pMemory = (LPTSTR)GlobalLock(hMemory))) == NULL)             // ���ڴ���������
	{
		_tprintf_s(TEXT("�����ڴ����!!!"));
		CloseClipboard();
		return false;
	}

	memcpy_s(pMemory, writeSize, _pData, writeSize);   // �����ݸ��ƽ����ڴ�����

	GlobalUnlock(pMemory);                   // ����ڴ�����

	if (SetClipboardData(_dataFormat, pMemory) == NULL)
	{
		_tprintf_s(TEXT("���ü��а�����ʧ��!!!"));
		CloseClipboard();
		return false;
	}

	CloseClipboard();

	return true;
}
