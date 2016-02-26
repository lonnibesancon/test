#include "Particles.h"


Vector3 particleJitter()
{
  return Vector3(
    (float(std::rand()) / RAND_MAX),
    (float(std::rand()) / RAND_MAX),
    (float(std::rand()) / RAND_MAX)
  ) * 1.0f;
}


void integrateParticleMotion(Particle& p,vtkSmartPointer<vtkImageData> velocityData, int *dataDim)
{
  if (!p.valid)
    return;

  SYSTEMTIME now;
  GetSystemTime (&now);


  //double* dataDim ;       //TO FIX, Dimensions are never set

  int elapsedMs = (now.wSecond - p.lastTime.wSecond) * 1000
    + (now.wSecond - p.lastTime.wSecond) / 1000000;

  p.lastTime = now;

  if (p.delayMs > 0) {
    p.delayMs -= elapsedMs;
    if (p.delayMs < 0)
      elapsedMs = -p.delayMs;
    else
      return;
  }

  if (p.stallMs > 0) {
    p.stallMs -= elapsedMs;
    if (p.stallMs < 0)
      p.valid = false;
    return;
  }

  vtkSmartPointer<vtkDataArray> vectors = velocityData->GetPointData()->GetVectors();


  while (elapsedMs > 0) {
    --elapsedMs;

    DataCoords coords = DataCoords(p.pos.x, p.pos.y, p.pos.z);

    if (coords.x < 0 || coords.y < 0 || coords.z < 0
        || coords.x >= dataDim[0] || coords.y >= dataDim[1] || coords.z >= dataDim[2])
    {
      // LOGD("particle moved outside bounds");
      p.valid = false;
      return;
    }

    double* v = vectors->GetTuple3(coords.z*(dataDim[0]*dataDim[1]) + coords.y*dataDim[0] + coords.x);
    //printf("v = %f, %f, %f", v[0], v[1], v[2]);

    // Vector3 vel(v[0], v[1], v[2]);
    Vector3 vel(v[1], v[0], v[2]); // XXX: workaround for a wrong data orientation

    if (!vel.isNull()) {
      p.pos += vel * particleSpeed;
	  printf("p.pos= = %f, %f, %f \n", p.pos.x, p.pos.y, p.pos.z);
    } else {
      // LOGD("particle stopped");
      p.stallMs = particleStallDuration;
      break;
    }
  }
}

void releaseParticles(const int *dataDim, std::vector<Particle> particles, double* pos)
{

  Matrix4 smm;

  // TOFIX 

  /*synchronized (state->stylusModelMatrix) {
    smm = state->stylusModelMatrix;
  }*/ 

  //double* dataDim ;       //TO FIX, Dimensions are never set
  Vector3 dataSpacing ;
  // I set it myself to 1
  dataSpacing.x = 1 ;
  dataSpacing.y = 1 ;
  dataSpacing.z = 1 ;

  //TOFIX
  //const float size = 0.5f * (stylusEffectorDist + (std::max)(dataSpacing.x*dataDim[0], (std::max)(dataSpacing.y*dataDim[1], dataSpacing.z*dataDim[2])));
  //Vector3 dataPos ;//= posToDataCoords(smm * Matrix4::makeTransform(Vector3(-size, 0, 0) /* *settings->zoomFactor*/) * Vector3::zero());
  Vector3 dataPos ;
  dataPos.x = pos[0] ;
  dataPos.y = pos[1] ;
  dataPos.z = pos[2] ;
  if (dataPos.x < 0 || dataPos.y < 0 || dataPos.z < 0
      || dataPos.x >= dataDim[0] || dataPos.y >= dataDim[1] || dataPos.z >= dataDim[2])
  {
    cout << "outside bounds" << endl ;
    return;
  }

  DataCoords coords(dataPos.x, dataPos.y, dataPos.z);
  // LOGD("coords = %s", Utility::toString(coords).c_str());

  // vtkDataArray* vectors = velocityData->GetPointData()->GetVectors();
  // double* v = vectors->GetTuple3(coords.z*(dataDim[0]*dataDim[1]) + coords.y*dataDim[0] + coords.x);
  // LOGD("v = %f, %f, %f", v[0], v[1], v[2]);

  // XXX: test streamlines
  // vtkNew<vtkStreamLine> streamLine;
  // streamLine->SetInputData(velocityData);
  // // streamLine->SetStartPosition(coords.x, coords.y, coords.z);
  // streamLine->SetStartPosition(dataPos.x, dataPos.y, dataPos.z);
  // streamLine->SetMaximumPropagationTime(200);
  // streamLine->SetIntegrationStepLength(.2);
  // streamLine->SetStepLength(.001);
  // streamLine->SetNumberOfThreads(1);
  // streamLine->SetIntegrationDirectionToForward();
  // streamLine->Update();
  // vtkDataArray* vectors = streamLine->GetPointData()->GetVectors();
  // android_assert(vectors);
  // unsigned int num = vectors->GetNumberOfTuples();
  // LOGD("num = %d", num);
  // for (unsigned int i = 0; i < num; ++j) {
  //  double* v = vectors->GetTuple3(i);
  //  Vector3 pos(v[0], v[1], v[2]);
  //  LOGD("pos = %s", Utility::toString(pos).c_str());
  // }



  //TOFIX
  GetSystemTime (&particleSTime);
  //clock_gettime(CLOCK_REALTIME, &particleStartTime);

  int delay = 0;
  Particle p ;
  //synchronized (particles) {
    for (int i = 0 ; i < 200 ; i++) {
      p = particles[i] ;
      p.pos = Vector3(coords.x, coords.y, coords.z) + particleJitter();
	  //cout << " Coord.x " << coords.x << " coords.y = " << coords.y << " coords.z = " << coords.z << endl ;
	  //cout << " p.pos.x " << p.pos.x << " p.pos.y = " << p.pos.y << " p.pos.z = " << p.pos.z << endl ;
	  //cout << "Particle jitter X = " << particleJitter().x << endl ;
      p.lastTime = particleSTime;
      p.delayMs = delay;
      delay += particleReleaseDuration/particles.size();
      p.stallMs = 0;
      p.valid = true;
    }
  //} //TO FIX
}



