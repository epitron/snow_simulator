OBJS = HeightFieldGenerator.o Snow.o flake.o glm.o vector.o main.o

LIBS = -L/usr/local/Mesa/lib -lglut -lMesaGLU -lMesaGL -lX11 -lXext -lXmu -lXi -lsocket -lnsl -lm
INCS = -I/usr/local/Mesa/include

#OPTS=-O3 -ffast-math -fexpensive-optimizations -fomit-frame-pointer
OPTS=-O3 

CXXFLAGS = $(INCS) $(OPTS)
CXX      = g++
CC	 = gcc
CFLAGS	 = $(INCS) $(OPTS)

snow:	$(OBJS)
	$(CXX) $(CXXFLAGS) -o snow $(OBJS) $(LIBS)

.SUFFIXES:	.cpp .c

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f *.o *~ core Makefile.bak

depend:	
	makedepend -Y *.h *.cpp

# DO NOT DELETE

HeightFieldGenerator.o: HeightFieldGenerator.h
Snow.o: Snow.h
vector.o: vector.h
glm.o: glm.h
flake.o: flake.h
main.o: HeightFieldGenerator.h Snow.h
