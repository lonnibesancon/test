/*=========================================================================
  Program:   Visualization Toolkit
  Module:    myInteractorStyle.cxx
  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.
     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.
=========================================================================*/
#include "myInteractorStyle.h"

#include <string>
#include <iostream>
#include <sstream>

#include "vtkCamera.h"
#include "vtkCellPicker.h"
#include "vtkCallbackCommand.h"
#include "vtkMath.h"
#include "vtkMatrix4x4.h"
#include "vtkObjectFactory.h"
#include "vtkProp3D.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkTransform.h"
#include "vtkActorCollection.h"

vtkStandardNewMacro(myInteractorStyle);

//----------------------------------------------------------------------------
myInteractorStyle::myInteractorStyle()
{
  this->MotionFactor    = 10.0;
  this->InteractionProp = NULL;
  this->InteractionPicker = vtkCellPicker::New();
  this->InteractionPicker->SetTolerance(0.001);
  this->userMatrix = vtkMatrix4x4::New() ;
  this->matrixTab = (double *) malloc(16*sizeof(double));
  this->MotionFactor   = 10.0;
  this->PointersDownCount = 0;
  for (int i = 0; i < VTKI_MAX_POINTERS; ++i)
  {
    this->PointersDown[i] = 0;
  }
}

//----------------------------------------------------------------------------
myInteractorStyle::~myInteractorStyle()
{
  this->InteractionPicker->Delete();
}

void myInteractorStyle::setRendererAndActorFromMain(vtkSmartPointer<vtkRenderer> rend, vtkSmartPointer<vtkActor> act){
  renderer = rend ;
  actor = act ;
}

//----------------------------------------------------------------------------
void myInteractorStyle::OnMouseMove()
{
  int x = this->Interactor->GetEventPosition()[0];
  int y = this->Interactor->GetEventPosition()[1];

  //LOGI("State = %d", this->State);
  switch (this->State)
    {

    case VTKIS_TWO_POINTER:
      //LOGI("TWO FINGERS = %d", this->State);
      this->TwoFingerInteraction();
      this->InvokeEvent(vtkCommand::InteractionEvent, NULL);
      break;
    case VTKIS_ROTATE:
      //this->FindPokedRenderer(x, y);
      //LOGI("==================== Rotate ====================");
      this->Rotate();
      this->InvokeEvent(vtkCommand::InteractionEvent, NULL);
      break;

    default:
      //LOGI("==================== Default ====================");
      this->UniformScale();
      this->Spin();
      this->Pan();
      break ;
    }

    //Update the renderer
    vtkRenderWindowInteractor *rwi = this->Interactor;
    //this->FindPokedRenderer(this->Interactor->GetEventPosition()[0],this->Interactor->GetEventPosition()[1]);
    //vtkActorCollection* actors = this->CurrentRenderer->GetActors();
    //vtkActor* act = actors->GetNextActor();
    std::string test = getStringFromVtkMatrix(actor->GetMatrix());
    //vtkMatrix4x4::DeepCopy(matrixTab,actor->GetUserMatrix());
    //LOGW("AZERTYUIOPPOIUYTREZAAZERTYUIOP ::::: %s", test.c_str());
    rwi->Render();
    //actor->Render();
    //renderer->Render();
    //setUserMatrix();

}

//----------------------------------------------------------------------------
void myInteractorStyle::OnLeftButtonDown()
{
  int x = this->Interactor->GetEventPosition()[0];
  int y = this->Interactor->GetEventPosition()[1];

  this->FindPokedRenderer(x, y);
  this->FindPickedActor(x, y);
  int pointer = this->Interactor->GetPointerIndex();
  vtkDebugMacro("pointer index down for " << pointer);

  if (this->CurrentRenderer == NULL || this->InteractionProp == NULL)
  {
    return;
  }
  if (this->PointersDown[pointer])
  {
    return;
  }

  this->PointersDown[pointer] = 1;
  this->PointersDownCount++;

  if(this->PointersDownCount == 1){
    this->GrabFocus(this->EventCallbackCommand);
    /*if (this->Interactor->GetShiftKey())
    {
      this->StartPan();
    }
    else if (this->Interactor->GetControlKey())
    {
      this->StartSpin();
    }
    else
    {*/
    this->StartRotate();
    //}
  }
    

  // if going from 1 to 2 pointers stop the one pointer action
  else if (this->PointersDownCount == 2)
  {
    this->LastState = this->State;
    switch (this->State)
      {
      /*case VTKIS_DOLLY:
        this->EndDolly();
        break;

      case VTKIS_USCALE:
        this->StartUniformScale();
        break;

      case VTKIS_PAN:
        this->StartPan();
        break;

      case VTKIS_SPIN:
        this->StartSpin();
        break;*/

      case VTKIS_ROTATE:
        this->EndRotate();
        break;
      }
    // start the multipointer action
    this->StartTwoPointer();
    this->GrabFocus(this->EventCallbackCommand);
    /*this->StartPan();
    this->StartSpin();
    this->StartUniformScale();*/
    
    
    return;
  }

  // if going from 2 to 3 pointers stop the two pointer action
  else if (this->PointersDownCount == 3 && this->State == VTKIS_TWO_POINTER)
    {
    this->EndTwoPointer();
    }
}

