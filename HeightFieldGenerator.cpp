// HeightFieldGenerator.cpp: implementation of the HeightFieldGenerator class.
//
//////////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include "HeightFieldGenerator.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

HeightFieldGenerator::HeightFieldGenerator(int width, int height)
: pmodel(NULL),
	w_width(width),
	w_height(height)
{
	spin.x = 0;
	spin.y = 0;

	anchor = spin;
	lastspin = spin;
}

HeightFieldGenerator::~HeightFieldGenerator()
{
}

floatbuffer*
HeightFieldGenerator::getdepthbuffer()
{

	floatbuffer *zbuf = new floatbuffer();

	zbuf->w = w_width; 
	zbuf->h = w_height;
	zbuf->create(zbuf->w*zbuf->h);

	glReadPixels(0, 0, zbuf->w, zbuf->h, GL_DEPTH_COMPONENT, GL_FLOAT, zbuf->buf);

	return zbuf;

}

void
HeightFieldGenerator::drawmodel(void)
{
    if (!pmodel) {
				char filename[1000];
				sprintf(filename, "%s%s", rootdir, "data/al.obj");
        pmodel = glmReadOBJ(filename);
        if (!pmodel) exit(0);
        glmUnitize(pmodel);
        glmFacetNormals(pmodel);
        glmVertexNormals(pmodel, 90.0);
    }
    
    glmDraw(pmodel, GLM_SMOOTH | GLM_MATERIAL);
}


void
HeightFieldGenerator::display(void)
{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
    glRotatef(spin.y, 1.0, 0.0, 0.0);
    glRotatef(spin.x, 0.0, 1.0, 0.0);
    drawmodel();
		glPopMatrix();
    
		glutSwapBuffers();
		glutPostRedisplay();
}


void
HeightFieldGenerator::keyboard(unsigned char key, int x, int y)
{
	switch (key) {
		case 27:
        exit(0);
  }
}


void
HeightFieldGenerator::mouse(int button, int state, int x, int y)
{
	switch (state)
	{
		case GLUT_DOWN:
			anchor.x = x;
			anchor.y = y;
    			break;
		case GLUT_UP:
			lastspin = spin;
			break;
	}
	glutPostRedisplay();
}

void
HeightFieldGenerator::motion(int x, int y)
{
    spin.x = lastspin.x + x - anchor.x;
    spin.y = lastspin.y + y - anchor.y;
    
    glutPostRedisplay();
}



void
HeightFieldGenerator::reshape(int width,  int height) 
{
	w_width = width;
	w_height = height;
		
	glViewport(0, 0, width, height);
		
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, 1, 1, 10);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 2,
						0, 0, 0,
						0, 1, 0);

	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

void
HeightFieldGenerator::loadmodel(char* name)
{
  pmodel = glmReadOBJ(name);
  if (!pmodel) exit(0);
  glmUnitize(pmodel);
  glmFacetNormals(pmodel);
  glmVertexNormals(pmodel, 90.0);
}
