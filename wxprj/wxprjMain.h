/***************************************************************
 * Name:      wxprjMain.h
 * Purpose:   Defines Application Frame
 * Author:    Crope (cropemail@gmail.com)
 * Created:   2016-11-06
 * Copyright: Crope ()
 * License:
 **************************************************************/

#ifndef WXPRJMAIN_H
#define WXPRJMAIN_H

//(*Headers(wxprjFrame)
#include <wx/menu.h>
#include <wx/frame.h>
#include <wx/statusbr.h>
//*)

class wxprjFrame: public wxFrame
{
    public:

        wxprjFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~wxprjFrame();

    private:

        //(*Handlers(wxprjFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnClose(wxCloseEvent& event);
        //*)

        //(*Identifiers(wxprjFrame)
        static const long idMenuQuit;
        static const long idMenuAbout;
        static const long ID_STATUSBAR1;
        //*)

        //(*Declarations(wxprjFrame)
        wxStatusBar* StatusBar1;
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // WXPRJMAIN_H
