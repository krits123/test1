#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <list>
#include "kriteGameEngineDefs.h"

struct colRGB {
	int r, g, b;

};


struct vec3d {

	float x, y, z;
	
};

struct KTriangle {
	vec3d p[3];
	float lumFactor ;
	vec3d text;
};


struct mat4x4 {

	float m[4][4] = { 0 };

};


struct mesh{

	std::vector<KTriangle> tris;
	bool readObjFile(std::string fName) {

		

		std::ifstream f(fName);
		if (!f.is_open())
			return false;

		std::vector<vec3d> verts;

		while (!f.eof()) {

			char line[128];
			f.getline(line, 128);

			std::stringstream s;
			s << line;

			char junk;

			if (line[0] == 'v') {

				vec3d v;
				s >> junk >> v.x >> v.y >> v.z;
				verts.push_back(v);
			}

			if (line[0] == 'f') {

				int f[3];

				s >> junk >> f[0] >> f[1] >> f[2];

				tris.push_back({ verts[f[0] - (int)1] , verts[f[1] - (int)1] , verts[f[2] - (int)1],1 });
			}

		}
		

		return true;

	}
};


class KritsTriangles : public sf::Drawable, public sf::Transformable
{
public:

	

	void setUp() {
		m_vertices.setPrimitiveType(sf::Triangles);

	}


	void clear() {
		m_vertices.clear();
	}

	void addNewTriangle(KTriangle tri ,sf::Color col) {


		sf::Vertex v1, v2, v3;

		v1.position = sf::Vector2f(tri.p[0].x, tri.p[0].y);
		v2.position = sf::Vector2f(tri.p[1].x, tri.p[1].y);
		v3.position = sf::Vector2f(tri.p[2].x, tri.p[2].y);


		v1.color = col;
		v2.color = col;
		v3.color = col;

		m_vertices.append(v1);
		m_vertices.append(v2);
		m_vertices.append(v3);
	}

private:

	virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const
	{
		// apply the transform
		states.transform *= getTransform();

		// apply the tileset texture
		//states.texture = &m_tileset;

		// draw the vertex array
		target.draw(m_vertices, states);
	}

	sf::VertexArray m_vertices;
	//sf::Texture m_tileset;
};
















class KritsGameEngine {

private:
	sf::RenderWindow window;
	int winWidth =0;
	int winHeight =0;

	mesh meshCube;
	mat4x4 matProj,matRotY;
	float theta = 0.0f;
	vec3d cam = { 0,0,0 };
	std::string Title;

	KritsTriangles taster;
	mat4x4 camMat;



public:

	int index2D(int x, int y) {
		return y * winWidth + x;
	}

	void createWin(int w, int h, const char *msg) {

		winWidth = w;
		winHeight = h;
		window.create(sf::VideoMode(winWidth, winHeight), msg);
		Title = std::string(msg);
		
		

	

	}

	int begin() {

		
		if(!KritsSetUp()) return 1;
		sf::Clock clock;
		while (window.isOpen())
		{
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed) {
					window.close();
				}
			}
			// Clear window 
			window.clear();

			// Calculate Elapsed Time and Fps
			sf::Time elapsed = clock.restart();
			float fElapsedTime = elapsed.asSeconds();
			float fps = 1.f / (fElapsedTime);


			// Fill pixelBuffer/Draw in window
			drawFrame(fElapsedTime);
			
			

		

