/***************************************************************
 * Name:      SikoraMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Egis ()
 * Created:   2026-02-11
 * Copyright: Egis ()
 * License:
 **************************************************************/

#include "SikoraMain.h"
#include <wx/msgdlg.h>

//(*InternalHeaders(SikoraFrame)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

#include <wx/fileconf.h>
#include <wx/sound.h>

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(SikoraFrame)
const long SikoraFrame::ID_STATICTEXT1 = wxNewId();
const long SikoraFrame::ID_STATICTEXT2 = wxNewId();
const long SikoraFrame::ID_TEXTCTRL1 = wxNewId();
const long SikoraFrame::ID_TEXTCTRL2 = wxNewId();
const long SikoraFrame::ID_BUTTON1 = wxNewId();
const long SikoraFrame::ID_TEXTCTRL3 = wxNewId();
const long SikoraFrame::ID_STATICTEXT3 = wxNewId();
const long SikoraFrame::ID_STATICTEXT4 = wxNewId();
const long SikoraFrame::ID_TEXTCTRL4 = wxNewId();
const long SikoraFrame::ID_TEXTCTRL5 = wxNewId();
const long SikoraFrame::ID_STATICTEXT5 = wxNewId();
const long SikoraFrame::ID_STATICTEXT6 = wxNewId();
const long SikoraFrame::ID_TEXTCTRL6 = wxNewId();
const long SikoraFrame::ID_TEXTCTRL7 = wxNewId();
const long SikoraFrame::ID_STATICTEXT7 = wxNewId();
const long SikoraFrame::ID_CHOICE1 = wxNewId();
const long SikoraFrame::ID_BUTTON2 = wxNewId();
const long SikoraFrame::idMenuQuit = wxNewId();
const long SikoraFrame::idMenuAbout = wxNewId();
const long SikoraFrame::ID_STATUSBAR1 = wxNewId();
const long SikoraFrame::ID_TIMER1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(SikoraFrame,wxFrame)
    //(*EventTable(SikoraFrame)
    //*)
END_EVENT_TABLE()

bool                isrunning = false;
HANDLE              comthread = 0;
int                 test = 0;
HANDLE              hComm = 0;
bool                dataready = false;
unsigned char       sdata;
float               skersmuo;
float               skersmuo_min = 0;
float               skersmuo_max = 0;
FILE                *logfile;
bool                error = false;
bool                isplaying = false;
wxSound             *alarmsound;

