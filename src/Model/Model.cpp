#include "Model.h"


Model::Model()
{

}

Model::~Model()
{
    Init();
}

/// @brief vide la liste des images
void Model::Init()
{
   m_ListDataImage.clear();
}


/// @brief Ajouter une image + filtre associé dans la liste des images
/// @param data 
void Model::AddImageInList(DataImage data)
{
   m_ListDataImage.push_back(data);
}

/// @brief retourne l'image de base qui correspond à la première image dans la liste
/// @return 
cv::Mat Model::GetImageBase()
{
   if(!m_ListDataImage.empty())
   {
      return m_ListDataImage.front().image;
   }else
   {
      throw "erreur image";
   }
}


/// @brief Filtre gaussien
/// @param im_in : image en entrée
/// @param size_x : longueur x du filtre
/// @param size_y : longueur y du filtre
/// @param brefresh : booleen pour savoir si ajoute le filtre à la liste ou non
/// @return 
cv::Mat Model::GaussienFilter(const cv::Mat& im_in, int size_x/*=3*/, int size_y/*=3*/, bool brefresh/*=false*/)
{
   // on vérifie que les dimensions du filtre restent impair
   if(size_x%2 == 0)
        size_x++;
    if(size_y%2 == 0)
        size_y++;

   cv::Mat im_out;
   cv::GaussianBlur(im_in, im_out, cv::Size(size_x, size_y), 3); // le sigma est fixé à 3

   if(!brefresh)
   {
      // donnée filtre gaussien
      DataImage data;
      data.image = im_out;
      Gaussien filtre;
      filtre.idTreatement = TREATEMENT_TYPE::ID_FILTRE_GAUSSIEN;
      filtre.visible=true;
      filtre.sizeX=size_x;
      filtre.sizeY=size_y;
      data.traitement = filtre;
      AddImageInList(data);
   }

   return im_out;
}


/// @brief Filtre médian
/// @param im_in : image en entrée
/// @param size_ : longueur du filtre
/// @param brefresh : booleen pour savoir si ajoute le filtre à la liste ou non
/// @return 
cv::Mat Model::MedianFilter(const cv::Mat& im_in, int size_ /*= 3*/, bool brefresh/*=false*/)
{
   // la taille du filtre doit être impaire
   if(size_%2 == 0)
        size_++;

    cv::Mat im_out;
    cv::medianBlur(im_in, im_out, size_);

    if(!brefresh)
    {
      // donnée filtre median
      DataImage data;
      data.image = im_out;
      Median filtre;
      filtre.idTreatement = TREATEMENT_TYPE::ID_FILTRE_MEDIAN;
      filtre.visible=true;
      filtre.size=size_;
      data.traitement = filtre;
      AddImageInList(data);
    }

    return im_out;
}

/// @brief filtre moyenneur
/// @param im_in : image en entrée
/// @param brefresh : booleen pour savoir si ajoute le filtre à la liste ou non
/// @return 
cv::Mat Model::AverageFilter(const cv::Mat& im_in, bool brefresh/*=false*/)
{
   // noyau moyenneur
   cv::Mat kernel = cv::Mat::ones(3,3,CV_32F)/9.0f;

   cv::Mat im_out;
   cv::filter2D(im_in, im_out, -1, kernel);

   if(!brefresh)
   {
      // donnée filtre moyenneur
      DataImage data;
      data.image = im_out;
      Convolution filtre;
      filtre.idTreatement = TREATEMENT_TYPE::ID_FILTRE_CONV_MOYENNEUR;
      filtre.visible=true;
      filtre.kernel=kernel;
      data.traitement = filtre;
      AddImageInList(data);
   }

    return im_out;
}

