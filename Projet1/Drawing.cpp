#include "Drawing.h"

vtkConeSource *cone ;
vtkPolyDataMapper *mapper ;
vtkSmartPointer<vtkTransformFilter> transformFilter;

// Callback for the interaction with the clipping plane
// This does the actual work: updates the vtkPlane implicit function.
// This in turn causes the pipeline to update and clip the object.
class vtkIPWCallback : public vtkCommand
{
public:
	static vtkIPWCallback *New() 
	{ 
		return new vtkIPWCallback;
	}
	virtual void Execute(vtkObject *caller, unsigned long, void*)
	{
		vtkImplicitPlaneWidget2 *planeWidget = reinterpret_cast<vtkImplicitPlaneWidget2*>(caller);
		myImplicitPlaneRepresentation *rep = reinterpret_cast<myImplicitPlaneRepresentation*>(planeWidget->GetRepresentation());
		rep->GetPlane(this->Plane);
	}
	vtkIPWCallback():Plane(0),Actor(0) {}
	vtkPlane *Plane;
	vtkActor *Actor;
 
};

Drawing::Drawing(void)
{
	//translation = vtkSmartPointer<vtkTransform>::New();
	ctxView = vtkContextView::New();
	RenderWindow = ctxView->GetRenderWindow();
											//For the bunny
											//ren = ctxView->GetRenderer();
											//cam = ren->GetActiveCamera();
											//iren = vtkRenderWindowInteractor::New();
											//ren->SetBackground(.0,.0,.0);
	filename = FTLE;
	velocitiesName = "Data/Velocities7.vtk" ;
											//filename = PARTICLES 										
	//clippingPlaneSet = false ;
	currentMapper = MAPPERMAIN ;

	points = vtkSmartPointer<vtkPoints>::New();
	pointData = vtkSmartPointer<vtkPolyData>::New();
	mapperPoint = vtkSmartPointer<vtkPolyDataMapper>::New();
	for(int i = 0 ; i < NBPARTICLES ; i++){
		Particle p ;
		particles.push_back(p);
	}

	/*
	baseNameScalar = "../data/Velocities_" ;
	baseNameVector = "../data/FTLE_horizon_00010_Init_00";
	number_of_timesteps = 35 ;*/
}


Drawing::~Drawing(void)
{
}

void Drawing::setFileName(std::string file){
	Drawing::filename = file ;
}

void Drawing::updateView(){
	ctxView->Render() ;
}

void Drawing::setNormalInteractionMode(){

}

void Drawing::setSeedInteractionMode(){

}

void Drawing::changeInteractionMode(int mode){
	if(interactionMode == mode){
		return ;
	}
	switch(mode){
		case NORMALINTERACTIONMODE:
			setNormalInteractionMode();
			break;
		case SEEDPLACEMENT:
			setSeedInteractionMode();
			break;
		default:
			cerr << "The mode is not recognized" << endl ;
			break;
	}

}

void Drawing::setTransformationMatrix(const int interactionMode, const double* mat){
		//cout << "--------------------------------------------------------------------------" << endl ;
		for(int i = 0 ; i < 16 ; i++){
			//mat[i] = 1 ;
			//cout << mat[i] << " ; " ;
		}
		//cout << "************************************************************************** " << endl ;
		//cout << endl ;
		vtkMatrix4x4* matrix = vtkMatrix4x4::New();
		matrix->DeepCopy(mat);
		//vtkTransform* t = vtkTransform::New();
		//t->SetMatrix(matrix);
		//mainActor->GetMatrix()->DeepCopy(matrix);
		mainActor->SetUserMatrix(matrix);
		//mainActor->PokeMatrix(matrix);
		//cout << "Matrix Changed" << endl ;
		//planeRep->SetNormalToZAxis(1);
		//planeRep->UpdatePlacement();
		outlineActor->SetUserMatrix(matrix);
		planeRep->SetMatrix(matrix);

	/*else if(interactionMode == SEEDPLACEMENT){
		//planerep->SetUserMatrix(matrix);
		vtkMatrix4x4* matrix = vtkMatrix4x4::New();
		matrix->DeepCopy(mat);
		outlineActor->SetUserMatrix(matrix);
		//mainActor->SetUserMatrix(matrix);
	}*/

}

void Drawing::setMapper(int mapper){
	cout << "TODO" << endl ;
	/*if(mapper!=currentMapper){
		this->currentMapper = mapper ;
		//cout << "Now current Mapper = " << currentMapper << endl ;
		if(mapper == MAPPERPLANE){
			cout << "coucou" << mapper << endl ;
			this->mainActor->SetMapper(mapperPlane);
		}
		else{
			//cout << "else"<< mapper << endl ;
			this->mainActor->SetMapper(mapperMain);
		}
	}*/
}


