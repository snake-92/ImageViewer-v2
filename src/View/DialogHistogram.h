#ifndef DIALOGHISTOGRAM_H
#define DIALOGHISTOGRAM_H

#include <wx/wx.h>

class DialogHistogram : public wxDialog
{
    public:
        DialogHistogram(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = "Dialog", const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE);
        
        int GetValueMin() const;
        int GetValueMax() const;

    private:
        void OnSliderMinChange(wxCommandEvent& event);
        void OnSliderMaxChange(wxCommandEvent& event);

        wxSlider* m_sliderValMin;
        wxSlider* m_sliderValMax;
        wxStaticText* m_ValMin;
        wxStaticText* m_ValMax;
};

#endif // DIALOGHISTOGRAM_H