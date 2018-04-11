//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "IteratorThread.h"

#include "Main.h"
#include <string>
#include <sstream>
#include <vector>
typedef vector<BYTE> DiskCluster;


#pragma package(smart_init)
//---------------------------------------------------------------------------

__fastcall IteratorThread::IteratorThread(wstring filePath, bool CreateSuspended)
	: TThread(CreateSuspended)
{
	FreeOnTerminate = true;
	// ������� �������� �������
	this->FileSystem = new exFAT_FileSystemClass();
	bool isOpen = this->FileSystem->open(filePath);
	if(!isOpen)
	{
		this->Terminate();	//����������
		delete this->FileSystem;
		return;
	}
	string isSetBootInfo ="";
	isSetBootInfo = FileSystem->setBootInfo();
	if(isSetBootInfo=="")
	{
		this->Terminate();	//����������
		delete this->FileSystem;
		return;//����������
	}
	MainForm->FSinfoLabel->Caption = isSetBootInfo.c_str();
	MainForm->SearchButton->Enabled =false;
	MainForm->StopButton->Enabled =true;
	MainForm->CheckPNG->Enabled = false;
	MainForm->CheckBMP->Enabled = false;
	MainForm->ProgressBar->Max = this->FileSystem->getTotalClusters();
	MainForm->ProgressBar->Visible = true;
	MainForm->ProgressLabel->Visible = true;
	MainForm->BitmapButton->Enabled =false;
	MainForm->FreeMemModeCheckBox->Enabled = false;




}
//---------------------------------------------------------------------------
void __fastcall IteratorThread::Execute()
{
	// ���������� ������ ��������
	int clusterSize = this->FileSystem->getBytesPerCluster();
//	dataBuffer = new BYTE[clusterSize];
	DiskCluster dataCluster(clusterSize);

	MySearchThread = new SearchThread(&dataCluster,clusterSize, &this->progress, false);

	Iterator<DiskCluster> *it;
	if(MainForm->FreeMemModeCheckBox->Checked)
	{
		it = new FreeMemoryModeIteratorDecorator<DiskCluster> (this->FileSystem->GetClusterIterator(), &progress,MainForm->BitmapBuffer);
	}
	else it = new IteratorDecorator<DiskCluster> (this->FileSystem->GetClusterIterator(), &progress);

	for(it->First();!it->IsDone(); it->Next())
	{
		Synchronize(&IterationProgress);

		it->GetCurrent(&dataCluster);
//		progress = it->GetCurrentIndex();

		MySearchThread->BufferReadyEvent->SetEvent();
		while(MySearchThread->BufferCopiedEvent->WaitFor(WaitDelayMs) != wrSignaled)
		{
		}
		MySearchThread->BufferCopiedEvent->ResetEvent();
       	if(Terminated) break;
	}
	delete it;


//	for(int i = 1; i < this->NTFS_FileSystem->getTotalClusters(); i++)
//	{
//		this->progress = i;
//		Synchronize(&IterationProgress);
//		// ������������� ������ � ������
//		//BufferAccessCS->Enter();
//
//		// ������� ������ � ��������� �����
//		this->NTFS_FileSystem->readClusters(i,1,dataBuffer);
//		// �������������� ������ � ������
//		//BufferAccessCS->Leave();
//
//		// ��������� ���� ���������� ������
//		MySearchThread->BufferReadyEvent->SetEvent();
//		// ������� ��������� ����������� ������
//		while(MySearchThread->BufferCopiedEvent->WaitFor(WaitDelayMs) != wrSignaled)
//		{
//		}
//		MySearchThread->BufferCopiedEvent->ResetEvent();
//		if(Terminated) break;
//	}
	// ��������� �����
	MySearchThread->Terminate();
//	delete[] dataBuffer;
	FileSystem->close();
	delete FileSystem;
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
	MainForm->BitmapButton->Enabled =true;
	if(MainForm->BitmapBuffer != NULL )
	{
		MainForm->FreeMemModeCheckBox->Enabled = true;
    }
}
void __fastcall IteratorThread::IterationProgress()
{
	MainForm->ProgressBar->Position = this->progress;
    std::stringstream ProgressStr;
	ProgressStr << "cluster "<< this->progress << " of "<< this->FileSystem->getTotalClusters();
    MainForm->ProgressLabel->Caption = ProgressStr.str().c_str();
}
//---------------------------------------------------------------------------
