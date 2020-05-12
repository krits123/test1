#include "KritsSimulator.h"
#include <cmath>

KritsSimulator::KritsSimulator()
{

	th1 = 0.0f;
	th1_d = 0.0f;
	th1_dd = 0.0f;

	th2 = 0.0f;
	th2_d = 0.0f;
	th2_dd = 0.0f;

	u1 = 0.0f;
	u2 = 0.0f;

	m1 = 0.1f;
	m2 = 0.1f;

	L1 = 1.0f;
	L2 = 1.0f;

	G = 9.81f;

	fr_coef = 0.0707f;
	static_fr = 0.1f;
	maxTime = 20.0f;

	totalTime = 0.0f;

	numOfSteps = 10;

	// Control Parameters

	kd1 = 1.0f;
	kd2 = 1.0f;
	kp1 = 1.0f;
	kp2 = 1.0f;

	th1_des = 3.14159f / 3;
	th2_des = 3.14159f / 4;
}

void KritsSimulator::simulateStep(float fElapsedTime)
{
	if (totalTime < maxTime) {

		totalTime += fElapsedTime;
		float simTime = fElapsedTime / (float)numOfSteps;

		for (int i = 0;i < numOfSteps;i++) {

			getInput();
			getAcc();
			update(simTime);

		}


	}
}

void KritsSimulator::getAcc()
{

	// Calculate Mass Matrix
	float M1, M12, M2;

	M1 = L2 * L2 * m2 + 2 * L1 * L2 * m2 * cos(th2) + L1 * L1 * (m1 + m2);

	M12 = L2 * L2 * m2 + L1 * L2 * m2 * cos(th2);

	M2 = L2 * L2 * m2;

	// calculate Force vectors
	float v1, v2, g1, g2,fr1,fr2;

	v1 = -L1 * L2 * m2 * sin(th2) * th2_d * (th2_d + 2 * th1_d);
	v2 = m2 * L1 * L2 * sin(th2) * th1_d * th1_d;

	g1 = m2 * L2 * G * cos(th1 + th2) + (m1 + m2) * L1 * G * cos(th1);
	g2 = m2 * G*L2 * cos(th1 + th2);

	// Implement dynamic friction 
	fr1 = -fr_coef * th1_d;
	fr2 = -fr_coef * th2_d;

	// Implement static friction 
	if ((th1_d) > 0.1f)
		fr1 -= static_fr;
	else if (th1_d < -0.1f)
		fr1 += static_fr;
	if ((th2_d) > 0.1f)
		fr2 -= static_fr;
	else if (th2_d < -0.1f)
		fr2 += static_fr;



	float k1, k2;
	k1 = u1 + fr1 - v1 - g1;
	k2 = u2 + fr2 - v2 - g2;

	// Solve system for angular accelerations

	float D, D1, D2;

	D = M1 * M2 - M12 * M12;
	D1 = k1 * M2 - k2 * M12;
	D2 = M1 * k2 - M12 * k1;

	if (D != 0.0f) {
		th1_dd = D1 / D;
		th2_dd = D2 / D;
	}
}

void KritsSimulator::getInput()
{
	float g1 = m2 * L2 * G * cos(th1 + th2) + (m1 + m2) * L1 * G * cos(th1);
	float g2 = m2 * G * L2 * cos(th1 + th2);

	u1 = (th1_des - th1) * kp1 - kd1 * th1_d + g1;

	u2 = (th2_des - th2) * kp2 - kd2 * th2_d + g2;
}

void KritsSimulator::update(float dt)
{
	// Intgrate both state variables
	th1_d += th1_dd * dt;
	th1 += th1_d * dt + 0.5f * th1_dd * dt * dt;

	th2_d += th2_dd * dt;
	th2 += th2_d * dt + 0.5f * th2_dd * dt * dt;

}

void KritsSimulator::getTransfomarions(const Mat4& TW0, const Mat4& T1M, const Mat4& T2M, Mat4& TWM_1, Mat4& TWM_2)
{



	Mat4 T01, T12, tmp1, tmp2;


	T01.setRotZ(th1);


	T12.setRotZ(th2);
	T12.setTransPart({ L1,0.0f,0.0f });


	tmp1.MatMatMult(TW0, T01);


	TWM_1.MatMatMult(tmp1, T1M);

	tmp2.MatMatMult(tmp1, T12);

	TWM_2.MatMatMult(tmp2, T2M);




}
