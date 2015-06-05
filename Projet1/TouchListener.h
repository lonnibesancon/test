#pragma once

#ifndef TUIOLISTENER
#define TUIOLISTENER

#include <iostream>
#include "TuioClient.h"
#include "TuioListener.h"
#include "TuioPoint.h"
#include "TouchRenderer.h"
#include "TuioObject.h"
#include "TuioCursor.h"
#include "TuioTime.h"

using namespace TUIO;

class TouchListener : public TuioListener

{
public:
	TouchListener(TouchRenderer* tr);
	~TouchListener(void);

	TouchRenderer* touchrenderer ;

	void addTuioObject(TuioObject *tobj);
	void updateTuioObject(TuioObject *tobj);
	void removeTuioObject(TuioObject *tobj);

	void addTuioCursor(TuioCursor *tcur);
	void updateTuioCursor(TuioCursor*tcur);
	void removeTuioCursor(TuioCursor *tcur);

	void refresh(TuioTime frameTime);

};

#endif

