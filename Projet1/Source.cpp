#include <iostream>
#include "System.h"

// First include the required header files for the VTK classes we are using.
#define vtkRenderingCore_AUTOINIT 4(vtkInteractionStyle,vtkRenderingFreeType,vtkRenderingFreeTypeOpenGL,vtkRenderingOpenGL)
#define vtkRenderingVolume_AUTOINIT 1(vtkRenderingVolumeOpenGL)

#define JETLINES "jetLinesSmall2.vtp"
#define PARTICLES "particles.vtp"
#define BUNNY "bunny.vtp"
#include "vtkConeSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkCamera.h"
#include "vtkActor.h"
#include "vtkRenderer.h"
#include "vtkCommand.h"
#include "vtkBoxWidget.h"
#include "vtkSphereWidget.h"
#include "vtkTransform.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkInteractorStyleTrackballActor.h"
#include "vtkInteractorStyleJoystickActor.h"
#include "vtkInteractorStyleMultiTouchCamera.h"
#include "vtkVolumeProperty.h"
#include "vtkStructuredPointsReader.h"

#include <vtkActor2D.h>
#include <vtkActorCollection.h>
#include <vtkAlgorithm.h>
#include <vtkCollection.h>
#include <vtkDataReader.h>
#include <vtkImageAlgorithm.h>
#include <vtkImageReader2.h>
#include <vtkMapper.h>
#include <vtkMoleculeReaderBase.h>
#include <vtkObject.h>
#include <vtkObjectBase.h>
#include <vtkProp3D.h>
#include <vtkRenderViewBase.h>
#include <vtkViewport.h>
#include <vtkVolumeMapper.h>
#include <vtkWindow.h>
#include <vtkXMLReader.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyle.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkCamera.h>
#include <vtkRenderWindow.h>
#include <vtkWin32OpenGLRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkCallbackCommand.h>

#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkConeSource.h>
#include <vtkToolkits.h>
#include <vtkXMLHierarchicalBoxDataReader.h>
#include <vtkHierarchicalDataSetGeometryFilter.h>
#include <vtkShrinkPolyData.h>
#include <vtkHierarchicalPolyDataMapper.h>
#include <vtkMultiBlockDataSet.h>
#include <vtkXMLMultiBlockDataReader.h>
#include <vtkOBJReader.h>
#include <vtkTestUtilities.h>
#include <vtkContextView.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkCubeSource.h>
#include <vtkLookupTable.h>
#include <vtkProperty.h>
#include <vtkGenericDataObjectReader.h>
#include <vtkDataSetMapper.h>
#include <vtkBoundingBox.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>
#include <vtkTimerLog.h>
#include <vtkSmartPointer.h>
#include <vtkHAVSVolumeMapper.h>
#include <vtkStructuredPointsReader.h>
#include <vtkSLCReader.h>
#include <vtkStructuredPoints.h>
#include <vtkUnstructuredGrid.h>
#include <vtkThreshold.h>
#include <vtkDataSetTriangleFilter.h>
#include <vtkPiecewiseFunction.h>
#include <vtkColorTransferFunction.h>
#include <vtkVolumeProperty.h>
#include <vtkVolume.h>
#include <vtkContourFilter.h>
#include <vtkPolyDataMapper.h>

//Protein ribbon
#include <vtkProteinRibbonFilter.h>

#include <vtkVolumeTextureMapper2D.h>
#include <vtkVolumeTextureMapper3D.h>

#include <vtkXMLPolyDataReader.h>
#include <vtkPDBReader.h>
#include <vtkPDBReader.h>
#include <vtkSphereSource.h>
#include <vtkTubeFilter.h>
#include <vtkGlyph3D.h>
#include <vtkLODActor.h>
#include <vtkNew.h>
#include <vtkMoleculeMapper.h>
#include <vtkLight.h>
#include <vtkRibbonFilter.h>
#include <vtkImageReader.h>
#include <vtkOutlineFilter.h>
#include <vtkCutter.h>
#include <vtkPlane.h>
//Configuration of the multiple passes
#include <vtkCameraPass.h>
#include <vtkLightsPass.h>
#include <vtkOpaquePass.h>
#include <vtkRenderPassCollection.h>
#include <vtkSequencePass.h>
#include <vtkOpenGLRenderer.h>
#include <vtkImplicitPlaneRepresentation.h>