//----------------------------------------------------------------------------
void myInteractorStyle::OnLeftButtonUp()
{

  int pointer = this->Interactor->GetPointerIndex();

  vtkDebugMacro("pointer index up for " << pointer);
  //LOGW("Pointer UP");

  // if it is already up, ignore this event
  if (!this->PointersDown[pointer])
  {
    return;
  }

  this->PointersDownCount--;
  this->PointersDown[pointer] = 0;
  if(this->PointersDownCount == 0){
    /*switch (this->State)
    {
    case VTKIS_PAN:
      this->EndPan();
      break;

    case VTKIS_SPIN:
      this->EndSpin();
      break;

    case VTKIS_ROTATE:*/
      this->EndRotate();
      //break;
    //}
  }
  else{
    switch (this->State)
    {
    case VTKIS_TWO_POINTER:
      this->EndTwoPointer();
      for (int i = 0; i < VTKI_MAX_POINTERS; ++i)
        {
        if (this->PointersDown[i] != 0)
        this->Interactor->SetPointerIndex(i);
        switch (this->LastState)
          {
          /*case VTKIS_DOLLY:
            this->StartDolly();
            break;

          case VTKIS_USCALE:
            this->StartUniformScale();
            break;

          case VTKIS_PAN:
            this->StartPan();
            break;

          case VTKIS_SPIN:
            this->StartSpin();
            break;

          /*case VTKIS_PAN:
            this->StartPan();
            break;

          case VTKIS_SPIN:
            this->StartSpin();
            break;*/

          case VTKIS_ROTATE:
            this->StartRotate();
            break;

          /*default:
            this->EndPan();
            this->EndSpin();
            this->EndUniformScale();*/
          }
        }
      break;
    }
  }
  

  if ( this->Interactor )
    {
    this->ReleaseFocus();
    }
}

double myInteractorStyle::distance2D(int *a, int *b)
{
  return sqrt((double)(a[0] - b[0])*(a[0] - b[0]) + (double)(a[1]-b[1])*(a[1]-b[1]));
}


