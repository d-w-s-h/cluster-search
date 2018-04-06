//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Main.h"
#include "IteratorThread.h"
#include <sstream>
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
	delete[]  BitmapBuffer;

}
//---------------------------------------------------------------------------

void __fastcall TMainForm::StopButtonClick(TObject *Sender)
{
   myIteratorThread->Terminate();
   isTerminated =true;
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::BitmapButtonClick(TObject *Sender)
{

	if(OpenBitmapDialog->Execute())
	{
		HANDLE BitmapHandle = CreateFileW(
			OpenBitmapDialog->FileName.c_str(), // Имя файла (WCHAR*)
			GENERIC_READ,	  // Режим доступа
			FILE_SHARE_READ | FILE_SHARE_WRITE, // Режим совместной работы
			NULL, // Атрибуты безопасности
			OPEN_EXISTING, // Способ открытия
			FILE_ATTRIBUTE_NORMAL, // Флаги и атрибуты
			NULL // Описатель (идентификатор) файла шаблона с правами доступа GENERIC_READ.
		);
		if(BitmapHandle == INVALID_HANDLE_VALUE)
		{
			// Обработка ошибки
			Application->MessageBoxW(L"Не удаётся открыть файл Bitmap", L"Ошибка", MB_OK);
			CloseHandle(BitmapHandle);
			return;
		}
		//узнаем размер файла
		LONGLONG nFileLen = 0;
		DWORD dwSizeHigh=0, dwSizeLow=0;
		dwSizeLow = GetFileSize(BitmapHandle, &dwSizeHigh);
		nFileLen = (dwSizeHigh * (MAXDWORD+1)) + dwSizeLow;
		BitmapBuffer = new BYTE[nFileLen];
		DWORD bytesRead;
		bool readBitmap = ReadFile(BitmapHandle, this->BitmapBuffer, nFileLen, &bytesRead,NULL);
		if(!readBitmap || bytesRead != nFileLen)
		{
			Application->MessageBoxW(L"Не удаётся прочесть файл Bitmap", L"Ошибка", MB_OK);
			CloseHandle(BitmapHandle);
			return;
		}
		CloseHandle(BitmapHandle);
		wstringstream BitPathString;
		BitPathString << "loaded: " << OpenBitmapDialog->FileName.c_str()  ;
		BitmapLabel->Caption = BitPathString.str().c_str();
		this->FreeMemModeCheckBox->Enabled = true;
	}
}
//---------------------------------------------------------------------------



