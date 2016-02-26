#pragma once

#ifndef PARTICLE
#define PARTICLE
#include <windows.h>
#include "toolbox.h"
#include "linear_math.h"
#include "vtkQuaternion.h"
#include "vtkTransform.h"
#include "vtkMatrix4x4.h"
#include "vtkMath.h"
#include "vtkSmartPointer.h"
#include <errno.h>
#include <sstream>
#include <string>
#include <string>

static SYSTEMTIME particleSTime;
const float particleSpeed = 0.15f;
//const int particleReleaseDuration = 500; // ms
const int particleReleaseDuration = 700; // ms
const int particleStallDuration = 1000; // ms
const float stylusEffectorDist = 24.0f;

	struct Particle
	{
		Vector3 pos;
		bool valid;
		int delayMs, stallMs;
		SYSTEMTIME lastTime;
	};
	
	

	

	Vector3 particleJitter() ;
	void releaseParticles(const int *dataDim, std::vector<Particle> particles, double* pos);
	void integrateParticleMotion(Particle& p,vtkSmartPointer<vtkImageData> velocityData, int *dataDim);

  
#endif