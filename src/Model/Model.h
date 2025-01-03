#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED

#include "DataTreatments.h"
#include <vector>

class Model 
{
public:

    struct DataImage
    {
        cv::Mat image;
        FiltreData traitement;
    };
    
    Model();
    ~Model();
    void Init();
    void AddImageInList(DataImage data);
    cv::Mat GetImageBase();
    void SetImageBase(const cv::Mat& image);
    void SetConfigPath(const std::string& path);

    // histogramme
    void GetHistogram(const cv::Mat& img, std::vector<int>& histRed, std::vector<int>& histGreen, std::vector<int>& histBlue);
    cv::Mat AjustementHistogram(const cv::Mat& img, int valMin, int valMax);

    // filtres
    cv::Mat GaussienFilter(const cv::Mat& im_in, int size_x=3, int size_y=3, bool brefresh=false);
    cv::Mat MedianFilter(const cv::Mat& im_in, int size_ = 3, bool brefresh=false);
    cv::Mat AverageFilter(const cv::Mat& im_in, bool brefresh=false);
    cv::Mat SharpenFilter(const cv::Mat& im_in, bool brefresh=false);
    cv::Mat LaplacienFilter(const cv::Mat& im_in, bool brefresh=false);
    cv::Mat SobelXFilter(const cv::Mat& im_in, bool brefresh=false);
    cv::Mat SobelYFilter(const cv::Mat& im_in, bool brefresh=false);
    cv::Mat CannyFilter(const cv::Mat& im_in, int thresh1, int thresh2, bool brefresh=false);
    cv::Mat Threshold(const cv::Mat& im_in, bool brefresh_=false);
    cv::Mat Erode(const cv::Mat& im_in, int size_x=3, int size_y=3, int type=0, bool brefresh_=false);
    cv::Mat Dilate(const cv::Mat& im_in, int size_x=3, int size_y=3, int type=0, bool brefresh_=false);
    cv::Mat OpenFilter(const cv::Mat& im_in, int size_x=3, int size_y=3, int type=0, bool brefresh_=false);
    cv::Mat CloseFilter(const cv::Mat& im_in, int size_x=3, int size_y=3, int type=0, bool brefresh_=false);

    cv::Mat HideFilter(int idx_, bool bhide_);
    cv::Mat RemoveFilterImage(int idx_);

    // detections
    cv::Mat DetectFace();
    cv::Mat DetectFaceIA(const cv::Mat& im_in);

private:
    cv::Mat UpdateListFilterImage();
    cv::Mat ApplyFilter(DataImage data);
    void CalculHistogram(const cv::Mat& img, std::vector<int>& histRed);
    void RemiseEchelle(cv::Mat& img, int newMin, int newMax, const std::vector<int>& hist);
    cv::Mat FormatToSquare(const cv::Mat &source);

    std::vector<DataImage> m_ListDataImage; // list de toutes les images + leur traitement
    std::string m_ConfigPath;
    
};

#endif