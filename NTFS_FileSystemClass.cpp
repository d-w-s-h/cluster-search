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
	// ... инициализация
}

bool NTFS_FileSystemClass::ReadClusters(ULONGLONG startCluster, DWORD numberOfClusters, BYTE *outBuffer)
{
	if(FileHandle == 0)
	{
		return false;
	}

	// Вычисление смещение
	// Позиционирование
	// Чтение
}
bool NTFS_FileSystemClass::Open(WCHAR *FileSystemPath)
{
	//WCHAR *FileSystemPath =  L"\\\\.\\E:";
	HANDLE FileSystemHandle = CreateFileW(
			FileSystemPath, // Имя файла (WCHAR*)
			GENERIC_READ,	  // Режим доступа
			FILE_SHARE_READ | FILE_SHARE_WRITE, // Режим совместной работы
			NULL, // Атрибуты безопасности
			OPEN_EXISTING, // Способ открытия
			FILE_ATTRIBUTE_NORMAL, // Флаги и атрибуты
			NULL // Описатель (идентификатор) файла шаблона с правами доступа GENERIC_READ.
		);
	if(FileSystemHandle == INVALID_HANDLE_VALUE)
	{
		// Обработка ошибки
		Application->MessageBoxW(L"Не удаётся открыть раздел", L"Ошибка", MB_OK);
		return FALSE;

	}

	NTFS_BootRecord *pBootRecord;
	// Объявляем буфер для хранения загрузочной записи
	BYTE buffer[1024];
    // Считываем данные в буфер

	// Инициализируем указатель
	//pBootRecord = (ExFAT_BootRecord*)buffer;

	// Работаем с данными
	//int bytesPerSector = (0x01 << pBootRecord->SectorFactor);
	//int sectorsPerCluster = (0x01 << pBootRecord->ClusterFactor);
	//int bytesPerCluster = bytesPerSector*sectorsPerCluster;
	// и т. д.

}
