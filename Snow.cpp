// Snow.cpp: implementation of the Snow class.
//
//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include "Snow.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


Snow::Snow(int width,  int height)
:	w_width(width),
	w_height(height),
	font_style(GLUT_BITMAP_HELVETICA_12),
	num_flakes(1000),
	hf(NULL),
	wind(NULL)
{
	reset();
}


Snow::~Snow()
{
}

void Snow::reset()
{
	init_camera();

	spin.x = 0; spin.y = 0;
	anchor = spin;
	lastspin = spin;

	gustiness = 0.0005f;

	point_surface = true;
	avalanches = true;
	labels = true;

	if (wind)
		delete wind;
	wind = new vector(0.004f, 0.0f, 0.007f);

	windinc = 0.001f;
	gustinc = 0.001f;
}

void
Snow::moresnow(int n, bool spawn)
{
	flake f;
	if (spawn)
		for (int i = 0; i < n; i++)
		{
			f.respawn();
			snow.push_back(f);
		}
	else
		for (int i = 0; i < n; i++)
		{
			f.init();
			snow.push_back(f);
		}
}

void
Snow::lesssnow(int n)
{
	while (!snow.empty() && n > 0)
	{
		snow.pop_back();
		n--;
	}
}


void
Snow::initialize(floatbuffer* zbuf)
{
	snow.clear();
  moresnow(num_flakes, false);
	hf = new Heightfield(zbuf);
}

void
Snow::setfont(char* name, int size)
{
    font_style = GLUT_BITMAP_HELVETICA_10;
    if (strcmp(name, "helvetica") == 0) {
        if (size == 12) 
            font_style = GLUT_BITMAP_HELVETICA_12;
        else if (size == 18)
            font_style = GLUT_BITMAP_HELVETICA_18;
    } else if (strcmp(name, "times roman") == 0) {
        font_style = GLUT_BITMAP_TIMES_ROMAN_10;
        if (size == 24)
            font_style = GLUT_BITMAP_TIMES_ROMAN_24;
    } else if (strcmp(name, "8x13") == 0) {
        font_style = GLUT_BITMAP_8_BY_13;
    } else if (strcmp(name, "9x15") == 0) {
        font_style = GLUT_BITMAP_9_BY_15;
    }
}

void 
Snow::drawstr(float x, float y, float z, char* format, ...)
{
    va_list args;
    char buffer[255], *s;
    
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);
    
    glRasterPos3f(x, y, z);
    for (s = buffer; *s; s++)
        glutBitmapCharacter(font_style, *s);
}


void
Snow::drawgrid()
{
	float j;

	glColor3f(1, 0, 0);
	glBegin(GL_LINES);
		for (j = 0.0f; j < 1.01f; j+=0.1f) 
		{
			// x plane, first quadrant
			glColor3f(0, 1, 0);
			glVertex3f(j, 0, 1); glVertex3f(j, 0, 0);
			glVertex3f(1, 0, j); glVertex3f(0, 0, j);
		}
	glEnd();
}

void
Snow::drawflakes() 
{
		HFElement* e;
		list<flake>::iterator f;

		int x,y;

		glColor3f(1,1,1);
		glBegin(GL_POINTS);

    for( f = snow.begin(); f != snow.end(); f++ ) 
		{
			f->next_pos(gustiness, wind);
			
			x = (int)(f->pos.x * (hf->w - 1));
		  y = (int)(f->pos.z * (hf->h - 1));
			e = &hf->elements[x][y];

			if (f->pos.y < e->accum)
			{
				if (avalanches)
					e->accumulate(f->size);
				else
					e->accum += f->size;

				f->respawn();
			}
			else 
				f->plot();
		}

		glEnd();
}

