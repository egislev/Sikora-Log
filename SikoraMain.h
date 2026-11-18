/***************************************************************
 * Name:      SikoraMain.h
 * Purpose:   Defines Application Frame
 * Author:    Egis ()
 * Created:   2026-02-11
 * Copyright: Egis ()
 * License:
 **************************************************************/

#ifndef SIKORAMAIN_H
#define SIKORAMAIN_H

#include <string>
#include <list>

//(*Headers(SikoraFrame)
#include <wx/button.h>
#include <wx/choice.h>
#include <wx/frame.h>
#include <wx/menu.h>
#include <wx/stattext.h>
#include <wx/statusbr.h>
#include <wx/textctrl.h>
#include <wx/timer.h>
//*)
#include <wx/thread.h>
#include <winsock2.h>
#include <windows.h>

class SikoraFrame: public wxFrame
{
    public:

        SikoraFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~SikoraFrame();
    private:

        //(*Handlers(SikoraFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnTextCtrl1Text(wxCommandEvent& event);
        void OnButton1Click(wxCommandEvent& event);
        void OnTimer1Trigger(wxTimerEvent& event);
        void OnsmaxText(wxCommandEvent& event);
        void OnerrresetClick(wxCommandEvent& event);
        //*)
        //(*Identifiers(SikoraFrame)
        static const long ID_STATICTEXT1;
        static const long ID_STATICTEXT2;
        static const long ID_TEXTCTRL1;
        static const long ID_TEXTCTRL2;
        static const long ID_BUTTON1;
        static const long ID_TEXTCTRL3;
        static const long ID_STATICTEXT3;
        static const long ID_STATICTEXT4;
        static const long ID_TEXTCTRL4;
        static const long ID_TEXTCTRL5;
        static const long ID_STATICTEXT5;
        static const long ID_STATICTEXT6;
        static const long ID_TEXTCTRL6;
        static const long ID_TEXTCTRL7;
        static const long ID_STATICTEXT7;
        static const long ID_CHOICE1;
        static const long ID_BUTTON2;
        static const long idMenuQuit;
        static const long idMenuAbout;
        static const long ID_STATUSBAR1;
        static const long ID_TIMER1;
        //*)

        //(*Declarations(SikoraFrame)
        wxButton* errreset;
        wxButton* startstop;
        wxChoice* comport;
        wxStaticText* StaticText1;
        wxStaticText* StaticText2;
        wxStaticText* StaticText3;
        wxStaticText* StaticText4;
        wxStaticText* StaticText5;
        wxStaticText* StaticText6;
        wxStaticText* StaticText7;
        wxStatusBar* StatusBar1;
        wxTextCtrl* TextCtrl2;
        wxTextCtrl* log;
        wxTextCtrl* skersmox;
        wxTextCtrl* skersmuostr;
        wxTextCtrl* skersmuoy;
        wxTextCtrl* smax;
        wxTextCtrl* smin;
        wxTimer Timer1;
        //*)
        void  enumeratecomports ();
        void  Log (char *data);
        DWORD WINAPI  COMThread(void* threadParams);
        void  OnIdle(wxIdleEvent &event);
        DECLARE_EVENT_TABLE()
};

#endif // SIKORAMAIN_H
