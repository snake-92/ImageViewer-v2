#ifndef DATATREATMENTS_H_INCLUDED
#define DATATREATMENTS_H_INCLUDED

#include <opencv2/opencv.hpp>
#include "ListEnums.h"

/// @brief structure mère de tous les filtres
struct FiltreData
{
    bool visible; // visible ou non
    TREATEMENT_TYPE idTreatement; // identifiant du filtre

    // donnée filtre gaussien
    int sizeX; // taille en x. utilise aussi pour les filtres morphologiques
    int sizeY; // taille en y. utilise aussi pour les filtres morphologiques

    // filtre médian
    int size; // taille du filtre médian

    // filtre convolution
    cv::Mat kernel; // noyau de convolution

    // filtre canny
    int seuil1; // seuil 1
    int seuil2; // seuil 2

    // filtre morphologique
    int type; // 0:rectangle, 1:Croix, 2:ellipse
};

#endif