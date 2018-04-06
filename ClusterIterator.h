//---------------------------------------------------------------------------

#ifndef ClusterIteratorH
#define ClusterIteratorH
#include "intsafe.h"
#include "ClusterIterator.h"


//---------------------------------------------------------------------------
template <class Type> class Iterator {
    protected:

	public:
		virtual void First(__int64 *progress) = 0;
        virtual void Next(__int64 *progress) = 0;
        virtual bool IsDone() const = 0;
        virtual void GetCurrentCluster(BYTE *outBuffer) const =0;

};
//---------------------------------------------------------------------------
template <class Type> class ClusterIterator : public Iterator<Type>
{
    private:
        Type *Filesystem;
        int ClusterSize;
        __int64 TotalClusters;
        __int64 CurrentCusterIndex; //чтобы хранить текущий индекс

    public:
        ClusterIterator(Type *filesystem)    //потому что шаблонный класс, иначе не видит реализации
        {
			this->Filesystem = filesystem;
			this->ClusterSize = filesystem->getBytesPerCluster();
			this->TotalClusters = filesystem->getTotalClusters();
			this->CurrentCusterIndex = 1;
        };

        ~ClusterIterator()
        {
            delete this->Filesystem;
        };

        virtual void First(__int64 *progress)
        {

			CurrentCusterIndex = 1;
			*progress = CurrentCusterIndex;
        };
        virtual void Next(__int64 *progress)
        {
			CurrentCusterIndex++;
			*progress =  CurrentCusterIndex;
        };
        virtual bool IsDone() const {return (CurrentCusterIndex >= TotalClusters ); }; //-1  не нужно
        virtual void GetCurrentCluster(BYTE *outBuffer) const
        {

			this->Filesystem->readClusters(CurrentCusterIndex,1,outBuffer);
		};
};
#endif