void Drawing::setPositionAndOrientation(double* position,double* orientation, const int interactionMode){
	//cout << "Set origin to " << origin[0] << ", " << origin[1] << ", " << origin[2] << endl ;
	//plane->SetOrigin(origin);
	//planerep->SetOrigin(origin);
	//ren->Render();
	//planeWidget->SetOrigin(origin[0], origin[1], origin[2]);
	//planeWidget->UpdatePlacement();
	//cout << "Position = " << position [0] << "    " << position[1] << "   " << position[2] << endl ;
	//cout << "Orientation = " << orientation [0] << "    " << orientation[1] << "   " << orientation[2] << endl << endl << endl ;
	vtkSmartPointer<vtkTransform> t = vtkTransform::SafeDownCast(plane->GetTransform());
		//t->RotateWXYZ(orientation[0],orientation[1],orientation[2],orientation[3]);
		//t->Translate(position);
		//plane->SetTransform(t);
		plane->SetOrigin(position);
		plane->SetNormal(orientation);
		//plane->SetNormal(orientation);

	//if(interactionMode == SEEDPLACEMENT ){
		planeRep->SetOrigin(plane->GetOrigin());
		planeRep->SetNormal(plane->GetNormal());
		double * origin = planeRep->GetOrigin();
		//cout << "Plan Origin " << origin[2] << " ;; " << origin[1]  << " ;; " << origin[0] << endl ;
		//planeRep->SetOrigin(plane->GetNormal());
		//planeRep->SetInteractionState(myImplicitPlaneRepresentation::Moving);
		planeRep->BuildRepresentation();
		double* planeO = plane->GetOrigin();
		double* planeRepO = planeRep->GetOrigin();
		//cout << "PlaneOrigin = " << planeO[2] ;
		//cout << " ---- PlaneRepOrigin = "  << planeRepO[2] << endl ;
		/*double * origin = planeRep->GetOrigin();
		cout << "Plan Origin " << origin[2] << " ;; " << origin[1]  << " ;; " << origin[0] << endl ;
		if(position[2] > dataBounds[5]){
			return ;
		}
		else if(position[2] < dataBounds[4]){
			return ;
		}
		double push = origin[2] - position[2] ;
		/*if(position[2] < origin[2]{
			push = origin[2]
		}
		else{

		}
		cout << "Push ======" << push << endl << endl << endl ;
		planeRep->PushPlane(push);
		planeRep->UpdatePlacement();*/
	//}

	/*else{
		vtkSmartPointer<vtkTransform> t = vtkTransform::SafeDownCast(plane->GetTransform());
		//t->RotateWXYZ(orientation[0],orientation[1],orientation[2],orientation[3]);
		t->Translate(position);
		plane->SetTransform(t);
	}*/
	


	
}

void Drawing::updateClipping(){

	int normalDirection = -1 ;
	vtkSmartPointer<vtkPlane> tmp = plane ;
	double * normal = tmp->GetNormal();
	normal[2] *= normalDirection ;
	tmp->SetNormal(normal);
	vtkSmartPointer<vtkPlaneCollection> coll = vtkSmartPointer<vtkPlaneCollection>::New();
	coll->AddItem(tmp);
	/*vtkSmartPointer<vtkPlanes> planes = vtkSmartPointer<vtkPlanes>::New();
	vtkSmartPointer<vtkPoints> origins = vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkPlaneCollection> coll = vtkSmartPointer<vtkPlaneCollection>::New();
	coll->AddItem(plane);
	origins->SetNumberOfPoints(1);
	origins->InsertNextPoint(plane->GetOrigin());
	vtkSmartPointer<vtkDoubleArray> normalsArray = vtkSmartPointer<vtkDoubleArray>::New();
	normalsArray->SetNumberOfComponents(3);
	normalsArray->SetNumberOfTuples(1);
	normalsArray->SetComponent(0,0,plane->GetNormal()[0]);
	normalsArray->SetComponent(0,1,plane->GetNormal()[1]);
	normalsArray->SetComponent(0,2,plane->GetNormal()[2]);
	planes->SetPoints(origins);
	planes->SetNormals(normalsArray);*/
	mainMapper->SetClippingPlanes(coll);
	mainMapper->Update();

}

