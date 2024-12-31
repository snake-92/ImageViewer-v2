#ifndef CHARTHISTCONTROL_H
#define CHARTHISTCONTROL_H

#include <wx/wx.h>
#include <vector>
#include <string>


class ChartHistControl : public wxWindow
{
    public:
        ChartHistControl(wxWindow* parent, wxWindowID id, const wxPoint &pos, const wxSize &size);

        void SetPosVerticalLine(int xmin, int xmax);
        std::vector<int> m_values;
        std::string m_title;
        
    private:
        void OnPaint(wxPaintEvent& event);
        double m_Xmin;
        double m_Xmax;
};

#endif // CHARTHISTCONTROL_H