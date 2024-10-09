#ifndef VIEWMODELCLASS_H
#define VIEWMODELCLASS_H

#include "wx/wx.h"
#include "../Model/Model.h"

class ViewModel
{
    public:
        ViewModel(wxImage& image);
        ~ViewModel();

    void InitListFiltre();
    void SetImageBase();
    void HideFilter(int idx_, bool bhide_);
    void ResetImageBase();
    void SetConfigPath(const wxString& path);

    // filtres
    void Gaussien(int size_x, int size_y);
    void Median(int size_);
    void Average();
    void Sharpen();
    void LaplacienFilter();
    void SobelXFilter();
    void SobelYFilter();
    void CannyFilter(int thresh1, int thresh2);
    void Threshold();
    void Erode(int size_x, int size_y, int type);
    void Dilate(int size_x, int size_y, int type);
    void OpenFilter(int size_x, int size_y, int type);
    void CloseFilter(int size_x, int size_y, int type);

    // detections
    void DetectFace();

    protected:
        Model* m_model;

    private:
        cv::Mat ConvertWxImageToCvMat(wxImage& image_);
        void CopyCvMatToWxImage(const cv::Mat& img_);
        
        wxImage& m_Image;
};

#endif // VIEWMODELCLASS_H