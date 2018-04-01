//---------------------------------------------------------------------------

#ifndef NTFS_FileSystemClassH
#define NTFS_FileSystemClassH
#include <windows.h>
#include <string>
using namespace std;
//---------------------------------------------------------------------------
#endif

#pragma pack(push, 1)

typedef struct
{
        //в комментари€х к пол€м указано смещение в 10 и 16 формате
    BYTE djmp[3];   //0
	char OEM_ID[8];   //3
	WORD dBytesPerSector;  //11  b
	BYTE dSectorPerCluster;  //13  d
	BYTE dUnusedA[7];  //14  e
	BYTE dMediaID;  //21  15
	WORD dUnusedB;  //22 16
	WORD dSectorPerTrack; //24 18
	WORD dHeadsCount; // 26 1a
	BYTE dUnusedC[8];  // 28 1c
	BYTE dUsualy[4];   //36 24
	ULONGLONG dTotalSectors;  //40 28
	INT64 dLCNofMFT;  //48 30
	INT64 dLCNofMFTMirr;  //56 38
	DWORD ClusterPerMFT;  //64 40
	DWORD ClustersPerIndexes;  //68 44
	ULONGLONG dSerialNumber;  //72 48
    BYTE dDataCode[432];
} NTFS_BootRecord;

#pragma pack(pop)

class NTFS_FileSystemClass
{
protected:
	HANDLE FileHandle;
	DWORD TotalClusters;
	BYTE ClusterFactor;
	DWORD BytesPerCluster;
	NTFS_BootRecord *pBootRecord;
	char *DebugInfo;


public:
	NTFS_FileSystemClass();
	bool open(wstring FileSystemPath);
	bool setBootInfo();
	DWORD getTotalClusters();
	DWORD getBytesPerCluster();
	bool readClusters(ULONGLONG startCluster, DWORD numberOfClusters, BYTE *outBuffer);
	void close();
} ;
