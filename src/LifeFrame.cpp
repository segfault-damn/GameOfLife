#include "LifeFrame.h"



wxBEGIN_EVENT_TABLE(LifeFrame, wxFrame)
    EVT_TIMER(ID_Timer, LifeFrame::OnTimer)
    EVT_COMBOBOX(ID_GridComboBox, LifeFrame::OnGridEnter)
    EVT_CLOSE(LifeFrame::OnClose)
wxEND_EVENT_TABLE()

LifeFrame::LifeFrame() : wxFrame(NULL, wxID_ANY, "Game of Life", wxPoint(10,10), wxSize(600,600)) , game(1,1), timer(this, ID_Timer)
{
    SetMinSize(wxSize(350, 200));
    wxMenu* menuConfig = new wxMenu;
    menuConfig->Append(ID_SetGrid, "&Grid...\tCtrl-G",
        "Set world size");

    wxMenu* menuFile = new wxMenu;
    menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
        "Help string shown in status bar for this menu item");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    wxMenu* menuRun = new wxMenu;
    menuRun->Append(ID_Run, "&Run...\tCtrl-R",
        "Start the game!");
    menuRun->Append(ID_Pause, "&Pause...\tCtrl-P",
        "Pause the game!");
    menuRun->Append(ID_Restart, "&Restart",
        "Restart the game!");

    wxMenu* menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuRun, "&Game");
    menuBar->Append(menuConfig, "&Config");
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");
    SetMenuBar(menuBar);

    CreateStatusBar();
    SetStatusText("Thanks to Conway!");


    this->rowNum = 10;
    this->colNum = 10;
    this->grid = NULL;
    this->trace_mode = 0;
    this->day_night_mode = 1;
    if (day_night_mode) {
        this->backgroundColour = wxColour(249, 252, 242);
        this->cellColour = wxColour(1, 28, 17);
        this->traceColour = wxColour(182, 219, 218);
        this->lineColour = wxColour(213, 217, 212);
    }
    else {
        this->backgroundColour = wxColour(249, 252, 242);
        this->cellColour = wxColour(19, 12, 54);
        this->traceColour = wxColour(19, 12, 54);
        this->lineColour = wxColour(249, 252, 242);
    }

    // ------------- Content -------------------------
    // Create a wxGrid object   

    SetGrid();

    InitialGame();
    Bind(wxEVT_MENU, &LifeFrame::OnHello, this, ID_Hello);
    Bind(wxEVT_MENU, &LifeFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &LifeFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_MENU, &LifeFrame::OnGrid, this, ID_SetGrid);
    Bind(wxEVT_MENU, &LifeFrame::OnRun, this, ID_Run);
    Bind(wxEVT_MENU, &LifeFrame::OnPause, this, ID_Pause);
    Bind(wxEVT_MENU, &LifeFrame::OnRestart, this, ID_Restart);
}

LifeFrame::~LifeFrame() {
    this->prev_state_grid_toBe_cleared.clear();
    delete this->grid;
    delete this->gridComboBox;
}

void LifeFrame::InitialGame() {
    Game game = Game(rowNum, colNum);
    this->game = game;
    this->game.set_status(make_pair(5, 5), true);
    this->game.set_status(make_pair(6, 6), true);
    this->game.set_status(make_pair(7, 6), true);
    this->game.set_status(make_pair(7, 5), true);
    this->game.set_status(make_pair(7, 4), true);

    vector<pair<int, int>> live_cells = this->game.get_live_cells();
    updateGrid(live_cells);
}

void LifeFrame::OnExit(wxCommandEvent& event)
{
    if (timer.IsRunning())
        timer.Stop();
    Close(true);
}

void LifeFrame::OnClose(wxCloseEvent& event)
{
    if (timer.IsRunning())
        timer.Stop();
    Destroy();
}

void LifeFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("This is a wxWidgets Hello World example",
        "About Hello World", wxOK | wxICON_INFORMATION);
}
void LifeFrame::OnHello(wxCommandEvent& event)
{
    wxLogMessage("Hello world from wxWidgets!");
}


void LifeFrame::OnGrid(wxCommandEvent& event)
{
    static const wxString choiceArray[5] =
    { "Ultra Small (5x5)", "Small (15x15)", "Middle (30x30)", "Large (70x50)", "Ultra Large (120x70)" };
    this->gridComboBox = new wxComboBox(this, ID_GridComboBox, "", wxPoint(30, 30), wxSize(100, 100), 5, choiceArray);
    this->gridComboBox->Popup();
}

void LifeFrame::OnRun(wxCommandEvent& event)
{
    timer.Start(10);
}

void LifeFrame::OnGridEnter(wxCommandEvent& event)
{

    int comboValue = this->gridComboBox->GetSelection();
    switch (comboValue) {
        case 0:
            this->rowNum = 5;
            this->colNum = 5;
            break;
        case 1:
            this->rowNum = 15;
            this->colNum = 15;
            break;
        case 2:
            this->rowNum = 30;
            this->colNum = 30;
            break;
        case 3:
            this->rowNum = 50;
            this->colNum = 70;
            this->SetClientSize(1400, 1000);
            break;
        case 4:
            this->rowNum = 70;
            this->colNum = 120;
         //   long styleflag = GetWindowStyle();
          //  SetWindowStyle(styleflag | wxSTAY_ON_TOP);
          //  this->ShowFullScreen(true, wxFULLSCREEN_NOCAPTION);
            this->SetClientSize(2000,2000);
            break;
    }
    SetGrid();
    this->gridComboBox->Hide();   
    InitialGame();
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
    timer.Stop();
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

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(this->grid, 1, wxEXPAND, 0);
    SetSizer(sizer);
    sizer->Fit(this);

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
    vector<pair<int, int>> live_cells = game.get_live_cells();
    updateGrid(live_cells);
}

void LifeFrame::updateGrid(vector<pair<int, int>> live_cells)
{ 
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
