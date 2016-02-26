
#ifndef TOOLBOX
#define TOOLBOX


#define DOUBLE_PRECISION
#include "linear_math.h"
#undef DOUBLE_PRECISION

#include "vtkQuaternion.h"
#include "vtkTransform.h"
#include "vtkMatrix4x4.h"
#include "vtkMath.h"
#include "vtkSmartPointer.h"
#include <errno.h>
#include <sstream>
#include <string>
#include <string>
#include <vtkImageData.h>
#include <vtkPointData.h>
#include <vtkDataArray.h>  

typedef LinearMath::Vector3<int> DataCoords;


void printMatrix(const Matrix4& mat);
void getRotationMatrixFromVector(double* R, double* rotationVector, int sizeR, int sizeVec);
Matrix4 getMatrixFromVTK(vtkSmartPointer<vtkMatrix4x4> mat);
int getIndexofLargerDataDimension(int * dimension);
vtkSmartPointer<vtkMatrix4x4> getVTKFromMatrix(const Matrix4& mat);

Vector3 posToDataCoords(const Vector3& pos, 
                        vtkSmartPointer<vtkMatrix4x4> matrix, 
                        double zoomFactor, const Vector3& dataDim, 
                        double dataSpacing);

Vector3 dataCoordsToPos(const Vector3& dataCoords,
                        vtkSmartPointer<vtkMatrix4x4> matrix, 
                        double zoomFactor, const Vector3& dataDim, 
                        double dataSpacing);

#endif