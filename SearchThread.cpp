//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SearchThread.h"
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
//      void __fastcall SearchThread::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall SearchThread::SearchThread(BYTE *dataBufferPtr, int clusterSize, bool CreateSuspended)
	: TThread(CreateSuspended)
{
	FreeOnTerminate = true;
	CurrentCluster = 1;
	NodeId=1;
	BufferReadyEvent  = new TEvent(NULL, true, false,"",false);
	BufferCopiedEvent = new TEvent(NULL, true, false,"",false);
	BufferAccessCS = new TCriticalSection;
	Synchronize(&GetCheckedBoxes);

	ClusterSize = clusterSize;
	OutBufferPtr = dataBufferPtr;
	DataBuffer = new BYTE[clusterSize];
}
//---------------------------------------------------------------------------
void __fastcall SearchThread::Execute()
{
	while(true)
	{
		// Ждать, пока не будет подготовлен буфер для копирования
		if(BufferReadyEvent->WaitFor(WaitDelayMs) == wrSignaled)
		{
			if(BufferAccessCS->TryEnter())
			{
				// Скопировать данные
				CopyData();

				// Отпустить буфер
				BufferReadyEvent->ResetEvent();
				BufferCopiedEvent->SetEvent();
				BufferAccessCS->Leave();
				// Запустить поиск
				SearchData();
				CurrentCluster++;

			}
		}
		if(Terminated) break;
	}
	// Удалить события
	delete BufferReadyEvent;
	delete BufferCopiedEvent;
	delete BufferAccessCS;
  // Удалить буфер
	delete[] DataBuffer;
	Synchronize(&CompleteSearch);
}
//---------------------------------------------------------------------------
void SearchThread::CopyData()
{
	memcpy(DataBuffer, OutBufferPtr, ClusterSize);
}
//---------------------------------------------------------------------------
void SearchThread::SearchData()
{
	// Провести поиск
	char *Signature1 = "JFIF";       //jpg
	char *Signature2 = "Exif";       //jpg
	char *Signature3 ="PNG";
	char *Signature4 ="BM";         //bmp
	bool matchFound1 = memcmp(OutBufferPtr+6 , Signature1, 4);
	bool matchFound2 = memcmp(OutBufferPtr+6 , Signature2, 4);


	if(!matchFound1 ||!matchFound2 )
	{
		Signature = L"jpeg";
		Synchronize(&AddMatch);
	}

	if(this->isChecked[0])
	{
		if(!memcmp(OutBufferPtr+1 , Signature3, 3))
		{
			Signature = L"png";
			Synchronize(&AddMatch);
		}
	}
	if(this->isChecked[1])
	{
		if(!memcmp(OutBufferPtr , Signature4, 2))
		{
			Signature = L"bmp";
			Synchronize(&AddMatch);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall SearchThread::AddMatch()
{
	PVirtualNode newNode = MainForm->ResultTree->AddChild(MainForm->ResultTree->RootNode);
	DBstruct *nodeData = (DBstruct*)MainForm->ResultTree->GetNodeData(newNode);
	nodeData->id  = NodeId;
	nodeData->cluster = CurrentCluster;
	wcscpy(nodeData->type, Signature);
	NodeId++;
}
void __fastcall SearchThread::GetCheckedBoxes()
{
	 this->isChecked[0] = MainForm->CheckPNG->Checked;
	 this->isChecked[1] = MainForm->CheckBMP->Checked;
}
//---------------------------------------------------------------------------
void __fastcall SearchThread::CompleteSearch()
{
	  //if(!MainForm->OnClose)
	  //{
	 //		Application->MessageBoxW(L"Поиск завершен!", L"", MB_OK);
	  //}

}
