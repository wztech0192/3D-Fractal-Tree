/*
	Jacob Barr & Zheng Wei
	CSCI 350 - Final Project
	Fractal Tree Using Truncated Cone
	4/12/2019
*/
#include "GL/glut.h"
#include <math.h>
#include "myObjects.cpp"


//------ global variables and constants------------
GLsizei ww = 512;
GLsizei wh = 512;
GLfloat left, right, bottom, top, near = 5, far = 20;
const float REGION_DIM = 4;

GLfloat fov = 45.0, aspect = 1;
GLfloat theta = 30, phi = 60, rho = 10;
GLfloat dTheta = 5, dPhi = 5, dRho = 0.5;
GLfloat alpha = 0, beta = 0, gama = 0;
GLfloat dAlpha = 5, dBeta = 5, dGama = 5;
GLfloat direction = 1.0;
GLfloat topRadius = 1.25, botRadius = 2, height = 3, slices = 50, stacks = 50;

GLfloat lightPos[] = { 2, 4, 6, 1 };


typedef struct materialStruct {
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat shininess;
} materialStruct;

typedef struct lightingStruct {
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
} lightingStruct;

//----- prototypes ------
void display(void);
void init(void);
void reshape(GLsizei w, GLsizei h);
void keys(unsigned char k, int x, int y);
//void mouse(int, int, int, int);
void idle(void);
void truncatedCone(float topRadius, float botRadius, float height, int slices, int stacks);


materialStruct *currentMaterials;
lightingStruct *currentLighting;

materialStruct Materials = {
	{0.4, 0.2, 0.2, 1.0},
	{0.4, 0.2, 0.2, 1.0},
	{0.5, 0.5, 0.5, 1.0},
	32.0
};

lightingStruct Lighting = {
	{0.0, 0.0, 0.0, 1.0},
	{0.4, 0.2, 0.2, 1.0},
	{1.0, 1.0, 1.0, 1.0}
};

void materials(materialStruct *m)
{
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m->ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m->diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m->specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, m->shininess);
}

void lighting(lightingStruct *li)
{
	glLightfv(GL_LIGHT0, GL_AMBIENT, li->ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, li->diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, li->specular);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(rho*sin(theta*DtoR)*sin(phi*DtoR), rho*cos(phi*DtoR), rho*cos(theta*DtoR)*sin(phi*DtoR), 0, 0, 0, 0, 1, 0);

	//----- rotations -----------------
	glRotatef(alpha, 1, 0, 0);
	glRotatef(beta, 0, 1, 0);
	glRotatef(gama, 0, 0, 1);
	glRotatef(-90, 1, 0, 0);

	//----- the object -----------------
	currentMaterials = &Materials;
	materials(currentMaterials);

	currentLighting = &Lighting;
	lighting(currentLighting);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	truncatedCone(topRadius, botRadius, height, slices, stacks);

	glutSwapBuffers();
	glutPostRedisplay();
}

void init(void)
{
	float ratio = ww * 1.0 / (wh*1.0);
	glClearColor(0.5, 0.5, 0.5, 1.0);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	currentMaterials = &Materials;
	materials(currentMaterials);

	currentLighting = &Lighting;
	lighting(currentLighting);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-2, 2, -2, 2, -10, 10);
	glMatrixMode(GL_MODELVIEW);

	if (ratio >= 1)
	{
		left = -1.0 * REGION_DIM * ratio;
		right = REGION_DIM * ratio;
		bottom = -1.0 * REGION_DIM;
		top = REGION_DIM;
	}
	else
	{
		left = -1.0 * REGION_DIM;
		right = REGION_DIM;
		bottom = -1.0 * REGION_DIM / ratio;
		top = REGION_DIM / ratio;
	}

	glFrustum(left, right, bottom, top, near, far);
	glMatrixMode(GL_MODELVIEW);
}

