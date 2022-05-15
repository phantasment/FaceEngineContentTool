#ifndef FACEENGINECT_GUI_H_
#define FACEENGINECT_GUI_H_

#include "FaceEngineCT/Texture2D.h"
#include "FaceEngineCT/ContentGenerator.h"

#include <wx/wx.h>
#include <wx/rawbmp.h>
#include <wx/progdlg.h>

#define ID_NEWCONTENTFILE 1
#define ID_GENTEX2D 2

namespace FaceEngineCT
{
    class ImagePanel : public wxPanel
    {
    public:
        inline ImagePanel(wxFrame* parent) : wxPanel(parent)
        {
            currentTexture = nullptr;
        }

        void UpdateImage(Texture2D*);
    private:
        wxBitmap bitmap;
        Texture2D* currentTexture;
        void OnPaint(wxPaintEvent&);

        wxDECLARE_EVENT_TABLE();
    };

    class GUI : public wxFrame
    {
    public:
        GUI();
    private:
        ImagePanel* imagePanel;
        std::string currentPath;
        Texture2D* texture;

        void OnCreateNew(wxCommandEvent&);
        void OnExit(wxCommandEvent&);
        void OnFrameClose(wxCloseEvent&);

        void OnGenTexture(wxCommandEvent&);

        wxDECLARE_EVENT_TABLE();
    };

    class FaceEngineCTApp : public wxApp
    {
    public:
        bool OnInit() override;
    };
}

#endif