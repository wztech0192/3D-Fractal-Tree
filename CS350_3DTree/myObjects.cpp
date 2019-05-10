#include <math.h>
#include <GL\glut.h>

const float DtoR = 0.017453;
const float Pi = 3.14159265;
const float root3 = 1.73205081;
const float root6 = 2.44948974;

void square(float center_x0, float center_y0, float side);
void triangle(float midBase_x0, float midBase_y0, float height);
void ellipse(float center_x0, float center_y0, float a, float b);
void circle(float center_x0, float center_y0, float r);
void sphere(float radius, int slices, int stacks);
void cone(float r, float h);
void cylinder(float r, float h);
void paraboloid(float a, float b, float c, int slices, int stacks);
void axes(float a);
void colorCube(float size);
void colorCube(float x0, float y0, float z0, float size);

void colorCube(float x0, float y0, float z0, float s)
{
	//--------------- face 1 --------------------------------
	glBegin(GL_POLYGON);
	glColor3f(0, 0, 0); glVertex3f(x0-s/2, y0-s/2, z0-s/2);
	glColor3f(1, 0, 0); glVertex3f(x0+s/2, y0-s/2, z0-s/2);
	glColor3f(1, 1, 0); glVertex3f(x0+s/2, y0+s/2, z0-s/2);
	glColor3f(0, 1, 0); glVertex3f(x0-s/2, y0+s/2, z0-s/2);
	glEnd();
	//---------------------------------------------------

	//--------------- face 2 --------------------------------
	glBegin(GL_POLYGON);
	glColor3f(1, 1, 0); glVertex3f(x0+s/2, y0+s/2, z0-s/2);
	glColor3f(0, 1, 0); glVertex3f(x0-s/2, y0+s/2, z0-s/2);
	glColor3f(0, 1, 1); glVertex3f(x0-s/2, y0+s/2, z0+s/2);
	glColor3f(1, 1, 1); glVertex3f(x0+s/2, y0+s/2, z0+s/2);
	glEnd();
	//---------------------------------------------------

	//-------------- face 3 -----------------------------
	glBegin(GL_POLYGON);
	glColor3f(1, 1, 1); glVertex3f(x0 + s / 2, y0 + s / 2, z0 + s / 2);
	glColor3f(0, 1, 1); glVertex3f(x0 - s / 2, y0 + s / 2, z0 + s / 2);
	glColor3f(0, 0, 1); glVertex3f(x0 - s / 2, y0 - s / 2, z0 + s / 2);
	glColor3f(1, 0, 1); glVertex3f(x0 + s / 2, y0 - s / 2, z0 + s / 2);
	glEnd();
	//------------------------------------------------------

	//----------- face 4 -------------------------------
	glBegin(GL_POLYGON);
	glColor3f(0, 0, 1); glVertex3f(x0 - s / 2, y0 - s / 2, z0 + s / 2);
	glColor3f(1, 0, 1); glVertex3f(x0 + s / 2, y0 - s / 2, z0 + s / 2);
	glColor3f(1, 0, 0); glVertex3f(x0 + s / 2, y0 - s / 2, z0 - s / 2);
	glColor3f(0, 0, 0); glVertex3f(x0 - s / 2, y0 - s / 2, z0 - s / 2);
	glEnd();
	//------------------------------------------------------

	//----------- face 5 -------------------------------
	glBegin(GL_POLYGON);
	glColor3f(1, 0, 1); glVertex3f(x0 + s / 2, y0 - s / 2, z0 + s / 2);
	glColor3f(1, 0, 0); glVertex3f(x0 + s / 2, y0 - s / 2, z0 - s / 2);
	glColor3f(1, 1, 0); glVertex3f(x0 + s / 2, y0 + s / 2, z0 - s / 2);
	glColor3f(1, 1, 1); glVertex3f(x0 + s / 2, y0 + s / 2, z0 + s / 2);
	glEnd();
	//------------------------------------------------------

	//----------- face 6 -------------------------------
	glBegin(GL_POLYGON);
	glColor3f(0, 0, 0); glVertex3f(x0 - s / 2, y0 - s / 2, z0 - s / 2);
	glColor3f(0, 1, 0); glVertex3f(x0 - s / 2, y0 + s / 2, z0 - s / 2);
	glColor3f(0, 1, 1); glVertex3f(x0 - s / 2, y0 + s / 2, z0 + s / 2);
	glColor3f(0, 0, 1); glVertex3f(x0 - s / 2, y0 - s / 2, z0 + s / 2);
	glEnd();
	//------------------------------------------------------
}

