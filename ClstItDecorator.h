//---------------------------------------------------------------------------

#ifndef ClstItDecoratorH
#define ClstItDecoratorH
#include "ClusterIterator.h"
#define bit_test(x, y)  ((((BYTE*)&(x))[(y)>>3] ) >> ((y)>>0x70))


//---------------------------------------------------------------------------
//делаем прозрачный декоратор, который пробрасывает функции
template<class Type1> class IteratorDecorator : public Iterator<Type1>
{
	protected:
		Iterator<Type1> *It; //указатель на итератор
		__int64 Progress  ;

	public:
		IteratorDecorator(Iterator<Type1> *it ){It = it;};
		virtual ~IteratorDecorator(){delete It;};
		virtual void First(){It->First(); };
		virtual void Next(){It->Next();};
		virtual bool IsDone() const {return It->IsDone();};
		virtual void GetCurrent(Type1 *out) const {It->GetCurrent(out);};
		virtual __int64 GetCurrentIndex(){return It->GetCurrentIndex();};
};
////---------------------------------------------------------------------------
//декоратор для Free Memory Mode
template<class Type1> class FreeMemoryModeIteratorDecorator : public IteratorDecorator<Type1>
{
	protected:
		NTFSClusterIterator<Type1> *It; //указатель на итератор
		BYTE *Bitmap;
		__int64 localprogress;
	public:
		FreeMemoryModeIteratorDecorator(NTFSClusterIterator<Type1> *it, BYTE *bitmapBuffer)	: IteratorDecorator<Type1>(it)
		{
			this->It=it;
			this->Bitmap = bitmapBuffer;
			localprogress =1;

		}
		virtual ~FreeMemoryModeIteratorDecorator(){delete It;};
		virtual void First()
		{
			It->First();
			localprogress =1;
		};
		virtual void Next()
		{
			for(It->Next(); !It->IsDone(); It->Next())
			{
				localprogress++ ;
				if (!bit_test(*Bitmap, localprogress))  //FF FF FF 07 читаются как 11100000 в конце
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
