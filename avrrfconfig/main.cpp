#include "main.h"

IMPLEMENT_APP(ConfiguratorApp)

//~ BEGIN_EVENT_TABLE(ConfiguratorApp , wxApp)
    //~ EVT_CLOSE(ConfiguratorApp::OnExit)
//~ END_EVENT_TABLE()

bool ConfiguratorApp::OnInit()
{
	dlg = new ConfiguratorDialog(NULL);

    dlg->Show();
	return true;
}

int ConfiguratorApp::OnExit()
{
	return 0;
}