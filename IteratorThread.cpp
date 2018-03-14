//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "IteratorThread.h"
#include "NTFS_FileSystemClass.h"
#include "Main.h"


#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(&UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall IteratorThread::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall IteratorThread::IteratorThread(WCHAR *filePath, bool CreateSuspended)
	: TThread(CreateSuspended)
{
	FreeOnTerminate = true;
	// Открыть файловую систему
	NTFS_FileSystemClass *NTFS_FileSystem = new NTFS_FileSystemClass();
	WCHAR *FileSystemPath =  L"\\\\.\\E:" ;
	bool isOpen = NTFS_FileSystem->open(FileSystemPath);
    bool isSetBootInfo = NTFS_FileSystem->setBootInfo();


}
//---------------------------------------------------------------------------
void __fastcall IteratorThread::Execute()
{
	// Определить размер кластера
	int clusterSize = 4096;
	BYTE *dataBuffer = new BYTE[clusterSize];
	MySearchThread = new SearchThread(dataBuffer,clusterSize,false);

	// Перебор кластеров диска
	for(int i = 0; i<10000; i++)
	{
		// Заблокировать доступ к буферу
		//BufferAccessCS->Enter();

		// Считать данные в локальный буфер
		Sleep(50);

		// Разблокировать доступ к буферу
		//BufferAccessCS->Leave();

		// Выставить флаг готовности буфера
		MySearchThread->BufferReadyEvent->SetEvent();



		// Ожидать окончания копирования буфера
		while(MySearchThread->BufferCopiedEvent->WaitFor(WaitDelayMs) != wrSignaled)
		{
		}

		MySearchThread->BufferCopiedEvent->ResetEvent();
	}

	// Завершить поиск
	MySearchThread->Terminate();

	delete[] dataBuffer;
}
//---------------------------------------------------------------------------
