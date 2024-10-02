#ifndef DIALOGCONFIGFILTER_H
#define DIALOGCONFIGFILTER_H

#include <wx/wx.h>
#include <utility>
#include <array>
#include "IdMenu.h"


class DialogConfigFilter : public wxDialog
{
    public:
        DialogConfigFilter(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = "Dialog", const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE, const wxString& name="");

        void InitDialog(MENU_FILTRE filter);
        std::array<int,3> GetParams();

    private:
        void DialogGaussien();
        void DialogMedian();
        void DialogCanny();
        void DialogMorpho();

        std::array<int,3> m_params; // chaque parametre occupe une case 
        wxString m_typeMorph; // uniquement pour le type du filtre morpho
};

#endif // DIALOGCONFIGFILTER_H