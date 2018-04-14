//---------------------------------------------------------------------------

#pragma hdrstop

#include "exFAT_FileSystemClass.h"
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

exFAT_FileSystemClass::exFAT_FileSystemClass()
{
	FileHandle = 0;
	TotalClusters=0;
	BytesPerCluster=512;
}
string exFAT_FileSystemClass::setBootInfo()
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

	this->pBootRecord = (exFAT_BootRecord*)dataBuffer;

	if (!strcmp(pBootRecord->OEM_ID,"EXFAT   "))
	{
		this->BytesPerCluster=(2 << (pBootRecord->SectorPOW-1)) * (2 << (pBootRecord->ClusterPOW-1) );
		this->TotalClusters= pBootRecord->dTotalClusters;
		this->FirstClusterOffset = 512 * pBootRecord->StartBitmap -2 * this->BytesPerCluster;

		stringstream DebugInfo;
		DebugInfo << "FileSystem: "<<                 pBootRecord->OEM_ID <<  \
					 "\nSectorsPerCluster: " <<       int(2 << (pBootRecord->ClusterPOW-1)) << \
					 "\nBytesPerSector: " <<          int(2 << (pBootRecord->SectorPOW-1)) <<  \
					 "\nBytesPerCluster: " <<         this->BytesPerCluster << \
					 "\nTotalClusters: " <<           this->TotalClusters << \
					 "\nTotalSectors: " <<            pBootRecord->dTotalSectors;

		OutputDebugStringA(DebugInfo.str().c_str());
//		MainForm->FSinfoLabel->Caption = DebugInfo.str().c_str();
		return DebugInfo.str();
	}

	return "";
}

Iterator<DiskCluster> * exFAT_FileSystemClass::GetClusterIterator()
{
	return new exFATClusterIterator<DiskCluster>(this);
}
