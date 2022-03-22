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
    void SetGrid();
    void InitialGame();
    void updateGrid(vector<pair<int, int>> live_cells);
    void OnTimer(wxTimerEvent& event);
    void OnSize(wxSizeEvent& event);
    Game game;
    wxGrid* grid;
    wxTimer timer;
    int colNum;
    int rowNum;

    static const int ID_SetGrid = 1;
    static const int ID_Hello = 2;
    static const int ID_Run = 30;
    static const int ID_Timer = 3;
    static const int ID_Pause = 20;
    static const int ID_Restart = 10;

    vector<int_pair> prev_state_grid_toBe_cleared;

};

#endif LIFE_FRAME_H