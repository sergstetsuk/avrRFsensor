//http://wxwidgets.info/wxaui_tutorial_2_ru/
//http://zetcode.com/gui/wxwidgets/
#ifndef _MainDialog_H
#define _MainDialog_H

#include <wx/wx.h>
#include <wx/dialog.h>
#include <wx/timer.h>
#include <wx/mimetype.h>

#define Version "v.0.1"
#define ConfiguratorDialogTitle wxT("avrRFconfigurator")
#define MSG_AVRRFTOOL_READCONFIG "./modules/avrrftool.exe -o readconfig"
#define MSG_AVRRFTOOL_WRITECONFIG "./modules/avrrftool.exe  -o writeconfig -i %s -a %s -c %s -d %d"
//~ #define MSG_NOTCONNECTED wxT("Plug in RF module into USB socket")
//~ #define MSG_ID wxT("&ID")
//~ #define MSG_ALARMLIST wxT("&AlarmList")
//~ #define MSG_CHECKLIST wxT("&CheckList")
//~ #define MSG_DIAGNOSTICSMODE wxT("&Diagnostics Mode")
//~ #define MSG_OK wxT("OK")
//~ #define MSG_ABOUT wxT("About")
//~ #define MSG_HELP wxT("Help")
//~ #define MSG_CANCEL wxT("Cancel")
#define MSG_NOTCONNECTED wxT("Підключіть модуль в USB роз'єм.")
#define MSG_ID wxT("&Ідентифікатор")
#define MSG_ALARMLIST wxT("Список &Оповіщення")
#define MSG_CHECKLIST wxT("Список &Контролю")
#define MSG_DIAGNOSTICSMODE wxT("Режим &Діагностики")
#define MSG_OK wxT("Зберегти")
#define MSG_ABOUT wxT("Про програму ")
#define MSG_HELP wxT("Допомога")
#define MSG_CANCEL wxT("Відміна")

enum
{
    ID_MYID = 10001,
    ID_ALARMLIST,
    ID_CHECKLIST,
    ID_DIAGNOSTICSMODE,
    ID_TIMER
};

class ConfiguratorDialog : public wxDialog
{
public:
	ConfiguratorDialog();
	ConfiguratorDialog(wxWindow * parent, wxWindowID id = wxID_ANY,
		const wxString & title = ConfiguratorDialogTitle);
	~ConfiguratorDialog();
	bool Create(wxWindow * parent, wxWindowID id = wxID_ANY,
		const wxString & title = ConfiguratorDialogTitle);

    void CreateWaitControls();
    void CreateMainControls();

    DECLARE_EVENT_TABLE()
    void OnOK(wxCommandEvent & event);
    void OnInfo(wxCommandEvent & event);
    void OnAbout(wxCommandEvent & event);
    void OnHelp(wxCommandEvent & event);
    void OnCancel(wxCommandEvent & event);
    void OnClose(wxCloseEvent & event);
    void OnTimer(wxTimerEvent& event);
private:
    wxTimer* m_timer;
    wxTextCtrl* m_myid;
    wxTextCtrl* m_alarmlist;
    wxTextCtrl* m_checklist;
    wxCheckBox* m_diagnosticsmode;
    wxButton* m_ok;
};

#endif
