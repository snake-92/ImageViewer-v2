
#ifndef MAINFRAMEVIEW_H
#define MAINFRAMEVIEW_H

#include <wx/wx.h>
#include "BufferedBitmap.h" 
#include "IdMenu.h"
#include <map>
#include "../ViewModel/ViewModel.h"

class MainFrameView : public wxFrame
{
public:
    MainFrameView(const wxString &title, const wxPoint &pos, const wxSize &size, long style = wxDEFAULT_FRAME_STYLE);
    ~MainFrameView();

private:
    void InitMenuBar();
    void InitToolsBar();
    void InitStatusBar();

    // Event menu
    void OnLoadImage(wxCommandEvent& event);
    void OnQuit(wxCommandEvent& event);
    void OnHelp(wxCommandEvent& event);
    void OnFiltreSelect(wxCommandEvent& event);
    void OnImageOpSelect(wxCommandEvent& event);

    // Event tools
    void OnZoomIn(wxCommandEvent &event);
    void OnZoomOut(wxCommandEvent &event);

    // gestion image
    BufferedBitmap *m_bitmap;
    wxImage m_CurrentImage;
    void UpdateImage(const wxImage &image);
    
    // filtres
    wxScrolled<wxPanel>* m_panelFiltres;
    wxBoxSizer* m_SizerListFiltre;
    std::map<int, MENU_FILTRE> m_idFilterChecbox;
    void UpdateListCheckBoxFiltre(const wxString& name, MENU_FILTRE idFiltre);
    void ClickCheckBox(wxCommandEvent& evt);
    void ClearAllFilter();

    // view model
    ViewModel* m_viewModel;

    DECLARE_EVENT_TABLE()
};


#endif