void Drawing::dummyPlaneInteraction(double i){
	/*double origin[3] ;
	planeWidget->GetOrigin(origin);
	//origin[0] += 0.002 ;
	cout << "origin 0 " << origin[0] << endl ;
	planeWidget->SetOrigin(origin[0], origin[1], origin[2]);
	planeWidget->UpdatePlacement();*/
	vtkTransform* t = vtkTransform::SafeDownCast(plane->GetTransform());
	t->Identity();
	double trans[3] ;
	trans[0] = 0 ;
	trans[1] = 0 ;
	trans[2] = i ;
	t->Translate(trans);
	plane->SetOrigin(trans);
	plane->SetNormal(trans);
	double * normal = this->plane->GetNormal();
	double nNew[3] ;
	double* planeO = plane->GetOrigin();
	double* planeRepO = planeRep->GetOrigin();
	double* planeN = plane->GetNormal();
	double* planeRepN = planeRep->GetNormal();
	//cout << "PlaneOrigin = " << planeO[2] ;
	//cout << " ---- PlaneRepOrigin = "  << planeRepO[2] << endl ;
	cout << "PlaneNormal = " << planeN[0] << " ;;; " << planeN[1] << " ;;; " << planeN[2] ;
	cout << "PlaneRepNormal = " << planeRepN[0] << " ;;; " << planeRepN[1] << " ;;; " << planeRepN[2] << endl << endl ;
	//t->TransformNormal(normal, nNew);
	//planeRep->SetNormal(nNew);
	planeRep->SetOrigin(plane->GetOrigin());
	planeRep->SetNormal(plane->GetNormal());
	planeRep->Modified();
	planeRep->SetInteractionState(myImplicitPlaneRepresentation::Rotating);
	planeRep->BuildRepresentation();
	
	return ;
	//vtkSmartPointer<vtkPlane> p ;
	//planeRep->GetPlane(p);
	//plane->SetTransform(t);
	//planeRep->BuildRepresentation();
	
	//planeRep->SetOrigin(plane->GetOrigin());
	double * origin = planeRep->GetOrigin();
	cout << "Plan Origin " << origin[2] << " ;; " << origin[1]  << " ;; " << origin[0] << endl ;
	cout << "DataBounds 0 = " << dataBounds[0] << "DataBounds 1 = " << dataBounds[1] << endl 
		 << "DataBounds 2 = " << dataBounds[2] << "DataBounds 3 = " << dataBounds[3] << endl 
		 << "DataBounds 4 = " << dataBounds[4] << "DataBounds 5 = " << dataBounds[5] << endl 
		 << endl << endl ;
	if(origin[2] + i > dataBounds[5] ){
		cout << "Cas 1" << endl ;
		return ;
	}
	/*else if(origin[2] -i < dataBounds[4]){
		cout << "Cas 2" << endl ;
		return ;
	}*/
	planeRep->PushPlane(i);
	planeRep->UpdatePlacement();
	
	
	
	/*this->planeWidget->GetVector(v2);
	// Transform
	vtkSmartPointer<vtkTransform> transform
	= vtkSmartPointer<vtkTransform>::New();
	transform->PreMultiply();
	transform->Translate(wc[0],wc[1],wc[2]);
	transform->RotateWXYZ(90,v2[0],v2[1],v2[2]);
	transform->Translate(-wc[0],-wc[1],-wc[2]);
	// Modify and update planeWidget
	double newpt[3];
	transform->TransformPoint(this->planeWidget[whichPlane]->GetPoint1(),newpt);
	this->planeWidget[whichPlane]->SetPoint1(newpt);
	transform->TransformPoint(this->planeWidget[whichPlane]->GetPoint2(),newpt);
	this->planeWidget[whichPlane]->SetPoint2(newpt);
	transform->TransformPoint(this->planeWidget[whichPlane]->GetOrigin(),newpt);
	this->planeWidget[whichPlane]->SetOrigin(newpt);
	planeWidget[whichPlane]->UpdatePlacement();*/
}


//void Drawing::dummyPlaneInteraction(double i){
//	double* origin = plane->GetOrigin();
//	origin[2] -= i ;
//	plane->SetOrigin(origin);
//}

