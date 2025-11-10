#ifndef FBUF_H
#define FBUF_H

/* floatbuffer class
 *
 * This class is merely a container for the values copied from the zbuffer.
 * It is used by the glReadPixels routine.
 */

class floatbuffer
{

public:
	int 
		w, 
		h;

	float
		*buf;

	floatbuffer()
	{
		buf = 0;
		w = -1;
		h = -1;
	}

	~floatbuffer()
	{
		if (buf)
			delete buf;
	}

	// allocate a buffer of size "size"
	void create(int size)
	{
		if (buf)
			delete buf;

		buf = new float[size];
  }

};

#endif
