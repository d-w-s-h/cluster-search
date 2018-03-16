//---------------------------------------------------------------------------

#pragma hdrstop

#include "NTFS_FileSystemClass.h"
#include <windows.h>
#include "Main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

NTFS_FileSystemClass::NTFS_FileSystemClass()
{
	FileHandle = 0;
	TotalClusters=0;
	ClusterFactor=1;
	BytesPerCluster=512;
}
bool NTFS_FileSystemClass::setBootInfo()
{
	ULONGLONG startOffset = 0;
	DWORD bytesToRead = 512;
	DWORD bytesRead;
	BYTE dataBuffer[512];
	LARGE_INTEGER sectorOffset;
	sectorOffset.QuadPart = startOffset;
	unsigned long currentPosition = SetFilePointer(this->FileHandle,sectorOffset.LowPart,&sectorOffset.HighPart,FILE_BEGIN);
	if(currentPosition != sectorOffset.LowPart)
	{
		return false;
	}
	bool readResult = ReadFile(FileHandle,dataBuffer,bytesToRead,&bytesRead,NULL);
	if(!readResult || bytesRead != bytesToRead)
	{
		return false;
	}

	this->pBootRecord = (NTFS_BootRecord*)dataBuffer;
	if (!strcmp(pBootRecord->OEM_ID,"NTFS    "))
	{
		this->BytesPerCluster=pBootRecord->dBytesPerSector*pBootRecord->dSectorPerCluster;
		this->TotalClusters=pBootRecord->dTotalSectors/ pBootRecord->dSectorPerCluster;

		this->DebugInfo = new char[512];
		sprintf(this->DebugInfo,"FileSystem: %s\n"
							"BytesPerSector: %d\n"
							"SectorsPerCluster: %d\n"
							"BytesPerCluster: %d\n"
							"TotalClusters: %d\n"
							"TotalSectors: %d\n"
							,pBootRecord->OEM_ID,
							pBootRecord->dBytesPerSector,
							pBootRecord->dSectorPerCluster,
							this->BytesPerCluster,
							this->TotalClusters,
							pBootRecord->dTotalSectors
							);
		OutputDebugStringA(this->DebugInfo);
		MainForm->FSinfoLabel->Caption = this->DebugInfo;
		return true;
	}

	return false;
}

bool NTFS_FileSystemClass::readClusters(ULONGLONG startCluster, DWORD numberOfClusters, BYTE *outBuffer)
{
	if(FileHandle == 0)
	{
		return false;
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
		return false;
	}
	bool readResult = ReadFile(this->FileHandle,outBuffer,bytesToRead,&bytesRead,NULL);
	if(!readResult || bytesRead != bytesToRead)
	{
		return false;
	}
}
bool NTFS_FileSystemClass::open(WCHAR *FileSystemPath)
{
    this->FileHandle = CreateFileW(
			FileSystemPath, // Имя файла (WCHAR*)
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
DWORD NTFS_FileSystemClass::getBytesPerCluster()
{
	return  BytesPerCluster;
}
DWORD NTFS_FileSystemClass::getTotalClusters()
{
	return  TotalClusters;
}

void NTFS_FileSystemClass::close()
{
	CloseHandle(this->FileHandle);
	delete[] this->DebugInfo;
}