void Drawing::setCuttingPlane(){
	plane = vtkSmartPointer<vtkPlane>::New();
	plane->SetTransform(vtkTransform::New());
	plane->SetNormal(0,0,1);
	plane->SetOrigin(0,0,-60);

	double *bounds ;
	reader->GetOutput();
	bounds = reader->GetOutput()->GetBounds() ;
	dataBounds = bounds ;

	vtkSmartPointer<vtkImageData> box = vtkSmartPointer<vtkImageData>::New();
	box->SetDimensions(2,2,2);
	double origin[3];
	int boxSize = 100000 ;
	plane->GetOrigin(origin);
	origin[0] -= boxSize /2 ;
	origin[1] -= boxSize /2 ;
	origin[2] -= boxSize /2 ;
	box->SetOrigin(dataBounds);
	box->SetSpacing(dataBounds[1],dataBounds[1],dataBounds[1]);

	vtkSmartPointer<vtkOutlineFilter> outline = vtkSmartPointer<vtkOutlineFilter>::New();
	outline->SetInputData(box);
	vtkSmartPointer<vtkPolyDataMapper> outlineMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	outlineMapper->SetInputConnection(outline->GetOutputPort());
	outlineActor = vtkSmartPointer<vtkActor>::New();
	outlineActor->SetMapper(outlineMapper);
	outlineActor->SetUserMatrix(vtkMatrix4x4::New());
	outlineActor->GetProperty()->SetColor(1,1,0.5);
	outlineActor->GetProperty()->SetLineWidth(3);
	
	vtkSmartPointer<vtkCutter> cutter = vtkSmartPointer<vtkCutter>::New();
	//cutter->SetInputConnection(0,box);
	cutter->SetInputData(box);
	cutter->SetCutFunction(plane);
	//cutter->SetInputConnection(reader->GetOutputPort());
	//cutter->Update();

	mapperPlane = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapperPlane->SetInputConnection(cutter->GetOutputPort());

	actorCuttingPlane = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<vtkProperty> pro = vtkSmartPointer<vtkProperty>::New();


	pro->SetColor(1,1,1);
	pro->SetLineWidth(2);
	pro->SetOpacity(0.5);
	actorCuttingPlane->SetProperty(pro);
	actorCuttingPlane->SetMapper(mapperPlane) ;
	//Renderer->AddActor(actorCuttingPlane);
	planeRep = vtkSmartPointer<myImplicitPlaneRepresentation>::New(); 
	planeRep->SetPlaceFactor(1.0); // Use > 1.0 to make the BB larger than the actual data
	planeRep->OutsideBoundsOn();
	planeRep->DrawPlaneOn();
	planeRep->GetOutlineProperty()->SetColor(0.9,0.9,0.9); // Does not work because of bug: http://vtk.org/Bug/view.php?id=11867
	planeRep->HideOutline();
	planeRep->BuildRepresentation();
	//planeRep->PlaceWidget(reader->GetOutput()->GetBounds()); 
	planeRep->PlaceWidget(bounds); 
	planeRep->SetNormal(plane->GetNormal());
	planeRep->SetOrigin(plane->GetOrigin());
	Renderer->AddViewProp(planeRep);
	//Renderer->AddActor(outlineActor);
	planeRep->OutlineTranslationOn();

	clipper = vtkSmartPointer<vtkClipDataSet>::New();
	clipper->SetInputConnection(reader->GetOutputPort());
	clipper->SetClipFunction(plane);
	clipper->InsideOutOn();

	/*mapperSliceData = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapperSliceData->SetInputConnection(clipper->GetOutputPort());
  
	slicedActor = vtkSmartPointer<vtkActor>::New();
	slicedActor->SetMapper(mapperSliceData);


	Renderer->AddActor(slicedActor);
	ctxView->Render();*/


	//vtkSmartPointer<vtkHierarchicalPolyDataMapper> mapperPlane = vtkSmartPointer<vtkHierarchicalPolyDataMapper>::New();
	//mapperPlane->SetInputConnection(cutter->GetOutputPort());
	//mapperPlane->SetInputConnection(clipper->GetOutputPort());

	//cuttingActor = vtkSmartPointer<vtkActor>::New();
	//cuttingActor->SetMapper(mapperPlane);
	//Renderer->AddActor(cuttingActor);
	//mainActor->SetVisibility(0);
	//planeRep->Print(cout);
}




void Drawing::read(){
	Renderer = vtkRenderer::New();
	Renderer->SetBackground(.3, .6, .3);
	RenderWindow->AddRenderer(Renderer.Get());
	cout << "Reading the Dataset" << endl ;
	//Reading the dataset
	reader = vtkSmartPointer<vtkDataSetReader>::New();
	reader->SetFileName(filename.c_str());
	reader->Update();
	imageData = vtkSmartPointer<vtkImageData>::New();
	imageData->ShallowCopy(reader->GetOutput());
	dataDimensions = imageData->GetDimensions();


	//Visualization
	mainMapper = vtkOpenGLGPUVolumeRayCastMapper::New();
	mainMapper ->SetInputData(imageData);

	vtkSmartPointer<vtkVolumeProperty> volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
	volumeProperty->ShadeOff();
	volumeProperty->SetInterpolationType(VTK_LINEAR_INTERPOLATION);

	vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
	compositeOpacity->AddPoint(1.5,0.0);
	compositeOpacity->AddPoint(1.9,0.5);
	compositeOpacity->AddPoint(2.5,1.0);
	volumeProperty->SetScalarOpacity(compositeOpacity); // composite first.

	vtkSmartPointer<vtkColorTransferFunction> color = vtkSmartPointer<vtkColorTransferFunction>::New();
	color->AddRGBPoint(1.2  ,0.0,0.0,1.0);
	color->AddRGBPoint(2.0  ,0.0,1.,1.);
	color->AddRGBPoint(3.0, 1.0,0.0,0.0);
	color->SetColorSpaceToHSV();
	volumeProperty->SetColor(color);

	vtkSmartPointer<vtkVolume> volume = vtkSmartPointer<vtkVolume>::New();
	volume->SetMapper(mainMapper);
	volume->SetProperty(volumeProperty);
	mainActor = volume ;

	Renderer->AddViewProp(mainActor);
	Renderer->ResetCamera();

	Interactor = vtkRenderWindowInteractor::New();
	Interactor->SetRenderWindow(RenderWindow);
	vtkSmartPointer<myInteractorStyle> style = vtkSmartPointer<myInteractorStyle>::New(); 
	Interactor->SetInteractorStyle( style );

	readVelocities();
	RenderWindow->PolygonSmoothingOn();
	RenderWindow->SetSize(640,368);
	RenderWindow->Render();
	RenderWindow->Start();
	Renderer->SetBackground(0,0,0) ;
	setCuttingPlane();
	ctxView->Render();

	interactionMode = NORMALINTERACTIONMODE ;
}

