#ifndef FACEENGINECT_GUI_H_
#define FACEENGINECT_GUI_H_

#include <filesystem>

#include "FaceEngineCT/Texture2D.h"
#include "FaceEngineCT/TextureFont.h"
#include "FaceEngineCT/ContentGenerator.h"

#include <wx/wx.h>
#include <wx/rawbmp.h>
#include <wx/progdlg.h>
#include <wx/textdlg.h>

#define ID_NEWCONTENTFILE 1
#define ID_GENTEX2D 2
#define ID_GENTEXFONT 3

namespace FaceEngineCT
{
    class ContentDisplayPanel : public wxPanel
    {
    public:
        inline ContentDisplayPanel(wxFrame* parent) : wxPanel(parent)
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
        ContentDisplayPanel* displayPanel;
        std::filesystem::path currentPath;
        Texture2D* texture;
        TextureFont* textureFont;

        void OnCreateNew(wxCommandEvent&);
        void OnExit(wxCommandEvent&);
        void OnFrameClose(wxCloseEvent&);

        void OnGenTexture(wxCommandEvent&);
        void OnGenTexFont(wxCommandEvent&);

        wxDECLARE_EVENT_TABLE();
    };

    class FaceEngineCTApp : public wxApp
    {
    public:
        bool OnInit() override;
    };
}

#endif