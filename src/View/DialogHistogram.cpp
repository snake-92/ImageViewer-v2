#include "DialogHistogram.h"

DialogHistogram::DialogHistogram(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint &pos, const wxSize &size, long style)
 : wxDialog(parent, id, title, pos, size, style)
{
    m_chart = nullptr;
}


/// @brief initialisation de la fenetre
void DialogHistogram::Init()
{
    m_sliderValMin = new wxSlider(this, wxID_ANY, 0, 0, 255);
    m_sliderValMax = new wxSlider(this, wxID_ANY, 255, 0, 255);
    m_ValMin = new wxStaticText(this, wxID_ANY, "0");
    m_ValMax = new wxStaticText(this, wxID_ANY, "255");

    m_sliderValMin->Bind(wxEVT_SLIDER, &DialogHistogram::OnSliderMinChange, this);
    m_sliderValMax->Bind(wxEVT_SLIDER, &DialogHistogram::OnSliderMaxChange, this);

    auto sizer = new wxBoxSizer(wxVERTICAL);

    // graphe de l'histogramme
    m_chart = std::make_unique<ChartHistControl>(this, wxID_ANY, wxDefaultPosition, this->FromDIP(wxSize(640, 480))); // ChartHistControl(this, wxID_ANY, wxDefaultPosition, this->FromDIP(wxSize(640, 480)));
    m_chart->m_title = "Histogramme";
    m_chart->m_values = m_hist;
    sizer->Add(m_chart.get(), 1, wxEXPAND | wxALL, 10);

    // slider min
    auto sizerH = new wxBoxSizer(wxHORIZONTAL);
    sizerH->Add(m_sliderValMin, 0, wxEXPAND | wxALL, 10);
    sizerH->Add(m_ValMin, 0, wxEXPAND | wxALL, 10);

    // slider max
    auto sizerH2 = new wxBoxSizer(wxHORIZONTAL);
    sizerH2->Add(m_sliderValMax, 0, wxEXPAND | wxALL, 10);    
    sizerH2->Add(m_ValMax, 0, wxEXPAND | wxALL, 10);

    sizer->Add(sizerH, 0, wxEXPAND | wxALL, 10);
    sizer->Add(sizerH2, 0, wxEXPAND | wxALL, 10);

    sizer->Add(CreateButtonSizer(wxOK | wxCANCEL), 0, wxALIGN_CENTER | wxALL, FromDIP(10));
    this->SetSizerAndFit(sizer);

}

void DialogHistogram::OnSliderMinChange(wxCommandEvent& event)
{
    if(m_sliderValMax->GetValue() < event.GetInt())
    {
        m_sliderValMax->SetValue(event.GetInt());
        m_ValMax->SetLabel(wxString::Format(wxT("%d"), event.GetInt()));
    }
    m_chart->SetPosVerticalLine(event.GetInt(), m_sliderValMax->GetValue());
    m_ValMin->SetLabel(wxString::Format(wxT("%d"), event.GetInt()));
}

void DialogHistogram::OnSliderMaxChange(wxCommandEvent& event)
{
    if(m_sliderValMin->GetValue() > event.GetInt())
    {
        m_sliderValMin->SetValue(event.GetInt());
        m_ValMin->SetLabel(wxString::Format(wxT("%d"), event.GetInt()));
    }
    m_chart->SetPosVerticalLine(m_sliderValMin->GetValue(), event.GetInt());   
    m_ValMax->SetLabel(wxString::Format(wxT("%d"), event.GetInt()));
}


int DialogHistogram::GetValueMin() const
{
    return (int)m_sliderValMin->GetValue();
}

int DialogHistogram::GetValueMax() const
{
    return (int)m_sliderValMax->GetValue();
}


/// @brief copie des histogrammes de chaque composante de l'image
/// @param histRed 
/// @param histGreen 
/// @param histBlue 
void DialogHistogram::SetHistogram(const std::vector<int> &histRed, const std::vector<int> &histGreen, const std::vector<int> &histBlue)
{
    m_histRed = histRed;
    m_histGreen = histGreen;    
    m_histBlue = histBlue;

    m_hist = histRed;

    if(!histRed.empty() && !histGreen.empty() && !histBlue.empty())
    {
        // on fait la moyenne des trois histogrammes
        for(int i = 0; i < histRed.size(); i++)
        {
            m_hist[i] = (histRed[i] + histGreen[i] + histBlue[i]) / 3;
        }
    }

    Init();
}