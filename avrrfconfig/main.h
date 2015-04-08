#ifndef _MAIN_H
#define _MAIN_H

#include <wx/wx.h>
#include "maindialog.h"

class ConfiguratorApp : public wxApp
{
  public:
    virtual bool OnInit();
    int OnExit();
  private:
    ConfiguratorDialog *dlg;
};

#endif