void myInteractorStyle::TwoFingerInteraction(){
  if (this->CurrentRenderer == NULL || this->InteractionProp == NULL)
  {
   // LOGW("coucoucoucoucoucoucocuocuocuocucoucoucoluc");
    return;
  }

  vtkRenderWindowInteractor *rwi = this->Interactor;
  vtkCamera *cam = this->CurrentRenderer->GetActiveCamera();

    // find the moving and non moving points
  int eventPI = rwi->GetPointerIndex();
  int otherPI = 0;

  for (int i = 0; i < VTKI_MAX_POINTERS; ++i)
    {
    if (this->PointersDown[i] > 0 && i != eventPI)
      {
      otherPI = i;
      break;
      }
    }

  

  /**
    * Uniform Scale
    **/

  double finger1[2];
  double finger2[2];
  finger1[0] = (double)rwi->GetLastEventPositions(eventPI)[0];
  finger1[1] = (double)rwi->GetLastEventPositions(eventPI)[1];
  finger2[0] = (double)rwi->GetLastEventPositions(otherPI)[0];
  finger2[1] = (double)rwi->GetLastEventPositions(otherPI)[1];
  double oldDist = distance2D(rwi->GetLastEventPositions(otherPI), rwi->GetLastEventPositions(eventPI));
  double newDist = distance2D(rwi->GetEventPositions(otherPI), rwi->GetEventPositions(eventPI));
  //LOGW("oldDist = %f", oldDist);
  //LOGW("newDist = %f", newDist);
  

  int dy = rwi->GetEventPosition()[1] - rwi->GetLastEventPosition()[1];

  double *obj_center = this->InteractionProp->GetCenter();
  double *center = this->CurrentRenderer->GetCenter();

  double dyf = newDist/oldDist;
  double yf = dy / center[1] * this->MotionFactor;
  double scaleFactor = 1 * dyf;
  //LOGW("Scale factor = %f",scaleFactor);
  double **rotate = NULL;

  double scale[3];
  scale[0] = scale[1] = scale[2] = scaleFactor;

  /*this->Prop3DTransform(this->InteractionProp,
                        obj_center,
                        0,
                        rotate,
                        scale);*/

  this->Prop3DTransform(actor,
                        obj_center,
                        0,
                        rotate,
                        scale);                    

  if (this->AutoAdjustCameraClippingRange)
    {
    this->CurrentRenderer->ResetCameraClippingRange();
    }



  /**
    * Spin
    **/

  double motion_vector[3];
  double view_point[3];

  if (cam->GetParallelProjection())
    {
    // If parallel projection, want to get the view plane normal...
    cam->ComputeViewPlaneNormal();
    cam->GetViewPlaneNormal( motion_vector );
    }
  else
    {
    // Perspective projection, get vector from eye to center of actor
    cam->GetPosition( view_point );
    motion_vector[0] = view_point[0] - obj_center[0];
    motion_vector[1] = view_point[1] - obj_center[1];
    motion_vector[2] = view_point[2] - obj_center[2];
    vtkMath::Normalize(motion_vector);
    }

  double disp_obj_center[3];

  this->ComputeWorldToDisplay(obj_center[0], obj_center[1], obj_center[2],
                              disp_obj_center);
  // compute roll - 1 DOF
  double oldAngle =
    vtkMath::DegreesFromRadians( atan2( (double)rwi->GetLastEventPositions(eventPI)[1] - rwi->GetLastEventPositions(otherPI)[1],
                                        (double)rwi->GetLastEventPositions(eventPI)[0] - rwi->GetLastEventPositions(otherPI)[0] ) );

  double newAngle =
    vtkMath::DegreesFromRadians( atan2( (double)rwi->GetEventPositions(eventPI)[1] - rwi->GetEventPositions(otherPI)[1],
                                        (double)rwi->GetEventPositions(eventPI)[0] - rwi->GetEventPositions(otherPI)[0] ) );
  scale[0] = scale[1] = scale[2] = 1.0;

  rotate = new double*[1];
  rotate[0] = new double[4];

  rotate[0][0] = newAngle - oldAngle;
  rotate[0][1] = motion_vector[0];
  rotate[0][2] = motion_vector[1];
  rotate[0][3] = motion_vector[2];

  /*this->Prop3DTransform( this->InteractionProp,
                         obj_center,
                         1,
                         rotate,
                         scale );*/
  this->Prop3DTransform(actor,
                        obj_center,
                        1,
                        rotate,
                        scale);                         


  /**
    * Pan
    **/

  double new_pick_point[4];
  double old_pick_point[4];

  this->ComputeWorldToDisplay(obj_center[0], obj_center[1], obj_center[2],
                              disp_obj_center);

  /*this->ComputeDisplayToWorld(rwi->GetEventPosition()[0],
                              rwi->GetEventPosition()[1],
                              disp_obj_center[2],
                              new_pick_point);

  this->ComputeDisplayToWorld(rwi->GetLastEventPosition()[0],
                              rwi->GetLastEventPosition()[1],
                              disp_obj_center[2],
                              old_pick_point);
  */
  this->ComputeDisplayToWorld((rwi->GetEventPositions(eventPI)[0] + rwi->GetEventPositions(otherPI)[0])/2.0,
                              (rwi->GetEventPositions(eventPI)[1] + rwi->GetEventPositions(otherPI)[1])/2.0,
                              disp_obj_center[2],
                              new_pick_point);

  this->ComputeDisplayToWorld((rwi->GetLastEventPositions(eventPI)[0] + rwi->GetLastEventPositions(otherPI)[0])/2.0,
                              (rwi->GetLastEventPositions(eventPI)[1] + rwi->GetLastEventPositions(otherPI)[1])/2.0,
                              disp_obj_center[2],
                              old_pick_point);

  motion_vector[0] = new_pick_point[0] - old_pick_point[0];
  motion_vector[1] = new_pick_point[1] - old_pick_point[1];
  motion_vector[2] = new_pick_point[2] - old_pick_point[2];

  if (this->InteractionProp->GetUserMatrix() != NULL)
    {
    /*vtkTransform *t = vtkTransform::New();
    t->PostMultiply();
    t->SetMatrix(this->InteractionProp->GetUserMatrix());
    t->Translate(motion_vector[0], motion_vector[1], motion_vector[2]);
    this->InteractionProp->GetUserMatrix()->DeepCopy(t->GetMatrix());
    t->Delete();*/
    vtkTransform *t = vtkTransform::New();
    t->PostMultiply();
    t->SetMatrix(actor->GetUserMatrix());
    t->Translate(motion_vector[0], motion_vector[1], motion_vector[2]);
    actor->GetUserMatrix()->DeepCopy(t->GetMatrix());
    t->Delete();
    }
  else
    {
    /*this->InteractionProp->AddPosition(motion_vector[0],
                                       motion_vector[1],
                                       motion_vector[2]);*/
    actor->AddPosition(motion_vector[0],motion_vector[1],motion_vector[2]);
                                       
    }

  if (this->AutoAdjustCameraClippingRange)
    {
    this->CurrentRenderer->ResetCameraClippingRange();
    }
  
  //this->Pan();
//  this->Spin();
//  this->UniformScale();
//  vtkRenderWindowInteractor *rwi = this->Interactor;
  //rwi->Render();

    //std::string test = getStringFromVtkMatrix(this->InteractionProp->GetUserMatrix());
    //LOGW("MAtrix ======= %s",test.c_str());
  
}


