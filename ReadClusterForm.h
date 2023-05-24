//---------------------------------------------------------------------------

#ifndef ReadClusterFormH
#define ReadClusterFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "VirtualTrees.hpp"
#include <Vcl.ComCtrls.hpp>

#include <windows.h>
#include <stdio.h>
#include "RCThread.h"
#include "RCThread2.h"
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
typedef struct
{
	int ID;
	ULONGLONG NumberCluster;
	String FileType;
} DataTree;
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TVirtualStringTree *VirtualStringTree;
	TCheckBox *jpg;
	TCheckBox *png;
	TCheckBox *bmp;
	TComboBox *ChooseVolume;
	TLabel *ChooseFormatLabel;
	TProgressBar *ProgressBar;
	TButton *FindButton;
	TButton *ExitButton;
	TButton *StopButton;
	TLabel *InfoLabel;
	TLabel *ChooseVolumeLabel;
	void __fastcall ChooseVolumeSelect(TObject *Sender);
	void __fastcall FindButtonClick(TObject *Sender);
	void __fastcall VirtualStringTreeGetText(TBaseVirtualTree *Sender, PVirtualNode Node,
          TColumnIndex Column, TVSTTextType TextType, UnicodeString &CellText);
	void __fastcall StopButtonClick(TObject *Sender);
	void __fastcall ExitButtonClick(TObject *Sender);

private:	// User declarations
public:		// User declarations
    bool NeedStop;
	std::wstring path;
	RCThread *RCT1;
    RCThread2 *RCT2;
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