void Drawing::readVelocities(){

	cout << "Reading Velocities" << endl ;
	vtkSmartPointer<vtkDataSetReader> reader = vtkSmartPointer<vtkDataSetReader>::New();
	reader->SetFileName(filename.c_str());
	reader->Update();
	velocityData = vtkSmartPointer<vtkImageData>::New();
	velocityData->ShallowCopy(reader->GetOutput());

}

void Drawing::setSeedPoints(double* seedPoint){
	
	if(seedPoint[0] == -1){
		return ;
	}

	releaseParticles(dataDimensions, particles, seedPoint);
	
	this->setVTKPoints();
	this->drawPoints();
}

void Drawing::setVTKPoints(){
	Particle p;
	for(int i = 0 ; i < 200 ; i++ ){
		p = particles[i];
		if(p.valid){
			integrateParticleMotion(p, velocityData, dataDimensions);
			points->InsertNextPoint(p.pos.x, p.pos.y, p.pos.z);
		}
	}
}

void Drawing::drawPoints(){

}




//void Drawing::setCuttingPlane(){
//	/// Setup a visualization pipeline
//	plane = vtkSmartPointer<vtkPlane>::New();
//	plane->SetTransform(vtkTransform::New()); 
//	clipper = vtkSmartPointer<vtkClipPolyData>::New();
//	clipper->SetClipFunction(plane);
//	clipper->InsideOutOn();
//
//	readerbunny = vtkSmartPointer<vtkXMLPolyDataReader>::New() ;
//	readerbunny->SetFileName(filename.c_str()) ;
//	clipper->SetInputConnection(readerbunny->GetOutputPort()) ;
//
//	// Create a mapper and actor
//	mapperPlane = vtkSmartPointer<vtkPolyDataMapper>::New();
//	mapperPlane->SetInputConnection(clipper->GetOutputPort());
//	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
//	//actor->SetMapper(mapper);
//	//mainActor->SetMapper(mapperPlane);
// 
//	vtkSmartPointer<vtkProperty> backFaces = vtkSmartPointer<vtkProperty>::New();
//	backFaces->SetDiffuseColor(.8, .1, .1);
//	//actor->SetBackfaceProperty(backFaces);
//	mainActor->SetBackfaceProperty(backFaces);
// 
///*	// A renderer and render window
//	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
//	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
//	renderWindow->AddRenderer(renderer);
//	renderer->AddActor(actor);
//			
//	// An interactor
//	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
//	vtkSmartPointer<vtkRenderWindowInteractor>::New();
//	renderWindowInteractor->SetRenderWindow(win);
// 
//	renderWindow->Render();*/
//
//
// 
//	// The callback will do the work of updating the view
//	vtkSmartPointer<vtkIPWCallback> myCallback = 
//	vtkSmartPointer<vtkIPWCallback>::New();
//	myCallback->Plane = plane;
//	//myCallback->Actor = actor;
//	myCallback->Actor = mainActor ;
// 
//	planerep = vtkSmartPointer<myImplicitPlaneRepresentation>::New();
//	planerep->SetPlaceFactor(1.5); // This must be set prior to placing the widget
//	//planerep->PlaceWidget(mainActor->GetBounds());
//	planerep->PlaceWidget(mainActor->GetBounds());
//	planerep->SetNormal(plane->GetNormal());
//	planerep->SetNormal(0,1,0);
//	planerep->SetOrigin(0,0,50); //this doesn't seem to work?
// 
//	//planeWidget = vtkSmartPointer<vtkImplicitPlaneWidget2>::New();
//	//planeWidget->SetInteractor(iren);
//	//planeWidget->SetRepresentation(planerep);
//	//planeWidget->AddObserver(vtkCommand::InteractionEvent,myCallback);
//
//	//ren->AddActor(actor);
//	//planeWidget->On();
// 
///*  // Render
// 
//  iren->Initialize();
//  win->Render();
//  planeWidget->On();
// 
//  // Begin mouse interaction
//  iren->Start();*/
//
//
//
//}



