------------------------------------------------------------------------
ASSIGNMENT #3 - CISC454
Chris Gahan (chris@ill-logic.com)
------------------------------------------------------------------------
Run the snow.exe file to play with the program...

Step 1: Pick a model! Right click for a selection of models courtesy of 
				Nate Robins' GLUT demos.

Step 2: Hold the left-button and rotate the object to an orientation that
				you think looks cool for snow to fall on.
				
Step 3: Right click and choose "Capture Depths and Begin Simulation"

It should start snowing on an invisible representation of the model!
Again, hold left-click to rotate.

If it quits without warning, it can't find any models. Make sure the
binary you are executing is located in the directory that contains
the "data" directory. Also, you can edit '#define rootdir "./"' at 
the top of HeightFieldGenerator.h if you have to run the thing in
a different directory from "data/" (i.e. VC++ profiling).

The keys to tweak the snow simulation are as follows:

	r : reset all settings to defaults

	a : toggle avalanches (Default = on)
	p : toggle point-surface, or quad-strip surface (quad's are experimental, 
			default = point-surface)
	l	: toggle labels (showing current program variables)

	g/G: in/decrease gustiness (wind turbulence)

	-/=: add/remove 3 snowflakes
	_/+: add/remove 50 snowflakes
	s/S: add/remove 1000 snowflakes
	d/D: dump/undump 10000 snowflakes!

	z/Z: in/decrease wind vector in x direction (right)
	x/X: de/increase wind vector in y direction (up)
	c/C: in/decrease wind vector in z direction (away)

	ESC: QUIT!
		
Enjoy!
