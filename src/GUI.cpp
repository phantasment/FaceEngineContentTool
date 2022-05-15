#include "FaceEngineCT/GUI.h"
#include <iostream>

namespace FaceEngineCT
{
    // ImagePanel

    void ImagePanel::OnPaint(wxPaintEvent& event)
    {
        wxPaintDC dc(this);

        if (currentTexture != nullptr)
        {
            dc.DrawBitmap(bitmap, 0, 0);
        }
    }

    void ImagePanel::UpdateImage(Texture2D* texture)
    {
        if (texture == nullptr)
        {
            currentTexture = nullptr;
            Refresh();
            return;
        }

        bitmap.Create(texture->Width(), texture->Height());
        std::size_t pos = 0;
        wxAlphaPixelData pixelData(bitmap);
        wxAlphaPixelData::Iterator it(pixelData);
        it.OffsetY(pixelData, texture->Height() - 1);
        std::uint8_t* data = texture->Data();

        for (std::size_t y = 0; y < texture->Height(); ++y)
        {
            wxAlphaPixelData::Iterator row = it;

            for (std::size_t x = 0; x < texture->Width(); ++x)
            {
                it.Red() = data[pos++];
                it.Green() = data[pos++];
                it.Blue() = data[pos++];
                it.Alpha() = data[pos++];
                ++it;
            }

            it = row;
            it.OffsetY(pixelData, -1);
        }

        currentTexture = texture;
        Refresh();
    }

    wxBEGIN_EVENT_TABLE(ImagePanel, wxPanel)
        EVT_PAINT(ImagePanel::OnPaint)
    wxEND_EVENT_TABLE()

    // GUI

    GUI::GUI() : wxFrame(NULL, wxID_ANY, "Face Engine Content Tool", wxDefaultPosition, wxSize(800, 600))
    {
        SetSize(1280, 720);
        Maximize();
        texture = new Texture2D;

        wxMenu* fileMenu = new wxMenu;
        fileMenu->Append(ID_NEWCONTENTFILE, "&New content file...\tCtrl-N");
        fileMenu->AppendSeparator();
        fileMenu->Append(wxID_EXIT);

        wxMenu* generateMenu = new wxMenu;
        generateMenu->Append(ID_GENTEX2D, "&Texture2D...");

        wxMenuBar* menuBar = new wxMenuBar;
        menuBar->Append(fileMenu, "&File");
        menuBar->Append(generateMenu, "&Generate");
        SetMenuBar(menuBar);

        wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
        imagePanel = new ImagePanel(this);
        sizer->Add(imagePanel, 1, wxEXPAND);
        SetSizer(sizer);
    }

    void GUI::OnCreateNew(wxCommandEvent& event)
    {
        wxFileDialog dialog(this, "Open File", "", "", "Image Files (*.png)|*.png|Font Files (*.ttf)|*.ttf", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

        if (dialog.ShowModal() == wxID_CANCEL)
        {
            return;
        }

        currentPath = dialog.GetPath().ToStdString();

        try
        {
            texture->FromPNGFile(currentPath);
        }
        catch (const std::string& e)
        {
            wxMessageBox(e, "Error", wxICON_ERROR | wxOK, this);
            return;
        }

        imagePanel->UpdateImage(texture);
        SetTitle("Face Engine Content Tool [Type: Texture2D, Path: " + currentPath + ']');
    }

    void GUI::OnExit(wxCommandEvent& event)
    {
        Close(true);
    }

    void GUI::OnFrameClose(wxCloseEvent& event)
    {
        delete texture;
        Destroy();
    }

    void GUI::OnGenTexture(wxCommandEvent& event)
    {
        if (!texture->HasData())
        {
            wxMessageBox("Load a texture first.", "Error", wxICON_ERROR | wxOK, this);
            return;
        }

        wxFileDialog dialog(this, "Save File", "", "", "Face Engine Content File (*.fecf)|*.fecf", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

        if (dialog.ShowModal() == wxID_CANCEL)
        {
            return;
        }

        std::string path = dialog.GetPath().ToStdString();
        /*
        wxProgressDialog progressDialog("Generator", "Generating content file for Texture2D...", 100, this);
        progressDialog.Show();
        wxSleep(2);
        progressDialog.Update(20, "20%");
        wxSleep(2);
        progressDialog.Update(40, "40%");
        wxSleep(2);
        progressDialog.Update(60, "60%");
        wxSleep(2);
        progressDialog.Update(80, "80%");
        wxSleep(2);
        */
        ContentGenerator::GenerateTexture2D(texture, path);
        wxMessageBox("Successfully generated content file for Texture2D.", "Success", wxICON_INFORMATION | wxOK, this);
    }

    wxBEGIN_EVENT_TABLE(GUI, wxFrame)
        EVT_CLOSE(GUI::OnFrameClose)
        EVT_MENU(ID_NEWCONTENTFILE, GUI::OnCreateNew)
        EVT_MENU(wxID_EXIT, GUI::OnExit)
        EVT_MENU(ID_GENTEX2D, GUI::OnGenTexture)
    wxEND_EVENT_TABLE()

    // FaceEngineCTApp

    bool FaceEngineCTApp::OnInit()
    {
        wxInitAllImageHandlers();
        GUI* gui = new GUI;
        gui->Show(true);
        return true;
    }

    wxIMPLEMENT_APP(FaceEngineCTApp);
}