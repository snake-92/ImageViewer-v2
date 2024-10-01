#ifndef DATATREATMENTS_H_INCLUDED
#define DATATREATMENTS_H_INCLUDED

#include <opencv2/opencv.hpp>
#include "ListEnums.h"


struct FiltreData
{
    bool visible;
    TREATEMENT_TYPE idTreatement;
};

struct Gaussien : FiltreData
{
    int sizeX;
    int sizeY;
    int sigma;
};

struct Median : FiltreData
{
    int size;
};

struct Convolution : FiltreData
{
    cv::Mat kernel;
};

struct Canny : FiltreData
{
    int seuil1;
    int seuil2;
};



#endif