#include "toolbox.h"


#include "toolbox.h"
#define DOUBLE_PRECISION
#include "linear_math.h"
#undef DOUBLE_PRECISION

void printMatrix(const Matrix4& mat){
  std::ostringstream strs ;
  strs << mat << endl ;
  cout << "Matrix4 values = %s", strs.str().c_str();
}

int getIndexofLargerDataDimension(int * dimension){
  if(dimension[0]>=dimension[1] && dimension[0]>=dimension[2]) return 0 ;
  if(dimension[1]>=dimension[0] && dimension[1]>=dimension[2]) return 1 ;
  if(dimension[2]>=dimension[1] && dimension[2]>=dimension[0]) return 2 ;
}

void getRotationMatrixFromVector(double* R, double* rotationVector, int sizeR, int sizeVec) {

    double q0;
    double q1 = rotationVector[0];
    double q2 = rotationVector[1];
    double q3 = rotationVector[2];

    if (sizeVec == 4) {
        q0 = rotationVector[3];
    } else {
        q0 = 1 - q1*q1 - q2*q2 - q3*q3;
        q0 = (q0 > 0) ? (float)(sqrt(q0)): 0;
    }

    double sq_q1 = 2 * q1 * q1;
    double sq_q2 = 2 * q2 * q2;
    double sq_q3 = 2 * q3 * q3;
    double q1_q2 = 2 * q1 * q2;
    double q3_q0 = 2 * q3 * q0;
    double q1_q3 = 2 * q1 * q3;
    double q2_q0 = 2 * q2 * q0;
    double q2_q3 = 2 * q2 * q3;
    double q1_q0 = 2 * q1 * q0;

    if(sizeR == 9) {
        R[0] = 1 - sq_q2 - sq_q3;
        R[1] = q1_q2 - q3_q0;
        R[2] = q1_q3 + q2_q0;

        R[3] = q1_q2 + q3_q0;
        R[4] = 1 - sq_q1 - sq_q3;
        R[5] = q2_q3 - q1_q0;

        R[6] = q1_q3 - q2_q0;
        R[7] = q2_q3 + q1_q0;
        R[8] = 1 - sq_q1 - sq_q2;
    } else if (sizeR == 16) {
        R[0] = 1 - sq_q2 - sq_q3;
        R[1] = q1_q2 - q3_q0;
        R[2] = q1_q3 + q2_q0;
        R[3] = 0.0f;

        R[4] = q1_q2 + q3_q0;
        R[5] = 1 - sq_q1 - sq_q3;
        R[6] = q2_q3 - q1_q0;
        R[7] = 0.0f;

        R[8] = q1_q3 - q2_q0;
        R[9] = q2_q3 + q1_q0;
        R[10] = 1 - sq_q1 - sq_q2;
        R[11] = 0.0f;

        R[12] = R[13] = R[14] = 0.0f;
        R[15] = 1.0f;
    }
}

Matrix4 getMatrixFromVTK(vtkSmartPointer<vtkMatrix4x4> mat){
    Matrix4 matrix((double*)mat->Element);
    matrix = matrix.transpose();
    return matrix;
}

vtkSmartPointer<vtkMatrix4x4> getVTKFromMatrix(const Matrix4& mat){
    //mat = mat.transpose();
    vtkSmartPointer<vtkMatrix4x4> matrix = vtkSmartPointer<vtkMatrix4x4>::New();
    matrix->DeepCopy(mat.transpose().data_);
    return matrix ;
}

Vector3 posToDataCoords(const Vector3& pos, 
                        vtkSmartPointer<vtkMatrix4x4> matrix, 
                        double zoomFactor, const Vector3& dataDim, 
                        double dataSpacing) // pos is in eye coordinates
{
  Vector3 result;
  Matrix4 modelMatrix = getMatrixFromVTK(matrix);

  //synchronized (state->modelMatrix) {
    // Transform "pos" into object space
    result = modelMatrix.inverse() * pos;
  //}

  // Compensate for the scale factor
  result *= 1/zoomFactor;

  // The data origin is on the corner, not the center
  result += Vector3(dataDim.x/2, dataDim.y/2, dataDim.z/2) * dataSpacing;

  return result;

}

Vector3 dataCoordsToPos(const Vector3& dataCoords,
                        vtkSmartPointer<vtkMatrix4x4> matrix, 
                        double zoomFactor, const Vector3& dataDim, 
                        double dataSpacing)
{
  Vector3 result = dataCoords;
  Matrix4 modelMatrix = getMatrixFromVTK(matrix);

  // The data origin is on the corner, not the center
  result -= Vector3(dataDim.x/2, dataDim.y/2, dataDim.z/2) * dataSpacing;

  // Compensate for the scale factor
  result *= zoomFactor;

  //synchronized(state->modelMatrix) {
    // Transform "result" into eye space
    result = modelMatrix * result;
  //}

  return result;

}