/***************************************************************
 * Name:      wxprjApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Crope (cropemail@gmail.com)
 * Created:   2016-11-06
 * Copyright: Crope ()
 * License:
 **************************************************************/

#include "wxprjApp.h"

//(*AppHeaders
#include "wxprjMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(wxprjApp);

bool wxprjApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	wxprjFrame* Frame = new wxprjFrame(0);
    	Frame->Show();
    	SetTopWindow(Frame);
    }
    //*)
    return wxsOK;

}
