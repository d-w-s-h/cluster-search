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
#include <Vcl.ComCtrls.hpp>
#include <string>
using namespace std;
//---------------------------------------------------------------------------
typedef struct
{
	int id;
	__int64 cluster;
	string type;

} DBstruct;


//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
	TLabeledEdit *PathEdit;
	TVirtualStringTree *ResultTree;
	TButton *SearchButton;
	TButton *StopButton;
	TLabel *FSinfoLabel;
	TCheckBox *CheckPNG;
	TCheckBox *CheckBMP;
	TProgressBar *ProgressBar;
    TLabel *ProgressLabel;
	void __fastcall SearchButtonClick(TObject *Sender);
	void __fastcall ResultTreeGetText(TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column,
          TVSTTextType TextType, UnicodeString &CellText);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall StopButtonClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TMainForm(TComponent* Owner);
    bool isTerminated;
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