void reshape(GLsizei w, GLsizei h)
{
	float ratio = w * 1.0 / (h*1.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (ratio >= 1)
	{
		left = -1.0 * REGION_DIM * ratio;
		right = REGION_DIM * ratio;
		bottom = -1.0 * REGION_DIM;
		top = REGION_DIM;
	}
	else
	{
		left = -1.0 * REGION_DIM;
		right = REGION_DIM;
		bottom = -1.0 * REGION_DIM / ratio;
		top = REGION_DIM / ratio;
	}

	glFrustum(left, right, bottom, top, near, far);

	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, w, h);

	ww = w;
	wh = h;
}

void idle()
{
	alpha += 0.05;
	beta += 0.05;
	gama += 0.05;

	if (alpha >= 360)
		alpha -= 360;

	if (beta >= 360)
		beta -= 360;

	if (gama >= 360)
		gama -= 360;

	glutPostRedisplay();
}

/*
void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		alpha += dAlpha * direction;
	}
	else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
	{
		beta += dBeta * direction;
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		gama += dGama * direction;
	}
}
*/

void keys(unsigned char k, int x, int y)
{
	if (k == 'w' || k == 'W')
		direction = 1.0;
	else if (k == 'e' || k == 'E')
		direction = -1.0;
	else if (k == 'i' || k == 'I')
		glutIdleFunc(idle);
	else if (k == 'o' || k == 'O')
		glutIdleFunc(NULL);
	else if (k == '+')
		rho -= dRho;
	else if (k == '-')
		rho += dRho;
	else if (k == 'a' || k == 'A') {
		if (direction == 1.0)
			topRadius += .25;
		else
			topRadius -= .25;
	}
	else if (k == 's' || k == 'S') {
		if (direction == 1.0)
			botRadius += .25;
		else
			botRadius -= .25;
	}
	else if (k == 'd' || k == 'D') {
		if (direction == 1.0)
			height += .25;
		else
			height -= .25;
	}
	else if (k == 'f' || k == 'F') {
		if (direction == 1.0) {
			topRadius += .25;
			botRadius += .25;
		}
		else {
			topRadius -= .25;
			botRadius -= .25;
		}
	}
	else if (k == 'r' || k == 'R')
	{
		topRadius = 1;
		botRadius = 2;
		height = 3;
		near = 5;
		far = 20;
		theta = 30;
		phi = 60;
		rho = 10;
		alpha = 0;
		beta = 0;
		gama = 0;
		direction = 1.0;
		glutIdleFunc(NULL);
	}
}

void specialKeys(int k, int x, int y)
{
	if (k == GLUT_KEY_LEFT)
		theta -= dTheta;

	else if (k == GLUT_KEY_RIGHT)
		theta += dTheta;

	else if (k == GLUT_KEY_UP)
	{
		if (phi > dPhi)
			phi -= dPhi;
		else if (phi == dPhi)
			phi = 0.01;		// to keep the direction vector of the camera
	}
	else if (k == GLUT_KEY_DOWN)
	{
		if (phi <= 180 - dPhi)
			phi += dPhi;
	}
}

void middle_menu(int id)
{

}

void increase_menu(int id)
{
	if (id == 1) { topRadius += .25; }
	else if (id == 2) { botRadius += .25; }
	else if (id == 3) { height += .25; }
	else if (id == 4) { topRadius += .25; botRadius += .25; }
}

void decrease_menu(int id)
{
	if (id == 1) { topRadius -= .25; }
	else if (id == 2) { botRadius -= .25; }
	else if (id == 3) { height -= .25; }
	else if (id == 4) { topRadius -= .25; botRadius -= .25; }
}

