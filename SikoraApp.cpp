/***************************************************************
 * Name:      SikoraApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Egis ()
 * Created:   2026-02-11
 * Copyright: Egis ()
 * License:
 **************************************************************/

#include "SikoraApp.h"
#include "SikoraMain.h"

//(*AppHeaders
#include "SikoraMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(SikoraApp);

bool SikoraApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
        SikoraFrame* Frame = new SikoraFrame(0);
        Frame->Show();
        SetTopWindow(Frame);
    }
    //*)

    return wxsOK;

}
