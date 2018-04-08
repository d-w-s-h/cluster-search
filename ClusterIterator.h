//---------------------------------------------------------------------------

#ifndef ClusterIteratorH
#define ClusterIteratorH
#include "intsafe.h"



//---------------------------------------------------------------------------
template <class Type> class Iterator {
    protected:

	public:
		virtual void First() = 0;
		virtual void Next() = 0;
        virtual bool IsDone() const = 0;
		virtual void GetCurrent(Type *out) const =0;
		virtual __int64 GetCurrentIndex() =0 ;

};
//---------------------------------------------------------------------------
template <class Type> class NTFSClusterIterator : public Iterator<Type>
{
	private:
		NTFS_FileSystemClass *Filesystem;
		int ClusterSize;
		__int64 TotalClusters;
		__int64 CurrentCusterIndex;
		BYTE* buffer;

	public:
		NTFSClusterIterator(NTFS_FileSystemClass *filesystem)
		{
			this->Filesystem = filesystem;
			this->ClusterSize = filesystem->getBytesPerCluster();
			this->TotalClusters = filesystem->getTotalClusters();
			this->CurrentCusterIndex = 1;
			buffer = new BYTE[this->ClusterSize] ;
        };

		~NTFSClusterIterator()
        {
			delete this->Filesystem;
			delete buffer;
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
//			outCluster->reserve(this->ClusterSize);
//			outCluster->insert(outCluster->begin(), buffer, buffer + this->ClusterSize);   //слишком медленно

		};
		virtual	__int64 GetCurrentIndex()
		{
			return  CurrentCusterIndex ;
		} ;
};
#endif
