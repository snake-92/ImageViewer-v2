#include "ChartHistControl.h"

#include <wx/settings.h>
#include <wx/graphics.h>
#include <wx/dcbuffer.h>


ChartHistControl::ChartHistControl(wxWindow* parent, wxWindowID id, const wxPoint &pos, const wxSize &size) :wxWindow(parent, id, pos, size, wxFULL_REPAINT_ON_RESIZE)
{
    this->SetBackgroundStyle(wxBG_STYLE_PAINT);
    this->Bind(wxEVT_PAINT, &ChartHistControl::OnPaint, this);
}

void ChartHistControl::OnPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);
    dc.Clear();

    wxGraphicsContext* gc = wxGraphicsContext::Create(dc);

    if(gc && m_values.size() > 0)
    {
        wxFont titleFont = wxFont(wxNORMAL_FONT->GetPointSize()*2.0, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
        gc->SetFont(titleFont, wxSystemSettings::GetAppearance().IsDark()?*wxWHITE:*wxBLACK);
        
        double tw,th;
        gc->GetTextExtent(m_title, &tw, &th);

        const int titleTopBottomMargin = this->FromDIP(10);

        wxRect2DDouble  fullArea{0, 0, static_cast<double>(GetSize().GetWidth()), static_cast<double>(GetSize().GetHeight())};

        const double marginX = fullArea.GetSize().GetWidth() / 8.0;
        const double marginTop = std::max(fullArea.GetSize().GetHeight() / 8.0, titleTopBottomMargin * 2.0 + th);
        const double marginBottom = fullArea.GetSize().GetHeight()/8.0;
        double labelsToChartAreaMarin = this->FromDIP(10);

        wxRect2DDouble  chartArea = fullArea;
        chartArea.Inset(marginX, marginTop, marginX, marginBottom);

        gc->DrawText(m_title, (fullArea.GetSize().GetWidth()-tw)/2.0, (marginTop-th)/2.0);
        gc->SetBrush(*wxWHITE_BRUSH);
        gc->DrawRectangle(chartArea.m_x, chartArea.m_y, chartArea.m_width, chartArea.m_height);

        wxAffineMatrix2D matrix{};
        matrix.Translate(chartArea.GetLeft(), chartArea.GetTop());
        matrix.Scale(chartArea.m_width, chartArea.m_height);

        int highValue = *std::max_element(m_values.begin(), m_values.end());;
        int lowValue = *std::min_element(m_values.begin(), m_values.end());;
        double yValueSpan = static_cast<double>(highValue - lowValue);

        wxAffineMatrix2D normalizedToValue{};
        normalizedToValue.Translate(0, highValue);
        normalizedToValue.Scale(1, -1);
        normalizedToValue.Scale(static_cast<double>(m_values.size() - 1), yValueSpan);

        const int yLinesCount = 11;

        gc->SetPen(wxPen(wxColor(128,128,128)));
        gc->SetFont(*wxNORMAL_FONT, wxSystemSettings::GetAppearance().IsDark() ? *wxWHITE : *wxBLACK);

        for(int i=0;i<yLinesCount;i++)
        {
            double normalizedY = static_cast<double>(i)/(yLinesCount-1);

            auto lineStartPoint = matrix.TransformPoint({0, normalizedY});
            auto lineEndPoint = matrix.TransformPoint({1, normalizedY});

            wxPoint2DDouble linePoints[]={lineStartPoint, lineEndPoint};
            gc->StrokeLines(2, linePoints);

            double valueAtLineY = normalizedToValue.TransformPoint({0, normalizedY}).m_y;
            auto text = wxString::Format("%.2f", valueAtLineY);
            text = wxControl::Ellipsize(text, dc, wxELLIPSIZE_MIDDLE, chartArea.GetLeft()-labelsToChartAreaMarin);

            double tw1,th1;
            gc->GetTextExtent(text, &tw1, &th1);
            gc->DrawText(text, chartArea.GetLeft()-labelsToChartAreaMarin-tw1, lineStartPoint.m_y+th1/2.0);
        }

        wxPoint2DDouble leftHLinePoints[]{matrix.TransformPoint({0,0}), matrix.TransformPoint({0,1})};
        wxPoint2DDouble rightHLinePoints[]{matrix.TransformPoint({1,0}), matrix.TransformPoint({1,1})};
        gc->StrokeLines(2, leftHLinePoints);
        gc->StrokeLines(2, rightHLinePoints);

        wxPoint2DDouble *pointArray = new wxPoint2DDouble[256];

        wxAffineMatrix2D valueToNormalized = normalizedToValue;
        valueToNormalized.Invert();
        wxAffineMatrix2D valueToChartArea = matrix;
        valueToChartArea.Concat(valueToNormalized);
        
        for (int i = 0; i < m_values.size(); i++)
        {
            pointArray[i] = valueToChartArea.TransformPoint({static_cast<double>(i), static_cast<double>(m_values[i])});
        }

        gc->SetPen(wxPen(wxSystemSettings::GetAppearance().IsDark() ? *wxCYAN : *wxBLUE, 3));
        gc->StrokeLines(m_values.size(), pointArray);

        delete[] pointArray;

        delete gc;
    }
}