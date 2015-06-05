#include "TouchListener.h"

using namespace std;
TouchListener::TouchListener(TouchRenderer* tr)
{
	touchrenderer = tr ;
	cout << "Touch Renderer Created" << endl ;
}


TouchListener::~TouchListener(void)
{
}


void TouchListener::addTuioObject(TuioObject *tobj){

}
void TouchListener::updateTuioObject(TuioObject *tobj){

}

void TouchListener::removeTuioObject(TuioObject *tobj){

}

void TouchListener::addTuioCursor(TuioCursor *tcur){
	touchrenderer->add(tcur->getCursorID(), tcur->getX(), tcur->getY());
	cout << "Added" << tcur->getCursorID() << endl ;
}

void TouchListener::updateTuioCursor(TuioCursor *tcur){
	touchrenderer->update(tcur->getCursorID(), tcur->getX(), tcur->getY());
	cout << "Updated" << tcur->getCursorID() << endl ;
}
void TouchListener::removeTuioCursor(TuioCursor *tcur){
	touchrenderer->remove(tcur->getCursorID());
	cout << "Removed" << tcur->getCursorID() << endl ;
}

void TouchListener::refresh(TuioTime frameTime){

}
