#pragma once
#ifndef LIFE_APP_H
#define LIFE_APP_H
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "LifeFrame.h"

using namespace std;
using namespace life_game;
class LifeApp : public wxApp
{
public:
    LifeApp();
    ~LifeApp();
    virtual bool OnInit();

};

#endif LIFE_APP_H