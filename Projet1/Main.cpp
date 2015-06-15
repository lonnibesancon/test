#ifndef MAIN
#define MAIN

#include "Server/tcp_server.h"
#include "TouchRenderer.h"
#include "TouchListener.h"
#include "Drawing.h"


TouchRenderer* touchRenderer ;
TouchListener* touchListener ;
tcp_server* server ;
Drawing* drawing ;


DWORD WINAPI myThread(LPVOID lpParameter)
{
	tcp_server* server = (tcp_server*)lpParameter ;
	server->start_listening();
	return 0 ;
}


void start(){
	int a ;
	touchRenderer = new TouchRenderer();
	touchListener = new TouchListener(touchRenderer);
	drawing = new Drawing();
	server = new tcp_server(8000,drawing);
	HANDLE myHandle = CreateThread(0, 0, myThread, server, 0,NULL);

	drawing->read();

}

int main()
{
	/*vtkContextView* ctxView = vtkContextView::New();
	vtkCamera* cam = ctxView->GetRenderer()->GetActiveCamera();
	ctxView->GetRenderer()->SetBackground(0.0f, 0.0f, 0.0f);
	vtkRenderWindow *win1 = ctxView->GetRenderWindow();
	vtkRenderer *ren1= vtkRenderer::New();
	loadPDBRibbons(ctxView->GetRenderer(), "");
	ctxView->Render();*/
	start();
  return 0;
}

#endif
