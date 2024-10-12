#include "ViewModel.h"


ViewModel::ViewModel(wxImage& image) : m_Image(image)
{
    m_model = new Model();
}


ViewModel::~ViewModel()
{
    delete m_model;
}

/// @brief recupération du chemin à partir duquel se trouve le .exe et de là on a la config
/// @param path 
void ViewModel::SetConfigPath(const wxString& path)
{
    m_model->SetConfigPath(path.ToStdString());
}

void ViewModel::SetImageBase()
{
    cv::Mat cvImage = ConvertWxImageToCvMat(m_Image); // on converti d'abord l'image wxWidget en cvMat avant de l'envoyer au model
    m_model->Init(); // initialisation de la liste des images
    m_model->AddImageInList({cvImage, {true, TREATEMENT_TYPE::ID_FILTRE_NONE}}); // ajout de l'image dans la liste des images
}

/// @brief Reset de l'image de base
void ViewModel::ResetImageBase()
{
    cv::Mat cvImage = ConvertWxImageToCvMat(m_Image); // on converti d'abord l'image wxWidget en cvMat avant de l'envoyer au model
    m_model->SetImageBase(cvImage);
    CopyCvMatToWxImage(cvImage);
}

/// @brief Initialisation de la liste des filtres
void ViewModel::InitListFiltre()
{
    cv::Mat imBase = m_model->GetImageBase();
    m_model->Init();
    m_model->AddImageInList({imBase, {true, TREATEMENT_TYPE::ID_FILTRE_NONE}}); 
    CopyCvMatToWxImage(imBase); // convertion en wxImage
}


/// @brief Conversion de l'image wxImage en cv::Mat
/// @param image_ 
/// @return image cv::Mat
cv::Mat ViewModel::ConvertWxImageToCvMat(wxImage& image_)
{
    // on cree une image 3 channel 8 bits
    // on travaille uniquement en 8 bits
    cv::Mat img = cv::Mat::zeros(image_.GetHeight(), image_.GetWidth(), CV_8UC3);

    try
    {
        for(int y=0;y<image_.GetHeight();y++)
        {
            for(int x=0;x<image_.GetWidth();x++)
            {
                img.at<cv::Vec3b>(y,x)[2] = image_.GetRed(x,y);
                img.at<cv::Vec3b>(y,x)[1] = image_.GetGreen(x,y);
                img.at<cv::Vec3b>(y,x)[0] = image_.GetBlue(x,y);
            }
        }
    }
    catch(const std::exception& e)
    {
        throw e.what();
    }

    return img;
}


/// @brief Conversion cv::Mat en wxImage
/// @param img_ 
/// @param bOneChannel 
void ViewModel::CopyCvMatToWxImage(const cv::Mat& img_)
{
    try
    {
        if(img_.channels()==1) // si image 1 channel, on met la même la même couche sur les cannaux R G et B 
        {
            for(int y=0;y<img_.rows;y++)
            {
                for(int x=0;x<img_.cols;x++)
                {
                    m_Image.SetRGB(x, y, img_.at<uchar>(y,x), img_.at<uchar>(y,x), img_.at<uchar>(y,x));
                }
            }
        }
        else
        {
            for(int y=0;y<img_.rows;y++)
            {
                for(int x=0;x<img_.cols;x++)
                {
                    m_Image.SetRGB(x, y, img_.at<cv::Vec3b>(y,x)[2], img_.at<cv::Vec3b>(y,x)[1], img_.at<cv::Vec3b>(y,x)[0]);
                }
            }
        }
    }
    catch(const std::exception& e)
    {
        throw e.what();
    }
}


/// @brief histogramme de l'image de base
/// @param histRed 
/// @param histGreen 
/// @param histBlue 
void ViewModel::GetHistogram(std::vector<int>& histRed, std::vector<int>& histGreen, std::vector<int>& histBlue)
{
    cv::Mat cvImage = ConvertWxImageToCvMat(m_Image);
    m_model->GetHistogram(cvImage, histRed, histGreen, histBlue);
}


/// @brief etalement d'histogramme
/// @param newMin 
/// @param newMax 
void ViewModel::ApplyHistogrammeStreching(int newMin, int newMax)
{
    cv::Mat cvImage = ConvertWxImageToCvMat(m_Image);
    cv::Mat imOut = m_model->AjustementHistogram(cvImage, newMin, newMax);
    CopyCvMatToWxImage(imOut);
}


