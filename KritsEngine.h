#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <list>
#include "vec3.h"
#include "Mat4.h"
#include "KCamera.h"
#include "KritsSimulator.h"
#include "vecMat3.h"

#define GREEN 0
#define BLUE 1
#define RED 2
#define YELLOW 3
#define BLACK 4
#define WHITE 5
#define CYAN 6
#define MAGNETA 7
#define TRANSPARENT 8




struct KTriangle {
	vec3 p[3];
	float lumFactor = 1.0f;

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

	void addNewTriangle(KTriangle tri, sf::Color col) {


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

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
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



struct mesh{

	std::vector<KTriangle> tris;
	Mat4 worldTransform;



	bool readObjFile(std::string fName) {



		std::ifstream f(fName);
		if (!f.is_open())
			return false;

		std::vector<vec3> verts;

		while (!f.eof()) {

			char line[128];
			f.getline(line, 128);

			std::stringstream s;
			s << line;

			char junk;

			if (line[0] == 'v') {

				vec3 v;
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



class KritsEngine {

private:
	sf::RenderWindow window;
	int winWidth = 0;
	int winHeight = 0;

	KritsSimulator sim;
	std::vector<mesh> parts;
	Mat4 matProj;
	float theta = 0.0f;
	
	std::string Title;

	KritsTriangles taster;
	KCamera cam;

	Mat4 TW0, T1M, T2M;

public:



	void createWin(int w, int h, const char* msg) {

		winWidth = w;
		winHeight = h;
		window.create(sf::VideoMode(winWidth, winHeight), msg);
		Title = std::string(msg);





	}

	int begin() {


		if (!KritsSetUp()) return 1;
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
			window.clear(sf::Color::Cyan);

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
			title = Title + std::string("\t FPS = ") + s;
			window.setTitle(title);
			window.display();



		}

		return 0;
	}





	bool KritsSetUp() {

		//if (!meshCube.readObjFile("mountains.obj"))

		mesh part1,part2;

		parts.push_back(part1);
		parts.push_back(part2);

		for (int i = 0;i < parts.size();i++) {
			//parts[i].worldTransform.setTransMat({ 0.0f,0.0f,3.0f });
			parts[i].worldTransform.setIdentity();

			if (!parts[i].readObjFile("arm1.obj"))
			{
				std::cout << "Couldn't open obj File!" << std::endl;
				return false;

			}
		}

		float fNear = 0.1f;
		float fFar = 1000.0f;

		float fov = 90.0f; // Degrees

		float fAspecyRatio = (float)winHeight / winWidth;

		float fov_coef = (float)1.0 / (float)tan(fov * 0.5 * 3.14159f / 180.0f);


		//setMatProj(fAspecyRatio, fov_coef, fNear, fFar, matProj);
		matProj.setProjMat(fov, fAspecyRatio, fNear, fFar);

		taster.setUp();


		Mat4 tmp1, tmp2;

		TW0.setIdentity();
		TW0.setTransPart({ 0.0f,0.0f,6.0f });

		TW0.m[0][0] = -1;
		TW0.m[2][2] = -1;


		T1M.setTransMat({ sim.L1 *0.5f ,0.0f,0.0f });
		T2M.setTransMat({ sim.L2 *0.5f ,0.0f,0.0f });

		return true;
	}







	bool drawFrame(float  fElapsedTime) {

		taster.clear();
		

		//theta += 0.2f*fElapsedTime;

		vec3 light_dir = { 0.0f,1.0f,-1.0f };		
		
		//worldTransformation.setTransMat({ 0.0f,-20.0f,6.0f }); // Used for mountatin terrain


		sim.simulateStep(fElapsedTime);
		sim.getTransfomarions(TW0, T1M, T2M, parts[0].worldTransform, parts[1].worldTransform);
		GetUserInputForCamera(fElapsedTime);


		for (int i = 0;i < parts.size();i++) {
			
			renderMesh(parts[i], parts[i].worldTransform, light_dir);
		}

		window.draw(taster);

		return true;
	}



	void renderMesh(mesh &meshCube, Mat4& wroldTransformation,vec3 light_dir) {


		

		std::vector<KTriangle> trigs2Draw;

		for (auto tri : meshCube.tris) {

			KTriangle transTri, projTri, camTri;

			// Step 0.
			// Copy current triangle from model 
			transTri = tri;

			// Step 1.
			// Apply the given world trnasformation to each point in the triangle
			wroldTransformation.vecMatMult(tri.p[0], transTri.p[0]);
			wroldTransformation.vecMatMult(tri.p[1], transTri.p[1]);
			wroldTransformation.vecMatMult(tri.p[2], transTri.p[2]);


			// Step 2.
			// Decide which triangles to draw

			// calculate triangle's normal
			vec3 normal = getTriNormal(transTri);

			// Check if triagnle is visible by the camera in world coordinates

			// Get Vectort from Camera to Triangle  
			vec3 camRay;
			vec3 cam_pos;

			cam_pos = cam.getCamPos();

			//getVecFromPoints(cam_pos, transTri.p[0], camRay);

			camRay = transTri.p[0] - cam_pos;



			if (camRay.dot_product(normal) < 0.0f) {

				// Make sure lighting Direction is normalized

				// Step 3.
				// Calculate how mych each drawn triangle is illuminated
				light_dir.normalize();

				float dp = light_dir.dot_product(normal);


				// Step 4. 
				// Invert camera orientation Matrix to get view Matrix
				// Apply the transormation with view matrix to get the coordinates of each point as viewed by the camera
				Mat4 invCam;

				cam.getViewMatrix(invCam);

				invCam.vecMatMult(transTri.p[0], camTri.p[0]);
				invCam.vecMatMult(transTri.p[1], camTri.p[1]);
				invCam.vecMatMult(transTri.p[2], camTri.p[2]);

				// Step 5.
				// Clip triangles in camera view - camera coordinates - world space 


				int nClippedTriangles = 0;
				KTriangle clipped[2];
				nClippedTriangles = clippedTriangle({ 0.0f, 0.0f, 0.1f }, { 0.0f, 0.0f, 1.0f }, camTri, clipped[0], clipped[1]);


				for (int n = 0;n < nClippedTriangles;n++) {


					//Step 6.
					// Aply Projection and invert x and y poses and then scale into view each point 
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

		// Step 7.
		// Sort by decreasing z order such that the closet triangles will be drawn last
		sort(trigs2Draw.begin(), trigs2Draw.end(), [](KTriangle & t1, KTriangle & t2) {

			float z1 = (t1.p[0].z + t1.p[1].z + t1.p[2].z) / 3.0f;
			float z2 = (t2.p[0].z + t2.p[1].z + t2.p[2].z) / 3.0f;

			return z1 > z2;

			});





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
					case 1:	nTrisToAdd = clippedTriangle({ 0.0f, (float)winHeight - 1, 0.0f }, { 0.0f, -1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
					case 2:	nTrisToAdd = clippedTriangle({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
					case 3:	nTrisToAdd = clippedTriangle({ (float)winWidth - 1, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
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

				int col = (int)(255 * dp);
				taster.addNewTriangle(projTri, sf::Color(col, col, col));

			}

		}

	}


	void GetUserInputForCamera(float fElapsedTime) {
		//Get User Input


		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			cam.resetCam();

		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			cam.rotateY(fElapsedTime * 2);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			cam.rotateY(-fElapsedTime * 2);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		{
			cam.rotateX(fElapsedTime * 2);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
		{
			cam.rotateX(-fElapsedTime * 2);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
		{
			cam.rotateZ(fElapsedTime * 2);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
		{
			cam.rotateZ(-fElapsedTime * 2);
		}


		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			cam.moveForward(8 * fElapsedTime);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			cam.moveForward(-8 * fElapsedTime);
		}


		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			cam.moveUp(8 * fElapsedTime);
		}


		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			cam.moveUp(-8 * fElapsedTime);
		}


		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			cam.moveRight(8 * fElapsedTime);
		}


		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			cam.moveRight(-8 * fElapsedTime);
		}


	}

	void fillTriangle(KTriangle tri, sf::Color col, std::vector<sf::Vertex> pixelBuffer) {


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



	int clippedTriangle(vec3 plane_p, vec3 plane_n, KTriangle & tri_in, KTriangle & tri_out1, KTriangle & tri_out2) {

		plane_n.normalize();
		int nPointsInside = 0;
		int nPointsOutside = 0;

		vec3* outside_points[3];
		vec3* inside_points[3];

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



		if (nPointsInside == 0) {

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


	vec3 Vector_IntersectPlane(vec3 & plane_p, vec3 & plane_n, vec3 & lineStart, vec3 & lineEnd)
	{
		
		float plane_d = -(plane_n.dot_product(plane_p));
		float ad = lineStart.dot_product(plane_n);
		float bd = lineEnd.dot_product(plane_n);
		float t = (-plane_d - ad) / (bd - ad);
		vec3 lineStartToEnd = lineEnd - lineStart;
		//vec3 lineStartToEnd = lineStart - lineEnd;
		vec3 lineToIntersect = lineStartToEnd * t;
		return (lineStart + lineToIntersect);
		/*
		plane_n = Vector_Normalise(plane_n);
		float plane_d = -dotProduct(plane_n, plane_p);
		float ad = dotProduct(lineStart, plane_n);
		float bd = dotProduct(lineEnd, plane_n);
		float t = (-plane_d - ad) / (bd - ad);
		vec3d lineStartToEnd = vector_Sub(lineEnd, lineStart);
		vec3d lineToIntersect = Vector_Mul(lineStartToEnd, t);
		return vector_Add(lineStart, lineToIntersect);
		*/
	}


	bool isPointInsideOfPlane( vec3& plane_p,vec3& plane_n,vec3& P) {


		vec3 tmp = P - plane_p;

		return (tmp.dot_product(plane_n) >= 0 );

		//return (dotProduct(tmp, plane_n) >= 0);

	}


	void applyProjOpp(const vec3 & i, vec3 & o, const  Mat4 & A) {


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
	void scaleIntoView(vec3 & i) {

		i.x += 1; i.y += 1;
		i.x *= winWidth / 2; i.y *= winHeight / 2;

	}

	void getVecFromPoints(vec3& p1, vec3& p2, vec3& po) {

		po = p2 - p1;

	}
	vec3 getTriNormal(KTriangle& tri) {


		vec3 v1, v2;

		v1.x = tri.p[1].x - tri.p[0].x;
		v1.y = tri.p[1].y - tri.p[0].y;
		v1.z = tri.p[1].z - tri.p[0].z;

		v2.x = tri.p[2].x - tri.p[0].x;
		v2.y = tri.p[2].y - tri.p[0].y;
		v2.z = tri.p[2].z - tri.p[0].z;


		vec3 normal;  //crossProduct(v1, v2, normal);

		normal = v1.cross_product(v2);


		

		normal.normalize();
		return normal;

	}



	sf::Color getColorFromCode1(int col) {
		sf::Color col1;
		switch (col) {
		case WHITE:
			col1 = (sf::Color::White);
			break;
		case BLACK:
			col1 = (sf::Color::Black);
			break;
		case BLUE:
			col1 = (sf::Color::Blue);
			break;
		case RED:
			col1 = (sf::Color::Red);
			break;
		case GREEN:
			col1 = (sf::Color::Green);
			break;
		case YELLOW:
			col1 = (sf::Color::Yellow);
			break;
		case CYAN:
			col1 = (sf::Color::Cyan);
			break;
		case MAGNETA:
			col1 = (sf::Color::Magenta);
			break;
		case TRANSPARENT:
			col1 = (sf::Color::Transparent);
			break;
		default:
			col1 = (sf::Color::White);
		}
		return col1;
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

		shape.setOutlineColor(getColorFromCode1(col));

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
		shape.setOutlineColor(sf::Color(r, g, b));
		shape.setOutlineThickness(1.f);

		window.draw(shape);
	}
	void fillCircle(float x, float y, float R, int r, int g, int b) {

		sf::CircleShape shape;
		// Set Radius
		shape.setRadius(R);

		// Define Center as the Basic Point
		shape.setOrigin(R, R);

		// Draw an empty circle at position
		shape.setPosition(x, y);
		shape.setFillColor(sf::Color(r, g, b));
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

	void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, int col) {


		drawLine(x1, y1, x2, y2, col);
		drawLine(x2, y2, x3, y3, col);
		drawLine(x3, y3, x1, y1, col);

	}
	void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, int r, int g, int b) {


		drawLine(x1, y1, x2, y2, r, g, b);
		drawLine(x2, y2, x3, y3, r, g, b);
		drawLine(x3, y3, x1, y1, r, g, b);

	}
	void fillTrinagle(float x1, float y1, float x2, float y2, float x3, float y3, int col) {

		sf::Vertex vertices[3] =
		{
			sf::Vertex(sf::Vector2f(x1, y1), getColorFromCode1(col)),
			sf::Vertex(sf::Vector2f(x2,y2), getColorFromCode1(col)),
			sf::Vertex(sf::Vector2f(x3,y3), getColorFromCode1(col))
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
	void drawQuad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, int col) {
		drawLine(x1, y1, x2, y2, col);
		drawLine(x2, y2, x3, y3, col);
		drawLine(x3, y3, x4, y4, col);
		drawLine(x4, y4, x1, y1, col);
	}
	void drawQuad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, int r, int g, int b) {
		drawLine(x1, y1, x2, y2, r, g, b);
		drawLine(x2, y2, x3, y3, r, g, b);
		drawLine(x3, y3, x4, y4, r, g, b);
		drawLine(x4, y4, x1, y1, r, g, b);
	}
	void fillQuad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, int col) {
		sf::Vertex vertices[4] =
		{
			sf::Vertex(sf::Vector2f(x1,y1), getColorFromCode1(col)),
			sf::Vertex(sf::Vector2f(x2,y2), getColorFromCode1(col)),
			sf::Vertex(sf::Vector2f(x3,y3), getColorFromCode1(col)),
			sf::Vertex(sf::Vector2f(x4,y4), getColorFromCode1(col))
		};

		window.draw(vertices, 4, sf::Quads);

	}
	void fillQuad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, int r, int g, int b) {
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

	void drawLine(float x1, float y1, float x2, float y2, int col) {

		sf::Vertex vertices[2] =
		{
			sf::Vertex(sf::Vector2f(x1, y1), getColorFromCode1(col)),
			sf::Vertex(sf::Vector2f(x2,y2), getColorFromCode1(col))
		};

		window.draw(vertices, 2, sf::Lines);
	}
	void drawLine(float x1, float y1, float x2, float y2, int r, int g, int b) {


		sf::Vertex vertices[2] =
		{
			sf::Vertex(sf::Vector2f(x1, y1), sf::Color(r,g,b)),
			sf::Vertex(sf::Vector2f(x2,y2), sf::Color(r,g,b))
		};

		window.draw(vertices, 2, sf::Lines);

	}


};
