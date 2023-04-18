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
//---------------------------------------------------------------------------
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
private:	// User declarations
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
