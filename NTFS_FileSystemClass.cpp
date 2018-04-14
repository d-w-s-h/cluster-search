//---------------------------------------------------------------------------

#pragma hdrstop

#include "NTFS_FileSystemClass.h"
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

NTFS_FileSystemClass::NTFS_FileSystemClass()
{
	FileHandle = 0;
	TotalClusters=0;
	BytesPerCluster=512;
}
string NTFS_FileSystemClass::setBootInfo()
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
		return "";
	}
	bool readResult = ReadFile(FileHandle,dataBuffer,bytesToRead,&bytesRead,NULL);
	if(!readResult || bytesRead != bytesToRead)
	{
		return "";
	}

	this->pBootRecord = (NTFS_BootRecord*)dataBuffer;

	if (!strcmp(pBootRecord->OEM_ID,"NTFS    "))
	{
		this->BytesPerCluster=pBootRecord->dBytesPerSector*pBootRecord->dSectorPerCluster;
		this->TotalClusters=pBootRecord->dTotalSectors/ pBootRecord->dSectorPerCluster;

		stringstream DebugInfo;
		DebugInfo << "FileSystem: "<<                 pBootRecord->OEM_ID <<  \
					 "\nSectorsPerCluster: " <<       int(pBootRecord->dSectorPerCluster) << \
					 "\nBytesPerSector: " <<          pBootRecord->dBytesPerSector <<  \
					 "\nBytesPerCluster: " <<         this->BytesPerCluster << \
					 "\nTotalClusters: " <<           this->TotalClusters << \
					 "\nTotalSectors: " <<            pBootRecord->dTotalSectors;

		OutputDebugStringA(DebugInfo.str().c_str());
//		MainForm->FSinfoLabel->Caption = DebugInfo.str().c_str();
		return DebugInfo.str();
	}

	return "";
}

Iterator<DiskCluster> * NTFS_FileSystemClass::GetClusterIterator()
{
	return new NTFSClusterIterator<DiskCluster>(this);
}



