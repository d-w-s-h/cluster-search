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
		__int64 *Progress  ;  //������� ������, �� ������ ������� ����� ��������� ��������

	public:
		IteratorDecorator(Iterator<Type1> *it,__int64 *progress)
		{
			It = it;
			this->Progress = progress;
		};
		virtual ~IteratorDecorator(){delete It;};
		virtual void First()
		{
			It->First();
			*this->Progress = 1;
		};
		virtual void Next()
		{
			It->Next();
			*this->Progress=*Progress+1;
		};
		virtual bool IsDone() const {return It->IsDone();};
		virtual void GetCurrent(Type1 *out) const {It->GetCurrent(out);};
//		virtual __int64 GetCurrentIndex(){return It->GetCurrentIndex();};
};
////---------------------------------------------------------------------------
//��������� ��� Free Memory Mode
template<class Type1> class FreeMemoryModeIteratorDecorator : public IteratorDecorator<Type1>
{
	protected:
		Iterator<Type1> *It; //��������� �� ��������
		BYTE *Bitmap;
		__int64 localprogress;
		__int64 *Progress  ;
	public:
		FreeMemoryModeIteratorDecorator(Iterator<Type1> *it,__int64 *progress, BYTE *bitmapBuffer)  	: IteratorDecorator<Type1>(it, progress)
		{
			this->It=it;
			this->Bitmap = bitmapBuffer;
			localprogress =1;
			this->Progress =progress ;
		}
		virtual ~FreeMemoryModeIteratorDecorator(){delete It;};
//		virtual void First()
//		{
//			It->First();
//			localprogress =1;
//			*Progress=1;
//		};
		virtual void Next()
		{
			for(It->Next(); !It->IsDone(); It->Next())
			{
				localprogress++ ;
				*this->Progress = *Progress + 1;
				if (!bit_test(*Bitmap, localprogress))  //FF FF FF 07 �������� ��� 11100000 � �����
				{
					break;
				}
			}

		};
//		virtual bool IsDone() const {return It->IsDone();};
//		virtual void GetCurrent(Type1 *out) const {It->GetCurrent(out);};
//		virtual void GetCurrentIndex(__int64 *progress){It->GetCurrentIndex(progress);};
};
#endif
