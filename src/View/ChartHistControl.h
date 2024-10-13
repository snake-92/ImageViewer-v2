#ifndef CHARTHISTCONTROL_H
#define CHARTHISTCONTROL_H

#include <wx/wx.h>
#include <vector>
#include <string>


class ChartHistControl : public wxWindow
{
    public:
        ChartHistControl(wxWindow* parent, wxWindowID id, const wxPoint &pos, const wxSize &size);

        std::vector<int> m_values;
        std::string m_title;
    private:
        void OnPaint(wxPaintEvent& event);
};

#endif // CHARTHISTCONTROL_H