#include "TUIO\TuioListener.h"

#include "TouchListener.h"

#include "TouchRenderer.h"
#include "Server/tcp_server.h"

using namespace std ;


vtkSmartPointer<vtkPlane> clipPlane;
vtkSmartPointer<vtkImplicitPlaneRepresentation> planeRep;
vtkSmartPointer<vtkActor> actorPlaneSource;

class vtkMyCallback : public vtkCommand
{
public:
  static vtkMyCallback *New()
    { return new vtkMyCallback; }
  virtual void Execute(vtkObject *caller, unsigned long, void*)
    {
	  vtkMatrix4x4 *matrix = vtkMatrix4x4::New();
	  matrix->Identity();
	  matrix->SetElement(0,0,2);
	  matrix->SetElement(1,1,2);
	  matrix->SetElement(2,2,2);
	  matrix->SetElement(0,1,0.6);
	  matrix->SetElement(0,2,0.001);
	  matrix->SetElement(1,0,-0.6);
	  matrix->SetElement(1,2,0.02);
	  matrix->SetElement(2,0,-0.015);
	  matrix->SetElement(2,1,0.015);
      vtkTransform *t = vtkTransform::New();
      vtkBoxWidget *widget = reinterpret_cast<vtkBoxWidget*>(caller);
      //int translation = widget->Get
      widget->GetProp3D()->SetUserMatrix(matrix);
      //t->Delete();
    }
};



DWORD WINAPI myThread(LPVOID lpParameter)
{
	tcp_server* server = (tcp_server*)lpParameter ;
	server->start_listening();
	return 0 ;
}

void setupClippingPlane()
{
	/*
	// Define a clipping plane
	clipPlane = vtkSmartPointer<vtkPlane>::New();
	clipPlane->SetNormal(0, 0, -1);

	vtkSmartPointer<vtkImageData> box = vtkSmartPointer<vtkImageData>::New();
	box->SetDimensions(2,2,2);
	int boxSize = 100000;
	double origin[3];
	clipPlane->GetOrigin(origin);
	origin[0] -= boxSize / 2;
	origin[1] -= boxSize / 2;
	origin[2] -= boxSize / 2;
	box->SetOrigin(origin);
	box->SetSpacing(boxSize,boxSize,boxSize);

	vtkSmartPointer<vtkOutlineFilter> outline = vtkSmartPointer<vtkOutlineFilter>::New();
	outline->SetInput(box);
	vtkSmartPointer<vtkCutter> cutter = vtkSmartPointer<vtkCutter>::New();
	cutter->SetInput(box);
	cutter->SetCutFunction(clipPlane);


	//vtkSmartPointer<vtkPlaneSource> clipPlaneSource = vtkSmartPointer<vtkPlaneSource>::New();
	//clipPlaneSource->SetOrigin(0, 0, 60);
	//clipPlaneSource->SetPoint1(0,60,0);
	//clipPlaneSource->SetPoint2(60,0,60);
	// Create a mapper and actor
	vtkSmartPointer<vtkPolyDataMapper> mapperPlaneSource = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapperPlaneSource->SetInput(cutter->GetOutput());

	actorPlaneSource = vtkSmartPointer<vtkActor>::New();
	actorPlaneSource->SetMapper(mapperPlaneSource);
	//ren[0]->AddActor(actorPlaneSource);*/
}


