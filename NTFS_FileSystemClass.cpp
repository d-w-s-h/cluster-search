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
	// ... �������������
}

bool NTFS_FileSystemClass::ReadClusters(ULONGLONG startCluster, DWORD numberOfClusters, BYTE *outBuffer)
{
	if(FileHandle == 0)
	{
		return false;
	}

	// ���������� ��������
	// ����������������
	// ������
}
bool NTFS_FileSystemClass::Open(WCHAR *FileSystemPath)
{
	//WCHAR *FileSystemPath =  L"\\\\.\\E:";
	HANDLE FileSystemHandle = CreateFileW(
			FileSystemPath, // ��� ����� (WCHAR*)
			GENERIC_READ,	  // ����� �������
			FILE_SHARE_READ | FILE_SHARE_WRITE, // ����� ���������� ������
			NULL, // �������� ������������
			OPEN_EXISTING, // ������ ��������
			FILE_ATTRIBUTE_NORMAL, // ����� � ��������
			NULL // ��������� (�������������) ����� ������� � ������� ������� GENERIC_READ.
		);
	if(FileSystemHandle == INVALID_HANDLE_VALUE)
	{
		// ��������� ������
		Application->MessageBoxW(L"�� ������ ������� ������", L"������", MB_OK);
		return FALSE;

	}

	NTFS_BootRecord *pBootRecord;
	// ��������� ����� ��� �������� ����������� ������
	BYTE buffer[1024];
    // ��������� ������ � �����

	// �������������� ���������
	//pBootRecord = (ExFAT_BootRecord*)buffer;

	// �������� � �������
	//int bytesPerSector = (0x01 << pBootRecord->SectorFactor);
	//int sectorsPerCluster = (0x01 << pBootRecord->ClusterFactor);
	//int bytesPerCluster = bytesPerSector*sectorsPerCluster;
	// � �. �.

}