/// @brief Cache le filtre dont l'index est passé en paramètre
/// @param idx_ : index du filtre 
/// @param bhide_ : booleen pour savoir si cacher ou non le filtre true cache, false affiche
void ViewModel::HideFilter(int idx_, bool bhide_)
{
    cv::Mat img = m_model->HideFilter(idx_, bhide_);
    // convertion en wxImage
    CopyCvMatToWxImage(img);
}

void ViewModel::Gaussien(int size_x, int size_y)
{
    cv::Mat img = ConvertWxImageToCvMat(m_Image); // conversion en cv::Mat
    cv::Mat imOut = m_model->GaussienFilter(img, size_x, size_y);
    CopyCvMatToWxImage(imOut); // convertion en wxImage
}

void ViewModel::Median(int size_)
{    
    cv::Mat img = ConvertWxImageToCvMat(m_Image); // conversion en cv::Mat
    cv::Mat imOut = m_model->MedianFilter(img, size_); 
    CopyCvMatToWxImage(imOut); // convertion en wxImage
}

void ViewModel::Average()
{   
    cv::Mat img = ConvertWxImageToCvMat(m_Image); // conversion en cv::Mat
    cv::Mat imOut = m_model->AverageFilter(img);
    CopyCvMatToWxImage(imOut); // convertion en wxImage
}

void ViewModel::Sharpen()
{
    cv::Mat img = ConvertWxImageToCvMat(m_Image); // conversion en cv::Mat
    cv::Mat imOut = m_model->SharpenFilter(img);
    CopyCvMatToWxImage(imOut); // convertion en wxImage
}

void ViewModel::LaplacienFilter()
{
    cv::Mat img = ConvertWxImageToCvMat(m_Image); // conversion en cv::Mat
    cv::Mat imOut = m_model->LaplacienFilter(img);
    CopyCvMatToWxImage(imOut); // convertion en wxImage
}

void ViewModel::SobelXFilter()
{
    cv::Mat img = ConvertWxImageToCvMat(m_Image); // conversion en cv::Mat
    cv::Mat imOut = m_model->SobelXFilter(img);
    CopyCvMatToWxImage(imOut); // convertion en wxImage
}

void ViewModel::SobelYFilter()
{
    cv::Mat img = ConvertWxImageToCvMat(m_Image); // conversion en cv::Mat
    cv::Mat imOut = m_model->SobelYFilter(img);
    CopyCvMatToWxImage(imOut); // convertion en wxImage
}

void ViewModel::CannyFilter(int thresh1, int thresh2)
{
    cv::Mat img = ConvertWxImageToCvMat(m_Image); // conversion en cv::Mat
    cv::Mat imOut = m_model->CannyFilter(img,thresh1,thresh2);
    CopyCvMatToWxImage(imOut); // convertion en wxImage
}

void ViewModel::Threshold()
{
    cv::Mat img = ConvertWxImageToCvMat(m_Image); // conversion en cv::Mat
    cv::Mat imOut = m_model->Threshold(img);
    CopyCvMatToWxImage(imOut); // convertion en wxImage
}

void ViewModel::Erode(int size_x, int size_y, int type)
{
    cv::Mat img = ConvertWxImageToCvMat(m_Image); // conversion en cv::Mat
    cv::Mat imOut = m_model->Erode(img,size_x,size_y,type);
    CopyCvMatToWxImage(imOut); // convertion en wxImage
}

void ViewModel::Dilate(int size_x, int size_y, int type)
{
    cv::Mat img = ConvertWxImageToCvMat(m_Image); // conversion en cv::Mat
    cv::Mat imOut = m_model->Dilate(img,size_x,size_y,type);
    CopyCvMatToWxImage(imOut); // convertion en wxImage
}

void ViewModel::OpenFilter(int size_x, int size_y, int type)
{
    cv::Mat img = ConvertWxImageToCvMat(m_Image); // conversion en cv::Mat
    cv::Mat imOut = m_model->OpenFilter(img,size_x,size_y,type);
    CopyCvMatToWxImage(imOut); // convertion en wxImage
}

void ViewModel::CloseFilter(int size_x, int size_y, int type)
{
    cv::Mat img = ConvertWxImageToCvMat(m_Image); // conversion en cv::Mat
    cv::Mat imOut = m_model->CloseFilter(img,size_x,size_y,type);
    CopyCvMatToWxImage(imOut); // convertion en wxImage
}


/// @brief Detection de visage
void ViewModel::DetectFace()
{
    cv::Mat imOut = m_model->DetectFace();
    CopyCvMatToWxImage(imOut); // convertion en wxImage
}