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
	BytesPerSector= 4;
	SectorPerCluster= 128;
	BytesPerCluster=512;

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
	ULONGLONG startOffset = startCluster*BytesPerCluster;
	DWORD bytesToRead =  numberOfClusters*BytesPerCluster;
	DWORD bytesRead;
    LARGE_INTEGER sectorOffset;
	sectorOffset.QuadPart = startOffset;
		// ������ �������
	unsigned long currentPosition = SetFilePointer(
				FileHandle,
				sectorOffset.LowPart,
				&sectorOffset.HighPart,
				FILE_BEGIN // ����� � �����, ������������ ������� ���������� ����������������� (FILE_BEGIN, FILE_CURRENT, FILE_END)
			);

	if(currentPosition != sectorOffset.LowPart)
	{
		// ��������� ������
	}

	// ������ ������
	bool readResult = ReadFile(
				FileHandle,
				outBuffer,
				bytesToRead,
				&bytesRead,
				NULL
			);

	if(!readResult || bytesRead != bytesToRead)
	{
		// ��������� ������
	}

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
	ReadClusters(0,1,buffer);
	// �������������� ���������
	pBootRecord = (NTFS_BootRecord*)buffer;


	// �������� � �������
	//int bytesPerSector = (0x01 << pBootRecord->SectorFactor);
	//int sectorsPerCluster = (0x01 << pBootRecord->ClusterFactor);
	//int bytesPerCluster = bytesPerSector*sectorsPerCluster;
	// � �. �.

	int bytesPerSector = (0x01 << pBootRecord->dBytesPerSector);
	int sectorsPerCluster = (0x01 << pBootRecord->dSectorPerCluster);
	BytesPerCluster = bytesPerSector*sectorsPerCluster;;

}

