//---------------------------------------------------------------------------

#ifndef NTFS_FileSystemClassH
#define NTFS_FileSystemClassH
#include <windows.h>
//---------------------------------------------------------------------------
#endif

#pragma pack(push, 1)

typedef struct
{
        //� ������������ � ����� ������� �������� � 10 � 16 �������
    BYTE djmp[3];   //0
    BYTE dSystemID[8];   //3
	WORD dBytesPerSector;  //11  b
	BYTE dSectorPerCluster;  //13  d
    BYTE dUnusedA[7];  //14  e
    BYTE dMediaID;  //21  15
    WORD dUnusedB;  //22 16
    WORD dSectorPerTrack; //24 18
    WORD dHeadsCount; // 26 1a
    BYTE dUnusedC[8];  // 28 1c
    BYTE dUsualy[4];   //36 24
    INT64 dNumberOfSectors;  //40 28
    INT64 dLCNofMFT;  //48 30
    INT64 dLCNofMFTMirr;  //56 38
    DWORD ClusterPerMFT;  //64 40
    DWORD ClustersPerIndexes;  //68 44
    BYTE dSerialNumber[8];  //72 48
    BYTE dDataCode[432];
} NTFS_BootRecord;

#pragma pack(pop)

class NTFS_FileSystemClass
{
protected:
	HANDLE FileHandle;
	DWORD TotalClusters;
	BYTE ClusterFactor;
	WORD BytesPerSector;
	BYTE SectorPerCluster;
	DWORD BytesPerCluster;
	// � �. �.

public:
	NTFS_FileSystemClass();
	bool Open(WCHAR *FileSystemPath);

	DWORD GetTotalClusters() const;
	DWORD GetBytesPerCluster() const;
	// ... ��� �����-�� ������� ��� ������ ���������� � �������� �������

	bool ReadClusters(ULONGLONG startCluster, DWORD numberOfClusters, BYTE *outBuffer);

	void Close();
} ;
