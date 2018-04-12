//---------------------------------------------------------------------------

#ifndef FSClassH
#define FSClassH
#include <windows.h>
#include <string>
#include <vector>

#include "ClusterIterator.h"
using namespace std;
typedef vector<BYTE> DiskCluster;
//---------------------------------------------------------------------------


#pragma pack(push, 1)

typedef struct
{
		//� ������������ � ����� ������� �������� � 10 � 16 �������
	BYTE djmp[3];   //0
	char OEM_ID[8];   //3

} BootRecord;
//---------------------------------------------------------------------------
#pragma pack(pop)

class FSClass
{
protected:
	HANDLE FileHandle;
	DWORD TotalClusters;
	BYTE ClusterFactor;
	DWORD BytesPerCluster;
	ULONGLONG FirstClusterOffset;

public:
	virtual Iterator<DiskCluster> * GetClusterIterator() = 0;
	virtual string setBootInfo() = 0;
	static FSClass *Create(wstring FileSystemPath);

	FSClass();
	bool open(wstring FileSystemPath);
	DWORD getTotalClusters();
	DWORD getBytesPerCluster();
	DiskCluster readClusters(ULONGLONG startCluster, DWORD numberOfClusters, DiskCluster inBuffer);
	void close();
} ;
//---------------------------------------------------------------------------

 #endif
