#include "Mat4.h"
#pragma once
class KritsSimulator
{
public:

	// State variables 
	float th1,th1_d,th1_dd;
	float th2, th2_d, th2_dd;
	float u1, u2;

	// Control Parameters
	float th1_des,th2_des;
	float kp1, kp2, kd1, kd2;

	// System parameters
	float m1, m2, L1, L2;
	float G;
	float fr_coef; // Friction coefficient
	float static_fr;

	// Simulation Parameters
	float maxTime, totalTime;
	int numOfSteps;


	KritsSimulator();



	// Simulating functions 
	void simulateStep(float fElapsedTime);

	void getAcc();
	void getInput();
	void update(float dt);
	
	void getTransfomarions(const Mat4& TW0, const Mat4& T1M, const Mat4& T2M, Mat4& TWM_1, Mat4& TWM_2);

	
};

