//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "IteratorThread.h"
#include "Main.h"
#include <string>
#include <sstream>
#include <vector>


#pragma package(smart_init)
//---------------------------------------------------------------------------

__fastcall IteratorThread::IteratorThread(wstring filePath, bool CreateSuspended)
	: TThread(CreateSuspended)
{
	FreeOnTerminate = true;
	// ������� �������� �������
	this->NTFS_FileSystem = new NTFS_FileSystemClass();
	bool isOpen = NTFS_FileSystem->open(filePath);
	if(!isOpen)
	{
		this->Terminate();	//����������
		delete NTFS_FileSystem;
		return;
	}
	bool isSetBootInfo = NTFS_FileSystem->setBootInfo();
	if(!isSetBootInfo)
	{
		this->Terminate();	//����������
		delete NTFS_FileSystem;
		return;//����������
	}
	MainForm->SearchButton->Enabled =false;
	MainForm->StopButton->Enabled =true;
	MainForm->CheckPNG->Enabled = false;
	MainForm->CheckBMP->Enabled = false;
	MainForm->ProgressBar->Max = this->NTFS_FileSystem->getTotalClusters();
	MainForm->ProgressBar->Visible = true;
    MainForm->ProgressLabel->Visible = true;

}
//---------------------------------------------------------------------------
void __fastcall IteratorThread::Execute()
{
	// ���������� ������ ��������
	int clusterSize = this->NTFS_FileSystem->getBytesPerCluster();
	dataBuffer = new BYTE[clusterSize];
	MySearchThread = new SearchThread(dataBuffer,clusterSize,false);

	// ������� ��������� �����
	for(int i = 1; i < this->NTFS_FileSystem->getTotalClusters(); i++)
	{
		this->progress = i;
		Synchronize(&IterationProgress);
		// ������������� ������ � ������
		//BufferAccessCS->Enter();

		// ������� ������ � ��������� �����
		this->NTFS_FileSystem->readClusters(i,1,dataBuffer);
		// �������������� ������ � ������
		//BufferAccessCS->Leave();

		// ��������� ���� ���������� ������
		MySearchThread->BufferReadyEvent->SetEvent();
		// ������� ��������� ����������� ������
		while(MySearchThread->BufferCopiedEvent->WaitFor(WaitDelayMs) != wrSignaled)
		{
		}
        MySearchThread->BufferCopiedEvent->ResetEvent();
       	if(Terminated) break;
	}
	// ��������� �����
	MySearchThread->Terminate();
	delete[] dataBuffer;
	NTFS_FileSystem->close();
	delete NTFS_FileSystem;
	Synchronize(&ActivateButtons);
    return;
}
void __fastcall IteratorThread::ActivateButtons()
{
	MainForm->SearchButton->Enabled =true;
	MainForm->StopButton->Enabled =false;
	MainForm->CheckPNG->Enabled = true;
	MainForm->CheckBMP->Enabled = true;
	MainForm->ProgressBar->Visible = false;
    MainForm->ProgressLabel->Visible = false;
    MainForm->isTerminated = true;
}
void __fastcall IteratorThread::IterationProgress()
{
	MainForm->ProgressBar->Position = this->progress;
    std::stringstream ProgressStr;
    ProgressStr << "cluster "<< this->progress << " of "<< this->NTFS_FileSystem->getTotalClusters();
    MainForm->ProgressLabel->Caption = ProgressStr.str().c_str();
}
//---------------------------------------------------------------------------
