#include "LifeFrame.h"

wxBEGIN_EVENT_TABLE(LifeFrame, wxFrame)
    EVT_TIMER(ID_Timer, LifeFrame::OnTimer)
    EVT_CLOSE(LifeFrame::OnClose)
wxEND_EVENT_TABLE()

LifeFrame::LifeFrame() : wxFrame(NULL, wxID_ANY, "Game of Life", wxPoint(10,10), wxSize(600,600)) , game(1,1), timer(this, ID_Timer)
{
    wxIcon* icon = new wxIcon(iconbitmap);
    this->SetIcon(*icon);

    wxMenu* menuGrid = new wxMenu;
    menuGrid->Append(ID_GRID_US, "Ultra Small (5x5)");
    menuGrid->Append(ID_GRID_S, "Small (15x15)");
    menuGrid->Append(ID_GRID_M, "Medium (30x30)");
    menuGrid->Append(ID_GRID_L, "Large (50x40)");
    menuGrid->Append(ID_GRID_UL, "Ultra Large (70x50)");

    wxMenu* menuSpeed = new wxMenu;
    menuSpeed->AppendRadioItem(ID_Speed + 1, "Very Slow (2s)");
    menuSpeed->AppendRadioItem(ID_Speed + 2, "Slow (1s)");
    menuSpeed->AppendRadioItem(ID_Speed + 3, "Medium (0.3s)");
    menuSpeed->AppendRadioItem(ID_Speed + 4, "Fast (0.08s)");
    menuSpeed->AppendRadioItem(ID_Speed + 5, "Very Fast (0.04s) [HIGH CPU REQUIREMENT]");
    menuSpeed->Check(ID_Speed + 3, true);

    this->menuConfig = new wxMenu;
    menuConfig->AppendSubMenu(menuGrid, "&Grid",
        "Set world size...");
    menuConfig->Append(ID_SetGame, "&Cell",
        "Choose live cells to start...");
    menuConfig->AppendSubMenu(menuSpeed , "&Refresh Rate",
        "Choose the speed of the game...");
    menuConfig->AppendSeparator();
    menuConfig->AppendCheckItem(ID_ColourMode, "&Night Mode",
        "Toggle Day/Night Mode");
    menuConfig->AppendCheckItem(ID_TraceMode, "&Trace Mode",
        "Toggle Trace Mode");
    menuConfig->AppendCheckItem(ID_FullScreen, "&Full Screen",
        "Toggle Full Screen");   

    this->menuRun = new wxMenu;
    menuRun->Append(ID_Run, "&Run\tCtrl-R",
        "Start the game!");
    menuRun->Append(ID_Pause, "&Pause\tCtrl-P",
        "Pause the game!");
    menuRun->Append(ID_Restart, "&Restart",
        "Restart the game!");
    menuRun->AppendSeparator();
    menuRun->Append(wxID_EXIT);

    this->menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    menuBar = new wxMenuBar;
    menuBar->Append(menuRun, "&Game");
    menuBar->Append(menuConfig, "&Config");
    menuBar->Append(menuHelp, "&Help");
    SetMenuBar(menuBar);
    this->SetColour();
    this->SetBackgroundColour(this->btnBackColour);
    CreateStatusBar();
    SetStatusText("Thanks to Conway!");

    this->rowNum = 15;
    this->colNum = 15;
    this->grid = NULL;
    this->btn = NULL;
    this->grid_sizer = new wxBoxSizer(wxVERTICAL);
    this->button_grid_sizer = NULL;

    // ------------- Content -------------------------
    // Create a wxGrid object   

    SetColour();
    SetGrid();
    InitialGame();
    InitialBtn();
    
    Bind(wxEVT_MENU, &LifeFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &LifeFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_MENU, &LifeFrame::OnRun, this, ID_Run);
    Bind(wxEVT_MENU, &LifeFrame::OnPause, this, ID_Pause);
    Bind(wxEVT_MENU, &LifeFrame::OnRestart, this, ID_Restart);
    Bind(wxEVT_MENU, &LifeFrame::OnColourMode, this, ID_ColourMode);
    Bind(wxEVT_MENU, &LifeFrame::OnTraceMode, this, ID_TraceMode);
    Bind(wxEVT_MENU, &LifeFrame::OnFullScreen, this, ID_FullScreen);
    Bind(wxEVT_MENU, &LifeFrame::OnGridUS, this, ID_GRID_US);
    Bind(wxEVT_MENU, &LifeFrame::OnGridS, this, ID_GRID_S);
    Bind(wxEVT_MENU, &LifeFrame::OnGridM, this, ID_GRID_M);
    Bind(wxEVT_MENU, &LifeFrame::OnGridL, this, ID_GRID_L);
    Bind(wxEVT_MENU, &LifeFrame::OnGridUL, this, ID_GRID_UL);
    Bind(wxEVT_MENU, &LifeFrame::OnCell, this, ID_SetGame);
    for (int i = 1; i <= 5; i++) {
        Bind(wxEVT_MENU, &LifeFrame::OnSpeed, this, ID_Speed + i);
    }
}

