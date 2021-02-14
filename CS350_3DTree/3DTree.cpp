/*
	Jacob Barr & Zheng Wei
	CSCI 350 - Final Project
	Fractal Tree Using Truncated Cone
	4/12/2019
*/

#include <math.h>
#include "iostream"

#include <GL\glut.h>


//------ global variables and constants------------
GLsizei ww = 1000;
GLsizei wh = 800;
const float DtoR = 0.017453;
GLfloat left, right, bottom, top, near = 4, far = 2000;
GLfloat cameraX = 0;
GLfloat cameraY = 0;
const float REGION_DIM = 4;
GLfloat fov = 45.0, aspect = 3;
GLfloat theta = 30, phi = 60, rho = 100;
GLfloat dTheta = 5, dPhi = 5, dRho = 1;
GLfloat worldX = -90;



// tree param
GLfloat root_height = 20, sizeRatio = 5, separate = 0, target_separate = 0, leafSize = 8, sunRise = 1;
int  angle = 25, slices = 8, treeLevel = 8, extraTree = 0;
bool startTimer = false, moreBranch = false, hasLeaf = true, hasLand=true, night=false;
GLfloat lightPos[] = { 0, 0, 200, 0};

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


materialStruct Materials = {
	{0.8, 0.7, 0.7, 1.0},
	{0.4, 0.2, 0.2, 1.0},
	{0.2, 0.1, 0.1, 1.0},
	32.0
};

materialStruct LandMaterial = {
	{0.3, 0.8, 0.2, 0.5},
	{0.2, 0.5, 0.2, 0.5},
	{0.2, 0.5, 0.2, 0.5},
	50
};

materialStruct LeafMaterial = {
	{0.3, 0.8, 0.2, 0.5},
	{0.2, 0.5, 0.2, 0.5},
	{0.2, 0.5, 0.2, 0.5},
	50
};

materialStruct SunMaterial = {
	{1, 1, 0.2, 0.5},
	{1, 1, 0.2, 0.5},
	{0.2, 0.5, 0.2, 0.5},
	100
};

materialStruct MoonMaterial = {
	{1, 1, 1, 0.5},
	{1, 0.9, 0.9, 0.5},
	{0.2, 0.5, 0.2, 0.5},
	100
};

lightingStruct Lighting = {
	{0.05, 0.05, 0.0, 0.5},
	{0.4, 0.4, 0.2, 1.0},
	{0.8, 0.7, 1.0, 0.5}
};

