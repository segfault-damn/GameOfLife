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
    void OnGrid(wxCommandEvent& event);
    void OnGridEnter(wxCommandEvent& event);
    void OnTimer(wxTimerEvent& event);
    void OnSize(wxSizeEvent& event);
    void OnColourMode(wxCommandEvent& event);
    void OnTraceMode(wxCommandEvent& event);
    void SetGrid();
    void SetColour();
    void InitialGame();
    void InitialGridComboBox();
    void updateGrid(vector<pair<int, int>> live_cells);

    Game game;
    wxGrid* grid;
    wxComboBox* gridComboBox;
    wxTimer timer;
    int colNum;
    int rowNum;
 
    wxColour backgroundColour;
    wxColour cellColour;
    wxColour traceColour;
    wxColour lineColour;
    bool trace_mode;
    bool day_night_mode;
    static const int ID_SetGrid = 1;
    static const int ID_Hello = 2;
    static const int ID_Run = 30;
    static const int ID_Timer = 3;
    static const int ID_Pause = 20;
    static const int ID_Restart = 10;
    static const int ID_GridComboBox = 15;
    static const int ID_ColourMode = 14;
    static const int ID_TraceMode = 13;
    vector<int_pair> prev_state_grid_toBe_cleared;

};

#endif LIFE_FRAME_H