LifeFrame::~LifeFrame() {
    if (this->GetSizer() == this->grid_sizer) {
        if (button_grid_sizer != NULL)
            delete this->button_grid_sizer;
    }
    else if (this->GetSizer() == this->button_grid_sizer) {
        if (grid_sizer != NULL)
            delete this->grid_sizer;
    }
    this->prev_state_grid_toBe_cleared.clear();
    delete this->grid;
    DeleteBtn();
}

void LifeFrame::InitialGame() {
    Game game = Game(rowNum, colNum);
    this->game = game;
    for (int i = 0; i < this->initial_live_cells.size(); i++) {
        int_pair pos = initial_live_cells[i];
        this->game.set_status(pos, true);
    }
 //   this->game.set_status(make_pair(5, 5), true);
 //   this->game.set_status(make_pair(6, 6), true);
 //   this->game.set_status(make_pair(7, 6), true);
  //  this->game.set_status(make_pair(7, 5), true);
  //  this->game.set_status(make_pair(7, 4), true);

    updateGrid();
}

void LifeFrame::InitialBtn() {
    btn = new wxButton*[rowNum * colNum];
    for (int i = 0; i < rowNum; i++) {
        for (int j = 0; j < colNum; j++) {
            btn[i * colNum + j] = new wxButton(this, 1000 + (i * colNum + j));
            btn[i * colNum + j]->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &LifeFrame::OnButtonClicked, this);
            btn[i * colNum + j]->SetBackgroundColour(this->backgroundColour);
            btn[i * colNum + j]->Hide();
        }
    }
    Layout();
}

void LifeFrame::DeleteBtn() {
    for (int i = 0; i < rowNum; i++) {
        for (int j = 0; j < colNum; j++) {
            delete this->btn[i * colNum + j];
        }
    }
    delete [] this->btn;
    this->btn = NULL;
}

void LifeFrame::OnExit(wxCommandEvent& event)
{
    if (timer.IsRunning())
        timer.Stop();
    Close(true);
}
void LifeFrame::OnButtonClicked(wxCommandEvent& event)
{
    
    int x = (event.GetId() - 1000) / (this->colNum);
    int y = (event.GetId() - 1000) % (this->colNum);
    wxButton* btn = (wxButton *) event.GetEventObject();
    if (this->game.get_status(make_pair(x, y))) {
        this->game.set_status(make_pair(x, y), false); 
        btn->SetBackgroundColour(this->backgroundColour);
    }
    else {
        this->game.set_status(make_pair(x, y), true);
        btn->SetBackgroundColour(this->cellColour);
    }
}


void LifeFrame::OnClose(wxCloseEvent& event)
{
    if (timer.IsRunning())
        timer.Stop();
    Destroy();
}