void colorCube(float s)
{
	//--------------- face 1 --------------------------------
	glBegin(GL_POLYGON);
	glColor3f(0, 0, 0); glVertex3f(0, 0, 0);
	glColor3f(1, 0, 0); glVertex3f(s, 0, 0);
	glColor3f(1, 1, 0); glVertex3f(s, s, 0);
	glColor3f(0, 1, 0); glVertex3f(0, s, 0);
	glEnd();
	//---------------------------------------------------

	//--------------- face 2 --------------------------------
	glBegin(GL_POLYGON);
	glColor3f(1, 1, 0); glVertex3f(s, s, 0);
	glColor3f(0, 1, 0); glVertex3f(0, s, 0);
	glColor3f(0, 1, 1); glVertex3f(0, s, s);
	glColor3f(1, 1, 1); glVertex3f(s, s, s);
	glEnd();
	//---------------------------------------------------

	//-------------- face 3 -----------------------------
	glBegin(GL_POLYGON);
	glColor3f(1, 1, 1); glVertex3f(s, s, s);
	glColor3f(0, 1, 1); glVertex3f(0, s, s);
	glColor3f(0, 0, 1); glVertex3f(0, 0, s);
	glColor3f(1, 0, 1); glVertex3f(s, 0, s);
	glEnd();
	//------------------------------------------------------

	//----------- face 4 -------------------------------
	glBegin(GL_POLYGON);
	glColor3f(0, 0, 1); glVertex3f(0, 0, s);
	glColor3f(1, 0, 1); glVertex3f(s, 0, s);
	glColor3f(1, 0, 0); glVertex3f(s, 0, 0);
	glColor3f(0, 0, 0); glVertex3f(0, 0, 0);
	glEnd();
	//------------------------------------------------------

	//----------- face 5 -------------------------------
	glBegin(GL_POLYGON);
	glColor3f(1, 0, 1); glVertex3f(s, 0, s);
	glColor3f(1, 0, 0); glVertex3f(s, 0, 0);
	glColor3f(1, 1, 0); glVertex3f(s, s, 0);
	glColor3f(1, 1, 1); glVertex3f(s, s, s);
	glEnd();
	//------------------------------------------------------

	//----------- face 6 -------------------------------
	glBegin(GL_POLYGON);
	glColor3f(0, 0, 0); glVertex3f(0, 0, 0);
	glColor3f(0, 1, 0); glVertex3f(0, s, 0);
	glColor3f(0, 1, 1); glVertex3f(0, s, s);
	glColor3f(0, 0, 1); glVertex3f(0, 0, s);
	glEnd();
	//------------------------------------------------------
}

void axes(float a)
{
	glPushMatrix();
	glPushAttrib(GL_CURRENT_BIT);
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex3f(-1 * a, 0, 0);
	glVertex3f(a, 0, 0);

	glColor3f(0, 1, 0);
	glVertex3f(0, -1 * a, 0);
	glVertex3f(0, a, 0);

	glColor3f(0, 0, 1);
	glVertex3f(0, 0, -1 * a);
	glVertex3f(0, 0, a);
	glEnd();

	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslatef(a, 0, 0);
	glRotatef(90, 0, 1, 0);
	glutSolidCone(a / 40, a / 8, 20, 10);
	glPopMatrix();
	
	glPushMatrix();
	glColor3f(0, 1, 0);
	glTranslatef(0, a, 0);
	glRotatef(-90, 1, 0, 0);
	glutSolidCone(a / 40, a / 8, 20, 10);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 1);
	glTranslatef(0, 0, a);
	glutSolidCone(a / 40, a / 8, 20, 10);
	glPopMatrix();

	glPopAttrib();
	glPopMatrix();

	
}

