#include "DialogConfigFilter.h"
#include <wx/valnum.h>
#include <wx/valtext.h>

#define VAL_MAX 1000
#define VAL_MIN 1
#define VAL_MAX_MORPH_TYPE 3
#define VAL_MIN_MORPH_TYPE 0

#define RECT "rectangle"
#define CROSS "croix"
#define ELLIPSE "ellipse"

DialogConfigFilter::DialogConfigFilter(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint &pos, const wxSize &size, long style, const wxString& name)
 : wxDialog(parent, id, title, pos, size, style, name)
{
    m_params = {3,3,-1}; // valeurs par defaut
    m_typeMorph = RECT;
}


/// @brief Recupération du prametrage choisi
/// @return 
std::array<int,3> DialogConfigFilter::GetParams()
{
    if(m_typeMorph==RECT)
    {
        m_params[2]=0;
    }
    else if(m_typeMorph==CROSS)
    {
        m_params[2]=1;
    }
    else if(ELLIPSE)
    {
        m_params[2]=2;
    }

    return m_params;
}

/// @brief Initialisation de la fenêtre de dialog en fonction du filtre
/// @param filter 
void DialogConfigFilter::InitDialog(MENU_FILTRE filter)
{
    switch(filter)
    {
        case MENU_FILTRE::ID_GAUSSIEN:
            DialogGaussien();
            break;
        case MENU_FILTRE::ID_MEDIAN:
            DialogMedian();
            break;
        case MENU_FILTRE::ID_CONV_MOYENNEUR:
        case MENU_FILTRE::ID_CONV_SHARPEN:
        case MENU_FILTRE::ID_CONV_LAPLACIEN:
        case MENU_FILTRE::ID_CONV_SOBEL_X:
        case MENU_FILTRE::ID_CONV_SOBEL_Y:
        case MENU_FILTRE::ID_SEUILLAGE:
            break;
        case MENU_FILTRE::ID_CANNY:
            DialogCanny();
            break;
        case MENU_FILTRE::ID_EROSION:
        case MENU_FILTRE::ID_DILATATION:
        case MENU_FILTRE::ID_OUVERTURE:
        case MENU_FILTRE::ID_FERMETURE:
            DialogMorpho();
            break;
    }
}

/// @brief Fenetre pour le filtre gaussien
void DialogConfigFilter::DialogGaussien()
{
    // permet d'autoriser uniquement la saisi de nombre dans wxTextCtrl
    wxFloatingPointValidator<int> val_x(0,&m_params[0],wxNUM_VAL_DEFAULT);
    wxFloatingPointValidator<int> val_y(0,&m_params[1],wxNUM_VAL_DEFAULT);
    val_x.SetRange(VAL_MIN, VAL_MAX);
    val_y.SetRange(VAL_MIN, VAL_MAX);

    auto ctrlX = new wxTextCtrl(this, wxID_ANY, "3", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER, val_x, wxTextCtrlNameStr);
    auto ctrlY = new wxTextCtrl(this, wxID_ANY, "3", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER, val_y, wxTextCtrlNameStr);

    auto txt_x = new wxStaticText(this,wxID_ANY,"size x ");
    auto txt_y = new wxStaticText(this,wxID_ANY,"size y ");

    auto mainHorX = new wxBoxSizer(wxHORIZONTAL);
    mainHorX->Add(txt_x, 0, wxEXPAND | wxRIGHT, 10);
    mainHorX->Add(ctrlX, 0, wxEXPAND | wxALL, 1);

    auto mainHorY = new wxBoxSizer(wxHORIZONTAL);
    mainHorY->Add(txt_y, 0, wxEXPAND | wxRIGHT, 10);
    mainHorY->Add(ctrlY, 0, wxEXPAND | wxALL, 1);

    auto mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(mainHorX, 0, wxEXPAND | wxALL, 10);
    mainSizer->Add(mainHorY, 0, wxEXPAND | wxALL, 10);
    mainSizer->Add(CreateButtonSizer(wxOK | wxCANCEL), 0, wxALIGN_CENTER | wxALL, FromDIP(10));
    this->SetSizerAndFit(mainSizer);
}

/// @brief fenetre pour le filtre médian
void DialogConfigFilter::DialogMedian()
{
    // permet d'autoriser uniquement la saisi de nombre dans wxTextCtrl
    wxFloatingPointValidator<int> val(0,&m_params[0],wxNUM_VAL_DEFAULT);
    val.SetRange(VAL_MIN, VAL_MAX);

    auto ctrlSize = new wxTextCtrl(this, wxID_ANY, "3", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER, val, wxTextCtrlNameStr);

    auto txt_size = new wxStaticText(this,wxID_ANY,"size ");

    auto mainHor = new wxBoxSizer(wxHORIZONTAL);
    mainHor->Add(txt_size, 0, wxEXPAND | wxRIGHT, 10);
    mainHor->Add(ctrlSize, 0, wxEXPAND | wxALL, 1);

    auto mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(mainHor, 0, wxEXPAND | wxALL, 10);
    mainSizer->Add(CreateButtonSizer(wxOK | wxCANCEL), 0, wxALIGN_CENTER | wxALL, FromDIP(10));
    this->SetSizerAndFit(mainSizer);
}

