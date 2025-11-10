#include <GL/glut.h>
#include <math.h>
#include <list>
#include "floatbuffer.h"

#ifndef M_PI
	// as much of PI as I could remember...
	#define M_PI 3.141592653589793238462643383f 
#endif

// Maximum angle of repose. If the angle of the slope between two adjacent 
// heights in the Heightfield is greater than this, then an avalanche
// happens. It's in radians. Making it low slows down the program sub-
// stantially since many avalanches have to be calculated, and the depth
// of the recursion gets very high.
#define maxaor 70.0*M_PI/180.0f


// Heightfield element class.
// This class represents one height in the field.
class HFElement {

public:
		GLfloat
			base,			// the height of the "ground"
			accum,		// the height of the accumulated snow

			// this element's position in worldspace
			worldx,		
			worldy;

		bool
			hassnow;	// has some snow on it.

		/*
		 * Array of pointers to neighbour elements. It's indexed thusly:
		 *
		 *            0 1 2
		 *            3 n 4
		 *            5 6 7
		 *
		 * 'n' is the current node. If there is no neighbour, the pointer == NULL.
		 *
		 */
		HFElement*
			neighbour[8];

		// Initialize stuff
		HFElement()
		: base(0.0),
			accum(0.0),
			worldx(0.0),
			worldy(0.0)
		{
		}

		~HFElement()
		{
		}

		// plot the base of the voxel
		inline void plotbase()
		{
			glVertex3f(worldx, base, worldy);
		}

		// plot the top of the voxel
		inline void plotaccum()
		{
			glVertex3f(worldx, accum, worldy);
		}

		// plot the surface of the accumulated snow giving it a colour.
		inline void colorplotaccum()
		{
			if (hassnow)
				glColor3f(0.8f, 0.8f, 0.8f);
			else
				glColor3f(0, 0, 0);

			glVertex3f(worldx, accum, worldy);
		}

	// add snow to this voxel
	void accumulate(float amount)
	{
		HFElement* 
			n;

		// accumulate snow
  	accum += amount;

		if (!hassnow)
			hassnow = true;
		else for (int i = 0; i < 8; i++)
		{
			// Scan through neghbours, testing for avalanches.
			
			n = neighbour[i];
			if (n && accum > n->accum)
			{
/*
	Angle of Repose works like this.. You figure out the angle of the slope of
	the snow between the current element and the neighbouring element, and
	if it's greater than the maximum angle of repose, you avalance as much snow
	is necessary to make the new angle of repose equal to the maximum angle
	of repose.

	Here's a side-view of the accumulated snow on the two points:

        |--w--|
        
			 	*----------------------
		  	|\aor   -
		  	| \     |
		  	|  \    h
		  	|   \   |
        |    \  -
			  |     *
        |     |
        |     |
				|     |
				|     |
				|     |
   this-^     ^-neighbour

	For more info, check Paul Fearing's Siggraph 2000 paper, "Computer Modelling of Fallen Snow"

*/
				float 
					w, 					// x/z planar distance to neighbour
					h, 					// difference in heights
					d, 					// difference in height for maximum angle of repose
					aor, 	     	// angle of repose between this element and neighbour
					avalanche; 	// amount of snow to avalanche
				
				w = sqrt( (worldx - n->worldx) * (worldx - n->worldx) 
								+ (worldy - n->worldy) * (worldy - n->worldy) );
				h = accum - n->accum;
				aor = atan(h / w);

				if (aor > maxaor) 
				{
					d = w / tan(maxaor);
					
					// avalanche as much snow as would balance the two heights
					// so they'll have exactly maxaor angle of repose between them.
					avalanche = (accum - n->accum - d) / 2.0f;

					// make sure that we're not avalanching more snow than we have
					if (avalanche > accum - base)
					{
						avalanche = accum - base;
						accum = base;
						hassnow = false;
					}
					else
						accum -= avalanche;
					
					// recurse
					n->accumulate(avalanche);
				}
			}
		}

	}

};


class Heightfield {

public:

	// width and height of this heightfield
	int
		w,
		h;


	// the 2D array of HFElements
	HFElement
		**elements;
	
	// Create a new heightfield from the captured zbuffer 'zbuf'
  Heightfield(floatbuffer* zbuf)
	: elements(0),
		w(0),
		h(0)
	{
		
		HFElement* 
			e;
			
		int 
			x, 
			y;
		
		w = zbuf->w;
		h = zbuf->h;
		
		// Convert zbuffer into a heightfield (array of HFElements)
		elements = new HFElement*[w];
		for (x = 0; x < w; x++) 
		{
			elements[x] = new HFElement[h];
			
			for (y = 0; y < h; y++) 
			{
				e = &(elements[x][y]);
				e->base = 1.0 - zbuf->buf[x + w*y]; // invert range of values				
				e->accum = e->base;
				e->hassnow = false;
				e->worldx = (float)x / w;
				e->worldy = (float)y / h;
			}
		}


		// Set pointers to neighbour elements
		for (x = 0; x < w; x++) 
		{
			for (y = 0; y < h; y++) 
			{
				e = &(elements[x][y]);
		    e->neighbour[0] = getneighbour( x-1, y-1 );
				e->neighbour[1] = getneighbour( x  , y-1 );
				e->neighbour[2] = getneighbour( x+1, y-1 );
				e->neighbour[3] = getneighbour( x-1, y   );
				e->neighbour[4] = getneighbour( x+1, y   );
				e->neighbour[5] = getneighbour( x-1, y+1 );
				e->neighbour[6] = getneighbour( x  , y+1 );
				e->neighbour[7] = getneighbour( x+1, y+1 );
			}
		}

		delete zbuf;
		
	}

	~Heightfield()
	{
		if (elements)
			delete elements;
	}

	// Return a pointer to the element at x,y, making sure x&y are in a 
	// valid range. Slow, so not useful for actual simulation. It makes the 
	// above code easier to read. :)
  HFElement* getneighbour(int x, int y)
	{
		if (x >= 0 && y >= 0 && x < w && y < h) 
			return &elements[x][y];
		else
			return NULL;
	}
	
};
