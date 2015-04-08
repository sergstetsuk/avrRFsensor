#include "maindialog.h"
#include <wx/aboutdlg.h>
#include <wx/button.h>

//Icons
#include "icons/configurator.xpm"

BEGIN_EVENT_TABLE(ConfiguratorDialog , wxDialog)
    EVT_BUTTON(wxID_OK, ConfiguratorDialog::OnOK)
    EVT_BUTTON(wxID_ABOUT, ConfiguratorDialog::OnAbout)
    EVT_BUTTON(wxID_HELP, ConfiguratorDialog::OnHelp)
    EVT_BUTTON(wxID_CANCEL, ConfiguratorDialog::OnCancel)
    EVT_TIMER(ID_TIMER, ConfiguratorDialog::OnTimer)
    EVT_CLOSE(ConfiguratorDialog::OnClose)
END_EVENT_TABLE()


ConfiguratorDialog::ConfiguratorDialog ()
{
}

ConfiguratorDialog::ConfiguratorDialog (wxWindow * parent, wxWindowID id,const wxString & title)
    : wxDialog(parent, id, title, wxDefaultPosition, wxDefaultSize,
        wxDIALOG_NO_PARENT | wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
	Create(parent, id, title);
    m_timer = new wxTimer(this, ID_TIMER);
    m_timer->Start(100,wxTIMER_ONE_SHOT);
}

ConfiguratorDialog::~ConfiguratorDialog()
{
}

bool ConfiguratorDialog::Create(wxWindow * parent, wxWindowID id,const wxString & title)
{
    SetIcon(wxIcon(configurator_xpm));
    CreateWaitControls();
    //start timer for waiting RF module
    return true;
}


void ConfiguratorDialog::CreateWaitControls()
{	
    wxBoxSizer *topSizer;
    wxStaticBox* staticBox;
    wxStaticBoxSizer* staticSizer;
    
    topSizer = new wxBoxSizer( wxVERTICAL );
    
    staticBox = new wxStaticBox(this, wxID_ANY,
        MSG_ID);
    staticSizer = new wxStaticBoxSizer(staticBox,
        wxVERTICAL);
    m_myid = new wxTextCtrl( this, ID_MYID, MSG_NOTCONNECTED,
        wxDefaultPosition, wxSize(100,25), 0);
    m_myid->Disable();
    staticSizer->Add(m_myid,
        0,          // make vertically stretchable
        wxEXPAND|   // make horizontally stretchable
        wxALL,      // and make border all around
        2 );       // set border width to 2
    topSizer->Add(staticSizer,
        0,          // make vertically stretchable
        wxEXPAND|   // make horizontally stretchable
        wxALL,      // and make border all around
        5 );       // set border width to 5

    staticBox = new wxStaticBox(this, wxID_ANY,
        MSG_ALARMLIST);
    staticSizer = new wxStaticBoxSizer(staticBox,
        wxVERTICAL);
    m_alarmlist = new wxTextCtrl( this, ID_ALARMLIST, MSG_NOTCONNECTED,
        wxDefaultPosition, wxSize(100,60), wxTE_MULTILINE);
    m_alarmlist->Disable();
    staticSizer->Add(m_alarmlist,
        1,          // make vertically stretchable
        wxEXPAND|   // make horizontally stretchable
        wxALL,      // and make border all around
        2 );       // set border width to 2
    topSizer->Add(staticSizer,
        1,          // make vertically stretchable
        wxEXPAND|   // make horizontally stretchable
        wxALL,      // and make border all around
        5 );       // set border width to 5

    staticBox = new wxStaticBox(this, wxID_ANY,
        MSG_CHECKLIST);
    staticSizer = new wxStaticBoxSizer(staticBox,
        wxVERTICAL);
    m_checklist = new wxTextCtrl( this, ID_CHECKLIST, MSG_NOTCONNECTED,
        wxDefaultPosition, wxSize(100,60), wxTE_MULTILINE);
    m_checklist->Disable();
    staticSizer->Add(m_checklist,
        1,          // make vertically stretchable
        wxEXPAND|   // make horizontally stretchable
        wxALL,      // and make border all around
        2 );       // set border width to 2
    topSizer->Add(staticSizer,
        1,          // make vertically stretchable
        wxEXPAND|   // make horizontally stretchable
        wxALL,      // and make border all around
        5 );       // set border width to 5

    m_diagnosticsmode = new wxCheckBox( this, ID_DIAGNOSTICSMODE, MSG_DIAGNOSTICSMODE,
        wxDefaultPosition, wxSize(100,25));
    m_diagnosticsmode->Disable();
    topSizer->Add(m_diagnosticsmode,
        0,          // make vertically stretchable
        wxEXPAND|   // make horizontally stretchable
        wxALL,      // and make border all around
        5 );       // set border width to 10
    wxBoxSizer *buttonSizer = new wxBoxSizer( wxHORIZONTAL );
    
    m_ok = new wxButton( this, wxID_OK, MSG_OK );
    m_ok->Disable();
    buttonSizer->Add(m_ok,
        0,          // make horizontally unstretchable
        wxALL,
        10 );       // make border all around: implicit top alignment
                    // set border width to 10
    buttonSizer->Add(new wxButton( this, wxID_ABOUT, MSG_ABOUT ),
        0,          // make horizontally unstretchable196
        wxALL,
        10 );       // make border all around: implicit top alignment
                    // set border width to 10
    buttonSizer->Add(new wxButton( this, wxID_HELP, MSG_HELP ),
        0,          // make horizontally unstretchable196
        wxALL,
        10 );       // make border all around: implicit top alignment
                    // set border width to 10
    buttonSizer->Add(new wxButton( this, wxID_CANCEL, MSG_CANCEL ),
        0,          // make horizontally unstretchable
        wxALL,      // make border all around (implicit top alignment)
        10 );       // set border width to 10
    topSizer->Add(buttonSizer,
        0,          // make vertically unstretchable
        wxALIGN_CENTER );   // no border and centre horizontally
    SetSizer( topSizer );   // use the sizer for layout
    topSizer->Fit( this );  // fit the dialog to the contents
    topSizer->SetSizeHints( this ); // set hints to honor min size
}