//----------------------------------------------------------------------------
void myInteractorStyle::Rotate()
{
  /*if (this->CurrentRenderer == NULL || this->InteractionProp == NULL)
    {
    return;
    }*/

  vtkRenderWindowInteractor *rwi = this->Interactor;
  vtkCamera *cam = this->CurrentRenderer->GetActiveCamera();

  // First get the origin of the assembly
  double *obj_center = this->InteractionProp->GetCenter();

  // GetLength gets the length of the diagonal of the bounding box
  double boundRadius = this->InteractionProp->GetLength() * 0.5;

  // Get the view up and view right vectors
  double view_up[3], view_look[3], view_right[3];

  cam->OrthogonalizeViewUp();
  cam->ComputeViewPlaneNormal();
  cam->GetViewUp(view_up);
  vtkMath::Normalize(view_up);
  cam->GetViewPlaneNormal(view_look);
  vtkMath::Cross(view_up, view_look, view_right);
  vtkMath::Normalize(view_right);

  // Get the furtherest point from object position+origin
  double outsidept[3];

  outsidept[0] = obj_center[0] + view_right[0] * boundRadius;
  outsidept[1] = obj_center[1] + view_right[1] * boundRadius;
  outsidept[2] = obj_center[2] + view_right[2] * boundRadius;

  // Convert them to display coord
  double disp_obj_center[3];

  this->ComputeWorldToDisplay(obj_center[0], obj_center[1], obj_center[2],
                              disp_obj_center);

  this->ComputeWorldToDisplay(outsidept[0], outsidept[1], outsidept[2],
                              outsidept);

  double radius = sqrt(vtkMath::Distance2BetweenPoints(disp_obj_center,
                                                       outsidept));
  double nxf = (rwi->GetEventPosition()[0] - disp_obj_center[0]) / radius;

  double nyf = (rwi->GetEventPosition()[1] - disp_obj_center[1]) / radius;

  double oxf = (rwi->GetLastEventPosition()[0] - disp_obj_center[0]) / radius;

  double oyf = (rwi->GetLastEventPosition()[1] - disp_obj_center[1]) / radius;

  if (((nxf * nxf + nyf * nyf) <= 1.0) &&
      ((oxf * oxf + oyf * oyf) <= 1.0))
    {
    double newXAngle = vtkMath::DegreesFromRadians( asin( nxf ) );
    double newYAngle = vtkMath::DegreesFromRadians( asin( nyf ) );
    double oldXAngle = vtkMath::DegreesFromRadians( asin( oxf ) );
    double oldYAngle = vtkMath::DegreesFromRadians( asin( oyf ) );

    double scale[3];
    scale[0] = scale[1] = scale[2] = 1.0;

    double **rotate = new double*[2];

    rotate[0] = new double[4];
    rotate[1] = new double[4];

    rotate[0][0] = newXAngle - oldXAngle;
    rotate[0][1] = view_up[0];
    rotate[0][2] = view_up[1];
    rotate[0][3] = view_up[2];

    rotate[1][0] = oldYAngle - newYAngle;
    rotate[1][1] = view_right[0];
    rotate[1][2] = view_right[1];
    rotate[1][3] = view_right[2];


    /*this->Prop3DTransform(this->InteractionProp,
                          obj_center,
                          2,
                          rotate,
                          scale);
    */
    this->Prop3DTransform(actor,obj_center,2,rotate,scale);

    delete [] rotate[0];
    delete [] rotate[1];
    delete [] rotate;

    if (this->AutoAdjustCameraClippingRange)
      {
      this->CurrentRenderer->ResetCameraClippingRange();
      }

    //rwi->Render();
    }

    //std::string test = getStringFromVtkMatrix(this->InteractionProp->GetUserMatrix());
    //LOGW("MAtrix ======= %s",test.c_str());
}