/*-----------------------------------------------------------------------------------------------------------------------*/
SikoraFrame::SikoraFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(SikoraFrame)
    wxMenu* Menu1;
    wxMenu* Menu2;
    wxMenuBar* MenuBar1;
    wxMenuItem* MenuItem1;
    wxMenuItem* MenuItem2;

    Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("id"));
    SetClientSize(wxSize(995,693));
    StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Skersmuo"), wxPoint(24,24), wxSize(168,50), 0, _T("ID_STATICTEXT1"));
    wxFont StaticText1Font(26,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Sans"),wxFONTENCODING_DEFAULT);
    StaticText1->SetFont(StaticText1Font);
    StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Ovalumas"), wxPoint(24,88), wxSize(168,50), 0, _T("ID_STATICTEXT2"));
    StaticText2->Hide();
    wxFont StaticText2Font(26,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Sans"),wxFONTENCODING_DEFAULT);
    StaticText2->SetFont(StaticText2Font);
    skersmuostr = new wxTextCtrl(this, ID_TEXTCTRL1, wxEmptyString, wxPoint(200,24), wxSize(120,40), 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
    skersmuostr->Disable();
    skersmuostr->SetForegroundColour(wxColour(39,143,101));
    wxFont skersmuostrFont(26,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Sans"),wxFONTENCODING_DEFAULT);
    skersmuostr->SetFont(skersmuostrFont);
    TextCtrl2 = new wxTextCtrl(this, ID_TEXTCTRL2, wxEmptyString, wxPoint(208,88), wxSize(104,40), 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
    TextCtrl2->Disable();
    TextCtrl2->Hide();
    TextCtrl2->SetForegroundColour(wxColour(39,143,101));
    wxFont TextCtrl2Font(26,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Sans"),wxFONTENCODING_DEFAULT);
    TextCtrl2->SetFont(TextCtrl2Font);
    startstop = new wxButton(this, ID_BUTTON1, _("START"), wxPoint(768,24), wxSize(216,40), 0, wxDefaultValidator, _T("ID_BUTTON1"));
    wxFont startstopFont(26,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Sans"),wxFONTENCODING_DEFAULT);
    startstop->SetFont(startstopFont);
    log = new wxTextCtrl(this, ID_TEXTCTRL3, wxEmptyString, wxPoint(16,200), wxSize(968,416), wxTE_MULTILINE|wxTE_READONLY|wxTE_DONTWRAP, wxDefaultValidator, _T("ID_TEXTCTRL3"));
    StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Min."), wxPoint(336,24), wxSize(72,48), 0, _T("ID_STATICTEXT3"));
    wxFont StaticText3Font(26,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Sans"),wxFONTENCODING_DEFAULT);
    StaticText3->SetFont(StaticText3Font);
    StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("Max."), wxPoint(544,24), wxSize(72,41), 0, _T("ID_STATICTEXT4"));
    wxFont StaticText4Font(26,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Sans"),wxFONTENCODING_DEFAULT);
    StaticText4->SetFont(StaticText4Font);
    smin = new wxTextCtrl(this, ID_TEXTCTRL4, wxEmptyString, wxPoint(408,24), wxSize(120,40), 0, wxDefaultValidator, _T("ID_TEXTCTRL4"));
    wxFont sminFont(26,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Sans"),wxFONTENCODING_DEFAULT);
    smin->SetFont(sminFont);
    smax = new wxTextCtrl(this, ID_TEXTCTRL5, wxEmptyString, wxPoint(624,24), wxSize(120,40), 0, wxDefaultValidator, _T("ID_TEXTCTRL5"));
    wxFont smaxFont(26,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Sans"),wxFONTENCODING_DEFAULT);
    smax->SetFont(smaxFont);
    StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _("COM portas"), wxPoint(24,152), wxSize(200,40), 0, _T("ID_STATICTEXT5"));
    wxFont StaticText5Font(26,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Sans"),wxFONTENCODING_DEFAULT);
    StaticText5->SetFont(StaticText5Font);
    StaticText6 = new wxStaticText(this, ID_STATICTEXT6, _("X :"), wxPoint(336,88), wxSize(64,48), 0, _T("ID_STATICTEXT6"));
    StaticText6->Hide();
    wxFont StaticText6Font(26,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Sans"),wxFONTENCODING_DEFAULT);
    StaticText6->SetFont(StaticText6Font);
    skersmox = new wxTextCtrl(this, ID_TEXTCTRL6, wxEmptyString, wxPoint(408,88), wxSize(120,40), 0, wxDefaultValidator, _T("ID_TEXTCTRL6"));
    skersmox->Disable();
    skersmox->Hide();
    skersmox->SetForegroundColour(wxColour(39,143,101));
    wxFont skersmoxFont(26,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Sans"),wxFONTENCODING_DEFAULT);
    skersmox->SetFont(skersmoxFont);
    skersmuoy = new wxTextCtrl(this, ID_TEXTCTRL7, wxEmptyString, wxPoint(624,88), wxSize(120,40), 0, wxDefaultValidator, _T("ID_TEXTCTRL7"));
    skersmuoy->Disable();
    skersmuoy->Hide();
    skersmuoy->SetForegroundColour(wxColour(39,143,101));
    wxFont skersmuoyFont(26,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Sans"),wxFONTENCODING_DEFAULT);
    skersmuoy->SetFont(skersmuoyFont);
    StaticText7 = new wxStaticText(this, ID_STATICTEXT7, _("Y :"), wxPoint(544,88), wxSize(56,41), 0, _T("ID_STATICTEXT7"));
    StaticText7->Hide();
    wxFont StaticText7Font(26,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Sans"),wxFONTENCODING_DEFAULT);
    StaticText7->SetFont(StaticText7Font);
    comport = new wxChoice(this, ID_CHOICE1, wxPoint(232,160), wxSize(80,26), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE1"));
    wxFont comportFont(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT);
    comport->SetFont(comportFont);
    errreset = new wxButton(this, ID_BUTTON2, _("KLAIDA"), wxPoint(768,88), wxSize(216,40), 0, wxDefaultValidator, _T("ID_BUTTON2"));
    errreset->SetMaxSize(wxSize(0,0));
    errreset->Disable();
    wxFont errresetFont(26,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT);
    errreset->SetFont(errresetFont);
    MenuBar1 = new wxMenuBar();
    Menu1 = new wxMenu();
    MenuItem1 = new wxMenuItem(Menu1, idMenuQuit, _("Baigti darba\tAlt-F4"), _("Quit the application"), wxITEM_NORMAL);
    Menu1->Append(MenuItem1);
    MenuBar1->Append(Menu1, _("&File"));
    Menu2 = new wxMenu();
    MenuItem2 = new wxMenuItem(Menu2, idMenuAbout, _("Apie\tF1"), _("Show info about this application"), wxITEM_NORMAL);
    Menu2->Append(MenuItem2);
    MenuBar1->Append(Menu2, _("Help"));
    SetMenuBar(MenuBar1);
    StatusBar1 = new wxStatusBar(this, ID_STATUSBAR1, 0, _T("ID_STATUSBAR1"));
    int __wxStatusBarWidths_1[1] = { -1 };
    int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
    StatusBar1->SetFieldsCount(1,__wxStatusBarWidths_1);
    StatusBar1->SetStatusStyles(1,__wxStatusBarStyles_1);
    SetStatusBar(StatusBar1);
    Timer1.SetOwner(this, ID_TIMER1);

    Connect(ID_TEXTCTRL1,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&SikoraFrame::OnTextCtrl1Text);
    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SikoraFrame::OnButton1Click);
    Connect(ID_TEXTCTRL5,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&SikoraFrame::OnsmaxText);
    Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SikoraFrame::OnerrresetClick);
    Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&SikoraFrame::OnQuit);
    Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&SikoraFrame::OnAbout);
    Connect(ID_TIMER1,wxEVT_TIMER,(wxObjectEventFunction)&SikoraFrame::OnTimer1Trigger);
    //*)
    SikoraFrame::enumeratecomports ();
    comthread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) &COMThread,(void*)NULL, 0, NULL);
    Bind(wxEVT_IDLE, OnIdle, this);

    wxFileConfig* config = new wxFileConfig ("MyApp", "MyCompany", "sikoralog.ini", "", wxCONFIG_USE_LOCAL_FILE);
    wxConfigBase::Set(config);
    wxConfigBase* cfg = wxConfigBase::Get();
    smin->SetValue (cfg->Read ("/Settings/DiamMin"));
    smax->SetValue (cfg->Read ("/Settings/DiamMax"));
    cfg->Flush();
    if  ((logfile = fopen ("sikora.log","a")) == NULL)  {
        Log ((char *)"Nepavyko sukurti LOG faila");
    }
    alarmsound = new wxSound();
    alarmsound->Create(_T("alarm.wav"), false);
}
/*-----------------------------------------------------------------------------------------------------------------------*/
SikoraFrame::~SikoraFrame()
{
        wxFileConfig* config = new wxFileConfig ("MyApp", "MyCompany", "sikoralog.ini", "", wxCONFIG_USE_LOCAL_FILE);
        wxConfigBase::Set(config);
        wxConfigBase* cfg = wxConfigBase::Get();
        cfg->Write("/Settings/DiamMin", smin->GetValue ());
        cfg->Write("/Settings/DiamMax", smax->GetValue ());
        cfg->Flush();
        fclose (logfile);
}
/*-----------------------------------------------------------------------------------------------------------------------*/
void SikoraFrame::OnQuit(wxCommandEvent& event)
{
    exit (0);
}
/*-----------------------------------------------------------------------------------------------------------------------*/

void SikoraFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("Sikora data logger", _("Sikora v0.2"));
}
/*-----------------------------------------------------------------------------------------------------------------------*/

void SikoraFrame::OnTextCtrl1Text(wxCommandEvent& event)
{

}
/*-----------------------------------------------------------------------------------------------------------------------*/
   DWORD WINAPI SikoraFrame::COMThread(_In_ LPVOID lpParameter)
{
        BOOL    Status;
        DWORD   NoBytesRead;
        char    _skersmuo[32];

        while (1)    {
            if  (isrunning && !dataready) {
                Status = ReadFile(hComm, &sdata, sizeof(sdata), &NoBytesRead, NULL);
                if (Status && NoBytesRead > 0)  {
                    if  (sdata == 0x3f) {// suomenu paketo markeris
                        memset (_skersmuo,0,sizeof (_skersmuo));
                        Status = ReadFile(hComm, _skersmuo, 6, &NoBytesRead, NULL);
                        if  (Status && NoBytesRead == 6)    {
                            sscanf (_skersmuo,"%f",&skersmuo);
                            skersmuo = skersmuo / 1000;
                            dataready = true;
                        }
                    }
                }
            }
        }
        return 0;
}
/*-----------------------------------------------------------------------------------------------------------------------*/
    void SikoraFrame::OnIdle(wxIdleEvent &event)
    {
        char str[100];

        if  (dataready) {
            sprintf (str,"%3.3f",skersmuo);
            skersmuostr->SetValue (str);
            dataready = false;
            if (skersmuo > skersmuo_max || skersmuo < skersmuo_min) {
                sprintf (str,"Skersmens klaida ! skersmuo %3.3f, min - %3.3f, max - %3.3f",skersmuo, skersmuo_min, skersmuo_max);
                Log ((char *)str);
                error = true;
                errreset->Enable ();
                errreset->SetBackgroundColour("red");
                errreset->Update ();
                if  (!isplaying) {
                    isplaying = true;
                    alarmsound->Play (wxSOUND_ASYNC|wxSOUND_LOOP);
                }
            }
        }
        event.RequestMore();
    }
/*-----------------------------------------------------------------------------------------------------------------------*/
void SikoraFrame::OnButton1Click(wxCommandEvent& event)
{
    if  (isrunning) {
        isrunning = false;
        startstop->SetLabel("START");
        startstop->SetBackgroundColour("");
        Log ((char *)"Programa sustabdyta");
        smin->Enable();
        smax->Enable();
        comport->Enable ();
        Timer1.Stop ();
        CloseHandle (hComm);
    } else {
        if  (comport->GetCount () == 0) {
            Log ((char *)"Nera prijungtu irenginiu, patikrinkite kabeli i kompiuteri");
            return;
        }
        if  (comport->GetSelection () == wxNOT_FOUND)   {
            Log ((char *)"Pasirinkite COM porta");
            return;
        }
        sscanf (smin->GetValue (),"%f",&skersmuo_min);
        if  (skersmuo_min == 0) {
            Log ((char *)"Nenurodytas minimalus skersmuo");
            return;
        }
        sscanf (smax->GetValue (),"%f",&skersmuo_max);
        if  (skersmuo_max == 0) {
            Log ((char *)"Nenurodytas maksimalus skersmuo");
            return;
        }
        if  (skersmuo_min >= skersmuo_max)  {
            Log ((char *)"Minimalus skersmuo negali buti didesnis arba lygus maksimaliam skersmeniui");
            return;
        }
        DCB dcbSerialParams = { 0 };
        hComm = CreateFile(comport->GetStringSelection() , GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
        if (hComm == INVALID_HANDLE_VALUE) {
            Log ((char *)"Nepavyko atidaryti COM porta");
            return;
        }
        dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
        BOOL   Status;

        Status = GetCommState(hComm, &dcbSerialParams); //retreives  the current settings
        if (Status == FALSE)    {
            Log ((char *)"Nepavyko nuskaityti COM parametru");
            return;
        }
        dcbSerialParams.BaudRate = CBR_9600;      //BaudRate = 9600
        dcbSerialParams.ByteSize = 8;             //ByteSize = 8
        dcbSerialParams.StopBits = ONESTOPBIT;    //StopBits = 1
        dcbSerialParams.Parity = NOPARITY;      //Parity = None
        Status = SetCommState(hComm, &dcbSerialParams);
        if (Status == FALSE)    {
            Log ((char *)"Nepavyko nustatyti COM DCB");
            return;
        }
        COMMTIMEOUTS timeouts = { 0 };

        timeouts.ReadIntervalTimeout = 100;
        timeouts.ReadTotalTimeoutConstant = 100;
        timeouts.ReadTotalTimeoutMultiplier = 10;
        if (SetCommTimeouts(hComm, &timeouts) == FALSE)
        {
            Log ((char *)"Error to Setting Time outs");
            return;
        }
        startstop->SetLabel("STOP");
        startstop->SetBackgroundColour("green");
        startstop->Update ();
        Log ((char *)"Programa vykdoma");
        smin->Disable();
        smax->Disable();
        comport->Disable ();
        isrunning = true;

    }
}
/*-----------------------------------------------------------------------------------------------------------------------*/
void    SikoraFrame::enumeratecomports ()
{
        TCHAR lpTargetPath[1000];
        DWORD test;

        for (int i = 1;i < 99;i++) {
            std::string str2 = "COM" + std::to_string(i);
            std::wstring stemp = std::wstring(str2.begin(), str2.end());
            test = QueryDosDevice(stemp.c_str (),(LPWSTR)lpTargetPath, 1000);
            if  (test != 0)
                comport->Append (stemp);
        }
        comport->SetSelection (0);
}
/*-----------------------------------------------------------------------------------------------------------------------*/
void    SikoraFrame::Log (char *data)
{
        char    logstr[1024];

        time_t t = time(NULL);
        struct tm tm = *localtime(&t);

        sprintf (logstr,"%d-%02d-%02d %02d:%02d:%02d %s\n",tm.tm_year + 1900,tm.tm_mon + 1,tm.tm_mday,tm.tm_hour, tm.tm_min, tm.tm_sec, data );
        log->AppendText (logstr);
        fputs (logstr,logfile);
}
/*-----------------------------------------------------------------------------------------------------------------------*/
void SikoraFrame::OnTimer1Trigger(wxTimerEvent& event)
{
    Log ((char *)"timer");
}

/*-----------------------------------------------------------------------------------------------------------------------*/

void SikoraFrame::OnsmaxText(wxCommandEvent& event)
{
}
/*-----------------------------------------------------------------------------------------------------------------------*/
void SikoraFrame::OnerrresetClick(wxCommandEvent& event)
{
        error = false;
        isplaying = false;
        alarmsound->Stop ();
        errreset->Disable ();
        errreset->SetBackgroundColour("");
}
