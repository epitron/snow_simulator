// flake.cpp: implementation of the flake class.
//
//////////////////////////////////////////////////////////////////////

#include <stdio.h>

#include "flake.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

flake::flake()
{
	init();
}

flake::~flake()
{

}

void
flake::init()
{
	respawn();
	pos.y = (float)rand() / RAND_MAX;
}

void
flake::respawn()
{
	pos.x = (float)rand() / RAND_MAX;
	pos.y = 1.0;
	pos.z = (float)rand() / RAND_MAX;

	if (pos.z > 1.0f || pos.x > 1.0f)
		printf("RESPAWN!!\n");

	vel.x = 0.0;
	vel.y = -0.01;
	vel.z = 0.0;

	size = 0.005; // how much snow gets added to the accumulated snow on 
								// the Heightfield when this piece lands
}

void
flake::next_pos(float gustiness, vector* wind)
{
	pos = vel + pos + *wind;

	// wrap flake around to other side of screen if it goes off (ala pac-man)
	if (pos.x < 0.0)
		pos.x = 1.0;
	else if(pos.x >= 1.0)
		pos.x = 0.0;

	if (pos.z < 0.0)
		pos.z = 1.0;
	else if(pos.z >= 1.0)
		pos.z = 0.0;

	// simulate wind turbulence
	vel.x += (((float)(rand()*2) / RAND_MAX) - 1.0) * gustiness;
	if (vel.x > 0.6)
		vel.x = 0.4;
		
	vel.z += (((float)(rand()*2) / RAND_MAX) - 1.0) * gustiness;
	if (vel.z > 0.6)
		vel.z = 0.4;
}

void
flake::plot()
{
	glVertex3f(pos.x, pos.y, pos.z);
}
