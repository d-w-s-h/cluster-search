//---------------------------------------------------------------------------

#ifndef NTFS_FileSystemClassH
#define NTFS_FileSystemClassH
#include <windows.h>
#include <string>
#include <vector>
#include "FSClass.h"

#include "ClusterIterator.h"
using namespace std;
typedef vector<BYTE> DiskCluster;
//---------------------------------------------------------------------------


#pragma pack(push, 1)

typedef struct
{
        //в комментариях к полям указано смещение в 10 и 16 формате
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
//---------------------------------------------------------------------------
#pragma pack(pop)

class NTFS_FileSystemClass   : public FSClass
{
protected:
	using FSClass::FileHandle;
	using FSClass::TotalClusters;
	using FSClass:: ClusterFactor;
	using FSClass::BytesPerCluster;
	NTFS_BootRecord *pBootRecord;



public:
	virtual Iterator<DiskCluster> * GetClusterIterator();
	virtual string setBootInfo();
	NTFS_FileSystemClass();

//	bool open(wstring FileSystemPath);
//	DWORD getTotalClusters();
//	DWORD getBytesPerCluster();
//	DiskCluster readClusters(ULONGLONG startCluster, DWORD numberOfClusters, DiskCluster inBuffer);
//	void close();
} ;

template <class Type> class NTFSClusterIterator : public Iterator<Type>
{
	private:
		NTFS_FileSystemClass *Filesystem;
		int ClusterSize;
		__int64 TotalClusters;
		__int64 CurrentCusterIndex;


	public:
		NTFSClusterIterator(NTFS_FileSystemClass *filesystem)
		{
			this->Filesystem = filesystem;
			this->ClusterSize = filesystem->getBytesPerCluster();
			this->TotalClusters = filesystem->getTotalClusters();
			this->CurrentCusterIndex = 1;

		};

		~NTFSClusterIterator()
		{
			delete this->Filesystem;

		};

		virtual void First()
		{
			CurrentCusterIndex = 1;
		};
		virtual void Next()
		{
			CurrentCusterIndex++;
		};
		virtual bool IsDone() const {return (CurrentCusterIndex >= TotalClusters ); }; //-1  не нужно

		virtual void GetCurrent(Type *outCluster) const
		{
			*outCluster = this->Filesystem->readClusters(CurrentCusterIndex,1,*outCluster);
			//outCluster->reserve(this->ClusterSize);
			//outCluster->insert(outCluster->begin(), buffer, buffer + this->ClusterSize);   //слишком медленно

		};

};
//---------------------------------------------------------------------------

 #endif