/*void Drawing::read(){

	// Read all the data from the file
	readerbunny =vtkSmartPointer<vtkXMLPolyDataReader>::New();
	readerbunny->SetFileName(filename.c_str());
	readerbunny->Update();
	inputPolyData = readerbunny->GetOutput();

	cout << "File Found and Loaded : " << filename << endl ;


	mapperMain = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapperMain->SetInputConnection(readerbunny->GetOutputPort());

	mainActor = vtkSmartPointer<vtkActor>::New();
	mainActor->SetMapper(mapperMain);
			//For the bunny
	//ren->AddActor(mainActor);

	iren->SetRenderWindow(win);
	//myInteractorStyle *style = myInteractorStyle::New();
	style = vtkSmartPointer<vtkInteractorStyleTrackballActor>::New();
	iren->SetInteractorStyle(style);
	
	
	setCuttingPlane();
	
	//Start the event loop
	iren->Initialize();
	//iren->Start();

	win->PolygonSmoothingOn();
	win->SetSize(640,368);
	win->Render();
	win->Start();

	ctxView->Render();
	
}*/




/*void Drawing::newRead(){

	// Create a render window
	//win = vtkRenderWindow::New();
	win->SetNumberOfLayers(2);

	// Create 3D renderers
	for(int i = 0; i < NUM_VIEWS; ++i)
	{
		double offset = (NUM_VIEWS - 1) / 20.0;
		//double colour = 0.5 + offset * (i - double(NUM_VIEWS)/2.0);
		double colour = 0.5;

		//cout << "Colour: " << colour << endl;

		ren[i] = vtkSmartPointer<vtkRenderer>::New();
		ren[i]->SetBackground(colour, colour, colour);
		ren[i]->SetViewport(double(i)/double(NUM_VIEWS), 0.0, double(i+1)/double(NUM_VIEWS), 1.0);
		ren[i]->SetLayer(0);
		win->AddRenderer(ren[i]);

		// Light
		ren[i]->LightFollowCameraOn();
		vtkSmartPointer<vtkLightKit> kit = vtkSmartPointer<vtkLightKit>::New();
		kit->AddLightsToRenderer(ren[i]);
	}

	win->SetAlphaBitPlanes(1);
	win->SetMultiSamples(0);

	time_t start, end;
	double loadTime;

	cout << "Loading scalar data -> " << baseNameScalar << "xxx.vtk(.bin)\n";
	start = time(0);
	for(int i = 0; i < this->number_of_timesteps; ++i)
	{
		cout << " - " << (i+1);
		vtkSmartPointer<vtkStructuredPointsReader> readerTmp = vtkSmartPointer<vtkStructuredPointsReader>::New();
		std::stringstream name;
		name << baseNameScalar << setfill('0') << setw(3) << (i+1) << ".vtk";
		readerTmp->SetFileName(name.str().c_str());
		name << ".bin";

		// Check for Binary file
		FILE* fileInfo = fopen(name.str().c_str(),"r");
		if(fileInfo!=NULL)
		{
			cout << " binary\n";
			readerTmp->SetFileName(name.str().c_str());
			readerTmp->Update();
		}
		else
		{
			cout << " ascii";
			readerTmp->Update();

			cout << " -> binary\n";
			vtkSmartPointer<vtkStructuredPointsWriter> binaryWriter = vtkSmartPointer<vtkStructuredPointsWriter>::New();
			binaryWriter->SetFileTypeToBinary();
			binaryWriter->SetInputConnection(readerTmp->GetOutputPort());
			binaryWriter->SetFileName(name.str().c_str());
			binaryWriter->Write();
		}
		reader.push_back(readerTmp);
	}

	end = time(0);
	loadTime = end - start;
	cout << "\nLoading took " << loadTime << " seconds ( " << loadTime / this->number_of_timesteps << " seconds per timestep)\n\n";

	// Assume min/max values are equal for all files, base it on the first file.
	double minMax[2];
	reader[0]->GetOutput()->GetScalarRange(minMax);
	if(!this->overWriteMinValue)
	{
		this->minValue = minMax[0];
	}
	if(!this->overWriteMaxValue)
	{
		this->maxValue = minMax[1];
	}
	double* bounds = reader[0]->GetOutput()->GetBounds();
	int delta[3];
	delta[0] = bounds[1] - bounds[0];
	delta[1] = bounds[3] - bounds[2];
	delta[2] = bounds[5] - bounds[4];
	cout << "Delta: " << delta[0] << ", "  << delta[1] << ", "  << delta[2] << endl;

	cout << "Loading scalar data -> " << baseNameVector << "xxx.vtk(.bin)\n";
	start = time(0);
	for(int i = 0; i < this->number_of_timesteps; ++i)
	{
		cout << " - " << (i+1);
		vtkSmartPointer<vtkStructuredPointsReader> readerVectorTmp = vtkSmartPointer<vtkStructuredPointsReader>::New();
		std::stringstream name;
		name << baseNameVector << setfill('0') << setw(3) << (i+1) << ".vtk";
		readerVectorTmp->SetFileName(name.str().c_str());
		name << ".bin";

		// Check for Binary file
		FILE* fileInfo = fopen(name.str().c_str(), "r");
		if(fileInfo!=NULL)
		{
			cout << " binary\n";
			readerVectorTmp->SetFileName(name.str().c_str());
			readerVectorTmp->Update();
		}
		else
		{
			cout << " ascii";
			readerVectorTmp->Update();

			cout << " -> binary\n";
			vtkSmartPointer<vtkStructuredPointsWriter> binaryWriter = vtkSmartPointer<vtkStructuredPointsWriter>::New();
			binaryWriter->SetFileTypeToBinary();
			binaryWriter->SetInputConnection(readerVectorTmp->GetOutputPort());
			binaryWriter->SetFileName(name.str().c_str());
			binaryWriter->Write();
		}
		readerVector.push_back(readerVectorTmp);

	}
	end = time(0);
	loadTime = end - start;
	cout << "\nLoading took " << loadTime << " seconds ( " << loadTime / this->number_of_timesteps << " seconds per timestep)\n\n";

	bounds = readerVector[0]->GetOutput()->GetBounds();
	for(int i = 0; i < 3; ++i)
	{
		delta[i] -= bounds[2*i+1] - bounds[2*i]; 
		delta[i] /= 2;
	}
	
	cout << "Delta: " << delta[0] << ", "  << delta[1] << ", "  << delta[2] << endl;

	for(int i = 0; i < this->number_of_timesteps; ++i)
	{
		vtkSmartPointer<vtkImageChangeInformation> transformVectorTmp = vtkSmartPointer<vtkImageChangeInformation>::New();
		transformVectorTmp->SetInputConnection(readerVector[i]->GetOutputPort());
		transformVectorTmp->SetInformationInputData(readerVector[i]->GetOutput());
		transformVectorTmp->SetExtentTranslation(delta);
		transformVector.push_back(transformVectorTmp);
	}
	win->PolygonSmoothingOn();
	win->SetSize(640,368);
	win->Render();
	win->Start();
	ctxView->Render();
}*/


