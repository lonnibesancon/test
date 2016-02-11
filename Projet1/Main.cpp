#ifndef MAIN
#define MAIN

#include "Server/tcp_server.h"
#include "TouchRenderer.h"
#include "TouchListener.h"
#include "Drawing.h"
#include "SCProtocol.h"

TouchRenderer* touchRenderer ;
TouchListener* touchListener ;
tcp_server* server ;
Drawing* drawing ;


DWORD WINAPI myThread(LPVOID lpParameter)
{
	server->start_listening();
	return 0 ;
}

std::string getStringFromMatrix(vtkMatrix4x4* mat){
	string s = "";
	for(int i = 0 ; i < 4 ; i++){
		for(int j = 0 ; j < 4 ; j++){
			s+=to_string(mat->GetElement(i,j))+";";
		}
	}
	return s ;
}


void start(){

	touchRenderer = new TouchRenderer();
	touchListener = new TouchListener(touchRenderer);
	drawing = new Drawing();
	//drawing->read();
	drawing->read();
	server = new tcp_server(SERVERPORT,drawing) ;
	//HANDLE myHandle = CreateThread(0, 0, myThread, server, 0,NULL);
	//WaitForMultipleObjects(1, &myHandle, TRUE, INFINITE);
	cout << "Server starts listenning" << endl ;

	server->start_listening();
	//vtkMatrix4x4* mat = drawing->mainActor->GetUserMatrix();
	//vtkMatrix4x4* matt = drawing->mainActor->GetMatrix();
	//cout << "User Matrix = " << getStringFromMatrix(mat) << endl << endl ;
	//cout << "     Matrix = " << getStringFromMatrix(matt) << endl << endl ;
	double i = 0.5 ;
	while(server->hasToClose == false){
		//cout << "loop" << endl ;
		server->waitForMessage();
		//drawing->dummyPlaneInteraction(i);
		drawing->updateView();	
		//out << "     Matrix = " << getStringFromMatrix(matt) << endl << endl ;
		

	}
	delete(server);
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
