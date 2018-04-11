//---------------------------------------------------------------------------

#pragma hdrstop

#include "FSClass.h"
#include <windows.h>
#include <string>
#include <sstream>
#include <iostream>
#include "intsafe.h"

using namespace std;
typedef vector<BYTE> DiskCluster;

#include "Main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

FSClass::FSClass()
{
	FileHandle = 0;
	TotalClusters=0;
	ClusterFactor=1;
	BytesPerCluster=512;
}


DiskCluster FSClass::readClusters(ULONGLONG startCluster, DWORD numberOfClusters, DiskCluster inBuffer)
{
	if(FileHandle == 0)
	{
		//return NULL;
	}
	// Вычисление смещение
	// Позиционирование
	// Чтение
	ULONGLONG startOffset = startCluster*this->BytesPerCluster;
	DWORD bytesToRead = numberOfClusters*this->BytesPerCluster;
	DWORD bytesRead;
    LARGE_INTEGER sectorOffset;
	sectorOffset.QuadPart = startOffset;
	unsigned long currentPosition = SetFilePointer(this->FileHandle,sectorOffset.LowPart,&sectorOffset.HighPart,FILE_BEGIN);
	if(currentPosition != sectorOffset.LowPart)
	{
		//return NULL;
	}
	bool readResult = ReadFile(this->FileHandle,&inBuffer[0],bytesToRead,&bytesRead,NULL);
	if(!readResult || bytesRead != bytesToRead)
	{
		//return NULL;
	}
	return  inBuffer;
}
bool FSClass::open(wstring FileSystemPath)
{
    this->FileHandle = CreateFileW(
			FileSystemPath.c_str(), // Имя файла (WCHAR*)
			GENERIC_READ,	  // Режим доступа
			FILE_SHARE_READ | FILE_SHARE_WRITE, // Режим совместной работы
			NULL, // Атрибуты безопасности
			OPEN_EXISTING, // Способ открытия
			FILE_ATTRIBUTE_NORMAL, // Флаги и атрибуты
			NULL // Описатель (идентификатор) файла шаблона с правами доступа GENERIC_READ.
		);
	if(this->FileHandle == INVALID_HANDLE_VALUE)
	{
		// Обработка ошибки
		Application->MessageBoxW(L"Не удаётся открыть раздел", L"Ошибка", MB_OK);
		return FALSE;
	}
	return true;

}
DWORD FSClass::getBytesPerCluster()
{
	return  BytesPerCluster;
}
DWORD FSClass::getTotalClusters()
{
	return  TotalClusters;
}

void FSClass::close()
{
	CloseHandle(this->FileHandle);
}

