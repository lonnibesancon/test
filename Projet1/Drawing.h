#ifndef DRAWING
#define DRAWING
#define vtkRenderingCore_AUTOINIT 4(vtkInteractionStyle,vtkRenderingFreeType,vtkRenderingFreeTypeOpenGL,vtkRenderingOpenGL)
#define vtkRenderingVolume_AUTOINIT 1(vtkRenderingVolumeOpenGL)
#define vtkRenderingContext2D_AUTOINIT 1(vtkRenderingContextOpenGL)


#define JETLINES "jetLinesSmall2.vtp"
#define PARTICLES "particles.vtp"
#define BUNNY "bunny.vtp"
#define FTLE "../Data/ftlelog.vtk"

#include "SCProtocol.h"
#include <chrono>
#include "vtkNetCDFCFReader.h"


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
#include <vtkGenericRenderWindowInteractor.h>

#include <vtkLightKit.h>

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
#include <vtkIdFilter.h>
#include <vtkBridgeDataSet.h>
#include <vtkGenericClip.h>
//Configuration of the multiple passes
#include <vtkCameraPass.h>
#include <vtkLightsPass.h>
#include <vtkOpaquePass.h>
#include <vtkRenderPassCollection.h>
#include <vtkSequencePass.h>
#include <vtkOpenGLRenderer.h>
#include "myImplicitPlaneRepresentation.h"
#include <vtkTransformFilter.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkMath.h>
#include <vtkVector.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkClipPolyData.h>
#include <vtkSampleFunction.h>
#include <vtkIdTypeArray.h>
#include <vtkIdFilter.h>
#include <vtkGPUVolumeRayCastMapper.h>
#include <vtkImplicitPlaneWidget2.h>
#include <vtkImplicitPlaneWidget.h>

//Doublons possibles
#include <vtkActor.h>
#include <vtkBridgeDataSet.h> // Must build VTK with BUILD_TESTING=ON
#include <vtkCellData.h>
#include <vtkContourFilter.h>
#include <vtkDataSetMapper.h>
#include <vtkDelaunay3D.h>
#include <vtkElevationFilter.h>
#include <vtkGenericClip.h>
#include <vtkImageData.h>
#include <vtkImplicitDataSet.h>
#include <vtkIdFilter.h>
#include <vtkIdTypeArray.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSampleFunction.h>
#include <vtkSmartPointer.h>
#include <vtkPlane.h>
#include <vtkPlanes.h>
#include <vtkUnstructuredGrid.h>
#include <vtkDoubleArray.h>
#include <vtkVertexGlyphFilter.h>

#include <vtkImageChangeInformation.h>
#include <vtkStructuredPointsWriter.h>
#include <vtkOpenGLGPUVolumeRayCastMapper.h>
#include <vtkDataSetReader.h>
#include <vtkClipDataSet.h>
#include <vtkPoints.h>
#include <vtkPlaneCollection.h>

#include "myInteractorStyle.h"
#include "Particles.h"
#include <sstream>


class Drawing
{
public:
	Drawing(void);
	~Drawing(void);

	/*Drawing related functions*/
	void rotate(float x, float y, float z);
	void translate(float x, float y, float z);
	void scale(float k);
	void rotateCamera(float x, float y, float z);
	void read();
	//void readOriginal();
	//void newRead();
	void dummy();
	//void defineClipping();
	void updateView();
	void setTransformationMatrix(const int interactionMode, const double* mat);
	void setCuttingPlane();
	void setPositionAndOrientation(double* position, double* orientation, const int interactionMode);
	void dummyPlaneInteraction(double i);
	void setMapper(int mapper);
	void readVelocities();
	void changeInteractionMode(int mode);
	void setNormalInteractionMode();
	void setSeedInteractionMode();
	void updateClipping();
	void setSeedPoints(double* seedPoint);
	

	/*Others*/
	void setFileName(std::string);

	vtkSmartPointer<vtkContextView> ctxView ;