/// @brief fenetre pour le canny
void DialogConfigFilter::DialogCanny()
{
    // permet d'autoriser uniquement la saisi de nombre dans wxTextCtrl
    wxFloatingPointValidator<int> val_x(0,&m_params[0],wxNUM_VAL_DEFAULT);
    wxFloatingPointValidator<int> val_y(0,&m_params[1],wxNUM_VAL_DEFAULT);
    val_x.SetRange(VAL_MIN, VAL_MAX);
    val_y.SetRange(VAL_MIN, VAL_MAX);

    auto ctrlX = new wxTextCtrl(this, wxID_ANY, "100", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER, val_x, wxTextCtrlNameStr);
    auto ctrlY = new wxTextCtrl(this, wxID_ANY, "200", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER, val_y, wxTextCtrlNameStr);

    auto txt_x = new wxStaticText(this,wxID_ANY,"Threshold 1 ");
    auto txt_y = new wxStaticText(this,wxID_ANY,"Threshold 2 ");

    auto mainHorX = new wxBoxSizer(wxHORIZONTAL);
    mainHorX->Add(txt_x, 0, wxEXPAND | wxRIGHT, 10);
    mainHorX->Add(ctrlX, 0, wxEXPAND | wxALL, 1);

    auto mainHorY = new wxBoxSizer(wxHORIZONTAL);
    mainHorY->Add(txt_y, 0, wxEXPAND | wxRIGHT, 10);
    mainHorY->Add(ctrlY, 0, wxEXPAND | wxALL, 1);

    auto mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(mainHorX, 0, wxEXPAND | wxALL, 10);
    mainSizer->Add(mainHorY, 0, wxEXPAND | wxALL, 10);
    mainSizer->Add(CreateButtonSizer(wxOK | wxCANCEL), 0, wxALIGN_CENTER | wxALL, FromDIP(10));
    this->SetSizerAndFit(mainSizer);
}

/// @brief fenetre pour tous les filtres morphologique
void DialogConfigFilter::DialogMorpho()
{
    // permet d'autoriser uniquement la saisi de nombre dans wxTextCtrl
    wxFloatingPointValidator<int> val_x(0,&m_params[0],wxNUM_VAL_DEFAULT);
    wxFloatingPointValidator<int> val_y(0,&m_params[1],wxNUM_VAL_DEFAULT);
    wxTextValidator val_type(wxFILTER_NONE,&m_typeMorph);
    val_x.SetRange(VAL_MIN, VAL_MAX);
    val_y.SetRange(VAL_MIN, VAL_MAX);

    auto ctrlX = new wxTextCtrl(this, wxID_ANY, "100", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER, val_x, wxTextCtrlNameStr);
    auto ctrlY = new wxTextCtrl(this, wxID_ANY, "200", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER, val_y, wxTextCtrlNameStr);

    // liste des choix
    auto listChoice = new wxArrayString(3);
    listChoice->Add(ELLIPSE);
    listChoice->Add(CROSS);
    listChoice->Add(RECT);
    auto box = new wxComboBox(this, wxID_ANY, RECT, wxDefaultPosition, wxDefaultSize, *listChoice, wxTE_PROCESS_ENTER | wxTE_PROCESS_TAB, val_type);

    auto txt_x = new wxStaticText(this,wxID_ANY,"size x ");
    auto txt_y = new wxStaticText(this,wxID_ANY,"size y ");
    auto txt_type = new wxStaticText(this,wxID_ANY,"type  ");

    auto mainHorX = new wxBoxSizer(wxHORIZONTAL);
    mainHorX->Add(txt_x, 0, wxEXPAND | wxRIGHT, 10);
    mainHorX->Add(ctrlX, 0, wxEXPAND | wxALL, 1);

    auto mainHorY = new wxBoxSizer(wxHORIZONTAL);
    mainHorY->Add(txt_y, 0, wxEXPAND | wxRIGHT, 10);
    mainHorY->Add(ctrlY, 0, wxEXPAND | wxALL, 1);
    
    auto mainHorType = new wxBoxSizer(wxHORIZONTAL);
    mainHorType->Add(txt_type, 0, wxEXPAND | wxRIGHT, 10);
    mainHorType->Add(box, 0, wxEXPAND | wxALL, 1);

    auto mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(mainHorX, 0, wxEXPAND | wxALL, 10);
    mainSizer->Add(mainHorY, 0, wxEXPAND | wxALL, 10);
    mainSizer->Add(mainHorType, 0, wxEXPAND | wxALL, 10);
    mainSizer->Add(CreateButtonSizer(wxOK | wxCANCEL), 0, wxALIGN_CENTER | wxALL, FromDIP(10));
    this->SetSizerAndFit(mainSizer);
}