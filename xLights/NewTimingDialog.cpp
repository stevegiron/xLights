#include "NewTimingDialog.h"

//(*InternalHeaders(NewTimingDialog)
#include <wx/button.h>
#include <wx/string.h>
#include <wx/intl.h>
//*)

//(*IdInit(NewTimingDialog)
const long NewTimingDialog::ID_STATICTEXT1 = wxNewId();
const long NewTimingDialog::ID_CHOICE_New_Fixed_Timing = wxNewId();
//*)

BEGIN_EVENT_TABLE(NewTimingDialog,wxDialog)
	//(*EventTable(NewTimingDialog)
	//*)
END_EVENT_TABLE()

NewTimingDialog::NewTimingDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(NewTimingDialog)
	wxFlexGridSizer* FlexGridSizer1;
	wxStdDialogButtonSizer* StdDialogButtonSizer1;

	Create(parent, wxID_ANY, _("New Timing"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
	FlexGridSizer1 = new wxFlexGridSizer(0, 1, 0, 0);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Select New Timing Interval:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	FlexGridSizer1->Add(StaticText1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	Choice_New_Fixed_Timing = new wxChoice(this, ID_CHOICE_New_Fixed_Timing, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE_New_Fixed_Timing"));
	Choice_New_Fixed_Timing->Append(_("25ms"));
	Choice_New_Fixed_Timing->SetSelection( Choice_New_Fixed_Timing->Append(_("50ms")) );
	Choice_New_Fixed_Timing->Append(_("100ms"));
	Choice_New_Fixed_Timing->Append(_("Empty"));
	FlexGridSizer1->Add(Choice_New_Fixed_Timing, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StdDialogButtonSizer1 = new wxStdDialogButtonSizer();
	StdDialogButtonSizer1->AddButton(new wxButton(this, wxID_OK, wxEmptyString));
	StdDialogButtonSizer1->AddButton(new wxButton(this, wxID_CANCEL, wxEmptyString));
	StdDialogButtonSizer1->Realize();
	FlexGridSizer1->Add(StdDialogButtonSizer1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(FlexGridSizer1);
	FlexGridSizer1->Fit(this);
	FlexGridSizer1->SetSizeHints(this);
	//*)
}

NewTimingDialog::~NewTimingDialog()
{
	//(*Destroy(NewTimingDialog)
	//*)
}

void NewTimingDialog::RemoveChoice(const wxString selection)
{
    for( int i = 0; i < Choice_New_Fixed_Timing->GetCount(); i++ )
    {
        if( Choice_New_Fixed_Timing->GetString(i) == selection )
        {
            Choice_New_Fixed_Timing->Delete(i);
            Choice_New_Fixed_Timing->SetSelection(0);
            return;
        }
    }
}