/// @brief filtre sharpen, réhausseur de contour
/// @param im_in : image en entrée
/// @param brefresh : booleen pour savoir si ajoute le filtre à la liste ou non
/// @return 
cv::Mat Model::SharpenFilter(const cv::Mat& im_in, bool brefresh/*=false*/)
{
   // noyau sharpen de type float
   cv::Mat kernel = (cv::Mat_<float>(3,3) <<  1, -3, 1, 
                                             -3, 9, -3, 
                                              1, -3, 1);

   cv::Mat im_out;
   cv::filter2D(im_in, im_out, -1, kernel);

   if(!brefresh)
   {
      // donnée filtre sharpen
      DataImage data;
      data.image = im_out;
      Convolution filtre;
      filtre.idTreatement = TREATEMENT_TYPE::ID_FILTRE_CONV_SHARPEN;
      filtre.visible=true;
      filtre.kernel=kernel;
      data.traitement = filtre;
      AddImageInList(data);
   }

    return im_out;
}

/// @brief Filtre Laplacien pour faire resortir les contours sur l'image 
/// @param im_in : image en entrée
/// @param brefresh : booleen pour savoir si ajoute le filtre à la liste ou non
/// @return 
cv::Mat Model::LaplacienFilter(const cv::Mat& im_in, bool brefresh/*=false*/)
{
   // noyau laplacien de type float
   cv::Mat kernel = (cv::Mat_<float>(3,3) <<  0, 1, 0, 
                                              1,-4, 1, 
                                              0, 1, 0);

   cv::Mat im_out;
   cv::filter2D(im_in, im_out, -1, kernel);

   if(!brefresh)
   {
      // donnée filtre laplacien
      DataImage data;
      data.image = im_out;
      Convolution filtre;
      filtre.idTreatement = TREATEMENT_TYPE::ID_FILTRE_CONV_LAPLACIEN;
      filtre.visible=true;
      filtre.kernel=kernel;
      data.traitement = filtre;
      AddImageInList(data);
   }

    return im_out;
}

/// @brief Sobel X contour dans le sans X
/// @param im_in : image en entrée
/// @param brefresh : booleen pour savoir si ajoute le filtre à la liste ou non
/// @return 
cv::Mat Model::SobelXFilter(const cv::Mat& im_in, bool brefresh/*=false*/)
{
   // noyau sobel x de type float
   cv::Mat kernel = (cv::Mat_<float>(3,3) << -1,-2,-1, 
                                              0, 0, 0, 
                                              1, 2, 1);

   cv::Mat im_out;
   cv::filter2D(im_in, im_out, -1, kernel);

   if(!brefresh)
   {
      // donnée filtre sobel
      DataImage data;
      data.image = im_out;
      Convolution filtre;
      filtre.idTreatement = TREATEMENT_TYPE::ID_FILTRE_CONV_SOBEL_X;
      filtre.visible=true;
      filtre.kernel=kernel;
      data.traitement = filtre;
      AddImageInList(data);
   }

    return im_out;
}

/// @brief Sobel Ycontour dans le sans Y
/// @param im_in : image en entrée
/// @param brefresh : booleen pour savoir si ajoute le filtre à la liste ou non
/// @return 
cv::Mat Model::SobelYFilter(const cv::Mat& im_in, bool brefresh/*=false*/)
{
   // noyau sobel y de type float
   cv::Mat kernel = (cv::Mat_<float>(3,3) << -1, 0, 1, 
                                             -2, 0, 2, 
                                             -1, 0, 1);

   cv::Mat im_out;
   cv::filter2D(im_in, im_out, -1, kernel);

   if(!brefresh)
   {
      // donnée filtre sobel
      DataImage data;
      data.image = im_out;
      Convolution filtre;
      filtre.idTreatement = TREATEMENT_TYPE::ID_FILTRE_CONV_SOBEL_Y;
      filtre.visible=true;
      filtre.kernel=kernel;
      data.traitement = filtre;
      AddImageInList(data);
   }

    return im_out;
}