void myInteractorStyle::Spin()
{
  if ( this->CurrentRenderer == NULL || this->InteractionProp == NULL )
    {
    return;
    }

  vtkRenderWindowInteractor *rwi = this->Interactor;
  vtkCamera *cam = this->CurrentRenderer->GetActiveCamera();

  // Get the axis to rotate around = vector from eye to origin

  double *obj_center = this->InteractionProp->GetCenter();

  double motion_vector[3];
  double view_point[3];

  int eventPI = rwi->GetPointerIndex();
  int otherPI = 0;

  for (int i = 0; i < VTKI_MAX_POINTERS; ++i)
    {
    if (this->PointersDown[i] > 0 && i != eventPI)
      {
      otherPI = i;
      break;
      }
    }

  if (cam->GetParallelProjection())
    {
    // If parallel projection, want to get the view plane normal...
    cam->ComputeViewPlaneNormal();
    cam->GetViewPlaneNormal( motion_vector );
    }
  else
    {
    // Perspective projection, get vector from eye to center of actor
    cam->GetPosition( view_point );
    motion_vector[0] = view_point[0] - obj_center[0];
    motion_vector[1] = view_point[1] - obj_center[1];
    motion_vector[2] = view_point[2] - obj_center[2];
    vtkMath::Normalize(motion_vector);
    }

  double disp_obj_center[3];

  this->ComputeWorldToDisplay(obj_center[0], obj_center[1], obj_center[2],
                              disp_obj_center);
  // compute roll - 1 DOF
  double oldAngle =
    vtkMath::DegreesFromRadians( atan2( (double)rwi->GetLastEventPositions(eventPI)[1] - rwi->GetLastEventPositions(otherPI)[1],
                                        (double)rwi->GetLastEventPositions(eventPI)[0] - rwi->GetLastEventPositions(otherPI)[0] ) );

  double newAngle =
    vtkMath::DegreesFromRadians( atan2( (double)rwi->GetEventPositions(eventPI)[1] - rwi->GetEventPositions(otherPI)[1],
                                        (double)rwi->GetEventPositions(eventPI)[0] - rwi->GetEventPositions(otherPI)[0] ) );
  double scale[3];
  scale[0] = scale[1] = scale[2] = 1.0;

  double **rotate = new double*[1];
  rotate[0] = new double[4];

  rotate[0][0] = newAngle - oldAngle;
  rotate[0][1] = motion_vector[0];
  rotate[0][2] = motion_vector[1];
  rotate[0][3] = motion_vector[2];

  this->Prop3DTransform( this->InteractionProp,
                         obj_center,
                         1,
                         rotate,
                         scale );


  delete [] rotate[0];
  delete [] rotate;

  if ( this->AutoAdjustCameraClippingRange )
  {
    this->CurrentRenderer->ResetCameraClippingRange();
  }

  //rwi->Render();
}



//----------------------------------------------------------------------------
void myInteractorStyle::Pan()
{
  if (this->CurrentRenderer == NULL || this->InteractionProp == NULL)
    {
    return;
    }

  vtkRenderWindowInteractor *rwi = this->Interactor;

  // Use initial center as the origin from which to pan

  double *obj_center = this->InteractionProp->GetCenter();

  double disp_obj_center[3], new_pick_point[4];
  double old_pick_point[4], motion_vector[3];

  this->ComputeWorldToDisplay(obj_center[0], obj_center[1], obj_center[2],
                              disp_obj_center);

  this->ComputeDisplayToWorld(rwi->GetEventPosition()[0],
                              rwi->GetEventPosition()[1],
                              disp_obj_center[2],
                              new_pick_point);

  this->ComputeDisplayToWorld(rwi->GetLastEventPosition()[0],
                              rwi->GetLastEventPosition()[1],
                              disp_obj_center[2],
                              old_pick_point);

  motion_vector[0] = new_pick_point[0] - old_pick_point[0];
  motion_vector[1] = new_pick_point[1] - old_pick_point[1];
  motion_vector[2] = new_pick_point[2] - old_pick_point[2];

  if (this->InteractionProp->GetUserMatrix() != NULL)
    {
    vtkTransform *t = vtkTransform::New();
    t->PostMultiply();
    t->SetMatrix(this->InteractionProp->GetUserMatrix());
    t->Translate(motion_vector[0], motion_vector[1], motion_vector[2]);
    this->InteractionProp->GetUserMatrix()->DeepCopy(t->GetMatrix());
    t->Delete();
    }
  else
    {
    this->InteractionProp->AddPosition(motion_vector[0],
                                       motion_vector[1],
                                       motion_vector[2]);
    }

  if (this->AutoAdjustCameraClippingRange)
    {
    this->CurrentRenderer->ResetCameraClippingRange();
    }

  //rwi->Render();
}

//----------------------------------------------------------------------------
void myInteractorStyle::Dolly()
{
  if (this->CurrentRenderer == NULL || this->InteractionProp == NULL)
    {
    return;
    }

  vtkRenderWindowInteractor *rwi = this->Interactor;
  vtkCamera *cam = this->CurrentRenderer->GetActiveCamera();

  double view_point[3], view_focus[3];
  double motion_vector[3];

  cam->GetPosition(view_point);
  cam->GetFocalPoint(view_focus);

  double *center = this->CurrentRenderer->GetCenter();

  int dy = rwi->GetEventPosition()[1] - rwi->GetLastEventPosition()[1];
  double yf = dy / center[1] * this->MotionFactor;
  double dollyFactor = pow(1.1, yf);

  dollyFactor -= 1.0;
  motion_vector[0] = (view_point[0] - view_focus[0]) * dollyFactor;
  motion_vector[1] = (view_point[1] - view_focus[1]) * dollyFactor;
  motion_vector[2] = (view_point[2] - view_focus[2]) * dollyFactor;

  if (this->InteractionProp->GetUserMatrix() != NULL)
    {
    vtkTransform *t = vtkTransform::New();
    t->PostMultiply();
    t->SetMatrix(this->InteractionProp->GetUserMatrix());
    t->Translate(motion_vector[0], motion_vector[1],
                 motion_vector[2]);
    this->InteractionProp->GetUserMatrix()->DeepCopy(t->GetMatrix());
    t->Delete();
    }
  else
    {
    this->InteractionProp->AddPosition(motion_vector);
    }

  if (this->AutoAdjustCameraClippingRange)
    {
    this->CurrentRenderer->ResetCameraClippingRange();
    }

  //rwi->Render();
}