lightingStruct SunLighting = {
	{1, 1, 1, 1},
	{1, 1, 1, 0.6},
	{1, 1, 1, 0.5}
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



//----- prototypes ----------
void timer(int v);
void display(void);
void init(void);
void reshape(GLsizei w, GLsizei h);
void keys(unsigned char k, int x, int y);
void mouse(int, int, int, int);
float truncatedCone(float botRadius, float height);
void drawTree(int level, float botRadius, float height, float angle, int rY, int rX);
void drawSquare(int size, materialStruct *material);
void drawMoreTree(int num, float radius);
void drawStatus();
void drawStar(int v, materialStruct *m);
void seedPos();
void paraPos();
void spikePos();
void rocketPos();
void placement_menu(int i);
void right_menu(int i);
void toggle_menu(int i);
void decorate_menu(int i);


// branch shape, return top circle radius
float truncatedCone(float botRadius, float height)
{
	float topRadius = botRadius * 0.75;
	glBegin(GL_QUAD_STRIP);
	int dTheta = 360 / slices;
	for (int theta = 0; theta <= 360; theta += dTheta) {
		glVertex3f(botRadius*cos(theta*DtoR), botRadius*sin(theta*DtoR), 0);
		glVertex3f(topRadius*cos(theta*DtoR), topRadius*sin(theta*DtoR), height);
	}
	glEnd();
	return topRadius;
}


// recursion method to draw tree
void drawTree(int level, float botRadius, float height, float a, int rX, int rY) {

	glPushMatrix();
	glRotatef(a, rX, rY, 0);

		materials(&Materials);
		glColor3f(0.6, 0.44, 0.24);
		botRadius = truncatedCone(botRadius, height);

	if (level-- > 0) {
		glTranslatef(0, 0, height+separate);
		height *= 0.8;
		if (level % 2 == 0) {	
			drawTree(level, botRadius, height, angle, 0, 1);
			drawTree(level, botRadius, height, -angle, 0, 1);
		}
		else {
			drawTree(level, botRadius, height, angle, 1, 0);
			drawTree(level, botRadius, height, -angle, 1, 0);
		}
		if (moreBranch)drawTree(level, botRadius, height, angle, 1, 1);
	}else if (hasLeaf ) {
		glColor3f(0, 1, 0);
		materials(&LeafMaterial);
	 truncatedCone(botRadius*leafSize, height);
	}
	glPopMatrix();
}

void drawSquare(int size, materialStruct *material) {
	materials(material);
	glBegin(GL_POLYGON);
	glVertex2f(-size, -size);
	glVertex2f(-size, size);
	glVertex2f(size, size);
	glVertex2f(size, -size);
	glEnd();
}

void drawStar(int v, materialStruct *m) {
	glPushMatrix();
	glRotated((sunRise-v)*180, 1, 0, 0);
	glColor3f(1, 1, 0);
	glTranslatef(0, 200, 1000);
	lighting(&SunLighting);
	drawSquare(50, m);
	glPopMatrix();
}

void drawMoreTree(int num, float radius) {
	
	int range;
	for (int i = 1; i <= extraTree; i++) {
		range = (root_height * 4) * i;

		if(i%2 == 0){
			glPushMatrix();
			glTranslatef(-range, 0, 0);
			drawTree(treeLevel, radius, root_height, 0, 0, 0);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(0, -range, 0);
			drawTree(treeLevel, radius, root_height, 0, 0, 0);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(range, 0, 0);
			drawTree(treeLevel, radius, root_height, 0, 0, 0);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(0, range, 0);
			drawTree(treeLevel, radius, root_height, 0, 0, 0);
			glPopMatrix();
		}
		else {
			glPushMatrix();
			glTranslatef(-range, -range, 0);
			drawTree(treeLevel, radius, root_height, 0, 0, 0);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(range, -range, 0);
			drawTree(treeLevel, radius, root_height, 0, 0, 0);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(range, range, 0);
			drawTree(treeLevel, radius, root_height, 0, 0, 0);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(-range, range, 0);
			drawTree(treeLevel, radius, root_height, 0, 0, 0);
			glPopMatrix();
		}
	}
	
}


void drawStatus() {
	/*GLfloat root_height = 20, slices = 10, sizeRatio = 4;
	int separate = 0, angle = 25;*/
	printf("-----------------\n");
	printf("Look: (%f, %f, %f)\n 1-2 Size: %f\n 3-4 Angle: %d\n 5-6 Height: %f\n 7-8 Slices: %d\n z-x Separate: %f\n",
		cameraX, cameraY, rho, sizeRatio,angle, root_height, slices, separate);
}

void display(void)
{	
	GLfloat a = abs(sunRise);
	glClearColor(a, a, a, 1);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(rho*sin(theta*DtoR)*sin(phi*DtoR), rho*cos(phi*DtoR), rho*cos(theta*DtoR)*sin(phi*DtoR), cameraX, cameraY, 0, 0, 1, 0);

	glRotatef(worldX, 1, 0, 0);
	//----- the object -----------------
	glColor3f(0.6, 0.44, 0.24);
	materials(&Materials);
	glTranslatef(0, 0, -root_height);
	float radius = root_height / sizeRatio;
	drawTree(treeLevel, radius, root_height, 0, 0, 0);


	
	if (extraTree > 0) {
		drawMoreTree(extraTree, radius);
	}

	if (hasLand) {
		glColor3f(0.3, 0.84, 0.3);
		drawSquare(1000, &LandMaterial);
	}

	//sun
	drawStar(1, &SunMaterial);

	//moon
	drawStar(0, &MoonMaterial);


	lighting(&Lighting);


	glutSwapBuffers();	
}



void init(void)
{
	seedPos();
	float ratio = ww*1.0 / (wh*1.0);

	glClearColor(1.0, 1.0, 1.0, 1.0);	// background color; default black; (R, G, B, Opacity)
	glColor3f(0, 1, 1);	// drawing color; default white

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

	glMatrixMode(GL_MODELVIEW);


	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	materials(&Materials);
	lighting(&Lighting);
}


void reshape(GLsizei w, GLsizei h)
{
	float ratio = w*1.0 / (h*1.0);

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

	// ------------------------ just use one of the following -----------------------
	glFrustum(left, right, bottom, top, near, far);	// perspective projection with frustum. use it as same time in init
	//gluPerspective(fov, aspect, near, far);

	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, w, h);		// important!

	//------ reset the window size
	ww = w;
	wh = h;
}


