//---------------------------------------------------------------------------

#ifndef IteratorThreadH
#define IteratorThreadH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
//---------------------------------------------------------------------------
#include "SearchThread.h"
#include "NTFS_FileSystemClass.h"
//---------------------------------------------------------------------------
class IteratorThread : public TThread
{
private:
	NTFS_FileSystemClass *NTFS_FileSystem;
	HANDLE FileSystemHandle;
	SearchThread *MySearchThread;

protected:
	void __fastcall Execute();
public:
	__fastcall IteratorThread(WCHAR *filePath, bool CreateSuspended);
};
//---------------------------------------------------------------------------
#endif
