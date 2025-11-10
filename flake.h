// flake.h: interface for the flake class.
//
//////////////////////////////////////////////////////////////////////

#include <stdlib.h>

#ifdef WIN32
	#include <glut.h>
#else
	#include <GL/glut.h>
#endif

#include "vector.h"

class flake  
{
public:
	vector
		pos,		// position of snowflake in worldspace
		vel;		// current velocity of the snowflake

	float
		size;		// how much to add to the heightfield when it lands

	flake();
	virtual ~flake();

	// Move the snowflake to a random position with a random initial velocity
	// vector
	void init();
	
	// Same as init, but places flake at the very top of the scene
	void respawn();
	
	// Move this flake to its next position, checking whether it went off
	// the screen, or landed on the Heightfield. Randomizes the velocity
	// vector based on gustiness, and adds the wind vector.
	void next_pos(float gustiness, vector* wind);
	
	// Draw this snowflake
	void plot();

};