void paraboloid(float a, float b, float c, int slices, int stacks)
{
	float z1, z2;
	float dz = c / stacks;
	int dTheta = (int)(360.0 / slices);
	float x, y;

	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i < stacks; i++)
	{
		for (int j = 0; j <= slices; j++)
		{
			z1 = i * dz;
			x = a * sqrt(z1 / c)*cos(j*dTheta*DtoR);
			y = b * sqrt(z1 / c) *sin(j*dTheta*DtoR);
			glVertex3f(x, y, z1);

			z2 = z1 + dz;
			x = a * sqrt(z2 / c)*cos(j*dTheta*DtoR);
			y = b * sqrt(z2 / c) *sin(j*dTheta*DtoR);
			glVertex3f(x, y, z2);
		}
	}
	glEnd();
}


void cylinder(float r, float h)
{
	glBegin(GL_QUAD_STRIP);
	for (int theta = 0; theta <= 360; theta += 5)
	{
		glVertex3f(r*cos(theta*DtoR), r*sin(theta*DtoR), 0);
		glVertex3f(r*cos(theta*DtoR), r*sin(theta*DtoR), h);
	}
	glEnd();
}

void cone(float r, float h)
{
	glBegin(GL_QUAD_STRIP);
	for (int theta = 0; theta <= 360; theta += 5)
	{
		glVertex3f(0, 0, h);
		glVertex3f(r*cos(theta*DtoR), r*sin(theta*DtoR), 0);
	}
	glEnd();
}


void sphere(float radius, int slices, int stacks)
{
	GLint longitude = (int)(360.0 / slices);
	GLint latitude = (int)(180.0 / stacks);
	GLint phi, theta;
	GLfloat p1[3], p2[3], p3[3], p4[3];

	for (phi = -90; phi <= 90 - latitude; phi += latitude)
	{
		for (theta = -180; theta <= 180; theta += longitude)
		{
			p1[0] = radius * cos(theta*DtoR) * cos(phi*DtoR);
			p1[1] = radius * sin(theta*DtoR) * cos(phi*DtoR);
			p1[2] = radius * sin(phi*DtoR);

			p2[0] = radius * cos((theta + longitude)*DtoR) * cos(phi*DtoR);
			p2[1] = radius * sin((theta + longitude)*DtoR) * cos(phi*DtoR);
			p2[2] = radius * sin(phi*DtoR);

			p3[0] = radius * cos((theta + longitude)*DtoR) * cos((phi + latitude)*DtoR);
			p3[1] = radius * sin((theta + longitude)*DtoR) * cos((phi + latitude)*DtoR);
			p3[2] = radius * sin((phi + latitude)*DtoR);

			p4[0] = radius * cos(theta*DtoR) * cos((phi + latitude)*DtoR);
			p4[1] = radius * sin(theta*DtoR) * cos((phi + latitude)*DtoR);
			p4[2] = radius * sin((phi + latitude)*DtoR);

			glBegin(GL_POLYGON);
			glVertex3fv(p1);
			glVertex3fv(p2);
			glVertex3fv(p3);
			glVertex3fv(p4);
			glEnd();
		}
	}
}

void square(float x0, float y0, float a)	// center (x0, y0), side length a
{
	glBegin(GL_POLYGON);
	glVertex2f(x0-a/2, y0-a/2);
	glVertex2f(x0+a/2, y0-a/2);
	glVertex2f(x0+a/2, y0+a/2);
	glVertex2f(x0-a/2, y0+a/2);
	glEnd();
}

void triangle(float x0, float y0, float h)	// equilateral triangle, mid-point of base (x0, y0), height h
{
	float d = root3 / 3;

	glBegin(GL_POLYGON);
	glVertex2f(x0 - d, y0);
	glVertex2f(x0 + d, y0);
	glVertex2f(x0, y0 + h);
	glEnd();
}

void ellipse(float x0, float y0, float a, float b)
{
	float theta, dTheta = 5;
	int count;
	float x, y;

	count = (int)(360 / dTheta);
	glBegin(GL_POLYGON);
	for (int i = 0; i <= count; i++)
	{
		theta = i*dTheta*DtoR;
		x = x0 + a*cos(theta);
		y = y0 + b*sin(theta);
		glVertex2f(x, y);
	}
	glEnd();
}

void circle(float x0, float y0, float r)
{
	float theta, dTheta = 5;
	int count;
	float x, y;

	count = (int)(360 / dTheta);
	glBegin(GL_POLYGON);
	for (int i = 0; i <= count; i++)
	{
		theta = i * dTheta*DtoR;
		x = x0 + r * cos(theta);
		y = y0 + r * sin(theta);
		glVertex2f(x, y);
	}
	glEnd();
}