//void Drawing::readOriginal(){
//
//	std::string filename = BUNNY;
//	// Read all the data from the file
//	vtkSmartPointer<vtkXMLPolyDataReader> reader =vtkSmartPointer<vtkXMLPolyDataReader>::New();
//	reader->SetFileName(filename.c_str());
//	reader->Update();
//
//	cout << "File Found and Loaded : " << filename << endl ;
//
//	vtkContourFilter* contour = vtkContourFilter::New();
//	contour->SetValue(0,0.26);
//	contour->SetInputConnection(reader->GetOutputPort());
// 
//	// Visualize
//	vtkLookupTable* lu = vtkLookupTable::New();
//	lu->SetNumberOfTableValues(256);
//	lu->SetHueRange(0.6667, 0);
//	lu->SetTableRange(0.981, 1.01);
//	lu->Build();
//	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
//	mapper->SetInputConnection(reader->GetOutputPort());
//	mapper->SetLookupTable(lu);
//	mapper->SetInterpolateScalarsBeforeMapping(1);
//	mapper->SetScalarMode(VTK_SCALAR_MODE_USE_POINT_FIELD_DATA);
//	mapper->SelectColorArray("RHO");
//	mapper->SetUseLookupTableScalarRange(1);
//
//	// Add ids to the points and cells of the sphere
//	/*vtkSmartPointer<vtkIdFilter> idFilter = 
//	vtkSmartPointer<vtkIdFilter>::New();
//	idFilter->SetInputConnection(sphereSource->GetOutputPort());
//	idFilter->Update();
//
//	vtkSmartPointer<vtkPlane> plane = vtkSmartPointer<vtkPlane>::New();
//	plane->SetOrigin(0, 0, 0);
//	plane->SetNormal(1, 1, 1); // Convert the DataSet to a GenericDataSet
//	vtkSmartPointer<vtkBridgeDataSet> bridgeDataSet = vtkSmartPointer<vtkBridgeDataSet>::New();
//	bridgeDataSet->SetDataSet(idFilter->GetOutput());*/
//
//	vtkSmartPointer<vtkGenericClip> clipper =
//	vtkSmartPointer<vtkGenericClip>::New();
//	//clipper->SetClipFunction(plane);
//	//clipper->SetInputData(bridgeDataSet);
//	//clipper->Update();
//
//
//	mainActor = vtkSmartPointer<vtkActor>::New();
//	mainActor->SetMapper(mapper);
// 
//	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
//	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
//	renderWindow->AddRenderer(renderer);
//	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
//	renderWindowInteractor->SetRenderWindow(renderWindow);
// 
//	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
//	iren->SetRenderWindow(renderWindow);
//
//	vtkInteractorStyleMultiTouchCamera *style =
//	vtkInteractorStyleMultiTouchCamera::New();
//	iren->SetInteractorStyle(style);
//
//	vtkBoxWidget *boxWidget = vtkBoxWidget::New();
//	boxWidget->SetInteractor(iren);
//	boxWidget->SetPlaceFactor(1.25);
//	vtkMyCallback *callback = vtkMyCallback::New();
//	boxWidget->AddObserver(vtkCommand::InteractionEvent, callback);
//	boxWidget->On();
//
//	renderer->AddActor(mainActor);
//	renderer->SetBackground(0,0,0); // Background color green
// 
//	renderWindow->PolygonSmoothingOn();
//	renderWindow->Render();
//	//renderWindowInteractor->Start();
//}


