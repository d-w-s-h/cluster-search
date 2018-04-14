//---------------------------------------------------------------------------

#pragma hdrstop

#include "FAT_FileSystemClass.h"
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

FAT_FileSystemClass::FAT_FileSystemClass()
{
	FileHandle = 0;
	TotalClusters=0;
	BytesPerCluster=512;
}
string FAT_FileSystemClass::setBootInfo()
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

	this->pBootRecord = (FAT_BootRecord*)dataBuffer;

	if (!strcmp(pBootRecord->OEM_ID,"MSDOS5.0"))
	{
		stringstream DebugInfo;
		if (!memcmp(pBootRecord->FATNAME, "FAT32   ", 5))
		{
			this->BytesPerCluster=pBootRecord->BytePerSector * pBootRecord->SectorsPerCluster  ;

			this->FirstClusterOffset = ((pBootRecord->FAT32size)*(pBootRecord->CountOfFAT)\
			+pBootRecord->Reserved )*pBootRecord->BytePerSector -2*this->BytesPerCluster;  //first cluster #2

			this->TotalClusters= pBootRecord->SectorsPerFS32 / pBootRecord->SectorsPerCluster \
			- (this->FirstClusterOffset)/this->BytesPerCluster - 2;

			DebugInfo << "FileSystem: "<<                 "FAT32" <<  \
						 "\nSectorsPerCluster: " <<       int(pBootRecord->SectorsPerCluster) << \
						 "\nBytesPerSector: " <<          int(pBootRecord->BytePerSector) <<  \
						 "\nBytesPerCluster: " <<         this->BytesPerCluster << \
						 "\nTotalClusters: " <<           this->TotalClusters << \
						 "\nTotalSectors: " <<            int(TotalClusters * pBootRecord->SectorsPerCluster);

		}
		else return "";


		OutputDebugStringA(DebugInfo.str().c_str());
//		MainForm->FSinfoLabel->Caption = DebugInfo.str().c_str();
		return DebugInfo.str();
	}

	return "";
}

Iterator<DiskCluster> * FAT_FileSystemClass::GetClusterIterator()
{
	return new FATClusterIterator<DiskCluster>(this);
}
