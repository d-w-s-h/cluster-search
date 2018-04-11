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
//		virtual __int64 GetCurrentIndex() = 0;
};
//---------------------------------------------------------------------------

#endif