int main(int argc, char** argv)
{
	int c_menu, p_menu, f_menu, g_menu;

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // GLUT_DOUBLE work with glutPostRedisplay
	glutInitWindowSize(ww, wh);	// default size 300 by 300
	glutInitWindowPosition(100, 100); // defualt at (0, 0)
	glutCreateWindow("Fractal Tree Using Truncated Cone - Jacob Barr & Zheng Wei");

	c_menu = glutCreateMenu(increase_menu);
	glutAddMenuEntry("Top Radius", 1);
	glutAddMenuEntry("Bottom Radius", 2);
	glutAddMenuEntry("Height", 3);
	glutAddMenuEntry("Top & Bottom Radius", 4);

	p_menu = glutCreateMenu(decrease_menu);
	glutAddMenuEntry("Top Radius", 1);
	glutAddMenuEntry("Bottom Radius", 2);
	glutAddMenuEntry("Height", 3);
	glutAddMenuEntry("Top & Bottom Radius", 4);

	glutCreateMenu(middle_menu);
	glutAddSubMenu("Increase", c_menu);
	glutAddSubMenu("Decrease", p_menu);
	glutAttachMenu(GLUT_MIDDLE_BUTTON);

	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	//glutMouseFunc(mouse);
	glutKeyboardFunc(keys);
	glutSpecialFunc(specialKeys);
	glutIdleFunc(NULL);
	glutMainLoop();
	return 1;
}


// ------------------------------------ this method goes into the MyObjects File--------------------------------------------

void truncatedCone(float topRadius, float botRadius, float height, int slices, int stacks)
{
	float z1, z2;
	float dz = height / stacks;
	int dTheta = (int)(360.0 / slices);
	float x, y;
	float p1[3], p2[3], p3[3], p4[3];
	float n1[3], n2[3], n3[3], n4[3];
	float xAxis1, yAxis1, xAxis2, yAxis2;

	glBegin(GL_QUAD_STRIP);

	/*for (int i = 0; i < stacks; i++)*/

	for (int theta = 0; theta <= 360; theta += 5) {

		glVertex3f(botRadius*cos(theta*DtoR), botRadius*sin(theta*DtoR), 0);
		glVertex3f(topRadius*cos(theta*DtoR), topRadius*sin(theta*DtoR), height);

		//for (int theta = 0; theta <= 360; theta += 5){

		//	//z1 = theta * dz;
		//	xAxis1 = topRadius;
		//	yAxis1 = botRadius;

		//	//z2 = z1 + dz;
		//	xAxis2 = topRadius;
		//	yAxis2 = botRadius;

		//	p1[0] = xAxis1 * cos(theta*DtoR);
		//	p1[1] = yAxis1 * sin(theta*DtoR);
		//	p1[2] = 0;
		//	n1[0] = 2 * p1[0] / topRadius / topRadius;
		//	n1[1] = 2 * p1[1] / botRadius / botRadius;
		//	n1[2] = -1.0 / height;
		//	normalize(n1);

		//	p2[0] = xAxis1 * cos((theta + dTheta)*DtoR);
		//	p2[1] = yAxis1 * sin((theta + dTheta)*DtoR);
		//	p2[2] = 0;
		//	n2[0] = 2 * p2[0] / topRadius / topRadius;
		//	n2[1] = 2 * p2[1] / botRadius / botRadius;
		//	n2[2] = -1.0 / height;
		//	normalize(n2);

		//	p3[0] = xAxis2 * cos((theta + dTheta)*DtoR);
		//	p3[1] = yAxis2 * sin((theta + dTheta)*DtoR);
		//	p3[2] = height;
		//	n3[0] = 2 * p3[0] / topRadius / topRadius;
		//	n3[1] = 2 * p3[1] / botRadius / botRadius;
		//	n3[2] = -1.0 / height;
		//	normalize(n3);

		//	p4[0] = xAxis2 * cos(theta*DtoR);
		//	p4[1] = yAxis2 * sin(theta*DtoR);
		//	p4[2] = height;
		//	n4[0] = 2 * p4[0] / topRadius / topRadius;
		//	n4[1] = 2 * p4[1] / botRadius / botRadius;
		//	n4[2] = -1.0 / height;
		//	normalize(n4);

		//	glBegin(GL_POLYGON);
		//	glNormal3fv(n1);
		//	glVertex3fv(p1);

		//	glNormal3fv(n2);
		//	glVertex3fv(p2);

		//	glNormal3fv(n3);
		//	glVertex3fv(p3);

		//	glNormal3fv(n4);
		//	glVertex3fv(p4);
		//	glEnd();

		//}

	}

	glEnd();
}