void
Snow::drawaccumulatedsnow()
{
	int x, y;
	HFElement* e;

	if (point_surface) 
	{		
		glColor3f(1, 1, 1);
		glBegin(GL_LINES);
		for (y = 0; y < hf->h; y++) 
		{
			for (x = 0; x < hf->w; x++) 
			{
				e = &(hf->elements[x][y]);
				if (e->hassnow) 
				{
					e->plotbase();
					e->plotaccum();
				}
			}
		}
		glEnd();
	}
	
	else
	{
		for (y = 0; y < hf->h - 1; y++) 
		{
			glBegin(GL_QUAD_STRIP);
			for (x = 0; x < hf->w; x++) 
			{
				hf->elements[x][y].colorplotaccum();
				hf->elements[x][y].colorplotaccum();
			}
			glEnd();
		}
	}
}

void
Snow::display(void)
{
		char stringy[500];

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
  	glTranslatef(0.5, 0.5, 0.5);
    glRotatef(spin.y, 1.0, 0.0, 0.0);
    glRotatef(spin.x, 0.0, 1.0, 0.0);
		glTranslatef(-0.5, -0.5, -0.5);
    
		drawgrid();		
		drawflakes();
		drawaccumulatedsnow();

		if (labels)
		{
			glColor3f(1,0,0);
			sprintf(stringy, "Flakes: %d", snow.size());
			drawstr(1, 1, 0, stringy);			
			
			glColor3f(0,1,0.5);
			sprintf(stringy, "Wind: (%.4f,%.4f,%.4f)", wind->x, wind->y, wind->z);
			drawstr(0, 1, 0, stringy);
			
			glColor3f(1,0,1);
			sprintf(stringy, "Gustiness: %f", gustiness);
			drawstr(0, 1, 1, stringy);

			glColor3f(1,1,0);
			sprintf(stringy, "Avalanches: %s", avalanches?"ON":"OFF");
			drawstr(1, 1, 1, stringy);

			glColor3f(0, 0.6f, 0);
			sprintf(stringy, "size=%dx%d", hf->w, hf->h);
			drawstr(0, 0, 0, stringy);
		}

		glPopMatrix();

		glutSwapBuffers();
		glutPostRedisplay();
}

void
Snow::init_camera()
{
	glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60, 1, 0.1, 10);
		
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

	glTranslatef(-0.5, -0.5, -0.5);
  gluLookAt(0, 0, 1.5, 
						0, 0, 0,
						0, 1, 0);

		
  glClearColor(0, 0, 0, 0);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);
  glDisable(GL_LIGHT0);
}

void
Snow::reshape(int width,  int height) 
{
  w_width = width;
  w_height = height;
		
	glViewport(0, 0, width, height);
	
	init_camera();
}

void 
Snow::keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'p':
			point_surface = !point_surface;
			break;
		case 'a':
			avalanches = !avalanches;
			break;
		case 'l':
			labels = !labels;
			break;

		case 'g':
			gustiness += gustinc;
			break;
		case 'G':
			gustiness -= gustinc;
			break;

		case 'z':
			wind->x += windinc;
			break;
		case 'Z':
			wind->x -= windinc;
			break;

		case 'x':
			wind->y -= windinc;
			break;
		case 'X':
			wind->y += windinc;
			break;

		case 'c':
			wind->z += windinc;
			break;
		case 'C':
			wind->z -= windinc;
			break;

		case '=':
			moresnow(3, false);
			break;
		case '-':
			lesssnow(3);
			break;

		case '+':
			moresnow(50, false);
			break;
		case '_':
			lesssnow(50);
			break;

		case 's':
			moresnow(1000, false);
			break;
		case 'S':
			lesssnow(1000);
			break;

		case 'd':
			moresnow(10000, true);
			break;
		case 'D':
			lesssnow(10000);
			break;

		case 'r':
			reset();
			break;

	}
	
	glutPostRedisplay();
}

void 
Snow::motion(int x, int y)
{
    spin.x = lastspin.x + x - anchor.x; 
    spin.y = lastspin.y + y - anchor.y;
    
    glutPostRedisplay();
}

void 
Snow::mouse(int button, int state, int x, int y)
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