void dummy (){

	TouchRenderer* touchRenderer = new TouchRenderer();
	TouchListener* touchlistener;
	touchlistener = new TouchListener(touchRenderer);

	tcp_server* server = new tcp_server(8000);
	HANDLE myHandle = CreateThread(0, 0, myThread, server, 0,NULL);
	
    
  //
  // Next we create an instance of vtkConeSource and set some of its
  // properties. The instance of vtkConeSource "cone" is part of a
  // visualization pipeline (it is a source process object); it produces data
  // (output type is vtkPolyData) which other filters may process.
  //
  vtkConeSource *cone = vtkConeSource::New();
  cone->SetHeight( 3.0 );
  cone->SetRadius( 1.0 );
  cone->SetResolution( 10 );

  //
  // In this example we terminate the pipeline with a mapper process object.
  // (Intermediate filters such as vtkShrinkPolyData could be inserted in
  // between the source and the mapper.)  We create an instance of
  // vtkPolyDataMapper to map the polygonal data into graphics primitives. We
  // connect the output of the cone souece to the input of this mapper.
  //
  vtkPolyDataMapper *coneMapper = vtkPolyDataMapper::New();
  coneMapper->SetInputConnection( cone->GetOutputPort() );

  //
  // Create an actor to represent the cone. The actor orchestrates rendering
  // of the mapper's graphics primitives. An actor also refers to properties
  // via a vtkProperty instance, and includes an internal transformation
  // matrix. We set this actor's mapper to be coneMapper which we created
  // above.
  //
  vtkActor *coneActor = vtkActor::New();
  coneActor->SetMapper( coneMapper );

  //
  // Create the Renderer and assign actors to it. A renderer is like a
  // viewport. It is part or all of a window on the screen and it is
  // responsible for drawing the actors it has.  We also set the background
  // color here.
  //
  vtkRenderer *ren1= vtkRenderer::New();
  ren1->AddActor( coneActor );
  ren1->SetBackground( 0.1, 0.2, 0.4 );

  //
  // Finally we create the render window which will show up on the screen.
  // We put our renderer into the render window using AddRenderer. We also
  // set the size to be 300 pixels by 300.
  //
  vtkRenderWindow *renWin = vtkRenderWindow::New();
  renWin->AddRenderer( ren1 );
  renWin->SetSize( 300, 300 );

  //
  // The vtkRenderWindowInteractor class watches for events (e.g., keypress,
  // mouse) in the vtkRenderWindow. These events are translated into
  // event invocations that VTK understands (see VTK/Common/vtkCommand.h
  // for all events that VTK processes). Then observers of these VTK
  // events can process them as appropriate.
  vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
  iren->SetRenderWindow(renWin);

  //
  // By default the vtkRenderWindowInteractor instantiates an instance
  // of vtkInteractorStyle. vtkInteractorStyle translates a set of events
  // it observes into operations on the camera, actors, and/or properties
  // in the vtkRenderWindow associated with the vtkRenderWinodwInteractor.
  // Here we specify a particular interactor style.
  vtkInteractorStyleMultiTouchCamera *style =
    vtkInteractorStyleMultiTouchCamera::New();
  iren->SetInteractorStyle(style);

  //
  // Here we use a vtkBoxWidget to transform the underlying coneActor (by
  // manipulating its transformation matrix). Many other types of widgets
  // are available for use, see the documentation for more details.
  //
  // The SetInteractor method is how 3D widgets are associated with the render
  // window interactor. Internally, SetInteractor sets up a bunch of callbacks
  // using the Command/Observer mechanism (AddObserver()). The place factor
  // controls the initial size of the widget with respect to the bounding box
  // of the input to the widget.
  vtkBoxWidget *boxWidget = vtkBoxWidget::New();
  boxWidget->SetInteractor(iren);
  boxWidget->SetPlaceFactor(1.25);
  vtkSphereWidget *sphereWidget = vtkSphereWidget::New();
  sphereWidget->SetInteractor(iren);
  sphereWidget->SetProp3D(coneActor);
  sphereWidget->PlaceWidget();
  vtkMyCallback *callback = vtkMyCallback::New();
  sphereWidget->AddObserver(vtkCommand::InteractionEvent, callback);

  //
  // Place the interactor initially. The input to a 3D widget is used to
  // initially position and scale the widget. The EndInteractionEvent is
  // observed which invokes the SelectPolygons callback.
  //
  

  //
  // Normally the user presses the "i" key to bring a 3D widget to life. Here
  // we will manually enable it so it appears with the cone.
  //
  //boxWidget->On();
  sphereWidget->On();

  //
  // Start the event loop.
  //
  iren->Initialize();
  iren->Start();

  //
  // Free up any objects we created. All instances in VTK are deleted by
  // using the Delete() method.
  //
  cone->Delete();
  coneMapper->Delete();
  coneActor->Delete();
  callback->Delete();
  boxWidget->Delete();
  ren1->Delete();
  renWin->Delete();
  iren->Delete();
  style->Delete();

}