void LifeFrame::OnAbout(wxCommandEvent& event)
{
    wxAboutDialogInfo info;
    info.SetWebSite("https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life");
    info.SetVersion(this->version);
    info.SetDescription(_("Click 'Config->Cell' to set the game!"));
    info.AddDeveloper("Daisy");
    info.AddDeveloper("Laisen");
    info.SetName("Game Of Life");
    wxAboutBox(info, this);
}

void LifeFrame::OnRun(wxCommandEvent& event)
{
    timer.Start(this->speed_mode);
}

void LifeFrame::OnSpeed(wxCommandEvent& event) {
    int speed_mode_ID = event.GetId() - ID_Speed;
    switch (speed_mode_ID) {
        case 1:
            this->speed_mode = 2000;
            break;
        case 2:
            this->speed_mode = 1000;
            break;
        case 3:
            this->speed_mode = 300;
            break;
        case 4:
            this->speed_mode = 80;
            break;
        case 5:
            this->speed_mode = 40;
            break;
    }
    if (timer.IsRunning()) {
        timer.Stop();
        timer.Start(this->speed_mode);
    }
}

void LifeFrame::OnGridUS(wxCommandEvent& event) {
    if (this->grid_size_mode != 1) {
        this->grid_size_mode = 1;
        GridSizeEnter();
    }
}

void LifeFrame::OnGridS(wxCommandEvent& event) {
    if (this->grid_size_mode != 2) {
        this->grid_size_mode = 2;
        GridSizeEnter();
    }
}

void LifeFrame::OnGridM(wxCommandEvent& event) {
    if (this->grid_size_mode != 3) {
        this->grid_size_mode = 3;
        GridSizeEnter();
    }
}

void LifeFrame::OnGridL(wxCommandEvent& event) {
    if (this->grid_size_mode != 4) {
        this->grid_size_mode = 4;
        GridSizeEnter();
    }
}

void LifeFrame::OnGridUL(wxCommandEvent& event) {
    if (this->grid_size_mode != 5) {
        this->grid_size_mode = 5;
        GridSizeEnter();
    }
}

void LifeFrame::GridSizeEnter()
{
    DeleteBtn();
    switch (this->grid_size_mode) {
        case 1:
            this->rowNum = 5;
            this->colNum = 5;
            break;
        case 2:
            this->rowNum = 15;
            this->colNum = 15;
            break;
        case 3:
            this->rowNum = 30;
            this->colNum = 30;
            break;
        case 4:
            this->rowNum = 40;
            this->colNum = 50;
            this->SetClientSize(1000, 800);
            break;
        case 5:
            this->rowNum = 50;
            this->colNum = 70;
            this->SetClientSize(1400, 1000);
            break;
    }
    if(timer.IsRunning())
        timer.Stop();
    prev_state_grid_toBe_cleared.clear();
    initial_live_cells.clear();
    SetGrid();
    InitialGame();
    InitialBtn();
}

void LifeFrame::OnColourMode(wxCommandEvent& event)
{
    this->day_night_mode = !this->day_night_mode;
    SetColour();
    SetGrid();
    updateGrid();
    this->SetBackgroundColour(this->btnBackColour);
    // update btn colour
    for (int i = 0; i < rowNum; i++) {
        for (int j = 0; j < colNum; j++) {
            btn[i * colNum + j]->SetBackgroundColour(this->backgroundColour);
        }
    }

    for (int i = 0; i < initial_live_cells.size(); i++) {
        int_pair pos = initial_live_cells[i];
        btn[pos.first * colNum + pos.second]->SetBackgroundColour(this->cellColour);
    }
}

