//---------------------------------------------------------------------------

#ifndef ClstItDecoratorH
#define ClstItDecoratorH
#include "ClusterIterator.h"
#define bit_test(x, y)  ((((BYTE*)&(x))[(y)>>3] ) >> ((y)>>0x70))


//---------------------------------------------------------------------------
//������ ���������� ���������, ������� ������������ �������
template<class Type1> class IteratorDecorator : public Iterator<Type1>
{
	protected:
		Iterator<Type1> *It; //��������� �� ��������

	public:
		IteratorDecorator(Iterator<Type1> *it){It = it;};
		virtual ~IteratorDecorator(){delete It;};
		virtual void First(__int64 *progress){It->First(progress);};
		virtual void Next(__int64 *progress){It->Next(progress);};
		virtual bool IsDone() const {return It->IsDone();};
		virtual void GetCurrentCluster(BYTE *outBuffer) const {It->GetCurrentCluster(outBuffer);};
};
//---------------------------------------------------------------------------
//��������� ��� Free Memory Mode
template<class Type1> class FreeMemoryModeIteratorDecorator : public IteratorDecorator<Type1>
{
	protected:
		ClusterIterator<Type1> *It; //��������� �� ��������
		BYTE *Bitmap;
	public:
		FreeMemoryModeIteratorDecorator(ClusterIterator<Type1> *it, BYTE *bitmapBuffer)	: IteratorDecorator<Type1>(it)
		{
			this->It=it;
			this->Bitmap = bitmapBuffer;
		}
		virtual ~FreeMemoryModeIteratorDecorator(){delete It;};
		virtual void First(__int64 *progress)
		{
			It->First(progress);
		};
		virtual void Next(__int64 *progress)
		{
			for(It->Next(progress); !It->IsDone(); It->Next(progress))
			{
				if (!bit_test(*Bitmap, *progress))  //FF FF FF 07 �������� ��� 11100000 � �����
				{
					break;
				}
			}
		};
		virtual bool IsDone() const {return It->IsDone();};
		virtual void GetCurrentCluster(BYTE *outBuffer) const {It->GetCurrentCluster(outBuffer);};
};
#endif
