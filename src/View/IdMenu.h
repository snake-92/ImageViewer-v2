#ifndef IDMENU_H
#define IDMENU_H

enum MENU_IMAGE
{
    CONVERT_NDG_8_BITS=20,
    AFFICHE_INFO_IMAGE,
    AJUSTER_HISTOGRAMME,
    EFFACE_FILTRES
};

// garder le même ordre que l'enum qui est dans ListEnums.h (Model)
enum MENU_FILTRE
{
    ID_GAUSSIEN=1,
    ID_MEDIAN,
   
    ID_CONV_MOYENNEUR,
    ID_CONV_SHARPEN,
    ID_CONV_LAPLACIEN,
    ID_CONV_SOBEL_X,
    ID_CONV_SOBEL_Y,
    ID_CANNY,

    ID_SEUILLAGE,
    ID_EROSION,
    ID_DILATATION,
    ID_OUVERTURE,
    ID_FERMETURE
};


enum MENU_DETECTION
{
    ID_FACE_DETECTION=60
};


enum TOOLS
{
    SELECT = 50,
    LINE,
    RECTANGLE,
    CLEAR
};



#endif