void LifeFrame::OnFullScreen(wxCommandEvent& event)
{
    if (this->full_screen_mode) {
       
        this->ShowFullScreen(false, wxFULLSCREEN_NOCAPTION);
    }
    else {
        long styleflag = GetWindowStyle();
        SetWindowStyle(styleflag | wxSTAY_ON_TOP);

        this->ShowFullScreen(true, wxFULLSCREEN_NOCAPTION);
    }
    this->full_screen_mode = !this->full_screen_mode;
}

void LifeFrame::OnTraceMode(wxCommandEvent& event)
{
    this->trace_mode = !this->trace_mode;
}

void LifeFrame::OnRestart(wxCommandEvent& event)
{
    timer.Stop();
    prev_state_grid_toBe_cleared.clear();
    SetGrid();
    InitialGame();
}

void LifeFrame::OnPause(wxCommandEvent& event)
{
    if (!timer.IsRunning()) {
        wxMessageBox("Please start the game before trying to pause!",
            "Error", wxOK | wxICON_INFORMATION);
    }
    else {
        timer.Stop();
        menuRun->Delete(ID_Pause);
        menuRun->Insert(1, ID_Resume, "&Resume\tCtrl-P",
            "Resume the game!");
        Bind(wxEVT_MENU, &LifeFrame::OnResume, this, ID_Resume);
    }
}

void LifeFrame::OnResume(wxCommandEvent& event)
{
    timer.Start(this->speed_mode);
    menuRun->Delete(ID_Resume);
    menuRun->Insert(1, ID_Pause, "&Pause\tCtrl-P",
        "Pause the game!");
    Bind(wxEVT_MENU, &LifeFrame::OnPause, this, ID_Pause);
}

void LifeFrame::OnSize(wxSizeEvent& event)
{
    wxSize size = this->GetClientSize();
    int height = size.GetHeight();
    int width = size.GetWidth();

    for (int i = 0; i < grid->GetNumberCols(); i++) {
        this->grid->SetColSize(i, width / colNum);
    }
    for (int i = 0; i < grid->GetNumberRows(); i++) {
        this->grid->SetRowSize(i, height / rowNum);
    }
    event.Skip();
}


void LifeFrame::OnTimer(wxTimerEvent& event)
{
    game.update_game();
    updateGrid();
}

void LifeFrame::OnCell(wxCommandEvent& event)
{
    if (timer.IsRunning()) {
        wxMessageBox("The program is currently running. Click 'Restart' to reset the game!",
            "Error", wxOK | wxICON_INFORMATION);
    }
    else {
        this->grid->Hide();
        this->button_grid_sizer = new wxGridSizer(rowNum, colNum, 0, 0);
        for (int i = 0; i < rowNum; i++) {
            for (int j = 0; j < colNum; j++) {
                btn[i * colNum + j]->Show();
                this->button_grid_sizer->Add(btn[i * colNum + j], 1, wxEXPAND | wxALL);
            }
        }
        this->SetSizer(this->button_grid_sizer, false);
        Layout();
        menuBar->Remove(0);
        menuBar->Remove(0);
        menuBar->Remove(0);
        wxMenu* menuConfirm = new wxMenu;
        menuConfirm->Append(ID_GRID_BTN_CLEAR, "Clear");
        menuConfirm->Append(ID_GRID_BTN_CFM, "Finish");
        menuBar->Append(menuConfirm, "Actions");
        Bind(wxEVT_MENU, &LifeFrame::OnConfirm, this, ID_GRID_BTN_CFM);
        Bind(wxEVT_MENU, &LifeFrame::OnClear, this, ID_GRID_BTN_CLEAR);
        InitialGame();
    }
}

void LifeFrame::OnConfirm(wxCommandEvent& event)
{
    this->initial_live_cells = this->game.get_live_cells();
    menuBar->Remove(0);
    for (int i = 0; i < rowNum; i++) {
        for (int j = 0; j < colNum; j++) {
            btn[i * colNum + j]->Hide();
        }
    }
    this->grid->Show();
    this->SetSizer(this->grid_sizer, true);
    button_grid_sizer = NULL;
    updateGrid();
    Layout();
    menuBar->Append(menuRun, "&Game");
    menuBar->Append(menuConfig, "&Config");
    menuBar->Append(menuHelp, "&Help");
}

