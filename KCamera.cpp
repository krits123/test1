#include "KCamera.h"



KCamera::KCamera()
{

	cam_ornt.setIdentity();
	

}

KCamera::KCamera(vec3 P)
{
	cam_ornt.setTransMat(P);
	
}

void KCamera::moveForward(float dz)
{
	Mat4 R,tmp;
	R.setTransMat(vec3(0, 0, dz));

	tmp.MatMatMult(cam_ornt, R);

	cam_ornt = tmp;

}

void KCamera::moveUp(float dy)
{

	Mat4 R, tmp;
	R.setTransMat(vec3(0, dy, 0));

	tmp.MatMatMult(cam_ornt, R);

	cam_ornt = tmp;
}

void KCamera::moveRight(float dx)
{

	Mat4 R, tmp;
	R.setTransMat(vec3(dx, 0, 0));

	tmp.MatMatMult(cam_ornt, R);

	cam_ornt = tmp;
}

void KCamera::rotateY(float dth)
{
	Mat4 R, tmp;
	R.setRotY(dth);

	tmp.MatMatMult(cam_ornt, R);

	cam_ornt = tmp;
}

void KCamera::rotateX(float dth)
{

	Mat4 R, tmp;
	R.setRotX(dth);

	tmp.MatMatMult(cam_ornt, R);

	cam_ornt = tmp;
}


void KCamera::rotateZ(float dth)
{

	Mat4 R, tmp;
	R.setRotZ(dth);

	tmp.MatMatMult(cam_ornt, R);

	cam_ornt = tmp;
}

void KCamera::resetCam()
{
	cam_ornt.setIdentity();
}

void KCamera::getViewMatrix(Mat4& o)
{
	o.Inverse(cam_ornt);

}

vec3 KCamera::getCamPos()
{
	vec3 o;
	cam_ornt.vecMatMult({ 0,0,0,}, o);


	return o;
}


