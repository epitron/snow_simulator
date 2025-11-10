/*
 * Snow.h: interface for the Snow class.
 *
 * This class contains glut callbacks and OpenGL calls to do the snow         
 * simulation. 
 *
 */

#include <GL/glut.h>
#include <list>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

#include "floatbuffer.h"
#include "flake.h"
#include "heightfield.h"
#include "vector.h"

using namespace std;

class Snow  
{
	int 
		// cached window size
		w_width, 
		w_height;

	vector
		anchor, // anchor for a mouse-drag
		spin,	// amount to spin camera
		lastspin;	// amount of last spin

	// user toggles (see readme.txt for meanings)	
	bool
		point_surface,
		avalanches,
		labels;


	GLvoid 
		*font_style;			// current raster-font style

	float
	
		gustiness,				// how turbulent the wind is

		
		gustinc,					// how much to increment gustiness/wind vector when 
		windinc;					// user presses the keys


  
	vector
		*wind;					  // wind vector, gets added to every snowflake's motion

	list<flake>
		snow;							// list of all the 'flake' elements

	int
		num_flakes;				// initial number of flake elements

	Heightfield
		*hf;							// Heightfield for the model the user selected
	
	// Draw a raster string in worldspace
	void drawstr(float x, float y, float z, char* format, ...);
	// Set the font when drawing the string
	void setfont(char* name, int size);
	
	// Draw a grid
	void drawgrid();
	// Move the snowflakes, then render them
	void drawflakes();
	// Draw accumulated snow
	void drawaccumulatedsnow();

public:
	// Initialize the class with a starting window size of width,height
	Snow(int width,  int height);
	virtual ~Snow();
	
	// Initialize the class' heightfield and get it ready to roll!
	void initialize(floatbuffer* zbuf);

	// Add n more snowflakes to the snow list
	void moresnow(int n, bool spawn);
	// Remove n snowflakes from the list (can be bigger than current amount 
	// of snow)
	void lesssnow(int n);

	// Reset all simulaiton-state variables (except list of snow)
	void reset();	
	// Move the camera to its initial position in the scene
	void init_camera();	
	// Draw the scene
	void display();
	
	// Glut callback
	void reshape(int width,  int height);
	// Glut callback
	void keyboard(unsigned char key, int x, int y);
	// Glut callback
	void motion(int x, int y);
	// Glut callback
	void mouse(int button, int state, int x, int y);

};
