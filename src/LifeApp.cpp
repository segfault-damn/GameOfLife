#include "LifeApp.h"

using namespace std;


LifeApp::LifeApp() {

}
LifeApp::~LifeApp() {

}

bool LifeApp::OnInit()
{
    LifeFrame* frame = new LifeFrame();
    frame->Show(true);
    return true;
}