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

	// ������� �������� �������
	this->NTFS_FileSystem = new NTFS_FileSystemClass();
	bool isOpen = NTFS_FileSystem->open(filePath);
	if(!isOpen)
	{
		//����������
	}
	bool isSetBootInfo = NTFS_FileSystem->setBootInfo();
	if(!isSetBootInfo)
	{
		//����������
	}



}
//---------------------------------------------------------------------------
void __fastcall IteratorThread::Execute()
{
	// ���������� ������ ��������
	int clusterSize = this->NTFS_FileSystem->getBytesPerCluster();
	BYTE *dataBuffer = new BYTE[clusterSize];
	MySearchThread = new SearchThread(dataBuffer,clusterSize,false);

	// ������� ��������� �����
	for(int i = 1; i < this->NTFS_FileSystem->getTotalClusters(); i++)
	{
		// ������������� ������ � ������
		BufferAccessCS->Enter();

		// ������� ������ � ��������� �����
		this->NTFS_FileSystem->readClusters(i,1,dataBuffer);
		// �������������� ������ � ������
		BufferAccessCS->Leave();

		// ��������� ���� ���������� ������
		MySearchThread->BufferReadyEvent->SetEvent();



		// ������� ��������� ����������� ������
		while(MySearchThread->BufferCopiedEvent->WaitFor(WaitDelayMs) != wrSignaled)
		{
		}

		MySearchThread->BufferCopiedEvent->ResetEvent();
	}

	// ��������� �����
	MySearchThread->Terminate();

	delete[] dataBuffer;
}
//---------------------------------------------------------------------------
