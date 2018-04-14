//---------------------------------------------------------------------------

#ifndef FAT_FileSystemClassH
#define FAT_FileSystemClassH
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
	WORD BytePerSector;   // 11 b
	BYTE SectorsPerCluster;   // 13 d
	WORD Reserved;
    BYTE CountOfFAT;
	BYTE jump[2];
	WORD SectorsPerFS16;// 19 0x13
	BYTE jump2;
	WORD FAT16size;//22 0x16
	BYTE jump3[4];
	DWORD what;//0x1C
	DWORD SectorsPerFS32;// 32 0x20
	DWORD FAT32size;//36 0x24
	BYTE jump4[4];
	DWORD RootDirectoryCluster;//44 0x2C
	BYTE jump5[34];
	char FATNAME[5];//82 0x52
	BYTE jump6[3];

} FAT_BootRecord;
//---------------------------------------------------------------------------
#pragma pack(pop)

class FAT_FileSystemClass : public FSClass
{
protected:
	using FSClass::FileHandle;
	using FSClass::TotalClusters;
	using FSClass::BytesPerCluster;
	using FSClass::FirstClusterOffset;
	FAT_BootRecord *pBootRecord;



public:
	virtual Iterator<DiskCluster> * GetClusterIterator();
	virtual string setBootInfo();
	FAT_FileSystemClass();

} ;

template <class Type> class FATClusterIterator : public Iterator<Type>
{
	private:
		FAT_FileSystemClass *Filesystem;
		int ClusterSize;
		__int64 TotalClusters;
		__int64 CurrentCusterIndex;


	public:
		FATClusterIterator(FAT_FileSystemClass *filesystem)
		{
			this->Filesystem = filesystem;
			this->ClusterSize = filesystem->getBytesPerCluster();
			this->TotalClusters = filesystem->getTotalClusters();
			this->CurrentCusterIndex = 1;
		};

		~FATClusterIterator()
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
