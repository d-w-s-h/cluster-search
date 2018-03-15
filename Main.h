//---------------------------------------------------------------------------

#ifndef MainH
#define MainH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "VirtualTrees.hpp"
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
typedef struct
{
	int id;
	__int64 cluster;
	WCHAR type[16];

} DBstruct;
//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
	TLabeledEdit *PathEdit;
	TVirtualStringTree *ResultTree;
	TButton *SearchButton;
	void __fastcall SearchButtonClick(TObject *Sender);
	void __fastcall ResultTreeGetText(TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column,
          TVSTTextType TextType, UnicodeString &CellText);
private:	// User declarations
public:		// User declarations
	__fastcall TMainForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