void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		rho -= dRho;
	}
	else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
	{
		//beta += dBeta ;
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		rho += dRho;
	}
	glutPostRedisplay();
}

void keys(unsigned char k, int x, int y)
{
	if (k == 'i' || k == 'I') {
		startTimer = !startTimer;
	 }
	else if (k == 'r' || k == 'R')
	{
		 root_height = 20;
		 sizeRatio = 5;
		 separate = 0;
		 angle = 25;
		 slices = 8;
		 rho = 100;
		 cameraX = 0;
		 cameraY = 0;
		 treeLevel = 8;
		 worldX = -90;
		 leafSize = 8;
		 LeafMaterial.diffuse[0] = 0.2;
		 LeafMaterial.diffuse[1] = 0.5;
		 LeafMaterial.diffuse[2] = 0.2;
	}

	else if (k == 'q') 
		rho -= dRho;
	else if (k == 'e') 
		rho +=dRho;
	else if (k == 'w')
		cameraY += 3;
	else if (k == 's')
		cameraY -= 3;
	else if (k == 'a') {
		cameraX-= 3;
	 }
	else if (k == 'd') {
		cameraX+= 3;
	 }
	else if (k == 'n') {
		worldX--;
	}
	else if (k == 'm') {
		worldX++;
	}
	else if (k == 'z') {
		 separate++;
	}
	else if (k == 'x') {
		 separate--;
	 }
	else if (k == '1') {
		 sizeRatio++;
	 }
	else if (k == '2' && sizeRatio>1) {
		 sizeRatio--;
	 }
	else if (k == '3') {
		 angle--;
	 }
	else if (k == '4') {
		 angle++;
	 }
	else if (k == '5') {
		 root_height--;
	 }
	else if (k == '6') {
		 root_height++;
	 }
	else if (k == '7' && slices>1) {
		 slices--;
	 }
	else if (k == '8') {
		 slices++;
	 }
	else if (k == '9') {
		leafSize++;
	}
	else if (k == '0') {
		leafSize--;
	}
	else if (k == 'c') {
		LeafMaterial.diffuse[0]+=0.1;
		LeafMaterial.diffuse[1]-=0.1;

	}
	else if (k == 'v') {
		LeafMaterial.diffuse[0]-=0.1;
		LeafMaterial.diffuse[2]+=0.1;
	}
	else if (k == 'b') {
		LeafMaterial.diffuse[2] -= 0.1;
		LeafMaterial.diffuse[1] += 0.1;
	}
	else if (k == 'l') {
		sunRise+=0.05;
		if (sunRise >= 1) {
			sunRise = -1;
		}
	}


	 drawStatus();
	 glutPostRedisplay();
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
	glutPostRedisplay();
}


