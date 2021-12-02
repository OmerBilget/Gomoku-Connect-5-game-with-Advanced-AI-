#include "gomokuApp.h"

wxIMPLEMENT_APP(gomokuApp);
gomokuApp::gomokuApp()
{
    //ctor
}

gomokuApp::~gomokuApp()
{
    //dtor
}

bool gomokuApp::OnInit(){
    FRAME=new main();
    FRAME->Show();

    return true;
}
