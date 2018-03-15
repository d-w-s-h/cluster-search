//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "IteratorThread.h"
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
	this->NTFS_FileSystem = new NTFS_FileSystemClass();
	bool isOpen = NTFS_FileSystem->open(filePath);
	if(!isOpen)
	{
		//обработать
	}
	bool isSetBootInfo = NTFS_FileSystem->setBootInfo();
	if(!isSetBootInfo)
	{
		//обработать
	}



}
//---------------------------------------------------------------------------
void __fastcall IteratorThread::Execute()
{
	// Определить размер кластера
	int clusterSize = this->NTFS_FileSystem->getBytesPerCluster();
	BYTE *dataBuffer = new BYTE[clusterSize];
	MySearchThread = new SearchThread(dataBuffer,clusterSize,false);

	// Перебор кластеров диска
	for(int i = 1; i < this->NTFS_FileSystem->getTotalClusters(); i++)
	{
		// Заблокировать доступ к буферу
		BufferAccessCS->Enter();

		// Считать данные в локальный буфер
		this->NTFS_FileSystem->readClusters(i,1,dataBuffer);
		// Разблокировать доступ к буферу
		BufferAccessCS->Leave();

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