#if 0
Vector3 particleJitter()
{
  return Vector3(
    (float(std::rand()) / RAND_MAX),
    (float(std::rand()) / RAND_MAX),
    (float(std::rand()) / RAND_MAX)
  ) * 1.0f;
}


void integrateParticleMotion(Particle& p)
{
  if (!p.valid)
    return;


  timespec now;
  clock_gettime(CLOCK_REALTIME, &now);

  double* dataDim ;       //TO FIX, Dimensions are never set

  int elapsedMs = (now.tv_sec - p.lastTime.tv_sec) * 1000
    + (now.tv_nsec - p.lastTime.tv_nsec) / 1000000;

  p.lastTime = now;

  if (p.delayMs > 0) {
    p.delayMs -= elapsedMs;
    if (p.delayMs < 0)
      elapsedMs = -p.delayMs;
    else
      return;
  }

  if (p.stallMs > 0) {
    p.stallMs -= elapsedMs;
    if (p.stallMs < 0)
      p.valid = false;
    return;
  }

  vtkDataArray* vectors = foo->velocityData->GetPointData()->GetVectors();


  while (elapsedMs > 0) {
    --elapsedMs;

    DataCoords coords = DataCoords(p.pos.x, p.pos.y, p.pos.z);

    if (coords.x < 0 || coords.y < 0 || coords.z < 0
        || coords.x >= dataDim[0] || coords.y >= dataDim[1] || coords.z >= dataDim[2])
    {
      // LOGD("particle moved outside bounds");
      p.valid = false;
      return;
    }

    double* v = vectors->GetTuple3(coords.z*(dataDim[0]*dataDim[1]) + coords.y*dataDim[0] + coords.x);
    // LOGD("v = %f, %f, %f", v[0], v[1], v[2]);

    // Vector3 vel(v[0], v[1], v[2]);
    Vector3 vel(v[1], v[0], v[2]); // XXX: workaround for a wrong data orientation

    if (!vel.isNull()) {
      p.pos += vel * particleSpeed;
    } else {
      // LOGD("particle stopped");
      p.stallMs = particleStallDuration;
      break;
    }
  }
}

void releaseParticles()
{

  Matrix4 smm;

  // TOFIX 

  /*synchronized (state->stylusModelMatrix) {
    smm = state->stylusModelMatrix;
  }*/ 

  double* dataDim ;       //TO FIX, Dimensions are never set
  Vector3 dataSpacing ;

  const float size = 0.5f * (stylusEffectorDist + std::max(dataSpacing.x*dataDim[0], std::max(dataSpacing.y*dataDim[1], dataSpacing.z*dataDim[2])));
  //TOFIX
  Vector3 dataPos = posToDataCoords(smm * Matrix4::makeTransform(Vector3(-size, 0, 0) /* *settings->zoomFactor*/) * Vector3::zero());

  if (dataPos.x < 0 || dataPos.y < 0 || dataPos.z < 0
      || dataPos.x >= dataDim[0] || dataPos.y >= dataDim[1] || dataPos.z >= dataDim[2])
  {
    LOGW("outside bounds");
    return;
  }

  DataCoords coords(dataPos.x, dataPos.y, dataPos.z);
  // LOGD("coords = %s", Utility::toString(coords).c_str());

  // vtkDataArray* vectors = velocityData->GetPointData()->GetVectors();
  // double* v = vectors->GetTuple3(coords.z*(dataDim[0]*dataDim[1]) + coords.y*dataDim[0] + coords.x);
  // LOGD("v = %f, %f, %f", v[0], v[1], v[2]);

  // XXX: test streamlines
  // vtkNew<vtkStreamLine> streamLine;
  // streamLine->SetInputData(velocityData);
  // // streamLine->SetStartPosition(coords.x, coords.y, coords.z);
  // streamLine->SetStartPosition(dataPos.x, dataPos.y, dataPos.z);
  // streamLine->SetMaximumPropagationTime(200);
  // streamLine->SetIntegrationStepLength(.2);
  // streamLine->SetStepLength(.001);
  // streamLine->SetNumberOfThreads(1);
  // streamLine->SetIntegrationDirectionToForward();
  // streamLine->Update();
  // vtkDataArray* vectors = streamLine->GetPointData()->GetVectors();
  // android_assert(vectors);
  // unsigned int num = vectors->GetNumberOfTuples();
  // LOGD("num = %d", num);
  // for (unsigned int i = 0; i < num; ++j) {
  //  double* v = vectors->GetTuple3(i);
  //  Vector3 pos(v[0], v[1], v[2]);
  //  LOGD("pos = %s", Utility::toString(pos).c_str());
  // }

  clock_gettime(CLOCK_REALTIME, &particleStartTime);

  int delay = 0;
  Particle p ;
  //synchronized (particles) {
    for (int i = 0 ; i < 200 ; i++) {
      p = particles[i] ;
      p.pos = Vector3(coords.x, coords.y, coords.z) + particleJitter();
      p.lastTime = particleStartTime;
      p.delayMs = delay;
      delay += particleReleaseDuration/particles.size();
      p.stallMs = 0;
      p.valid = true;
    }
  //} //TO FIX
}

#endif