/// @brief Filtre de Canny, contour de l'image dans toutes les directions
/// @param im_in : image en entrée
/// @param thresh1 : seuil 1
/// @param thresh2 : seuil 1
/// @param brefresh : booleen pour savoir si ajoute le filtre à la liste ou non
/// @return 
cv::Mat Model::CannyFilter(const cv::Mat& im_in, int thresh1, int thresh2, bool brefresh/*=false*/)
{
   cv::Mat im_out, grayMat;

   cv::cvtColor(im_in, grayMat, cv::COLOR_BGR2GRAY); // conversion de l'image couleur en image ndg
   cv::Canny(grayMat, im_out, thresh1, thresh2);

   if(!brefresh)
   {
      // donnée filtre canny
      DataImage data;
      data.image = im_out;
      Canny filtre;
      filtre.idTreatement = TREATEMENT_TYPE::ID_FILTRE_CANNY;
      filtre.visible=true;
      filtre.seuil1=thresh1;
      filtre.seuil2=thresh2;
      data.traitement = filtre;
      AddImageInList(data);
   }

    return im_out;
}

/// @brief Seuillage de l'image, avec la methode d'otsu
/// @param im_in : image en entrée
/// @param brefresh_ : booleen pour savoir si ajoute le filtre à la liste ou non
/// @return 
cv::Mat Model::Threshold(const cv::Mat& im_in, bool brefresh_/*=false*/)
{
   cv::Mat im_out, grayMat;

   cv::cvtColor(im_in, grayMat, cv::COLOR_BGR2GRAY); // conversion de l'image couleur en image ndg
   cv::threshold(grayMat, im_out, 0, 255, cv::THRESH_OTSU);

   if(!brefresh_)
   {
      // donnée filtre canny
      DataImage data;
      data.image = im_out;
      FiltreData filtre;
      filtre.idTreatement = TREATEMENT_TYPE::ID_FILTRE_SEUILLAGE;
      filtre.visible=true;
      data.traitement = filtre;
      AddImageInList(data);
   }

    return im_out;
}

/// @brief Erosion de l'image
/// @param im_in : image en entrée
/// @param size_x : dim x de l'element structurant
/// @param size_y : dim x de l'element structurant
/// @param type : type d'élement structurant. 0:rectangle, 1:Croix, 2:ellipse
/// @param brefresh_ : booleen pour savoir si ajoute le filtre à la liste ou non
/// @return 
cv::Mat Model::Erode(const cv::Mat& im_in, int size_x/*=3*/, int size_y/*=3*/, int type/*=0*/, bool brefresh_/*=false*/)
{
   int erosion_type = 0;
   if( type == 0 ){ erosion_type = cv::MORPH_RECT; }
   else if( type == 1 ){ erosion_type = cv::MORPH_CROSS; }
   else if( type == 2) { erosion_type = cv::MORPH_ELLIPSE; }

   // on vérifie que les dimensions du filtre restent impair
   if(size_x%2 == 0)
        size_x++;
    if(size_y%2 == 0)
        size_y++;

   // element structurant
   cv::Mat element = cv::getStructuringElement( erosion_type, cv::Size( size_x, size_y));

   cv::Mat im_out;
   cv::erode( im_in, im_out, element );

    if(!brefresh_)
   {
      // donnée erosion
      DataImage data;
      data.image = im_out;
      Gaussien filtre;
      filtre.idTreatement = TREATEMENT_TYPE::ID_FILTRE_EROSION;
      filtre.visible=true;
      filtre.sizeX=size_x;
      filtre.sizeY=size_y;
      data.traitement = filtre;
      AddImageInList(data);
   }

   return im_out;
}

/// @brief Dilatation de l'image
/// @param im_in : image en entrée
/// @param size_x : dim x de l'element structurant
/// @param size_y : dim x de l'element structurant
/// @param type : type d'élement structurant. 0:rectangle, 1:Croix, 2:ellipse
/// @param brefresh_ : booleen pour savoir si ajoute le filtre à la liste ou non
/// @return 
cv::Mat Model::Dilate(const cv::Mat& im_in, int size_x/*=3*/, int size_y/*=3*/, int type/*=0*/, bool brefresh_/*=false*/)
{
   int dilatation_type = 0;
   if( type == 0 ){ dilatation_type = cv::MORPH_RECT; }
   else if( type == 1 ){ dilatation_type = cv::MORPH_CROSS; }
   else if( type == 2) { dilatation_type = cv::MORPH_ELLIPSE; }

   // on vérifie que les dimensions du filtre restent impair
   if(size_x%2 == 0)
        size_x++;
    if(size_y%2 == 0)
        size_y++;

   // element structurant
   cv::Mat element = cv::getStructuringElement( dilatation_type, cv::Size( size_x, size_y));

   cv::Mat im_out;
   cv::dilate( im_in, im_out, element );

    if(!brefresh_)
   {
      // donnée dilatation
      DataImage data;
      data.image = im_out;
      Gaussien filtre;
      filtre.idTreatement = TREATEMENT_TYPE::ID_FILTRE_DILATATION;
      filtre.visible=true;
      filtre.sizeX=size_x;
      filtre.sizeY=size_y;
      data.traitement = filtre;
      AddImageInList(data);
   }

   return im_out;
}

