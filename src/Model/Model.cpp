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


/// @brief Permet de cacher un filtre
/// @param idx_ : index du filtre 
/// @param bhide_ : booleen pour savoir si cacher ou non 
/// @return 
cv::Mat Model::HideFilter(int idx_, bool bhide_)
{
   if(idx_ >= (int)m_ListDataImage.size() || m_ListDataImage.empty())
        throw std::invalid_argument("index hors de la liste ou liste d'image vide !");

    if(idx_ == 0) // pas de filtre sur l'image de base
        return m_ListDataImage.front().image.clone();

    m_ListDataImage[idx_].traitement.visible = !bhide_;
    return UpdateListFilterImage(); // mise à jour de la liste et retour de l'image sans le filtre
}


/// @brief permet de supprimer un filtre de la liste
/// @param idx_ : index du filtre
/// @return 
cv::Mat Model::RemoveFilterImage(int idx_)
{
   if(idx_ >= m_ListDataImage.size() || m_ListDataImage.empty())
        throw std::invalid_argument("index hors de la liste ou liste d'image vide !");

   // TODO
   return UpdateListFilterImage();
}


/// @brief mis à jour de la liste en fonction des filtres cachés ou non
/// @return 
cv::Mat Model::UpdateListFilterImage()
{
   if(m_ListDataImage.empty())
        throw std::invalid_argument("liste d'image vide !");

   cv::Mat im_out;
   if(m_ListDataImage.size()>1) // si il y a des images filtrées
   {
      im_out = m_ListDataImage.front().image.clone(); // recupération de l'image de base

      for(int i=1; i<m_ListDataImage.size(); i++)
      {
         if(m_ListDataImage[i].traitement.visible) // si filtre actif on l'applique
         {
            DataImage data = m_ListDataImage[i];
            data.image = im_out.clone();
            im_out = ApplyFilter(data);
         }
      }
   }

   return im_out;
}


/// @brief Applique un filtre à une image
/// @param data : donnees de l'image
/// @return
cv::Mat Model::ApplyFilter(DataImage data)
{
   cv::Mat im_out;
   
   switch(data.traitement.idTreatement)
   {
      case TREATEMENT_TYPE::ID_FILTRE_GAUSSIEN :
         im_out = GaussienFilter(data.image, data.traitement.sizeX, data.traitement.sizeY, true);
         break;
      case TREATEMENT_TYPE::ID_FILTRE_MEDIAN :
         im_out = MedianFilter(data.image, data.traitement.size, true);
         break;
      case TREATEMENT_TYPE::ID_FILTRE_CONV_MOYENNEUR :
         im_out = AverageFilter(data.image, true);
         break;
      case TREATEMENT_TYPE::ID_FILTRE_CONV_SHARPEN :
         im_out = SharpenFilter(data.image, true);
         break;
      case TREATEMENT_TYPE::ID_FILTRE_CONV_LAPLACIEN :
         im_out = LaplacienFilter(data.image, true);
         break;
      case TREATEMENT_TYPE::ID_FILTRE_CONV_SOBEL_X : 
         im_out = SobelXFilter(data.image, true);
         break;
      case TREATEMENT_TYPE::ID_FILTRE_CONV_SOBEL_Y :
         im_out = SobelYFilter(data.image, true);
         break;
      case TREATEMENT_TYPE::ID_FILTRE_SEUILLAGE :
         im_out = Threshold(data.image, true);
         break;
      case TREATEMENT_TYPE::ID_FILTRE_CANNY :
         im_out = CannyFilter(data.image, data.traitement.seuil1, data.traitement.seuil2, true);
         break;
      case TREATEMENT_TYPE::ID_FILTRE_EROSION :
         im_out = Erode(data.image, data.traitement.sizeX, data.traitement.sizeY, data.traitement.type,true);
         break;
      case TREATEMENT_TYPE::ID_FILTRE_DILATATION :
         im_out = Dilate(data.image, data.traitement.sizeX, data.traitement.sizeY, data.traitement.type,true);
         break;
      case TREATEMENT_TYPE::ID_FILTRE_OUVERTURE : 
         im_out = OpenFilter(data.image, data.traitement.sizeX, data.traitement.sizeY, data.traitement.type,true);
         break;
      case TREATEMENT_TYPE::ID_FILTRE_FERMETURE :
         im_out = CloseFilter(data.image,data.traitement.sizeX, data.traitement.sizeY, data.traitement.type, true);
         break;

      default: 
         return m_ListDataImage.front().image;
         break;
   }

    return im_out;
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
      data.traitement.idTreatement = TREATEMENT_TYPE::ID_FILTRE_GAUSSIEN;
      data.traitement.visible=true;
      data.traitement.sizeX=size_x;
      data.traitement.sizeY=size_y;
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
      data.traitement.idTreatement = TREATEMENT_TYPE::ID_FILTRE_MEDIAN;
      data.traitement.visible=true;
      data.traitement.size=size_;
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
      data.traitement.idTreatement = TREATEMENT_TYPE::ID_FILTRE_CONV_MOYENNEUR;
      data.traitement.visible=true;
      data.traitement.kernel=kernel;
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
      data.traitement.idTreatement = TREATEMENT_TYPE::ID_FILTRE_CONV_SHARPEN;
      data.traitement.visible=true;
      data.traitement.kernel=kernel;
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
      data.traitement.idTreatement = TREATEMENT_TYPE::ID_FILTRE_CONV_LAPLACIEN;
      data.traitement.visible=true;
      data.traitement.kernel=kernel;
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
      data.traitement.idTreatement = TREATEMENT_TYPE::ID_FILTRE_CONV_SOBEL_X;
      data.traitement.visible=true;
      data.traitement.kernel=kernel;
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
      data.traitement.idTreatement = TREATEMENT_TYPE::ID_FILTRE_CONV_SOBEL_Y;
      data.traitement.visible=true;
      data.traitement.kernel=kernel;
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
      data.traitement.idTreatement = TREATEMENT_TYPE::ID_FILTRE_CANNY;
      data.traitement.visible=true;
      data.traitement.seuil1=thresh1;
      data.traitement.seuil2=thresh2;
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
      data.traitement.idTreatement = TREATEMENT_TYPE::ID_FILTRE_EROSION;
      data.traitement.visible=true;
      data.traitement.sizeX=size_x;
      data.traitement.sizeY=size_y;
      data.traitement.type = type;
      AddImageInList(data);
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
      data.traitement.idTreatement = TREATEMENT_TYPE::ID_FILTRE_DILATATION;
      data.traitement.visible=true;
      data.traitement.sizeX=size_x;
      data.traitement.sizeY=size_y;
      data.traitement.type = type;
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
      data.traitement.idTreatement = TREATEMENT_TYPE::ID_FILTRE_OUVERTURE;
      data.traitement.visible=true;
      data.traitement.sizeX=size_x;
      data.traitement.sizeY=size_y;
      data.traitement.type = type;
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
      data.traitement.idTreatement = TREATEMENT_TYPE::ID_FILTRE_FERMETURE;
      data.traitement.visible=true;
      data.traitement.sizeX=size_x;
      data.traitement.sizeY=size_y;
      data.traitement.type = type;
      AddImageInList(data);
   }

   return im_out;
}