void ConfiguratorDialog::CreateMainControls()
{	
}

void ConfiguratorDialog::OnOK(wxCommandEvent & event)
{
    wxString cmd = wxString::Format(MSG_AVRRFTOOL_WRITECONFIG,
                                    m_myid->GetValue(),
                                    m_alarmlist->GetValue(),
                                    m_checklist->GetValue(),
                                    m_diagnosticsmode->GetValue());

    wxArrayString output, errors;
    int code = wxExecute(cmd, output, errors, wxEXEC_SYNC);
    if(code != 0) {
        m_timer->Start(1000,wxTIMER_ONE_SHOT);
        return;
    }
    
    if(code == 0) {
        Close();
    }
}

void ConfiguratorDialog::OnAbout(wxCommandEvent & event)
{
	wxAboutDialogInfo info;
	info.SetName(wxT("Configurator"));
	info.SetVersion(wxT(Version));
	info.SetWebSite(wxT("http://cosydvr.esy.es"));
	info.AddDeveloper(wxT("Serg Stetsuk <serg_stetsuk@ukr.net>"));
	info.AddDeveloper(wxT("Vadim Stetsuk"));
	wxAboutBox(info);
}

void ConfiguratorDialog::OnHelp(wxCommandEvent & event)
{
    wxString url = wxT("instruction.pdf");
    bool ok = false;
    wxFileType *ft = wxTheMimeTypesManager->GetFileTypeFromExtension(wxT("pdf"));
    if ( ft )
    {
        wxString cmd;
        ok = ft->GetOpenCommand(&cmd,wxFileType::MessageParameters(url, wxEmptyString));
        delete ft;
        if (ok)
        {
            ok = (wxExecute(cmd, wxEXEC_ASYNC) != 0);
        }
    }
}

void ConfiguratorDialog::OnCancel(wxCommandEvent& event)
{
    Close();
}

void ConfiguratorDialog::OnClose(wxCloseEvent& event)
{
    Destroy();
}

void ConfiguratorDialog::OnTimer(wxTimerEvent& event)
{
// Do whatever you want to do every second here
        wxString cmd = MSG_AVRRFTOOL_READCONFIG;

        wxArrayString output, errors;
        int code = wxExecute(cmd, output, errors, wxEXEC_SYNC);
    if(code != 0) {
        m_timer->Start(1000,wxTIMER_ONE_SHOT);
        return;
    }

    m_myid->Clear();
    m_alarmlist->Clear();
    m_checklist->Clear();
    
    for(int i = 0; i<output.GetCount();i++) {
        wxString rest="";
        if(output[i].StartsWith("ID: ",&rest)){
            m_myid->SetValue(rest);
        }
        if(output[i].StartsWith("ALARMLIST: ",&rest)){
            m_alarmlist->SetValue(rest);
        }
        if(output[i].StartsWith("CHECKLIST: ",&rest)){
            m_checklist->SetValue(rest);
        }
        if(output[i].StartsWith("DEBUGMODE: 1")){
            m_diagnosticsmode->SetValue(1);
        }
    }

    m_myid->Enable();
    m_alarmlist->Enable();
    m_checklist->Enable();
    m_diagnosticsmode->Enable();
    m_ok->Enable();
}
