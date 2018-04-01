//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Main.h"
#include "IteratorThread.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "VirtualTrees"
#pragma resource "*.dfm"
TMainForm *MainForm;
IteratorThread *myIteratorThread;


//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
	: TForm(Owner)
{
	ResultTree->NodeDataSize = sizeof(DBstruct);
    isTerminated=true;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SearchButtonClick(TObject *Sender)
{
	ResultTree-> Clear();
	myIteratorThread = new IteratorThread(PathEdit->Text.c_str(), false);
    isTerminated = false;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ResultTreeGetText(TBaseVirtualTree *Sender, PVirtualNode Node,
		  TColumnIndex Column, TVSTTextType TextType, UnicodeString &CellText)
{
	if(!Node)	return;
	DBstruct *nodeData = (DBstruct*)Sender->GetNodeData(Node);
	switch(Column)
	{
		case 0: CellText = nodeData->id; break;
		case 1: CellText = nodeData->cluster; break;
		case 2: CellText = nodeData->type.c_str(); break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	if(isTerminated)
	{
	  return;
	}
    myIteratorThread->Terminate();

}
//---------------------------------------------------------------------------

void __fastcall TMainForm::StopButtonClick(TObject *Sender)
{
   myIteratorThread->Terminate();
   isTerminated =true;

   //Application->MessageBoxW(L"Поиск завершен!", L"", MB_OK);
}
//---------------------------------------------------------------------------





