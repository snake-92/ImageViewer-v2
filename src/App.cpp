/*
    Name : SIDJE NZALI Ghislain
    Creation Date : 20/09/2024
    Description : main class of the viewer application 
*/

#include <iostream>
#include "View/MainFrameView.h"


class MyApp : public wxApp 
{
public:
    virtual bool OnInit();
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    wxInitAllImageHandlers(); // permet tous les évenements sur l'image

    MainFrameView *frame = new MainFrameView("Hello World", wxDefaultPosition, wxSize(1280, 720));
    frame->Show(true);
    return true;
}

