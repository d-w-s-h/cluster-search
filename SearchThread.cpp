//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SearchThread.h"
#include "Main.h"
#include <vector>
using namespace std;
typedef vector<BYTE> DiskCluster;
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

__fastcall SearchThread::SearchThread(DiskCluster *dataBufferPtr, int clusterSize, __int64 *progress, bool CreateSuspended)
	: TThread(CreateSuspended)
{
	FreeOnTerminate = true;
	CurrentIteratorCluster = progress; //ссылка
	NodeId=1;
	BufferReadyEvent  = new TEvent(NULL, true, false,"",false);
	BufferCopiedEvent = new TEvent(NULL, true, false,"",false);
	BufferAccessCS = new TCriticalSection;
	Synchronize(&GetCheckedBoxes);

	ClusterSize = clusterSize;
	OutBufferPtr = dataBufferPtr;
//	DataBuffer = new BYTE[clusterSize];

	Signatures.resize(16);
    Signatures[0]= "JFIF";
    Signatures[1]= "Exif";
    Signatures[2]= "PNG";
    Signatures[3]= "BM";
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
				FixedCurrentCluster = *CurrentIteratorCluster;  //чтобы не убежал
				// Скопировать данные
				CopyData();

				// Отпустить буфер
				BufferReadyEvent->ResetEvent();
				BufferCopiedEvent->SetEvent();
				BufferAccessCS->Leave();
				// Запустить поиск
				SearchData();
//				CurrentCluster++;

			}
		}
		if(Terminated) break;
	}
	// Удалить события
	delete BufferReadyEvent;
	delete BufferCopiedEvent;
	delete BufferAccessCS;
  // Удалить буфер
//	delete[] DataBuffer;
	//Synchronize(&CompleteSearch);
	return;
}
//---------------------------------------------------------------------------
void SearchThread::CopyData()
{
//	memcpy(DataBuffer, OutBufferPtr, ClusterSize);
//	DataBuffer= OutBufferPtr;
	DataBuffer = *OutBufferPtr;

}
//---------------------------------------------------------------------------
void SearchThread::SearchData()
{
	// Проведем поиск
	bool matchFound1 = memcmp(&DataBuffer[0]+6 , Signatures[0].c_str(), Signatures[0].length());
	bool matchFound2 = memcmp(&DataBuffer[0]+6 , Signatures[1].c_str(), Signatures[1].length());

	if(!matchFound1 ||!matchFound2 )
	{
		SignatureName = "jpeg";
		Synchronize(&AddMatch);
	}

	if(this->isChecked[0])
	{
		if(!memcmp(&DataBuffer[0]+1 , Signatures[2].c_str(), Signatures[2].length()))
		{
			SignatureName = "png";
			Synchronize(&AddMatch);
		}
	}
	if(this->isChecked[1])
	{
		if(!memcmp(&DataBuffer[0] , Signatures[3].c_str(), Signatures[3].length()))
		{
			SignatureName = "bmp";
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
	nodeData->cluster = FixedCurrentCluster;
    nodeData->type = SignatureName;
	NodeId++;
}
void __fastcall SearchThread::GetCheckedBoxes()
{
	 this->isChecked[0] = MainForm->CheckPNG->Checked;
	 this->isChecked[1] = MainForm->CheckBMP->Checked;
}
//---------------------------------------------------------------------------