	//New dataset
	vtkSmartPointer<vtkRenderWindow> RenderWindow;
	vtkSmartPointer<vtkRenderer> Renderer;
	vtkSmartPointer<vtkRenderWindowInteractor> Interactor;
	vtkSmartPointer<myInteractorStyle> style ;
	vtkSmartPointer<vtkProp3D> mainActor;
	vtkSmartPointer<vtkPolyDataMapper> mapperPlane ;
	vtkSmartPointer<vtkOpenGLGPUVolumeRayCastMapper> mainMapper ;
	vtkSmartPointer<vtkActor> actorCuttingPlane;
	vtkSmartPointer<vtkPlane> plane ;
	vtkSmartPointer<vtkClipDataSet> clipper ;
	vtkSmartPointer<vtkImplicitPlaneWidget2> planeWidget ;
	vtkSmartPointer<myImplicitPlaneRepresentation> planeRep ;
	//vtkSmartPointer<vtkXMLPolyDataReader> reader ;
	vtkSmartPointer<vtkDataSetReader> reader ;
	vtkSmartPointer<vtkActor> outlineActor ;
	vtkSmartPointer<vtkActor> cuttingActor ;
	vtkSmartPointer<vtkImageData> velocityData ;
	vtkSmartPointer<vtkImageData> imageData ; // The volume Data
	vtkSmartPointer<vtkPolyDataMapper> mapperSliceData ;
	vtkSmartPointer<vtkActor> slicedActor ;
	vtkSmartPointer<vtkPoints> points ;
	vtkSmartPointer<vtkPolyData> pointData ;
	vtkSmartPointer<vtkPolyDataMapper> mapperPoint ;
	vtkSmartPointer<vtkActor> pointsActor ;
	double* dataBounds ;
	int interactionMode ;
	std::vector<Particle> particles;
	int* dataDimensions ;


	//*vtkSmartPointer<vtkPlane> clipPlane;
	//vtkSmartPointer<myImplicitPlaneRepresentation> planeRep;
	//vtkSmartPointer<vtkActor> actorPlaneSource;*/

	//vtkSmartPointer<vtkActor> mainActor;
	//vtkSmartPointer<vtkXMLPolyDataReader> readerbunny ;
	//vtkSmartPointer<vtkPolyData> inputPolyData;

	//vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter;
	//vtkSmartPointer<vtkTransform> translation ;
	//vtkContextView* ctxView ;
	//vtkRenderWindow* win ;
	////vtkRenderer* ren ;
	//vtkCamera* cam ;
	//vtkRenderWindowInteractor *iren;
	//
	//vtkSmartPointer<vtkInteractorStyleTrackballActor> style ;

	////For the clipping plane
	//vtkSmartPointer<vtkPlane> plane ;
	//vtkSmartPointer<vtkPolyDataMapper> planemapper ;
	//vtkSmartPointer<vtkActor> planeActor ;
	//vtkSmartPointer<vtkMapper> mapperMain ;
	//vtkSmartPointer<vtkMapper> mapperPlane ;
	//vtkSmartPointer<vtkClipPolyData> clipper ;
	//vtkSmartPointer<vtkImplicitPlaneWidget2> planeWidget ;
	////vtkSmartPointer<vtkImplicitPlaneWidget> planeWidget ;
	//vtkSmartPointer<myImplicitPlaneRepresentation> planerep ;
	//bool clippingPlaneSet ;
	//
	//
	//
	////For the new dataset
	//int number_of_timesteps;
	//std::string baseNameVector;
	//std::string baseNameScalar;
	//bool overWriteMaxValue;
	//bool overWriteMinValue;
	//double minValue;
	//double maxValue;
	//std::vector<vtkSmartPointer<vtkStructuredPointsReader>> reader;

	//std::vector<vtkSmartPointer<vtkStructuredPointsReader>> readerVector;
	//std::vector<vtkSmartPointer<vtkImageChangeInformation>> transformVector;
	//static const int NUM_VIEWS = 2;
	//vtkSmartPointer<vtkRenderer> ren[NUM_VIEWS];

protected:

	std::string filename ;
	std::string velocitiesName ;
	int currentMapper ;
	void setVTKPoints();
	void drawPoints();

};

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
#endif

