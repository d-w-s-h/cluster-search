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
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
	: TForm(Owner)
{
    ResultTree->NodeDataSize = sizeof(DBstruct);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SearchButtonClick(TObject *Sender)
{
	IteratorThread *myIteratorThread = new IteratorThread(PathEdit->Text.c_str(), false);
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
		case 2: CellText = nodeData->type; break;
	}
}
//---------------------------------------------------------------------------