//void Drawing::defineClipping(){
//
//	 // Define a clipping plane
//	vtkSmartPointer<vtkPlane> clipPlane = vtkSmartPointer<vtkPlane>::New();
//	clipPlane->SetNormal(1.0, 1.0, 0.0);
//	clipPlane->SetOrigin(0.0, 0.0, 0.0);
//	// Clip the source with the plane
//	vtkSmartPointer<vtkClipPolyData> clipper = vtkSmartPointer<vtkClipPolyData>::New();
//	clipper->SetInputData(inputPolyData);
//	clipper->SetClipFunction(clipPlane);  
//	clipper->Update();
//	//Create a mapper and actor
//	vtkSmartPointer<vtkPolyDataMapper> superquadricMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
//	superquadricMapper->SetInputConnection(clipper->GetOutputPort());
//	vtkSmartPointer<vtkActor> superquadricActor = vtkSmartPointer<vtkActor>::New();
//	superquadricActor->SetMapper(superquadricMapper);
//	vtkSmartPointer<vtkProperty> backFaces = vtkSmartPointer<vtkProperty>::New();
//	backFaces->SetSpecular(0.0);
//	backFaces->SetDiffuse(0.0);
//	backFaces->SetAmbient(1.0);
//	backFaces->SetAmbientColor(1.0000, 0.3882, 0.2784);
//	superquadricActor->GetProperty()->SetColor(0,1,0);
//	superquadricActor->SetBackfaceProperty(backFaces);
//	// Create a renderer
//	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
//	//Add actors to the renderers
//	renderer->AddActor(superquadricActor);
//	win->AddRenderer(renderer);
//	//win->Render();
//}

/*int rotate_camera( vtkContextView* env){
	vtkCamera* cam = env->GetRenderer()->GetActiveCamera();

	float angle = 0;

	angle = msg.GetDataReal(8);
	cam->Yaw(angle);

	angle = msg.GetDataReal(4);
	cam->Pitch(angle);

	angle = msg.GetDataReal(0);
	cam->Roll(angle);


	//env.ren->ResetCameraClippingRange();
	//env.iren->GetRenderWindow()->StereoUpdate();
	//env.iren->Render();
	//env->Render();
	return 0;
}*/



void Drawing::scale(float k){
	//vtkSmartPointer<vtkTransform> transform =vtkSmartPointer<vtkTransform>::New();
	//transform->Scale(5,1,1);
	//vtkSmartPointer<vtkTransformFilter> transformFilter = vtkSmartPointer<vtkTransformFilter>::New();
	//transformFilter->SetInputConnection(cone->GetOutputPort());
	//transformFilter->SetTransform(transform);
	//mapper->SetInputConnection(transformFilter->GetOutputPort());
	
	mainActor->SetScale(k);
	//ren->Render();
}

void Drawing::translate(float x, float y, float z){
	cout << "Translate: " << x << " - " << " - " << y << " - " << z << endl ;
	vtkSmartPointer<vtkTransform> transform1a = vtkSmartPointer<vtkTransform>::New();
	//transform1a->Translate(x,y,z);
	//transformFilter->SetTransform(transform1a);
	//transformFilter->Update();
	double* position = mainActor->GetPosition();
	mainActor->SetPosition(position[0]+x,position[1]+y,position[2]+z);
	//ren->Render();
}

void Drawing::rotate(float x, float y, float z){
	cout << "Rotate: " << x << " - " << " - " << y << " - " << z << endl ;
	vtkSmartPointer<vtkTransform> transform1a = vtkSmartPointer<vtkTransform>::New();
	//transform1a->PostMultiply();
	//transform1a->RotateX(x);
	//transform1a->RotateY(y);
	//transform1a->RotateZ(z);
	//mainActor->SetUserTransform(transform1a);
	mainActor->RotateWXYZ(20,1,0,0);
	//ren->Render();
}

void Drawing::rotateCamera(float x, float y, float z){
	vtkCamera* camera = ctxView->GetRenderer()->GetActiveCamera();

	camera->Yaw(x);
	camera->Pitch(y);
	camera->Roll(z);

	//camera->UpdateViewport();
}


