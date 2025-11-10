// HeightFieldGenerator.h: interface for the HeightFieldGenerator class.
//
//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <GL/glut.h>
#include "floatbuffer.h"
#include "vector.h"

// glm by Nate Robbins. Incredibly useful pile of model-related
// OpenGL utility functions!
extern "C" {
	#include "glm.h"
}

// Set this to the directory which contains the data directory. Whatever
// dir you set, "data/model.obj" will be appended to it when loading.
#define rootdir "./"

//
// HeightFieldGenerator class.
//
// This class allows a user to select a model, rotate it, then capture
// the zbuffer. It contains GLUT callback functions so it takes over control
// of most of the program when in use.
//
class HeightFieldGenerator
{

private:


	int 
		// window dimensions
		w_width, 
		w_height;

	vector
		// mouse/motion callback variables
		spin,
		lastspin,
		anchor;

	// Pointer to the model currently being viewed.
	GLMmodel 
		*pmodel;

	// captured zbuffer
	floatbuffer
		zbuf;

	// draw the model!
	void drawmodel(void);

public:
	// Create a new HeightFieldGenerator with window dimensions of width, height
	HeightFieldGenerator(int width, int height);
	virtual ~HeightFieldGenerator();

	// glut callback
	void display();
	// glut callback
	void reshape(int width,  int height);
	// glut callback
	void menu(int value);
	// glut callback
	void keyboard(unsigned char key, int x, int y);
	// glut callback
	void motion(int x, int y);
	// glut callback
	void mouse(int button, int state, int x, int y);

	// loads a wavefront object file and associated .ppm's
	void loadmodel(char* name);
	
	// return the depthbuffer for the current scene
	floatbuffer* getdepthbuffer();

};

