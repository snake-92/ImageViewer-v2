#ifndef DATATREATMENTS_H_INCLUDED
#define DATATREATMENTS_H_INCLUDED

#include <opencv2/opencv.hpp>
#include "ListEnums.h"

/// @brief structure mère de tous les filtres
struct FiltreData
{
    bool visible; // visible ou non
    TREATEMENT_TYPE idTreatement; // identifiant du filtre
};

/// @brief structure filtre gaussien
struct Gaussien : FiltreData
{
    Gaussien(const FiltreData& p) : FiltreData(p) {} 
    int sizeX;
    int sizeY;
    int sigma;
};

/// @brief structure filtre median
struct Median : FiltreData
{
    Median(const FiltreData& p) : FiltreData(p) {}
    int size;
};

/// @brief structure filtre convolution
struct Convolution : FiltreData
{
    Convolution(const FiltreData& p) : FiltreData(p) {}
    cv::Mat kernel;
};

/// @brief structure filtre canny
struct Canny : FiltreData
{
    Canny(const FiltreData& p) : FiltreData(p) {}
    int seuil1;
    int seuil2;
};

/// @brief structure filtre morphologique
struct Morpho : FiltreData
{
    Morpho(const FiltreData& p) : FiltreData(p) {} 
    int sizeX;
    int sizeY;
    int type; // 0:rectangle, 1:Croix, 2:ellipse
};



#endif