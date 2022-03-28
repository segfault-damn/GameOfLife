#pragma once
#ifndef LIFE_FRAME_H
#define LIFE_FRAME_H
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include "game.h"
#include <wx/timer.h>
#include <wx/grid.h>


using namespace life_game;

class LifeFrame : public wxFrame
{
public:
    LifeFrame();
    ~LifeFrame();
    wxDECLARE_EVENT_TABLE();
private:
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnClose(wxCloseEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnRun(wxCommandEvent& event); 
    void OnPause(wxCommandEvent& event);
    void OnRestart(wxCommandEvent& event);
    void OnTimer(wxTimerEvent& event);
    void OnSize(wxSizeEvent& event);
    void OnColourMode(wxCommandEvent& event);
    void OnTraceMode(wxCommandEvent& event);
    void OnFullScreen(wxCommandEvent& event);
    void OnGridUS(wxCommandEvent& event);
    void OnGridS(wxCommandEvent& event);
    void OnGridM(wxCommandEvent& event);
    void OnGridL(wxCommandEvent& event);
    void OnGridUL(wxCommandEvent& event);
    void OnCell(wxCommandEvent& event);    
    void OnButtonClicked(wxCommandEvent& event);
    void OnConfirm(wxCommandEvent& event);
    void GridSizeEnter();
    void SetGrid();
    void SetColour();
    void InitialGame();
    void InitialBtn();
    void DeleteBtn();
    void updateGrid();

    Game game;
    wxGrid* grid;
    wxTimer timer;
    int colNum;
    int rowNum;
 
    wxColour backgroundColour;
    wxColour cellColour;
    wxColour traceColour;
    wxColour lineColour;
    bool trace_mode;
    bool day_night_mode;
    bool full_screen_mode;
    int grid_size_mode;
    wxMenuBar* menuBar;
    wxBoxSizer* grid_sizer;
    wxGridSizer* button_grid_sizer;
    static const int ID_SetGrid = 1;
    static const int ID_Hello = 2;
    static const int ID_Run = 30;
    static const int ID_Timer = 3;
    static const int ID_Pause = 20;
    static const int ID_Restart = 10;
    static const int ID_GridComboBox = 15;
    static const int ID_ColourMode = 14;
    static const int ID_TraceMode = 13;
    static const int ID_FullScreen = 12;
    static const int ID_GRID_US = 40;
    static const int ID_GRID_S = 41;
    static const int ID_GRID_M = 42;
    static const int ID_GRID_L = 43;
    static const int ID_GRID_UL = 44;
    static const int ID_SetGame = 50;
    static const int ID_GRID_BTN_CFM = 100;
    vector<int_pair> prev_state_grid_toBe_cleared; 
    vector<int_pair> initial_live_cells;
    wxButton ** btn;
};

#endif LIFE_FRAME_H