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
	BYTE *dataBuffer;
	__int64 progress;

protected:
	void __fastcall Execute();
public:
	__fastcall IteratorThread(wstring filePath, bool CreateSuspended);
	void __fastcall IteratorThread::ActivateButtons();
	void __fastcall IteratorThread::IterationProgress();
};
//---------------------------------------------------------------------------
#endif
