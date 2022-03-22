#include "LifeApp.h"

using namespace std;


LifeApp::LifeApp() {
   
}
LifeApp::~LifeApp() {

}

bool LifeApp::OnInit()
{
    frame = new LifeFrame();
    this->frame->Show(true);
    return true;
}