/*
 * Snow simulator by Chris Gahan (chris@ill-logic.com)
 */

#ifdef WIN32
	#include <glut.h>
#else
	#include <GL/glut.h>
#endif

#include <stdio.h>
#include <math.h>

// Core functional classes:
#include "HeightFieldGenerator.h"
#include "Snow.h"

// Credit, where due.
#define VERSIONSTRING "Snow Simulator v0.3 by Chris Gahan (chris@ill-logic.com)"

// Initial window dimensions
#define DEF_WIDTH		512
#define DEF_HEIGHT	512

// Initial window position
#define DEF_POS_X		50
#define DEF_POS_Y		50


int 
	// current window dimensions
	window_width, 
	window_height,
	
	// identifier for the GLUT context menu
	main_context_menu;

// important classes. these are core of the program.
HeightFieldGenerator *heightfield;
Snow *snow;

// Enumerated values to identify the current state the program is in.
enum {HEIGHTFIELD, SNOW};

// the program state (starts off in heightfield-creating mode)
int 
	state = HEIGHTFIELD;

// keyboard callback -- handles global keys, then if none were hit,
// doles out responsibility for handling the event to whichever class
// is currently controlling the program.
void
keyboard(unsigned char key, int x, int y)
{
	switch (key) {

	case 27:
      exit(0);
			break;

		default:
			switch(state)
			{
				case HEIGHTFIELD:
					heightfield->keyboard(key, x, y);
					break;
				case SNOW:
					snow->keyboard(key, x, y);
					break;
			}
	}

}

// mouse click callback -- 
// doles out responsibility for handling the event to whichever class
// is currently controlling the program.
void
mouse(int button, int s, int x, int y)
{
	switch (state) {
		case HEIGHTFIELD:
			heightfield->mouse(button, s, x, y);
			break;
		case SNOW:
			snow->mouse(button, s, x, y);
			break;
	}
}

// mouse motion callback -- 
// doles out responsibility for handling the event to whichever class
// is currently controlling the program.
void
motion(int x, int y)
{
	switch (state) {
		case HEIGHTFIELD:
			heightfield->motion(x, y);
			break;
		case SNOW:
			snow->motion(x, y);
			break;
	}
}

// display callback -- 
// doles out responsibility for handling the event to whichever class
// is currently controlling the program.
void
display(void)
{
	switch (state) {
		case HEIGHTFIELD:
			heightfield->display();
			break;
		case SNOW:
			snow->display();
			break;
	}
}

// window reshape callback -- 
// doles out responsibility for handling the event to whichever class
// is currently controlling the program.
void
reshape(int width,  int height) 
{
	switch (state) {
		case HEIGHTFIELD:
			heightfield->reshape(width, height);
			break;
		case SNOW:
			snow->reshape(width, height);
			break;
	}
}


// idle callback -- 
// if we're in the snow state, animate the snow continuously. in the
// HEIGHTFIELD state, the object is only drawn when the object is rotated
// by the user.
void
idle()
{
	if (state == SNOW)
	{
		snow->display();
	}
}

// context menu callback --
// this is only used by the HeightFieldGenerator class.
// it transfers control of the program to the Snow class once the
// user has decided on the position of the object they will grab the
// heightfield from.
void
context_menu(int value)
{

  char* name = 0;
	floatbuffer* zbuf;
    
  switch (value) 
	{
		// select models:
		
    case 'a':
        name = "data/al.obj";
        break;
    case 's':
        name = "data/soccerball.obj";
        break;
    case 'd':
        name = "data/dolphins.obj";
        break;
    case 'f':
        name = "data/flowers.obj";
        break;
    case 'j':
        name = "data/f-16.obj";
        break;
    case 'p':
        name = "data/porsche.obj";
        break;
    case 'r':
        name = "data/rose+vase.obj";
        break;
	
		// Capture zbuffer:
		case 'C':
			switch (state) 
			{
				case HEIGHTFIELD:
					// Switch to snow
					zbuf = heightfield->getdepthbuffer();
					snow->initialize(zbuf);
					snow->init_camera();
					glutDestroyMenu(main_context_menu);
					state = SNOW;
					break;
			}
			break;

	}
  
  // user picked a model... load it!
  if (name)
		heightfield->loadmodel(name);

  glutPostRedisplay();
}

int 
main(int argc, char * argv[]) 
{
	// glut window identifier
	GLuint 
		window;

	// Create a new window
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowSize(DEF_WIDTH, DEF_HEIGHT);
	glutInitWindowPosition(DEF_POS_X, DEF_POS_Y);
	glutInit(&argc, argv);
	window = glutCreateWindow(VERSIONSTRING);

	// Set GLUT event callbacks
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMotionFunc(motion);
	glutMouseFunc(mouse);
	glutIdleFunc(idle);

	// instantiate core classes
	heightfield = new HeightFieldGenerator(DEF_WIDTH, DEF_HEIGHT);
	snow = new Snow(DEF_WIDTH, DEF_HEIGHT);

	// Create a context menu for use by the HeightFieldGenerator class
	main_context_menu = glutCreateMenu(context_menu);
	glutAddMenuEntry("Pick a model:", 0);
	glutAddMenuEntry("-----------------------------------", 0);
	glutAddMenuEntry("Soccerball", 's');
	glutAddMenuEntry("Al Capone", 'a');
	glutAddMenuEntry("F-16 Jet", 'j');
	glutAddMenuEntry("Dolphins", 'd');
	glutAddMenuEntry("Flowers", 'f');
	glutAddMenuEntry("Porsche", 'p');
	glutAddMenuEntry("Rose", 'r');
	glutAddMenuEntry("-----------------------------------", 0);
	glutAddMenuEntry("Capture Depths and Begin Simulation", 'C');
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	// Transfer programmatic control to the GLUT event handler forever.
	glutMainLoop();
		
	// A mysterious place which exists outside time and space.
	return 0;

}
