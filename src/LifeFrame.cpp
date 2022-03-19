#include "LifeFrame.h"



wxBEGIN_EVENT_TABLE(LifeFrame, wxFrame)
    EVT_TIMER(ID_Timer, LifeFrame::OnTimer)
wxEND_EVENT_TABLE()

LifeFrame::LifeFrame() : wxFrame(NULL, wxID_ANY, "Game of Life") , game(1,1), timer(this, ID_Timer)
{
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

    wxMenu* menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuConfig, "&Config");
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");
    menuBar->Append(menuRun, "&Run");
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
        wxSize(100, 100));;
    SetGrid();
    // ------------- Game -----------------------------
    Game game = Game(rowNum, colNum);
    this->game = game;
    this->game.set_status(make_pair(8, 5), true);
    this->game.set_status(make_pair(9, 6), true);
    this->game.set_status(make_pair(10, 6), true);
    this->game.set_status(make_pair(10, 5), true);
    this->game.set_status(make_pair(10, 4), true);

    Bind(wxEVT_MENU, &LifeFrame::OnHello, this, ID_Hello);
    Bind(wxEVT_MENU, &LifeFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &LifeFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_MENU, &LifeFrame::OnRun, this, ID_Run);
}


LifeFrame::~LifeFrame() {

}
void LifeFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
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

void LifeFrame::SetGrid()
{
    this->grid->CreateGrid(this->rowNum, this->colNum);
    // We can set the sizes of individual rows and columns
    // in pixels
    this->grid->HideColLabels();
    this->grid->HideRowLabels();

    wxSize size = this->GetClientSize();
    int height = size.GetHeight();
    int width = size.GetWidth();
    wxSize fixed_size(width, height);
    this->grid->SetSize(fixed_size);

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(this->grid, 1, wxEXPAND, 0);
    SetSizer(sizer);
    for (int i = 0; i < grid->GetNumberCols(); i++) {
        this->grid->SetColSize(i, width / colNum);
        // Read-only for every cells
        for (int j = 0; j < grid->GetNumberRows(); j++) {
            grid->SetReadOnly(i, j);
        }
    }
    for (int i = 0; i < grid->GetNumberRows(); i++) {
        this->grid->SetRowSize(i, height / rowNum);
    }
    grid->SetReadOnly(0, 3);
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
    vector<pair<int, int>> live_cells;
    live_cells = game.get_live_cells();
    updateGrid(live_cells);
    game.update_game();
    wxWindow::Update();
}

void LifeFrame::updateGrid(vector<pair<int, int>> live_cells)
{   
    for (int i = 0; i < grid->GetNumberCols(); i++)
        for (int j = 0; j < grid->GetNumberRows(); j++)
            grid->SetCellBackgroundColour(i, j, wxColour("WHITE"));
   
    
    for (int i = 0; i < live_cells.size(); i++) {    
        pair<int, int> pos = live_cells[i];
        this->grid->SetCellBackgroundColour(pos.first, pos.second, wxColour(87, 104, 189));
    }
    grid->ForceRefresh();
}