void timer(int v)
{
	if (startTimer) {


		if (target_separate > 0.1) {
			target_separate -= 0.5;
			separate -= 0.5;
		}
		else if (target_separate < -0.1) {
			separate += 0.5;
			target_separate += 0.5;
		}
		else {
			bool finish = true;

			if (separate > 0.1) {
				separate -= 0.5;
				finish = false;
			}
			else if (separate < -0.1) {
				separate += 0.5;
				finish = false;
			}
			if (angle > 25) {
				angle -= 1;
				finish = false;
			}
			else if (angle < 25) {
				angle += 1;
				finish = false;
			}

			if (finish) {
				if (leafSize < 7.9){
					leafSize+=0.05;
				}
				else if (leafSize > 8.1){
					leafSize-=0.05;
				}
			}
		}

		sunRise += 0.005;
		if (sunRise >= 1) {
			sunRise = -1;
		}
	
		drawStatus();
		glutPostRedisplay();
	}
		glutTimerFunc(25, timer, v);

}

void seedPos() {
	angle = 180;
	separate = 0;
	target_separate = -10;
	leafSize = 1;
}

void paraPos() {
	angle = 270;
	separate = 30;
}


void spikePos() {
	angle = 0;
	separate = -30;
	target_separate = -30;
}

void rocketPos() {
	angle = 180;
	separate = 0;
	target_separate = -60;
}


void placement_menu(int id)
{
	if (id == 1) {
		seedPos();
	}
	else if (id == 2) {
		paraPos();
	}
	else if (id == 3) {
		spikePos();
	}
	else if (id == 4) {
		rocketPos();
	}
	glutPostRedisplay();
}

void right_menu(int i) {

	if (i == 1) {
		treeLevel++;
	}
	else if (i == 2 && treeLevel >1) {
		treeLevel--;
	}
	else if (i == 3) {
		extraTree++;
	}
	else if (i == 4 && treeLevel > 0) {
		extraTree--;
	}
	glutPostRedisplay();
}

void toggle_menu(int i) {
	if (i == 1) {
		startTimer = !startTimer;
	}
	else if (i == 2) {
		moreBranch = !moreBranch;
	}
	else if (i == 3) {
		hasLand = !hasLand;
	}
	else if (i == 4) {
		hasLeaf = !hasLeaf;
	}
	else if (i == 5) {
		if (sunRise <= 0 && sunRise <= 0.9) {
			sunRise = 1;
		}
		else {
			sunRise = 0;
		}
	}
	glutPostRedisplay();
}

void decorate_menu(int i) {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	if (i == 2) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else if (i == 3) {
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
		materials(&Materials);
		lighting(&Lighting);
	}
	glutPostRedisplay();
}

int main()
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // GLUT_DOUBLE work with glutPostRedisplay
	glutInitWindowSize(ww, wh);	// default size 300 by 300
	glutInitWindowPosition(100, 100); // defualt at (0, 0)
	glutCreateWindow("Fractal Tree Using Truncated Cone - Jacob Barr & Zheng Wei");
	init();

	int pos_menu= glutCreateMenu(placement_menu);
	glutAddMenuEntry("Seed Position", 1);
	glutAddMenuEntry("Parallel Position", 2);
	glutAddMenuEntry("Spike Position", 3);
	glutAddMenuEntry("Rocket Position", 4);

	int dec_menu = glutCreateMenu(decorate_menu);
	glutAddMenuEntry("Color", 1);
	glutAddMenuEntry("Wire", 2);
	glutAddMenuEntry("Material & Light", 3);

	int tog_menu = glutCreateMenu(toggle_menu);
	glutAddMenuEntry("Toggle Recover", 1);
	glutAddMenuEntry("Toggle More Branch", 2);
	glutAddMenuEntry("Toggle Land", 3);
	glutAddMenuEntry("Toggle Leaf", 4);
	glutAddMenuEntry("Toggle Day/Night", 5);
	

	glutCreateMenu(right_menu);
	glutAddMenuEntry("+ Level", 1);
	glutAddMenuEntry("- Level", 2);
	glutAddMenuEntry("+ Tree", 3);
	glutAddMenuEntry("- Tree", 4);
	
	glutAddSubMenu("Toggle", tog_menu);
	glutAddSubMenu("Position", pos_menu);
	glutAddSubMenu("Decoration", dec_menu);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keys);
	glutSpecialFunc(specialKeys);
	glutIdleFunc(NULL);
	glutTimerFunc(30, timer, 1);
	glutMainLoop();
	return 1; 
}