//----------------------------------------------------------------------------
void myInteractorStyle::UniformScale()
{
  if (this->CurrentRenderer == NULL || this->InteractionProp == NULL)
    {
    return;
    }

  vtkRenderWindowInteractor *rwi = this->Interactor;

  int dy = rwi->GetEventPosition()[1] - rwi->GetLastEventPosition()[1];

  double *obj_center = this->InteractionProp->GetCenter();
  double *center = this->CurrentRenderer->GetCenter();

  double yf = dy / center[1] * this->MotionFactor;
  double scaleFactor = pow(1.1, yf);

  double **rotate = NULL;

  double scale[3];
  scale[0] = scale[1] = scale[2] = scaleFactor;

  this->Prop3DTransform(this->InteractionProp,
                        obj_center,
                        0,
                        rotate,
                        scale);

  if (this->AutoAdjustCameraClippingRange)
    {
    this->CurrentRenderer->ResetCameraClippingRange();
    }

  //rwi->Render();
}

//----------------------------------------------------------------------------
void myInteractorStyle::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}

//----------------------------------------------------------------------------
void myInteractorStyle::FindPickedActor(int x, int y)
{
  // This is to get the prop only if fingers are on it
  /*this->InteractionPicker->Pick(x, y, 0.0, this->CurrentRenderer);
  vtkProp *prop = this->InteractionPicker->GetViewProp();
  if (prop != NULL)
    {
    this->InteractionProp = vtkProp3D::SafeDownCast(prop);
    }
  else
    {
    this->InteractionProp = NULL;
    }*/
    this->InteractionProp = vtkProp3D::SafeDownCast(actor);
}

//----------------------------------------------------------------------------
void myInteractorStyle::Prop3DTransform(vtkProp3D *prop3D,
                                                       double *boxCenter,
                                                       int numRotation,
                                                       double **rotate,
                                                       double *scale)
{
  vtkMatrix4x4 *oldMatrix = vtkMatrix4x4::New();
  prop3D->GetMatrix(oldMatrix);

  double orig[3];
  prop3D->GetOrigin(orig);

  vtkTransform *newTransform = vtkTransform::New();
  newTransform->PostMultiply();
  if (prop3D->GetUserMatrix() != NULL)
    {
    newTransform->SetMatrix(prop3D->GetUserMatrix());
    }
  else
    {
    newTransform->SetMatrix(oldMatrix);
    }

  newTransform->Translate(-(boxCenter[0]), -(boxCenter[1]), -(boxCenter[2]));

  for (int i = 0; i < numRotation; i++)
    {
    newTransform->RotateWXYZ(rotate[i][0], rotate[i][1],
                             rotate[i][2], rotate[i][3]);
    }

  if ((scale[0] * scale[1] * scale[2]) != 0.0)
    {
    newTransform->Scale(scale[0], scale[1], scale[2]);
    }

  newTransform->Translate(boxCenter[0], boxCenter[1], boxCenter[2]);

  // now try to get the composit of translate, rotate, and scale
  newTransform->Translate(-(orig[0]), -(orig[1]), -(orig[2]));
  newTransform->PreMultiply();
  newTransform->Translate(orig[0], orig[1], orig[2]);

  if (prop3D->GetUserMatrix() != NULL)
    {
    newTransform->GetMatrix(prop3D->GetUserMatrix());
    }
  else
    {
    prop3D->SetPosition(newTransform->GetPosition());
    prop3D->SetScale(newTransform->GetScale());
    prop3D->SetOrientation(newTransform->GetOrientation());
    }
  oldMatrix->Delete();
  newTransform->Delete();
}

vtkMatrix4x4* myInteractorStyle::getUserMatrix(){
  return this->userMatrix;
}

double* myInteractorStyle::getMatrixTab(){
  return this->matrixTab;
}

template <typename T> std::string to_string(T value)
{
  std::ostringstream os ;
  os << value ;
  return os.str() ;
}

std::string myInteractorStyle::getStringFromActorMatrix(){
  std::string s = "";
  
  for(int i = 0 ; i < 4 ; i ++){
    for(int j = 0 ; j < 4 ; j++){
        s += to_string(this->actor->GetMatrix()->GetElement(i,j))+";";
    }
  }
  /*for(int i = 0 ; i < 16 ; i ++){
    s += to_string(this->matrixTab[i])+";";
  }*/
  //free(mat);
  return s ;

}

