// ClipBoardConsole.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "ClipBoardIO.h"

volatile BOOL runFlag = TRUE;
const size_t BUFSIZE = 4096;

const TCHAR* title =TEXT(
"\
******************************************************************************\n\
**                              BetterCilpBoard                              *\n\
**                                by  NoCamel                                *\n\
******************************************************************************\n\n\n\
");

BOOL WINAPI HandlerRoutine(DWORD dwCtrlType)
{
	switch (dwCtrlType)
	{
	case CTRL_C_EVENT:
		_tprintf_s(TEXT("Closing...\n"));
		runFlag = FALSE;
		Sleep(10000);
		return TRUE;

	case CTRL_BREAK_EVENT:
		return FALSE;

	case CTRL_CLOSE_EVENT:
		_tprintf_s(TEXT("Closing...\n"));
		runFlag = FALSE;
		Sleep(10000);
		return TRUE;

	case CTRL_LOGOFF_EVENT:
		_tprintf_s(TEXT("Closing...\n"));
		runFlag = FALSE;
		Sleep(10000);
		return TRUE;

	case CTRL_SHUTDOWN_EVENT:
		_tprintf_s(TEXT("Closing...\n"));
		runFlag = FALSE;
		Sleep(10000);
		return TRUE;
	}
	return TRUE;
}

int main(int argc, char * argv[])
{
	SetConsoleCtrlHandler(HandlerRoutine, TRUE);

	TCHAR* pData = NULL;
	TCHAR* pLastData = NULL;
	TCHAR* pBuffer = NULL;

	size_t dataSize = 0;
	size_t maxMemorySize = 0;

	pData = (TCHAR*)calloc(BUFSIZE, sizeof(TCHAR));
	pLastData = (TCHAR*)calloc(BUFSIZE, sizeof(TCHAR));
	pBuffer = (TCHAR*)calloc(BUFSIZE, sizeof(TCHAR));
	if (!pData || !pLastData || !pBuffer)
	{
		_tprintf_s(TEXT("Out of memory!!!"));
		return 1;
	}
	maxMemorySize = dataSize = BUFSIZE * sizeof(TCHAR);

	_tprintf_s(title);

	while (runFlag)
	{
		Sleep(300);

		size_t tempSize = maxMemorySize;
		dataSize = GetDataFromClipboard(CF_TEXT, (void**)&pData, &maxMemorySize);
		if (dataSize == 0)
		{
			continue;
		}
		if (tempSize < dataSize)
		{
			pLastData = (TCHAR*)realloc(pLastData, maxMemorySize);

			free(pBuffer);
			pBuffer = (TCHAR*)malloc(maxMemorySize);

			if (!pLastData || !pBuffer)
			{
				_tprintf_s(TEXT("内存分配错误!!!"));
				return 1;
			}
		}

		if (_tcscmp(pData, pLastData) != 0)
		{
			for (size_t i = 0; i < dataSize / sizeof(TCHAR); ++i)
			{
				TCHAR temp = pData[i];

				if (temp == TEXT('\r'))
				{
					TCHAR c = pData[i + 1];
					if (c == TEXT('\n'))
					{
						pData[i] = TEXT(' ');
						pData[i+1] = TEXT(' ');
						++i;
						continue;
					}
				}

				if (temp == TEXT('\n'))
				{
					pData[i] = TEXT(' ');
					continue;
				}

				if (temp == TEXT('\t'))
				{
					pData[i] = TEXT(' ');
					continue;
				}
			}

			size_t bufferLength = 0;
			for (size_t i = 0; i < dataSize / sizeof(TCHAR); ++i)
			{
				TCHAR temp = pData[i];

				if (temp == TEXT(' '))
				{
					while (pData[i + 1] == TEXT(' '))
					{
						++i;
					}
				}

				pBuffer[bufferLength++] = temp;
			}
			pBuffer[bufferLength] = TEXT('\0');

			SetDataToClipboard(pBuffer, CF_TEXT, bufferLength, sizeof(TCHAR));

			memcpy_s(pLastData, maxMemorySize, pBuffer, bufferLength*sizeof(TCHAR));

			_tprintf_s(TEXT(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>TEXT<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n%s\n\n"), pBuffer);
		}
	}

	free(pData);
	free(pLastData);
	free(pBuffer);

	return 0;
}