void read(){
	/*std::string filename = BUNNY;
	// Read all the data from the file
  vtkSmartPointer<vtkXMLPolyDataReader> reader =vtkSmartPointer<vtkXMLPolyDataReader>::New();
  reader->SetFileName(filename.c_str());
  reader->Update();

  cout << "File Found and Loaded : " << filename << endl ;

	vtkContourFilter* contour = vtkContourFilter::New();
	contour->SetValue(0,0.26);
	contour->SetInputConnection(reader->GetOutputPort());
 
  // Visualize
	vtkLookupTable* lu = vtkLookupTable::New();
	lu->SetNumberOfTableValues(256);
	lu->SetHueRange(0.6667, 0);
	lu->SetTableRange(0.981, 1.01);
	lu->Build();
	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(reader->GetOutputPort());
	mapper->SetLookupTable(lu);
	mapper->SetInterpolateScalarsBeforeMapping(1);
	mapper->SetScalarMode(VTK_SCALAR_MODE_USE_POINT_FIELD_DATA);
	mapper->SelectColorArray("RHO");
	mapper->SetUseLookupTableScalarRange(1);

	// Add ids to the points and cells of the sphere
	vtkSmartPointer<vtkIdFilter> idFilter = 
	vtkSmartPointer<vtkIdFilter>::New();
	idFilter->SetInputConnection(sphereSource->GetOutputPort());
	idFilter->Update();

	vtkSmartPointer<vtkPlane> plane = vtkSmartPointer<vtkPlane>::New();
	plane->SetOrigin(0, 0, 0);
	plane->SetNormal(1, 1, 1); // Convert the DataSet to a GenericDataSet
	vtkSmartPointer<vtkBridgeDataSet> bridgeDataSet = vtkSmartPointer<vtkBridgeDataSet>::New();
	bridgeDataSet->SetDataSet(idFilter->GetOutput());

	vtkSmartPointer<vtkGenericClip> clipper =
	vtkSmartPointer<vtkGenericClip>::New();
	clipper->SetClipFunction(plane);
	clipper->SetInputData(bridgeDataSet);
	clipper->Update();


	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
 
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer);
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderWindowInteractor->SetRenderWindow(renderWindow);
 
	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renderWindow);

	vtkInteractorStyleMultiTouchCamera *style =
	vtkInteractorStyleMultiTouchCamera::New();
	iren->SetInteractorStyle(style);

	vtkBoxWidget *boxWidget = vtkBoxWidget::New();
	boxWidget->SetInteractor(iren);
	boxWidget->SetPlaceFactor(1.25);
	vtkMyCallback *callback = vtkMyCallback::New();
	boxWidget->AddObserver(vtkCommand::InteractionEvent, callback);
	boxWidget->On();

	renderer->AddActor(actor);
	renderer->SetBackground(0,0,0); // Background color green
 
	renderWindow->PolygonSmoothingOn();
	renderWindow->Render();
	renderWindowInteractor->Start();*/
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
	dummy();

  return 0;
}