std::string myInteractorStyle::getStringFromVtkMatrix(vtkMatrix4x4* mat){
  if(mat==NULL){
    //LOGW("____________ MAT IS NULL ____________");
    std::string s = "Mat is NUll";
    return  s;
  }
  std::string s = "";
  for(int i = 0 ; i < 4 ; i ++){
    for(int j = 0 ; j < 4 ; j++){
        s += to_string(mat->GetElement(i,j))+";";
    }
  }
  return s ;

}

/*void myInteractorStyle::setUserMatrix(){
  if(this->InteractionProp!=NULL){
    this->userMatrix = this->InteractionProp->GetUserMatrix() ;
    if(this->userMatrix == NULL){
      LOGW("SET USER MATRIX NULL");
    }
    else{
      this->InteractionProp->GetUserMatrix()->DeepCopy(matrixTab);
      std::string s = "";
      for(int i = 0 ; i < 4 ; i ++){
        for(int j = 0 ; j < 4 ; j++){
            s += to_string(this->InteractionProp->GetUserMatrix()->GetElement(i,j))+";";
        }
      }
      LOGW("Matrix when set: %s",s.c_str());
    }
    
  } 
  else this->userMatrix = NULL ;
  //if(this->InteractionProp!=NULL) return this->InteractionProp->GetUserMatrix();
  //else return NULL ;
}*/








/****** Old Functions ******/

//----------------------------------------------------------------------------
/*void myInteractorStyle::OnMiddleButtonDown()
{
  int x = this->Interactor->GetEventPosition()[0];
  int y = this->Interactor->GetEventPosition()[1];

  this->FindPokedRenderer(x, y);
  this->FindPickedActor(x, y);
  if (this->CurrentRenderer == NULL || this->InteractionProp == NULL)
    {
    return;
    }

  this->GrabFocus(this->EventCallbackCommand);
  if (this->Interactor->GetControlKey())
    {
    this->StartDolly();
    }
  else
    {
    this->StartPan();
    }
}

//----------------------------------------------------------------------------
void myInteractorStyle::OnMiddleButtonUp()
{
  switch (this->State)
    {
    case VTKIS_DOLLY:
      this->EndDolly();
      break;

    case VTKIS_PAN:
      this->EndPan();
      break;
    }

  if ( this->Interactor )
    {
    this->ReleaseFocus();
    }
}

//----------------------------------------------------------------------------
void myInteractorStyle::OnRightButtonDown()
{
  int x = this->Interactor->GetEventPosition()[0];
  int y = this->Interactor->GetEventPosition()[1];

  this->FindPokedRenderer(x, y);
  this->FindPickedActor(x, y);
  if (this->CurrentRenderer == NULL || this->InteractionProp == NULL)
    {
    return;
    }

  this->GrabFocus(this->EventCallbackCommand);
  this->StartUniformScale();
}

//----------------------------------------------------------------------------
void myInteractorStyle::OnRightButtonUp()
{
  switch (this->State)
    {
    case VTKIS_USCALE:
      this->EndUniformScale();
      break;
    }

  if ( this->Interactor )
    {
    this->ReleaseFocus();
    }
}
*/




//----------------------------------------------------------------------------
/*void myInteractorStyle::Spin()
{
  if ( this->CurrentRenderer == NULL || this->InteractionProp == NULL )
    {
    return;
    }

  vtkRenderWindowInteractor *rwi = this->Interactor;
  vtkCamera *cam = this->CurrentRenderer->GetActiveCamera();

  // Get the axis to rotate around = vector from eye to origin

  double *obj_center = this->InteractionProp->GetCenter();

  double motion_vector[3];
  double view_point[3];

  if (cam->GetParallelProjection())
    {
    // If parallel projection, want to get the view plane normal...
    cam->ComputeViewPlaneNormal();
    cam->GetViewPlaneNormal( motion_vector );
    }
  else
    {
    // Perspective projection, get vector from eye to center of actor
    cam->GetPosition( view_point );
    motion_vector[0] = view_point[0] - obj_center[0];
    motion_vector[1] = view_point[1] - obj_center[1];
    motion_vector[2] = view_point[2] - obj_center[2];
    vtkMath::Normalize(motion_vector);
    }

  double disp_obj_center[3];

  this->ComputeWorldToDisplay(obj_center[0], obj_center[1], obj_center[2],
                              disp_obj_center);

  double newAngle =
    vtkMath::DegreesFromRadians( atan2( rwi->GetEventPosition()[1] - disp_obj_center[1],
                                        rwi->GetEventPosition()[0] - disp_obj_center[0] ) );

  double oldAngle =
    vtkMath::DegreesFromRadians( atan2( rwi->GetLastEventPosition()[1] - disp_obj_center[1],
                                        rwi->GetLastEventPosition()[0] - disp_obj_center[0] ) );

  double scale[3];
  scale[0] = scale[1] = scale[2] = 1.0;

  double **rotate = new double*[1];
  rotate[0] = new double[4];

  rotate[0][0] = newAngle - oldAngle;
  rotate[0][1] = motion_vector[0];
  rotate[0][2] = motion_vector[1];
  rotate[0][3] = motion_vector[2];

  this->Prop3DTransform( this->InteractionProp,
                         obj_center,
                         1,
                         rotate,
                         scale );

  delete [] rotate[0];
  delete [] rotate;

  if ( this->AutoAdjustCameraClippingRange )
    {
    this->CurrentRenderer->ResetCameraClippingRange();
    }

  rwi->Render();
}*/