void LifeFrame::OnClear(wxCommandEvent& event)
{
    prev_state_grid_toBe_cleared.clear();
    for (int i = 0; i < rowNum; i++) {
        for (int j = 0; j < colNum; j++) {
            btn[i * colNum + j]->SetBackgroundColour(this->backgroundColour);
        }
    } 
    initial_live_cells.clear();
    InitialGame();
    for (int i = 0; i < grid->GetNumberCols(); i++) {
        for (int j = 0; j < grid->GetNumberRows(); j++) {
            this->grid->SetCellBackgroundColour(j, i, this->backgroundColour);
        }
    }
}

void LifeFrame::SetGrid()
{
    wxSize size = this->GetClientSize();
    if (this->grid != NULL)
        delete grid;
    this->grid = new wxGrid(this,
        -1,
        wxPoint(0, 0),
        size);
    this->grid->CreateGrid(this->rowNum, this->colNum);
    this->grid->HideColLabels();
    this->grid->HideRowLabels();

    this->grid_sizer->Clear();
    this->grid_sizer->Add(this->grid, 1, wxEXPAND, 0);
    this->SetSizer(this->grid_sizer, false); 
    this->grid_sizer->Fit(this);
    for (int i = 0; i < grid->GetNumberCols(); i++) {
        for (int j = 0; j < grid->GetNumberRows(); j++) {
            this->grid->SetReadOnly(j, i);
            this->grid->SetCellBackgroundColour(j, i, this->backgroundColour);
        }
    }

    int height = size.GetHeight();
    int width = size.GetWidth();
    for (int i = 0; i < grid->GetNumberCols(); i++) {
        this->grid->SetColSize(i, width / colNum);
        this->grid->DisableColResize(i);
    }
    for (int i = 0; i < grid->GetNumberRows(); i++) {
        this->grid->SetRowSize(i, height / rowNum);
        this->grid->DisableRowResize(i);
    }

    Bind(wxEVT_SIZE, &LifeFrame::OnSize, this);
    this->grid->SetGridLineColour(lineColour);
}

void LifeFrame::SetColour()
{
    if (this->day_night_mode) {
        this->backgroundColour = wxColour(249, 252, 242);
        this->cellColour = wxColour(1, 28, 17);
        this->traceColour = wxColour(189, 209, 178);
        this->lineColour = wxColour(213, 217, 212);
        this->btnBackColour = wxColour(218, 224, 215);
    }
    else {
        this->backgroundColour = wxColour(0, 15, 9);
        this->cellColour = wxColour(223, 242, 233);
        this->traceColour = wxColour(42, 46, 44);
        this->lineColour = wxColour(77, 79, 78);
        this->btnBackColour = wxColour(8, 36, 69);
    }
}

void LifeFrame::updateGrid()
{ 
    vector<pair<int, int>> live_cells = this->game.get_live_cells();
    for (int i = 0; i < prev_state_grid_toBe_cleared.size(); i++) {
        pair<int, int> prev_pos = prev_state_grid_toBe_cleared[i];
        if (trace_mode)
            this->grid->SetCellBackgroundColour(prev_pos.first, prev_pos.second, this->traceColour);
        else 
            this->grid->SetCellBackgroundColour(prev_pos.first, prev_pos.second, this->backgroundColour);
    }
    prev_state_grid_toBe_cleared.clear();
    
    for (int i = 0; i < live_cells.size(); i++) {    
        pair<int, int> pos = live_cells[i];
        this->grid->SetCellBackgroundColour(pos.first, pos.second, this->cellColour);
        prev_state_grid_toBe_cleared.push_back(pos);
    }
    grid->ForceRefresh();
}
