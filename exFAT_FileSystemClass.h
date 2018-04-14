//---------------------------------------------------------------------------

#ifndef exFAT_FileSystemClassH
#define exFAT_FileSystemClassH
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

	BYTE djmp[3];   //
	char OEM_ID[8];   //
	BYTE jump[61];   //
	ULONGLONG dTotalSectors; // 72 0x48
	BYTE jump2[8];
	DWORD StartBitmap; //88 0x58
	DWORD dTotalClusters; // 92 0x5C
	BYTE jump3[12];
	BYTE SectorPOW; //108 0x6C
	BYTE ClusterPOW; //109 0x6D

} exFAT_BootRecord;
//---------------------------------------------------------------------------
#pragma pack(pop)

class exFAT_FileSystemClass : public FSClass
{
protected:
	using FSClass::FileHandle;
	using FSClass::TotalClusters;
	using FSClass::BytesPerCluster;
	using FSClass::FirstClusterOffset;
	exFAT_BootRecord *pBootRecord;



public:
	virtual Iterator<DiskCluster> * GetClusterIterator();
	virtual string setBootInfo();
	exFAT_FileSystemClass();
} ;

template <class Type> class exFATClusterIterator : public Iterator<Type>
{
	private:
		exFAT_FileSystemClass *Filesystem;
		int ClusterSize;
		__int64 TotalClusters;
		__int64 CurrentCusterIndex;


	public:
		exFATClusterIterator(exFAT_FileSystemClass *filesystem)
		{
			this->Filesystem = filesystem;
			this->ClusterSize = filesystem->getBytesPerCluster();
			this->TotalClusters = filesystem->getTotalClusters();
			this->CurrentCusterIndex = 1;

		};

		~exFATClusterIterator()
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
		virtual bool IsDone() const {return (CurrentCusterIndex >= TotalClusters ); };

		virtual void GetCurrent(Type *outCluster) const
		{
			*outCluster = this->Filesystem->readClusters(CurrentCusterIndex,1,*outCluster);
		};
};
//---------------------------------------------------------------------------

 #endif
