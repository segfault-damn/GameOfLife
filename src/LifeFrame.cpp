#include "LifeFrame.h"



wxBEGIN_EVENT_TABLE(LifeFrame, wxFrame)
    EVT_TIMER(ID_Timer, LifeFrame::OnTimer)
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
    menuRun ->Append(ID_Run, "&Run...\tCtrl-R",
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

   
    this->rowNum = 100;
    this->colNum = 100;

    // ------------- Content -------------------------
    // Create a wxGrid object  
    this->grid = new wxGrid(this,
        -1,
        wxPoint(0, 0),
        wxSize(500, 500));;
    SetGrid();
    InitialGame();
    Bind(wxEVT_MENU, &LifeFrame::OnHello, this, ID_Hello);
    Bind(wxEVT_MENU, &LifeFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &LifeFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_MENU, &LifeFrame::OnRun, this, ID_Run);
    Bind(wxEVT_MENU, &LifeFrame::OnPause, this, ID_Pause);
    Bind(wxEVT_MENU, &LifeFrame::OnRestart, this, ID_Restart);
}

LifeFrame::~LifeFrame() {
    delete this->grid;
}

void LifeFrame::InitialGame() {
    Game game = Game(rowNum, colNum);
    this->game = game;
    this->game.set_status(make_pair(8, 5), true);
    this->game.set_status(make_pair(9, 6), true);
    this->game.set_status(make_pair(10, 6), true);
    this->game.set_status(make_pair(10, 5), true);
    this->game.set_status(make_pair(10, 4), true);

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

void LifeFrame::OnRun(wxCommandEvent& event)
{
    timer.Start(10);
}

void LifeFrame::OnRestart(wxCommandEvent& event)
{
    timer.Stop();
    InitialGame();
}

void LifeFrame::OnPause(wxCommandEvent& event)
{
    timer.Stop();
}

void LifeFrame::SetGrid()
{
    this->grid->CreateGrid(this->rowNum, this->colNum);
    this->grid->HideColLabels();
    this->grid->HideRowLabels();

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(this->grid, 1, wxEXPAND, 0);
    SetSizer(sizer);
    for (int i = 0; i < grid->GetNumberCols(); i++) {
        for (int j = 0; j < grid->GetNumberRows(); j++) {
            grid->SetReadOnly(i, j);
        }
    }
    Bind(wxEVT_SIZE, &LifeFrame::OnSize, this);
    grid->SetGridLineColour(wxColour(102, 101, 101));
}

void LifeFrame::OnSize(wxSizeEvent& event)
{
    wxSize size = this->GetClientSize();
    int height = size.GetHeight();
    int width = size.GetWidth();
    wxSize fixed_size(width, height);
    this->grid->SetSize(fixed_size);
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
        this->grid->SetCellBackgroundColour(prev_pos.first, prev_pos.second, wxColour("WHITE"));
    }
    prev_state_grid_toBe_cleared.clear();
    
    for (int i = 0; i < live_cells.size(); i++) {    
        pair<int, int> pos = live_cells[i];
        this->grid->SetCellBackgroundColour(pos.first, pos.second, wxColour(87, 104, 189));
        prev_state_grid_toBe_cleared.push_back(pos);
    }
    grid->ForceRefresh();
}
