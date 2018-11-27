#include <windows.h>
#include <math.h>
#include <stdio.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
using namespace std;

typedef struct {
	float x;
	float y;
} Point2D_t;
typedef struct {
	int x;
	int y;
} Point2D_i;
typedef struct {
	int x, y, z;
} Point3D_t;
typedef struct {
	float v[3];
} Vector3D_t;
typedef struct {
	float m[3][3];
} matrix3D_t;
typedef struct {
	int m[3][3];
} matrix3D_i;
typedef struct {
	float r;
	float g;
	float b;
} color_t;
typedef struct {
	int numofVertices;
	long int pnt[32];
} face_t;
typedef struct {
	int numofVertices;
	Point3D_t pnt[100];
	int numofFaces;
	face_t fc[32];
} object3D_t;

double sudutRotasi = (float)(0);

matrix3D_t create_identity() {
	matrix3D_t rotate;
	rotate.m[0][0] = 0.0;
	rotate.m[0][1] = 0.0;
	rotate.m[0][2] = 0.0;

	rotate.m[1][0] = 0.0;
	rotate.m[1][1] = 0.0;
	rotate.m[1][2] = 0.0;

	rotate.m[2][0] = 0.0;
	rotate.m[2][1] = 0.0;
	rotate.m[2][1] = 0.0;
	return rotate;
}

matrix3D_t rotasi_x(float teta) {
	matrix3D_t rotate = create_identity();
	rotate.m[0][0] = 1.0;
	rotate.m[0][1] = 0.0;
	rotate.m[0][2] = 0.0;

	rotate.m[1][0] = 0.0;
	rotate.m[1][1] = cos(teta / 57.3);
	rotate.m[1][2] = -sin(teta / 57.3);

	rotate.m[2][0] = 0.0;
	rotate.m[2][1] = sin(teta / 57.3);
	rotate.m[2][2] = cos(teta / 57.3);

	return rotate;
}

matrix3D_t rotasi_y(float teta) {
	matrix3D_t rotate = create_identity();
	rotate.m[0][0] = cos(teta / 57.3);
	rotate.m[0][1] = 0.0;
	rotate.m[0][2] = sin(teta / 57.3);

	rotate.m[1][0] = 0.0;
	rotate.m[1][1] = 1.0;
	rotate.m[1][2] = 0.0;

	rotate.m[2][0] = -sin(teta / 57.3);
	rotate.m[2][1] = 0.0;
	rotate.m[2][2] = cos(teta / 57.3);

	return rotate;
}

matrix3D_t rotasi_z(float teta) {
	matrix3D_t rotate = create_identity();
	rotate.m[0][0] = cos(teta / 57.3);
	rotate.m[0][1] = -sin(teta / 57.3);
	rotate.m[0][2] = 0.0;

	rotate.m[1][0] = sin(teta / 57.3);
	rotate.m[1][1] = cos(teta / 57.3);
	rotate.m[1][2] = 0.0;

	rotate.m[2][0] = 0.0;
	rotate.m[2][1] = 0.0;
	rotate.m[2][2] = 1.0;

	return rotate;
}

Vector3D_t operator +(Vector3D_t a, Vector3D_t b) {
	Vector3D_t c;
	for (int i = 0; i < 3; i++) {
		c.v[i] = a.v[i] + b.v[i];
	} return c;
}

Vector3D_t operator -(Vector3D_t a, Vector3D_t b) {
	Vector3D_t c;
	for (int i = 0; i < 3; i++) {
		c.v[i] = a.v[i] - b.v[i];
	} return c;
}

Vector3D_t operator *(matrix3D_t a, Vector3D_t b) {
	Vector3D_t c; for (int i = 0; i < 3; i++) {
		c.v[i] = 0; for (int j = 0; j < 3; j++) {
			c.v[i] += a.m[i][j] * b.v[j];
		}
	} return c;
}

void create3DObject(object3D_t object) {
	for (int i = 0; i < object.numofFaces; i++) {
		glColor3f(1.0,0.7,0.5);
		glBegin(GL_POLYGON);
		for (int j = 0; j < object.fc[i].numofVertices; j++) {
			int p = object.fc[i].pnt[j];
			float x = object.pnt[p].x;
			float y = object.pnt[p].y;
			glVertex3f(x, y, 0.0);
		}
		glEnd();
	}
}

void createLineObject(object3D_t object) {
	for (int i = 0; i < object.numofFaces; i++) {
		glColor3f(1.0,0,0.5);
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j < object.fc[i].numofVertices; j++) {
			int p = object.fc[i].pnt[j];
			float x = object.pnt[p].x;
			float y = object.pnt[p].y;
			glVertex3f(x, y, 0.0);
		}
		glEnd();
	}
}

void create_kerucut() {	
	glClear(GL_COLOR_BUFFER_BIT);	

	object3D_t Shape = {
		14, {
				{-50,50,50}, {50,50,50}, {50,50,-50}, {-50,50,-50},
				{-50,-50,50}, {50,-50,50}, {50,-50,-50}, {-50,-50,-50},
				{-100,0,0}, {0,0,100}, {100,0,0}, {0,0,-100},
				{0,100,0}, {0,-100,0}
			},
		24, {
				{3,{8,3,0}},{3,{8,0,4}},{3,{8,4,7}},{3,{8,7,3}},
				{3,{9,0,1}},{3,{9,1,5}},{3,{9,5,4}},{3,{9,4,0}},
				{3,{10,1,2}},{3,{10,2,6}},{3,{10,6,5}},{3,{10,5,1}},
				{3,{11,2,3}},{3,{11,3,7}},{3,{11,7,6}},{3,{11,6,2}},
				{3,{12,1,0}},{3,{12,0,3}},{3,{12,3,2}},{3,{12,2,1}},
				{3,{13,4,5}},{3,{13,5,6}},{3,{13,6,7}},{3,{13,7,4}},
			}
	};

	matrix3D_t matrix_X = rotasi_x(20.0);
	matrix3D_t matrix_Y = rotasi_y(sudutRotasi);
	matrix3D_t matrix_Z = rotasi_z(0);
	for (int i = 0; i < Shape.numofVertices; i++) {
		Vector3D_t p;
		p.v[0] = Shape.pnt[i].x;
		p.v[1] = Shape.pnt[i].y;
		p.v[2] = Shape.pnt[i].z;
		p = (matrix_Y)*(p);
		p = (matrix_X)*(p);
		p = (matrix_Z)*(p);
		Shape.pnt[i].x = p.v[0];
		Shape.pnt[i].y = p.v[1];
		Shape.pnt[i].z = p.v[2];
	}
	create3DObject(Shape);
	createLineObject(Shape);

	
	sudutRotasi++;

	if (sudutRotasi >= 360.0)
		sudutRotasi = 0.0;
	glFlush();
}

void timer(int value) {
	glutPostRedisplay();
	glutTimerFunc(10, timer, 0);
}

int main(int iArgc, char** cppArgv) {
	glutInit(&iArgc, cppArgv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(200, 200);
	glutCreateWindow("Dua Kerucut");
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(-200, 200, -200, 200);
	glutDisplayFunc(create_kerucut);
	glutTimerFunc(50, timer, 0);
	glutMainLoop();
	return 0;
}