/// @brief Ouverture de l'image
/// @param im_in : image en entrée
/// @param size_x : dim x de l'element structurant
/// @param size_y : dim x de l'element structurant
/// @param type : type d'élement structurant. 0:rectangle, 1:Croix, 2:ellipse
/// @param brefresh_ : booleen pour savoir si ajoute le filtre à la liste ou non
/// @return 
cv::Mat Model::OpenFilter(const cv::Mat& im_in, int size_x/*=3*/, int size_y/*=3*/, int type/*=0*/, bool brefresh_/*=false*/)
{
   int ouverure_type = 0;
   if( type == 0 ){ ouverure_type = cv::MORPH_RECT; }
   else if( type == 1 ){ ouverure_type = cv::MORPH_CROSS; }
   else if( type == 2) { ouverure_type = cv::MORPH_ELLIPSE; }

   // on vérifie que les dimensions du filtre restent impair
   if(size_x%2 == 0)
        size_x++;
    if(size_y%2 == 0)
        size_y++;

   // element structurant
   cv::Mat element = cv::getStructuringElement( ouverure_type, cv::Size( size_x, size_y));

   cv::Mat im_out;
   cv::morphologyEx( im_in, im_out,cv::MORPH_OPEN , element );

    if(!brefresh_)
   {
      // donnée ouverure
      DataImage data;
      data.image = im_out;
      Gaussien filtre;
      filtre.idTreatement = TREATEMENT_TYPE::ID_FILTRE_OUVERTURE;
      filtre.visible=true;
      filtre.sizeX=size_x;
      filtre.sizeY=size_y;
      data.traitement = filtre;
      AddImageInList(data);
   }

   return im_out;
}

/// @brief Fermeture de l'image
/// @param im_in : image en entrée
/// @param size_x : dim x de l'element structurant
/// @param size_y : dim x de l'element structurant
/// @param type : type d'élement structurant. 0:rectangle, 1:Croix, 2:ellipse
/// @param brefresh_ : booleen pour savoir si ajoute le filtre à la liste ou non
/// @return 
cv::Mat Model::CloseFilter(const cv::Mat& im_in, int size_x/*=3*/, int size_y/*=3*/, int type/*=0*/, bool brefresh_/*=false*/)
{
   int fermeture_type = 0;
   if( type == 0 ){ fermeture_type = cv::MORPH_RECT; }
   else if( type == 1 ){ fermeture_type = cv::MORPH_CROSS; }
   else if( type == 2) { fermeture_type = cv::MORPH_ELLIPSE; }

   // on vérifie que les dimensions du filtre restent impair
   if(size_x%2 == 0)
        size_x++;
    if(size_y%2 == 0)
        size_y++;

   // element structurant
   cv::Mat element = cv::getStructuringElement( fermeture_type, cv::Size( size_x, size_y));

   cv::Mat im_out;
   cv::morphologyEx( im_in, im_out,cv::MORPH_CLOSE , element );

    if(!brefresh_)
   {
      // donnée ouverure
      DataImage data;
      data.image = im_out;
      Gaussien filtre;
      filtre.idTreatement = TREATEMENT_TYPE::ID_FILTRE_FERMETURE;
      filtre.visible=true;
      filtre.sizeX=size_x;
      filtre.sizeY=size_y;
      data.traitement = filtre;
      AddImageInList(data);
   }

   return im_out;
}