#include "MainFrameView.h"
#include "icons.h"
#include "DialogConfigFilter.h"



MainFrameView::MainFrameView(const wxString &title, const wxPoint &pos, const wxSize &size, long style) : wxFrame(nullptr, wxID_ANY, title, pos, size, style)
{
    SetIcon(wxICON(monicone));
    SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
    
    // initialisation des menus
    InitMenuBar();

    // Initialisation des outils
    InitToolsBar();

    // panel viewer image
    auto sizer = new wxBoxSizer(wxHORIZONTAL);
    m_bitmap = new BufferedBitmap(this, wxID_ANY, wxBitmap(wxSize(1, 1)), wxDefaultPosition, FromDIP(wxSize(700, 500)));
    sizer->Add(m_bitmap, 1, wxEXPAND | wxALL, FromDIP(5));

    // panel liste des filtres
    m_panelFiltres = new wxScrolled<wxPanel>(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    m_panelFiltres->SetScrollRate(0,FromDIP(10));
    m_panelFiltres->SetBackgroundColour(wxColor(125,170,137));
    sizer->Add(m_panelFiltres, 0, wxEXPAND | wxALL, 10);
    m_SizerListFiltre = new wxBoxSizer(wxVERTICAL);

    this->SetSizerAndFit(sizer);

    // barre de status
    InitStatusBar();

    m_viewModel = new ViewModel(m_CurrentImage);
}


MainFrameView::~MainFrameView()
{
    delete m_viewModel;
}


BEGIN_EVENT_TABLE(MainFrameView, wxFrame)
    EVT_MENU(wxID_EXIT,  MainFrameView::OnQuit)
    EVT_MENU(wxID_HELP, MainFrameView::OnHelp)
    EVT_MENU(wxID_FILE, MainFrameView::OnLoadImage)

    EVT_MENU(MENU_IMAGE::CONVERT_NDG_8_BITS, MainFrameView::OnImageOpSelect)
    EVT_MENU(MENU_IMAGE::AFFICHE_INFO_IMAGE, MainFrameView::OnImageOpSelect)
    EVT_MENU(MENU_IMAGE::AJUSTER_HISTOGRAMME, MainFrameView::OnImageOpSelect)
    EVT_MENU(MENU_IMAGE::EFFACE_FILTRES, MainFrameView::OnImageOpSelect)

    EVT_MENU(MENU_FILTRE::ID_GAUSSIEN, MainFrameView::OnFiltreSelect)
    EVT_MENU(MENU_FILTRE::ID_MEDIAN, MainFrameView::OnFiltreSelect)
    EVT_MENU(MENU_FILTRE::ID_CONV_MOYENNEUR, MainFrameView::OnFiltreSelect)
    EVT_MENU(MENU_FILTRE::ID_CONV_SHARPEN, MainFrameView::OnFiltreSelect)
    EVT_MENU(MENU_FILTRE::ID_CONV_LAPLACIEN, MainFrameView::OnFiltreSelect)
    EVT_MENU(MENU_FILTRE::ID_CONV_SOBEL_X, MainFrameView::OnFiltreSelect)
    EVT_MENU(MENU_FILTRE::ID_CONV_SOBEL_Y, MainFrameView::OnFiltreSelect)
    EVT_MENU(MENU_FILTRE::ID_CANNY, MainFrameView::OnFiltreSelect)
    EVT_MENU(MENU_FILTRE::ID_SEUILLAGE, MainFrameView::OnFiltreSelect)
    EVT_MENU(MENU_FILTRE::ID_EROSION, MainFrameView::OnFiltreSelect)
    EVT_MENU(MENU_FILTRE::ID_DILATATION, MainFrameView::OnFiltreSelect)
    EVT_MENU(MENU_FILTRE::ID_OUVERTURE, MainFrameView::OnFiltreSelect)
    EVT_MENU(MENU_FILTRE::ID_FERMETURE, MainFrameView::OnFiltreSelect)

    EVT_TOOL(wxID_ZOOM_IN, MainFrameView::OnZoomIn)
    EVT_TOOL(wxID_ZOOM_OUT, MainFrameView::OnZoomOut)
END_EVENT_TABLE()


/// @brief initialisation de tous les onglets sur le menu de l'application
void MainFrameView::InitMenuBar()
{
    // menu Fichier
    wxMenu* menuFile = new wxMenu();
    wxMenuItem* fileItem = new wxMenuItem(menuFile, wxID_FILE, "ouvrir image");
    fileItem->SetBitmap(wxBitmap(loadImage));
    menuFile->Append(fileItem);
    menuFile->AppendSeparator();
    wxMenuItem* quitItem = new wxMenuItem(menuFile, wxID_EXIT, "Quitter");
    quitItem->SetBitmap(wxBitmap(closeIcon));
    menuFile->Append(quitItem);

    // menu image
    wxMenu* menuImage = new wxMenu();
    menuImage->Append(MENU_IMAGE::CONVERT_NDG_8_BITS,"Convertir en 8 bits (ndg)");
    menuImage->Append(MENU_IMAGE::AFFICHE_INFO_IMAGE,"Afficher informations image");
    menuImage->Append(MENU_IMAGE::AJUSTER_HISTOGRAMME,"Ajuster histogramme");
    menuImage->Append(MENU_IMAGE::EFFACE_FILTRES,"Clear all filter");

    // menu Filtres
    wxMenu* menuFilter = new wxMenu();
    menuFilter->Append(MENU_FILTRE::ID_GAUSSIEN,"Gaussien");
    menuFilter->Append(MENU_FILTRE::ID_MEDIAN,"Median");
    menuFilter->Append(MENU_FILTRE::ID_CONV_MOYENNEUR,"Moyenneur");
    menuFilter->Append(MENU_FILTRE::ID_CONV_SHARPEN,"Sharpen");
    menuFilter->Append(MENU_FILTRE::ID_CONV_LAPLACIEN,"Laplacien");
    menuFilter->Append(MENU_FILTRE::ID_CONV_SOBEL_X,"Sobel x");
    menuFilter->Append(MENU_FILTRE::ID_CONV_SOBEL_Y,"Sobel y");
    menuFilter->Append(MENU_FILTRE::ID_CANNY,"Canny");
    menuFilter->Append(MENU_FILTRE::ID_SEUILLAGE,"Seuillage");
    menuFilter->Append(MENU_FILTRE::ID_EROSION,"Erosion");
    menuFilter->Append(MENU_FILTRE::ID_DILATATION,"Dilatation");
    menuFilter->Append(MENU_FILTRE::ID_OUVERTURE,"Ouverture");
    menuFilter->Append(MENU_FILTRE::ID_FERMETURE,"Fermeture");

    // menu Detections
    wxMenu* menuDetection = new wxMenu();

    // menu Aide
    wxMenu* menuHelp = new wxMenu();
    wxMenuItem* helpItem = new wxMenuItem(menuHelp, wxID_HELP, "?");
    helpItem->SetBitmap(wxBitmap(helpIcon));
    menuHelp->Append(helpItem);

    // incorpore les différents menu 
    wxMenuBar* menuBarre = new wxMenuBar();
    menuBarre->Append(menuFile,("Fichier"));
    menuBarre->Append(menuImage,("Image"));
    menuBarre->Append(menuFilter,("Filtres"));
    menuBarre->Append(menuDetection,("Detections"));
    menuBarre->Append(menuHelp,("?"));

    SetMenuBar(menuBarre);
}


/// @brief initialisation de tous les outils
void MainFrameView::InitToolsBar()
{
    wxToolBar* toolbar = CreateToolBar();
    toolbar->AddSeparator();

    // undo et redo
    toolbar->AddTool(wxID_UNDO, _("undo"),wxBitmap(undoIcon));
    toolbar->AddTool(wxID_REDO, _("redo"),wxBitmap(redoIcon));
    toolbar->SetToolShortHelp(wxID_UNDO,_("undo"));
    toolbar->SetToolShortHelp(wxID_REDO,_("redo"));
    toolbar->AddSeparator();

    // zoom et dezoom
    toolbar->AddTool(wxID_ZOOM_IN, _("zoom"),wxBitmap(zoomInIcon));
    toolbar->AddTool(wxID_ZOOM_OUT, _("dezoom"),wxBitmap(zoomOutIcon));
    toolbar->SetToolShortHelp(wxID_ZOOM_IN,_("zoom"));
    toolbar->SetToolShortHelp(wxID_ZOOM_OUT,_("dezoom"));
    toolbar->AddSeparator();

    // bouton selection
    toolbar->AddTool(TOOLS::SELECT, _("select"),wxBitmap(cursorIcon));
    toolbar->SetToolShortHelp(TOOLS::SELECT,_("select"));
    toolbar->AddSeparator();

    // outils de dessin (ligne rectangle)
    toolbar->AddTool(TOOLS::LINE, _("line"),wxBitmap(lineIcon));
    toolbar->AddTool(TOOLS::RECTANGLE, _("rectangle"),wxBitmap(rectangleIcon));
    toolbar->AddTool(TOOLS::CLEAR, _("clear"),wxBitmap(eraseIcon));
    toolbar->SetToolShortHelp(TOOLS::LINE,_("line"));
    toolbar->SetToolShortHelp(TOOLS::RECTANGLE,_("rectangle"));
    toolbar->SetToolShortHelp(TOOLS::CLEAR,_("clear"));
    toolbar->AddSeparator();

    toolbar->Realize();
}


 /// @brief Initialisation de la barre de statut
 void MainFrameView::InitStatusBar()
 {
    CreateStatusBar(2); // barre de status avec deux emplacements
    SetStatusText(_("Ready !"),0); // texte sur emplacement 0
    SetStatusText(_(" mouse ..."),1);  // texte sur emplacement 1

    //PushStatusText(_("ton texte"),0);  // ecrire un texte sur l'emplecement 0
    //PopStatusText(0);   // revenir sur le texte de base de l'emplecement 0
 }


 /// @brief mise à jour de l'image dans le viewer
 /// @param image 
 void MainFrameView::UpdateImage(const wxImage &image)
{
    m_bitmap->SetBitmap(wxBitmap(image));
    this->Layout();
}


/// @brief Event chargement d'une image
/// @param event 
void MainFrameView::OnLoadImage(wxCommandEvent& event)
{
    // vide la liste de filtre si besoin
    if(m_CurrentImage.IsOk())
        ClearAllFilter();

    PushStatusText(_("Load an image ..."),0); 
    
    wxFileDialog openFileDialog(this, _("Charger une image"), "", "",
                       "images (*.bmp;*.gif;*.tif;*.png;*.jpg;*.jpeg)|*.bmp;*.gif;*.tif;*.png;*.jpg;*.jpeg", wxFD_OPEN|wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;

    if (!m_CurrentImage.LoadFile(openFileDialog.GetPath()))
    {
        wxMessageBox("Erreur d'ouverture de l'image", "Erreur", wxOK | wxICON_ERROR);
        return;
    }

    UpdateImage(m_CurrentImage);
    m_viewModel->SetImageBase(); // envoi l'image dans le model

    PopStatusText(0);
}


/// @brief zoom 
/// @param event 
void MainFrameView::OnZoomIn(wxCommandEvent &event)
{
    m_bitmap->ZoomIn();
}

/// @brief dezoom
/// @param event 
void MainFrameView::OnZoomOut(wxCommandEvent &event)
{
    m_bitmap->ZoomOut();
}


/// @brief Event sur menu image
/// @param event 
void MainFrameView::OnImageOpSelect(wxCommandEvent& event)
{
    if(!m_CurrentImage.IsOk()) // si aucune image n'est chargé on ne fait rien
        return;

    switch((MENU_FILTRE)event.GetId())
    {
        case MENU_IMAGE::CONVERT_NDG_8_BITS:
            break;
        case MENU_IMAGE::AFFICHE_INFO_IMAGE:
            break;
        case MENU_IMAGE::AJUSTER_HISTOGRAMME:
            break;
        case MENU_IMAGE::EFFACE_FILTRES:
            ClearAllFilter();
            break;
    }

    UpdateImage(m_CurrentImage);
}


/// @brief mise à jour de la liste des filtres sur l'IHM
/// @param name 
void MainFrameView::UpdateListCheckBoxFiltre(const wxString& name)
{
    int idCheckbox=((int)m_SizerListFiltre->GetItemCount()+1)+100; // id checkbox

    m_mapIndexCheckbox[idCheckbox] = (int)m_SizerListFiltre->GetItemCount()+1; // sauvegarde l'id du filtre

    wxCheckBox* check = new wxCheckBox(m_panelFiltres,idCheckbox,name);
    m_listCheckbox.push_back(check);
    check->SetValue(true);
    check->Bind(wxEVT_CHECKBOX, &MainFrameView::ClickCheckBox, this);
    m_SizerListFiltre->Add(check, 1, wxEXPAND | wxTop | wxBottom, 5);
    m_panelFiltres->SetSizerAndFit(m_SizerListFiltre);
}

/// @brief Vide la liste de filtre
void MainFrameView::ClearAllFilter()
{
    for(auto& check : m_listCheckbox) // suppression de tous les pointeurs vers les checkbox
    {
        if(check)
            delete check;
    }
    m_listCheckbox.clear();
    m_SizerListFiltre->Clear();
    m_panelFiltres->SetSizerAndFit(m_SizerListFiltre);
    m_viewModel->InitListFiltre();
    m_mapIndexCheckbox.clear();
}


/// @brief evenement de click sur une checkbox de filtre
/// @param evt 
void MainFrameView::ClickCheckBox(wxCommandEvent& evt)
{
    m_viewModel->HideFilter(m_mapIndexCheckbox[evt.GetId()], !evt.IsChecked());
    UpdateImage(m_CurrentImage);
}


/// @brief Event choix d'un filtre
/// @param event 
void MainFrameView::OnFiltreSelect(wxCommandEvent& event)
{
    if(!m_CurrentImage.IsOk()) // si aucune image n'est chargé on ne fait rien
        return;

    wxString filterName = "";
    PushStatusText(_("image processing ..."),0);

    std::unique_ptr<DialogConfigFilter> dlg;

    switch((MENU_FILTRE)event.GetId())
    {
        case MENU_FILTRE::ID_GAUSSIEN:
            filterName = "Gaussian";
            dlg = std::make_unique<DialogConfigFilter>(this, wxID_ANY, "Setting gaussian filter");
            dlg->InitDialog((MENU_FILTRE)event.GetId());
            if(dlg->ShowModal() == wxID_OK)
            {
                std::array<int,3> result = dlg->GetParams();
                m_viewModel->Gaussien(result[0],result[1]);
            }
            break;
        case MENU_FILTRE::ID_MEDIAN:
            filterName = "Median";
            dlg = std::make_unique<DialogConfigFilter>(this, wxID_ANY, "setting median filter");
            dlg->InitDialog((MENU_FILTRE)event.GetId());
            if(dlg->ShowModal() == wxID_OK)
            {
                std::array<int,3> result = dlg->GetParams();
                m_viewModel->Median(result[0]);
            }
            break;
        case MENU_FILTRE::ID_CONV_MOYENNEUR:
            filterName = "Average";
            m_viewModel->Average();
            break;
        case MENU_FILTRE::ID_CONV_SHARPEN:
            filterName = "Sharpen";
            m_viewModel->Sharpen();
            break;
        case MENU_FILTRE::ID_CONV_LAPLACIEN:
            filterName = "Laplacien";
            m_viewModel->LaplacienFilter();
            break;
        case MENU_FILTRE::ID_CONV_SOBEL_X:
            filterName = "Sobel X";
            m_viewModel->SobelXFilter();
            break;
        case MENU_FILTRE::ID_CONV_SOBEL_Y:
            filterName = "Sobel Y";
            m_viewModel->SobelYFilter();
            break;
        case MENU_FILTRE::ID_CANNY:
            filterName = "Canny";
            dlg = std::make_unique<DialogConfigFilter>(this, wxID_ANY, "Setting canny filter");
            dlg->InitDialog((MENU_FILTRE)event.GetId());
            if(dlg->ShowModal() == wxID_OK)
            {
                std::array<int,3> result = dlg->GetParams();
                m_viewModel->CannyFilter(result[0],result[1]);
            }
            break;
        case MENU_FILTRE::ID_SEUILLAGE:
            filterName = "Binary";
            m_viewModel->Threshold();
            break;
        case MENU_FILTRE::ID_EROSION:
            filterName = "Erode";
            dlg = std::make_unique<DialogConfigFilter>(this, wxID_ANY, "Setting erosion");
            dlg->InitDialog((MENU_FILTRE)event.GetId());
            if(dlg->ShowModal() == wxID_OK)
            {
                std::array<int,3> result = dlg->GetParams();
                m_viewModel->Erode(result[0],result[1],result[2]);
                
            }
            break;
        case MENU_FILTRE::ID_DILATATION:
            filterName = "Dilate";
            dlg = std::make_unique<DialogConfigFilter>(this, wxID_ANY, "Setting dilate");
            dlg->InitDialog((MENU_FILTRE)event.GetId());
            if(dlg->ShowModal() == wxID_OK)
            {
                std::array<int,3> result = dlg->GetParams();
                m_viewModel->Dilate(result[0],result[1],result[2]);
            }
            break;
        case MENU_FILTRE::ID_OUVERTURE:
            filterName = "Open";
            dlg = std::make_unique<DialogConfigFilter>(this, wxID_ANY, "Setting open morpho");
            dlg->InitDialog((MENU_FILTRE)event.GetId());
            if(dlg->ShowModal() == wxID_OK)
            {
                std::array<int,3> result = dlg->GetParams();
                m_viewModel->OpenFilter(result[0],result[1],result[2]);
            }
            break;
        case MENU_FILTRE::ID_FERMETURE:
            filterName = "Close";
            dlg = std::make_unique<DialogConfigFilter>(this, wxID_ANY, "Setting close morpho");
            dlg->InitDialog((MENU_FILTRE)event.GetId());
            if(dlg->ShowModal() == wxID_OK)
            {
                std::array<int,3> result = dlg->GetParams();
                m_viewModel->CloseFilter(result[0],result[1],result[2]);
            }
            break;
    }

    UpdateListCheckBoxFiltre(filterName) ; // mise à jour de la liste des checkboxs
    UpdateImage(m_CurrentImage);

    PopStatusText(0);
}


/// @brief Event fermeture de l'application
/// @param WXUNUSED 
void MainFrameView::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    int answer = wxMessageBox(_("Voulez-vous vraiment quitter l'application ?"), _("Fermeture"), wxYES_NO);
    if(answer == wxYES)
    {
        Close(true);
    }
}


/// @brief Event affichage de la documentation
/// @param event 
void MainFrameView::OnHelp(wxCommandEvent& event)
{

}
