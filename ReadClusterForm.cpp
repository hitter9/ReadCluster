//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ReadClusterForm.h"
#include "RCThread.h"
#include "RCThread2.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "VirtualTrees"
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
	VirtualStringTree->NodeDataSize = sizeof(DataTree);
	DWORD mask = GetLogicalDrives();
	for (int x = 0; x < 26; x++)
	{
		bool n = ((mask >> x) & 1);
		if (n)
		{
			ChooseVolume->AddItem((char)(65 + x), NULL);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ChooseVolumeSelect(TObject *Sender)
{
	path = ChooseVolume->Items->Strings[ChooseVolume->ItemIndex];
	FindButton->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FindButtonClick(TObject *Sender)
{
    VirtualStringTree->Clear();
	StopButton->Enabled = true;
	FindButton->Enabled = false;
	ProgressBar->Visible = true;
    InfoLabel->Visible = true;
	InfoLabel->Caption = "Идет поиск...";
	jpg->Enabled = false;
	png->Enabled = false;
    bmp->Enabled = false;
	RCT1 = new RCThread(false, path);
	RCT2 = new RCThread2(true);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::VirtualStringTreeGetText(TBaseVirtualTree *Sender, PVirtualNode Node,
          TColumnIndex Column, TVSTTextType TextType, UnicodeString &CellText)
{
	DataTree *datatree = (DataTree*)Sender->GetNodeData(Node);
	switch (Column) {
		case 0:
			CellText = datatree->ID;
			break;
		case 1:
			CellText = datatree->NumberCluster;
			break;
		case 2:
			CellText = datatree->FileType;
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::StopButtonClick(TObject *Sender)
{
	if (!RCT2->Finished)
		RCT2->Free();
	if (!RCT1->Finished)
		RCT1->Free();
	FindButton->Enabled = true;
	StopButton->Enabled = false;
	InfoLabel->Caption = "Поиск остановлен";
    ProgressBar->Visible = false;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ExitButtonClick(TObject *Sender)
{
	if (!RCT2->Finished)
		RCT2->Free();
	if (!RCT1->Finished)
		RCT1->Free();
	ExitProcess(0);
}
//---------------------------------------------------------------------------