/*void myInteractorStyle::Pan()
{
  if (this->CurrentRenderer == NULL || this->InteractionProp == NULL)
  {
    return;
  }

  vtkRenderWindowInteractor *rwi = this->Interactor;
    // find the moving and non moving points
  int eventPI = rwi->GetPointerIndex();
  int otherPI = 0;

  for (int i = 0; i < VTKI_MAX_POINTERS; ++i)
    {
    if (this->PointersDown[i] > 0 && i != eventPI)
      {
      otherPI = i;
      break;
      }
    }

  double position[2] ;
  position[0] = (double)rwi->GetLastEventPositions(eventPI)[0] + (double)rwi->GetLastEventPositions(otherPI)[0] ;
  position[1] = (double)rwi->GetLastEventPositions(eventPI)[1] + (double)rwi->GetLastEventPositions(otherPI)[1] ;




  /*vtkCamera *camera = this->CurrentRenderer->GetActiveCamera();
  double viewFocus[4], focalDepth, viewPoint[3];
  double newPickPoint[4], oldPickPoint[4], motionVector[3];

  // Calculate the focal depth since we'll be using it a lot
  camera->GetFocalPoint(viewFocus);
  this->ComputeWorldToDisplay(viewFocus[0], viewFocus[1], viewFocus[2],
                              viewFocus);
  focalDepth = viewFocus[2];

  this->ComputeDisplayToWorld((rwi->GetEventPositions(eventPI)[0] + rwi->GetEventPositions(otherPI)[0])/2.0,
                              (rwi->GetEventPositions(eventPI)[1] + rwi->GetEventPositions(otherPI)[1])/2.0,
                              focalDepth,
                              newPickPoint);

  // Has to recalc old mouse point since the viewport has moved,
  // so can't move it outside the loop
  this->ComputeDisplayToWorld((rwi->GetLastEventPositions(eventPI)[0] + rwi->GetLastEventPositions(otherPI)[0])/2.0,
                              (rwi->GetLastEventPositions(eventPI)[1] + rwi->GetLastEventPositions(otherPI)[1])/2.0,
                              focalDepth,
                              oldPickPoint);

  // Camera motion is reversed
  motionVector[0] = oldPickPoint[0] - newPickPoint[0];
  motionVector[1] = oldPickPoint[1] - newPickPoint[1];
  motionVector[2] = oldPickPoint[2] - newPickPoint[2];

  camera->GetFocalPoint(viewFocus);
  camera->GetPosition(viewPoint);
  camera->SetFocalPoint(motionVector[0] + viewFocus[0],
                        motionVector[1] + viewFocus[1],
                        motionVector[2] + viewFocus[2]);

  camera->SetPosition(motionVector[0] + viewPoint[0],
                      motionVector[1] + viewPoint[1],
                      motionVector[2] + viewPoint[2]);
*/



  
  // Use initial center as the origin from which to pan

/*  double *obj_center = this->InteractionProp->GetCenter();

  double disp_obj_center[3], new_pick_point[4];
  double old_pick_point[4], motion_vector[3];

  this->ComputeWorldToDisplay(obj_center[0], obj_center[1], obj_center[2],
                              disp_obj_center);

  this->ComputeDisplayToWorld(position[0],
                              position[1],
                              disp_obj_center[2],
                              new_pick_point);

  this->ComputeDisplayToWorld(position[0],
                              position[1],
                              disp_obj_center[2],
                              old_pick_point);

  motion_vector[0] = new_pick_point[0] - old_pick_point[0];
  motion_vector[1] = new_pick_point[1] - old_pick_point[1];
  motion_vector[2] = new_pick_point[2] - old_pick_point[2];
  LOGW("cbairubcuierbuyvb = %d,%d",motion_vector[0],motion_vector[1]);

  if (this->InteractionProp->GetUserMatrix() != NULL)
    {
    vtkTransform *t = vtkTransform::New();
    t->PostMultiply();
    t->SetMatrix(this->InteractionProp->GetUserMatrix());
    t->Translate(motion_vector[0], motion_vector[1], motion_vector[2]);
    this->InteractionProp->GetUserMatrix()->DeepCopy(t->GetMatrix());
    t->Delete();
    }
  else
    {
    this->InteractionProp->AddPosition(motion_vector[0],
                                       motion_vector[1],
                                       motion_vector[2]);
    }



  

  if (this->AutoAdjustCameraClippingRange)
    {
    this->CurrentRenderer->ResetCameraClippingRange();
    }

  //rwi->Render();
}*/


