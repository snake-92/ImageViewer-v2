#ifndef DIALOGHISTOGRAM_H
#define DIALOGHISTOGRAM_H

#include <wx/wx.h>
#include <memory>
#include "ChartHistControl.h"

class DialogHistogram : public wxDialog
{
    public:
        DialogHistogram(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = "Dialog", const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE);
        int GetValueMin() const;
        int GetValueMax() const;

        void SetHistogram(const std::vector<int> &histRed, const std::vector<int> &histGreen, const std::vector<int> &histBlue);

    private:
        void Init(); 
        void OnSliderMinChange(wxCommandEvent& event);
        void OnSliderMaxChange(wxCommandEvent& event);

        std::vector<int> m_histRed;
        std::vector<int> m_histGreen;
        std::vector<int> m_histBlue;

        std::vector<int> m_hist;

        wxSlider* m_sliderValMin;
        wxSlider* m_sliderValMax;
        wxStaticText* m_ValMin;
        wxStaticText* m_ValMax;

        std::unique_ptr<ChartHistControl> m_chart;
};

#endif // DIALOGHISTOGRAM_H