			// Display title and Fps
			std::string title;
			std::ostringstream ss;
			ss << fps;
			std::string s(ss.str());
			title = Title + std::string("\t FPS = " ) +  s;			
			window.setTitle(title);
			window.display();



		}

		return 0;
	}


	


	bool KritsSetUp() {

		cam.x = 0;
		cam.y = 0;
		cam.z = 0;
		//if(!meshCube.readObjFile("videoShip.obj"))
		//if (!meshCube.readObjFile("mountains.obj"))
			//return false;


		meshCube.tris = {

			// SOUTH
			{ 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f },

			// EAST                                                      
			{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f },
			{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f },

			// NORTH                                                     
			{ 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f },
			{ 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f },

			// WEST                                                      
			{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f },

			// TOP                                                       
			{ 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f },
			{ 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f },

			// BOTTOM                                                    
			{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f },
			{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f },

		};
		
		for (int i = 0;i < 4;i++)camMat.m[i][i] = 1;

		
		float fNear = 0.1f;
		float fFar = 1000.0f;
		
		float fov = 90.0f; // Degrees

		float fAspecyRatio = (float)winHeight / winWidth;

		float fov_coef = (float)1.0 / (float)tan(fov *0.5*3.14159f/180.0f);

		
		setMatProj(fAspecyRatio, fov_coef, fNear, fFar, matProj);
		

		taster.setUp();

		return true;
	}
	



	bool drawFrame(float  fElapsedTime) {
	
		taster.clear();
		float dYaw = 0.0f;
		float dPitch = 0.0f;

		sf::Clock timer1;

		//Get User Input
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{

			//dYaw = fElapsedTime * 2.0f;
			// left key is pressed: move our character
			theta += fElapsedTime * 2;
			//std::cout << "{"<<fElapsedTime <<"," <<theta << "}" << std::endl;

		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{

			//dYaw = -fElapsedTime * 2.0f;
			// left key is pressed: move our character
			theta -= fElapsedTime * 2;
			//std::cout << "{" << fElapsedTime << "," << theta << "}" << std::endl;
		}




		setMatRotY(theta, matRotY);
		//setMatRotZ(theta * 0.5, matRotZ);

		
		vec3d cam_for;


		MultMatVec({ 0,0,1 }, cam_for, matRotY);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			std::cout << cam_for.x << "," << cam_for.y << "," << cam_for.z << std::endl;
		}
		//Get User Input
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			// left key is pressed: move our character
			vector_Mult(cam_for, 8 * fElapsedTime);
			vector_Add_Acc(cam, cam_for);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			// left key is pressed: move our character
			vector_Mult(cam_for, 8 * fElapsedTime);
			vector_Sub_Acc(cam, cam_for);
		}


		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			// left key is pressed: move our character
			vec3d tmp = { 0,1,0 };
			vector_Mult(tmp, 3 * fElapsedTime);
			vector_Add_Acc(cam, tmp);
		}


		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			// left key is pressed: move our character
			vec3d tmp = { 0,1,0 };
			vector_Mult(tmp, 8 * fElapsedTime);
			vector_Sub_Acc(cam, tmp);
			
		}


		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			// left key is pressed: move our character
			vec3d tmp = { 0,1,0 };

			vec3d right;
			crossProduct(cam_for, tmp,right);

			vector_Mult(right, 8 * fElapsedTime);
			vector_Sub_Acc(cam, right);
		}


		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			// left key is pressed: move our character
			vec3d tmp = { 0,1,0 };

			vec3d right;
			crossProduct(cam_for, tmp, right);

			vector_Mult(right, 8 * fElapsedTime);
			vector_Add_Acc(cam, right);

		}



		mat4x4 invCam = invCamMat(cam, theta);

		 
		//mat4x4 newCamMat;

		//MultMatMat(camMat, yawMat, newCamMat);
		//MultMatMat(newCamMat, pitchMat, camMat);

		std::vector<KTriangle> trigs2Draw;
		
		for (auto tri : meshCube.tris) {

			KTriangle transTri,projTri,camTri;


			transTri = tri;

			transTri.p[0].z = tri.p[0].z + 4.0f;
			transTri.p[1].z = tri.p[1].z + 4.0f;
			transTri.p[2].z = tri.p[2].z + 4.0f;

			

			// calculate triangle's normal
			vec3d normal = getTriNormal(transTri);

			// Check if triagnle is visible by the camera in world coordinates

			// Get Vectort from Camera to mid Point 
			vec3d camRay;
			vec3d triMidP;

			// Get Tri mid Point
			triMidP.x = (transTri.p[0].x + transTri.p[1].x + transTri.p[2].x) / 3.0f;
			triMidP.y = (transTri.p[0].y + transTri.p[1].y + transTri.p[2].y) / 3.0f;
			triMidP.z = (transTri.p[0].z + transTri.p[1].z + transTri.p[2].z) / 3.0f;
			
			getVecFromPoints(cam, transTri.p[0], camRay);

			if (dotProduct(camRay,normal) < 0.0f) {
				

				vec3d light_dir = {0.0f,1.0f,-1.0f};

				


				float l = sqrt(light_dir.x * light_dir.x + light_dir.y * light_dir.y + light_dir.z * light_dir.z);
				light_dir.x /= l; light_dir.y /= l; light_dir.z /= l;

				float dp = dotProduct(light_dir,normal);

				// Apply the camera Transformation on each point 
				// Now each's points coordinates are as seen by the camera
				MultMatVec(transTri.p[0], camTri.p[0], invCam);
				MultMatVec(transTri.p[1], camTri.p[1], invCam);
				MultMatVec(transTri.p[2], camTri.p[2], invCam);
				

				// Clip triangles in camera view - camera coordinates - world space 


				int nClippedTriangles = 0;
				KTriangle clipped[2];
				nClippedTriangles = clippedTriangle({ 0.0f, 0.0f, 0.1f }, { 0.0f, 0.0f, 1.0f }, camTri, clipped[0], clipped[1]);


				for (int n = 0;n < nClippedTriangles;n++) {

					applyProjOpp(clipped[n].p[0], projTri.p[0], matProj);
					applyProjOpp(clipped[n].p[1], projTri.p[1], matProj);
					applyProjOpp(clipped[n].p[2], projTri.p[2], matProj);


					// Put x and y in non unverted pos 
					projTri.p[0].x *= -1.0f;
					projTri.p[1].x *= -1.0f;
					projTri.p[2].x *= -1.0f;
					projTri.p[0].y *= -1.0f;
					projTri.p[1].y *= -1.0f;
					projTri.p[2].y *= -1.0f;


					//scale each point

					scaleIntoView(projTri.p[0]);
					scaleIntoView(projTri.p[1]);
					scaleIntoView(projTri.p[2]);


					dp = fabs(dp);
					if (dp < 0.2f)dp = 0.2f;
					if (dp >= 1.0f) dp = 1.0f;


					projTri.lumFactor = dp;

					if (projTri.lumFactor > 1) projTri.lumFactor = 1.0f;

					trigs2Draw.push_back(projTri);

				}


			}
		 }






		sort(trigs2Draw.begin(), trigs2Draw.end(), [](KTriangle & t1, KTriangle & t2){

			float z1 = (t1.p[0].z + t1.p[1].z + t1.p[2].z)/3.0f;
			float z2 = (t2.p[0].z + t2.p[1].z + t2.p[2].z)/3.0f;
			
			return z1 > z2;
			
			});


		sf::Time elapsed = timer1.restart();

		float t1 = elapsed.asSeconds();
		for (auto& triToRaster : trigs2Draw)
		{
			// Clip triangles against all four screen edges, this could yield
			// a bunch of triangles, so create a queue that we traverse to 
			//  ensure we only test new triangles generated against planes
			KTriangle clipped[2];
			std::list<KTriangle> listTriangles;

			// Add initial triangle
			listTriangles.push_back(triToRaster);
			int nNewTriangles = 1;

			for (int p = 0; p < 4; p++)
			{
				int nTrisToAdd = 0;
				while (nNewTriangles > 0)
				{
					// Take triangle from front of queue
					KTriangle test = listTriangles.front();
					listTriangles.pop_front();
					nNewTriangles--;

					// Clip it against a plane. We only need to test each 
					// subsequent plane, against subsequent new triangles
					// as all triangles after a plane clip are guaranteed
					// to lie on the inside of the plane. I like how this
					// comment is almost completely and utterly justified
					switch (p)
					{
					case 0:	nTrisToAdd = clippedTriangle({ 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
					case 1:	nTrisToAdd = clippedTriangle({ 0.0f, (float)winHeight -1, 0.0f }, { 0.0f, -1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
					case 2:	nTrisToAdd = clippedTriangle({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
					case 3:	nTrisToAdd = clippedTriangle({ (float)winWidth -1, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
					}

					// Clipping may yield a variable number of triangles, so
					// add these new ones to the back of the queue for subsequent
					// clipping against next planes
					for (int w = 0; w < nTrisToAdd; w++)
						listTriangles.push_back(clipped[w]);
				}
				nNewTriangles = listTriangles.size();
			}


			for (auto projTri : listTriangles) {
				float dp = projTri.lumFactor;
				//fillTriangle(projTri, sf::Color::White);
				int col = (int)(255 * dp);
				taster.addNewTriangle(projTri, sf::Color(col, col, col));
				//fillTrinagle(projTri.p[0].x, projTri.p[0].y, projTri.p[1].x, projTri.p[1].y, projTri.p[2].x, projTri.p[2].y, (int)(255 * dp), (int)(255 * dp), (int)(255 * dp));
				//fillTrinagle(projTri.p[0].x, projTri.p[0].y, projTri.p[1].x, projTri.p[1].y, projTri.p[2].x, projTri.p[2].y, (int)(255 ), (int)(255 ), (int)(255 ));
				//drawTriangle(projTri.p[0].x, projTri.p[0].y, projTri.p[1].x, projTri.p[1].y, projTri.p[2].x, projTri.p[2].y, BLACK);
			}

			

		}

		elapsed = timer1.restart();

		float t2 = elapsed.asSeconds();


		if (sf::Keyboard::isKeyPressed(sf::Keyboard::T))
		{
			// left key is pressed: move our character
			std::cout << "Calculations time is :" << t1 << std::endl << "Drawing time is :" << t2 << std::endl << "Trigs to draw are : " << trigs2Draw.size() << std::endl;
		}

		window.draw(taster);
		return true;
	}

	void fillTriangle(KTriangle tri, sf::Color col,std::vector<sf::Vertex> pixelBuffer) {


		int y1 = (int)tri.p[0].y;
		int x1 = (int)tri.p[0].x;
		int y2 = (int)tri.p[1].y;
		int x2 = (int)tri.p[1].x;
		int y3 = (int)tri.p[2].y;
		int x3 = (int)tri.p[2].x;


		// Order triagnles 


		if (y2 < y1)
		{
			std::swap(y1, y2);
			std::swap(x1, x2);

		}

		if (y3 < y1)
		{
			std::swap(y1, y3);
			std::swap(x1, x3);

		}

		if (y3 < y2)
		{
			std::swap(y2, y3);
			std::swap(x2, x3);

		}

		int dx21 = x2 - x1;
		int dx32 = x3 - x2;
		int dx13 = x1 - x3;

		int dy21 = y2 - y1;
		int dy32 = y3 - y2;
		int dy13 = y1 - y3;


		// Get slope of each triagnle 
		float inv_grad21 = ((float)dx21) / (float)dy21;
		float inv_grad32 = ((float)dx32) / (float)dy32;
		float inv_grad13 = ((float)dx13) / (float)dy13;



		for (int y = y1 + 1;y < y2;y++) {


			int x_start = x1 + (int)((y - y1) * inv_grad21);
			int x_finish = x1 + (int)((y - y1) * inv_grad13);

			if (x_start > x_finish) std::swap(x_start, x_finish);

			for (int x = x_start;x < x_finish;x++)
				if (x >= 0 && x < winWidth && y >= 0 && y < winHeight) {
					pixelBuffer[x + y * winWidth].color = col;
				}
				

		}


		for (int y = y2;y < y3;y++) {


			int x_start = x2 + (int)((y - y2) * inv_grad32);
			int x_finish = x1 + (int)((y - y1) * inv_grad13);

			if (x_start > x_finish) std::swap(x_start, x_finish);

			for (int x = x_start;x < x_finish;x++)
				if (x >= 0 && x < winWidth && y >= 0 && y < winHeight) {
					pixelBuffer[x + y * winWidth].color = col;
				}
				


		}
	}
	


	int clippedTriangle(vec3d plane_p, vec3d plane_n, KTriangle& tri_in, KTriangle& tri_out1, KTriangle& tri_out2) {


		int nPointsInside = 0;
		int nPointsOutside = 0;

		vec3d* outside_points[3];
		vec3d* inside_points[3];

		// For each point in the triangle check if it lies on the inside of the Plane 
		// Update the appropriate counter and store the point's address in the appropriate array.
		if (isPointInsideOfPlane(plane_p, plane_n, tri_in.p[0]))
			inside_points[nPointsInside++] = &tri_in.p[0];
		else
			outside_points[nPointsOutside++] = &tri_in.p[0];

		if (isPointInsideOfPlane(plane_p, plane_n, tri_in.p[1]))
			inside_points[nPointsInside++] = &tri_in.p[1];
		else
			outside_points[nPointsOutside++] = &tri_in.p[1];

		if (isPointInsideOfPlane(plane_p, plane_n, tri_in.p[2]))
			inside_points[nPointsInside++] = &tri_in.p[2];
		else
			outside_points[nPointsOutside++] = &tri_in.p[2];



		if (nPointsInside == 0){
		
			// Return 0 meaning no triangle is valid for drawing
			
			return 0;
		}
			
		if (nPointsInside == 3) {

			// Return 1 meaning that the first out triangle is valid
			// The out triangle must be the same as the input so we just copy the input
			// before returning 

			tri_out1 = tri_in;

			return 1;
		}
		if (nPointsInside == 1) {


			// Return 1 meaning that the first out triangle is valid
			// The out triagnle has the inside vertex plus twe new ones at the inersection points

			tri_out1.p[0] = *inside_points[0];

			tri_out1.p[1] = Vector_IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[0]);
			tri_out1.p[2] = Vector_IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[1]);

			return 1; // Return the newly formed single triangle


		}
		if (nPointsInside == 1) {

			// Return 2 meaning that both out triangles are valid
			
			// First triangke constits of the two inside vertexes plus the first intersection point 
			tri_out1.p[0] = *inside_points[0];
			tri_out1.p[1] = *inside_points[1];
			tri_out1.p[2] = Vector_IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[0]);

			// Second triangle consists of the second inside point plus the two intersection points
			tri_out1.p[0] = *inside_points[1];
			tri_out1.p[1] = tri_out1.p[2];
			tri_out1.p[2] = Vector_IntersectPlane(plane_p, plane_n, *inside_points[1], *outside_points[0]);


			return 2;
		}

	}


	vec3d Vector_IntersectPlane(vec3d& plane_p, vec3d& plane_n, vec3d& lineStart, vec3d& lineEnd)
	{
		plane_n = Vector_Normalise(plane_n);
		float plane_d = -dotProduct(plane_n, plane_p);
		float ad = dotProduct(lineStart, plane_n);
		float bd = dotProduct(lineEnd, plane_n);
		float t = (-plane_d - ad) / (bd - ad);
		vec3d lineStartToEnd = vector_Sub(lineEnd, lineStart);
		vec3d lineToIntersect = Vector_Mul(lineStartToEnd, t);
		return vector_Add(lineStart, lineToIntersect);
	}


	bool isPointInsideOfPlane(vec3d plane_p, vec3d plane_n,vec3d P) {


		vec3d tmp = vector_Sub(P, plane_p);

		return (dotProduct(tmp, plane_n) >= 0);

	}

	float Vector_Length(vec3d& v)
	{
		return sqrtf(dotProduct(v, v));
	}

	vec3d Vector_Normalise(vec3d& v)
	{
		float l = Vector_Length(v);
		return { v.x / l, v.y / l, v.z / l };
	}

	vec3d Vector_Mul(vec3d& a, float t) {
		vec3d io;
		io.x = a.x*t;
		io.y = a.y*t;
		io.z = a.z*t;
		return io;
	}

	void vector_Mult(vec3d& io, float a) {

		io.x *= a;
		io.y *= a;
		io.z *= a;
	}


	void vector_Add_Acc(vec3d& o, vec3d& i) {

		o.x += i.x;
		o.y += i.y;
		o.z += i.z;
	}


	void vector_Sub_Acc(vec3d& o, vec3d& i) {

		o.x -= i.x;
		o.y -= i.y;
		o.z -= i.z;
	}


	vec3d vector_Add(vec3d& v1, vec3d& v2) {

		vec3d out;

		out.x = v1.x + v2.x;out.y = v1.y + v2.y;out.z = v1.z + v2.z;
		return out;
	}


	vec3d vector_Sub(vec3d& v1, vec3d& v2) {

		vec3d out;

		out.x = v1.x - v2.x;out.y = v1.y - v2.y;out.z = v1.z - v2.z;
		return out;
	}


	mat4x4 CamMat(vec3d& P, float theta) {

		mat4x4 res;

		setMatRotY(theta,res);

		res.m[0][3] = P.x;
		res.m[1][3] = P.y;
		res.m[2][3] = P.z;

		return res;
	}

	mat4x4 invCamMat(vec3d& P,float theta) {


		mat4x4 res;
		setMatRotY(-theta, res);
		vec3d P_transf;

		MultMatVec(P, P_transf, res);

		res.m[0][3] = -P_transf.x;
		res.m[1][3] = -P_transf.y;
		res.m[2][3] = -P_transf.z;

		return res;
	}

	

	void setMatProj(float fAspecyRatio, float fov_coef, float fNear, float fFar,mat4x4 &A) {

		matProj.m[0][0] = fAspecyRatio * fov_coef;
		matProj.m[1][1] = fov_coef;
		matProj.m[2][2] = fFar / (fFar - fNear);
		matProj.m[2][3] = -fNear * fFar / (fFar - fNear);
		matProj.m[3][2] = 1;

	}
	
	void setMatRotX(float theta,mat4x4 &matRotX) {
		matRotX.m[0][0] = 1;
		matRotX.m[1][1] = cos( theta);
		matRotX.m[1][2] = -sin( theta);
		matRotX.m[2][1] = sin( theta);
		matRotX.m[2][2] = cos( theta);
		matRotX.m[3][3] = 1;
	}

	void setMatRotZ(float theta, mat4x4 &matRotZ) {
		matRotZ.m[2][2] = 1;
		matRotZ.m[0][0] = cos(0.5f * theta);
		matRotZ.m[0][1] = -sin(0.5f * theta);
		matRotZ.m[1][0] = sin(0.5f * theta);
		matRotZ.m[1][1] = cos(0.5f * theta);
		matRotZ.m[3][3] = 1;
	}

	void setMatRotY(float theta, mat4x4 &matRotY) {
		matRotY.m[1][1] = 1;
		matRotY.m[0][0] = cos(0.5f * theta);
		matRotY.m[2][0] = -sin(0.5f * theta);
		matRotY.m[0][2] = sin(0.5f * theta);
		matRotY.m[2][2] = cos(0.5f * theta);
		matRotY.m[3][3] = 1;
	}

	void MultMatVec(const vec3d& i, vec3d& o, const  mat4x4& A) {


		o.x = A.m[0][0] * i.x + A.m[0][1] * i.y + A.m[0][2] * i.z + A.m[0][3];
		o.y = A.m[1][0] * i.x + A.m[1][1] * i.y + A.m[1][2] * i.z + A.m[1][3];
		o.z = A.m[2][0] * i.x + A.m[2][1] * i.y + A.m[2][2] * i.z + A.m[2][3];
		
	}

	void MultMatMat(mat4x4& A, mat4x4& B, mat4x4& C) {


		for(int i =0;i<4;i++)
			for (int j = 0;j < 4;j++) {
				C.m[i][j] = 0.0f;
				for (int k = 0;k < 4;k++)
					C.m[i][j] += A.m[i][k] * B.m[k][j]; 
			}
	}

	void setMatTrans(vec3d& P, mat4x4 &A) {

		A.m[0][0] = 1;
		A.m[1][1] = 1;
		A.m[2][2] = 1;
		A.m[3][3] = 1;

		A.m[0][3] = P.x;
		A.m[1][3] = P.y;
		A.m[2][3] = P.z;

	}

	void getVecFromPoints(vec3d& p1, vec3d& p2, vec3d& po) {


		po.x = p2.x - p1.x;
		po.y = p2.y - p1.y;
		po.z = p2.z - p1.z;
	}
	vec3d getTriNormal(KTriangle& tri) {


		vec3d v1, v2;

		v1.x = tri.p[1].x - tri.p[0].x;
		v1.y = tri.p[1].y - tri.p[0].y;
		v1.z = tri.p[1].z - tri.p[0].z;

		v2.x = tri.p[2].x - tri.p[0].x;
		v2.y = tri.p[2].y - tri.p[0].y;
		v2.z = tri.p[2].z - tri.p[0].z;


		vec3d normal;  crossProduct(v1, v2, normal);
		
		float norm = sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);


		normal.x /= norm; normal.y /= norm; normal.z /= norm;
		return normal;

	}

	float dotProduct(vec3d& v1, vec3d& v2) {

		return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
	}
	void crossProduct(vec3d& v1, vec3d& v2, vec3d& o) {

		o.x = v1.y * v2.z - v1.z * v2.y;
		o.y = v1.z * v2.x - v1.x * v2.z;
		o.z = v1.x * v2.y - v1.y * v2.x;

	}
	void applyProjOpp(const vec3d& i, vec3d& o, const  mat4x4& A) {


		o.x = A.m[0][0] * i.x + A.m[0][1] * i.y + A.m[0][2] * i.z + A.m[0][3];
		o.y = A.m[1][0] * i.x + A.m[1][1] * i.y + A.m[1][2] * i.z + A.m[1][3];
		o.z = A.m[2][0] * i.x + A.m[2][1] * i.y + A.m[2][2] * i.z + A.m[2][3];
		float w = A.m[3][0] * i.x + A.m[3][1] * i.y + A.m[3][2] * i.z + A.m[3][3];

		if (w != 0) {
			o.x /= w;
			o.y /= w;
			o.z /= w;
		}
	}
	void scaleIntoView(vec3d& i) {

		i.x += 1; i.y += 1;
		i.x *= winWidth/2; i.y *= winHeight/2;

	}

	

// These Functions draw/Fill Circles 

	void drawCircle(float x, float y, float R) {

		sf::CircleShape shape;
		// Set Radius
		shape.setRadius(R);
		
		// Define Center as the Basic Point
		shape.setOrigin(R, R);

		// draw an empty circle at position
		shape.setPosition(x, y);
		shape.setFillColor(sf::Color::Black);
		shape.setOutlineThickness(1.f);
		shape.setOutlineColor(sf::Color::White);
		window.draw(shape);
	}
	void drawCircle(float x, float y, float R, int col) {

		sf::CircleShape shape;
		// Set Radius
		shape.setRadius(R);

		// Define Center as the Basic Point
		shape.setOrigin(R, R);

		// Draw an empty circle at position
		shape.setPosition(x, y);
		shape.setFillColor(sf::Color::Black);
		shape.setOutlineThickness(1.f);

		

		// Set Line Color

		shape.setOutlineColor(getColorFromCode(col));

		window.draw(shape);
	}
	void drawCircle(float x, float y, float R, int r, int g, int b) {

		sf::CircleShape shape;
		// Set Radius
		shape.setRadius(R);

		// Define Center as the Basic Point
		shape.setOrigin(R, R);

		// Draw an empty circle at position
		shape.setPosition(x, y);
		shape.setFillColor(sf::Color::Black);
		shape.setOutlineThickness(1.f);
		shape.setOutlineColor(sf::Color(r,g,b));
		shape.setOutlineThickness(1.f);

		window.draw(shape);
	}
	void fillCircle(float x, float y, float R,int r,int g,int b) {

		sf::CircleShape shape;
		// Set Radius
		shape.setRadius(R);

		// Define Center as the Basic Point
		shape.setOrigin(R, R);

		// Draw an empty circle at position
		shape.setPosition(x, y);
		shape.setFillColor(sf::Color(r,g,b));
		shape.setOutlineThickness(1.f);

		window.draw(shape);
	}
	void fillCircle(float x, float y, float R, int col) {

		sf::CircleShape shape;
		// Set Radius
		shape.setRadius(R);

		// Define Center as the Basic Point
		shape.setOrigin(R, R);

		// Draw an empty circle at position
		shape.setPosition(x, y);
		
		switch (col) {
			case WHITE:
				shape.setFillColor(sf::Color::White);
				break;
			case BLACK:
				shape.setFillColor(sf::Color::Black);
				break;
			case BLUE:
				shape.setFillColor(sf::Color::Blue);
				break;
			case RED:
				shape.setFillColor(sf::Color::Red);
				break;
			case GREEN:
				shape.setFillColor(sf::Color::Green);
				break;
			case YELLOW:
				shape.setFillColor(sf::Color::Yellow);
				break;
			case CYAN:
				shape.setFillColor(sf::Color::Cyan);
				break;
			case MAGNETA:
				shape.setFillColor(sf::Color::Magenta);
				break;
			case TRANSPARENT:
				shape.setFillColor(sf::Color::Transparent);
				break;
			default:
				shape.setFillColor(sf::Color::White);
		}


		window.draw(shape);
	}


// These Functions Draw/Fill Triangles

	void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3,int col) {


		drawLine(x1, y1, x2, y2, col);
		drawLine(x2, y2, x3, y3, col);
		drawLine(x3, y3, x1, y1, col);

	}
	void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, int r,int g,int b) {


		drawLine(x1, y1, x2, y2, r,g,b);
		drawLine(x2, y2, x3, y3, r,g,b);
		drawLine(x3, y3, x1, y1, r,g,b);

	}
	void fillTrinagle(float x1, float y1, float x2, float y2, float x3, float y3, int col) {

		sf::Vertex vertices[3] =
		{
			sf::Vertex(sf::Vector2f(x1, y1), getColorFromCode(col)),
			sf::Vertex(sf::Vector2f(x2,y2), getColorFromCode(col)),
			sf::Vertex(sf::Vector2f(x3,y3), getColorFromCode(col))
		};

		window.draw(vertices, 3, sf::Triangles);

	}
	void fillTrinagle(float x1, float y1, float x2, float y2, float x3, float y3, int r, int g, int b) {

		sf::Vertex vertices[3] =
		{
			sf::Vertex(sf::Vector2f(x1,y1), sf::Color(r,g,b)),
			sf::Vertex(sf::Vector2f(x2,y2), sf::Color(r,g,b)),
			sf::Vertex(sf::Vector2f(x3,y3), sf::Color(r,g,b))
		};

		window.draw(vertices, 3, sf::Triangles);

	}

// These Functions Draw//Fill Quads
	void drawQuad(float x1, float y1, float x2, float y2, float x3, float y3,float x4,float y4, int col) {
		drawLine(x1, y1, x2, y2, col);
		drawLine(x2, y2, x3, y3, col);
		drawLine(x3, y3, x4, y4, col);
		drawLine(x4, y4, x1, y1, col);
	}
	void drawQuad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, int r,int g,int b) {
		drawLine(x1, y1, x2, y2, r,g,b);
		drawLine(x2, y2, x3, y3, r, g, b);
		drawLine(x3, y3, x4, y4, r, g, b);
		drawLine(x4, y4, x1, y1, r, g, b);
	}
	void fillQuad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, int col) {
		sf::Vertex vertices[4] =
		{
			sf::Vertex(sf::Vector2f(x1,y1), getColorFromCode(col)),
			sf::Vertex(sf::Vector2f(x2,y2), getColorFromCode(col)),
			sf::Vertex(sf::Vector2f(x3,y3), getColorFromCode(col)),
			sf::Vertex(sf::Vector2f(x4,y4), getColorFromCode(col))
		};

		window.draw(vertices, 4, sf::Quads);

	}
	void fillQuad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, int r,int g,int b) {
		sf::Vertex vertices[4] =
		{
			sf::Vertex(sf::Vector2f(x1,y1), sf::Color(r,g,b)),
			sf::Vertex(sf::Vector2f(x2,y2), sf::Color(r,g,b)),
			sf::Vertex(sf::Vector2f(x3,y3), sf::Color(r,g,b)),
			sf::Vertex(sf::Vector2f(x4,y4), sf::Color(r,g,b))
		};

		window.draw(vertices, 4, sf::Quads);

	}


// Draw Lines

	void drawLine(float x1,float y1,float x2, float y2, int col) {

		sf::Vertex vertices[2] =
		{
			sf::Vertex(sf::Vector2f(x1, y1), getColorFromCode(col)),
			sf::Vertex(sf::Vector2f(x2,y2), getColorFromCode(col))
		};

		window.draw(vertices, 2, sf::Lines);
	}
	void drawLine(float x1, float y1, float x2, float y2, int r,int g,int b) {


		sf::Vertex vertices[2] =
		{
			sf::Vertex(sf::Vector2f(x1, y1), sf::Color(r,g,b)),
			sf::Vertex(sf::Vector2f(x2,y2), sf::Color(r,g,b))
		};

		window.draw(vertices, 2, sf::Lines);

	}


};




int main()
{
	
	KritsGameEngine eng;

	eng.createWin(1024,800,"Krits Engine!");
	if (eng.begin())
		return 1;

	return 0;
}