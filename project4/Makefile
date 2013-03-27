###########################################################
# Project 1 Makefile

CC = g++
CFLAGS = 
#CFLAGS = -Wall -ggdb -g
#CFLAGS = -g
INCLUDE =
#INCLUDE = -I/lusr/X11/include -I/lusr/include
LIBDIR =
#LIBDIR = -L/lusr/X11/lib -L/lusr/lib
# Libraries that use native graphics hardware --
# appropriate for Linux machines in Taylor basement
#LIBS = -lglut -lGLU -lGL -lpthread -lm
LIBS = -lglut -lGLU -lGL -lm -ljpeg

###########################################################
# Options if compiling on Mac
UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
CC = g++
CFLAGS = -Wall -g -D__MAC__
INCLUDE = -I/opt/local/include
LIBDIR = -L/lusr/X11/lib -L/opt/local/lib
LIBS = -framework OpenGL -framework GLUT -ljpeg
endif

###########################################################
# Uncomment the following line if you are using Mesa
#LIBS = -lglut -lMesaGLU -lMesaGL -lm

viewer: main.o io.o mesh.o texture.o material.o
	./check-code main.cpp material.cpp material.h mesh.cpp mesh.h io.cpp io.h
	${CC} ${CFLAGS} $(INCLUDE) -o viewer main.o io.o mesh.o texture.o material.o ${LIBDIR} ${LIBS}

main.o: main.cpp io.h mesh.h texture.o
	${CC} ${CFLAGS} -c -o main.o $(INCLUDE) main.cpp

io.o: io.cpp io.h mesh.h
	${CC} ${CFLAGS} -c -o io.o $(INCLUDE) io.cpp

mesh.o: mesh.cpp mesh.h
	${CC} ${CFLAGS} -c -o mesh.o $(INCLUDE) mesh.cpp

texture.o: texture.cpp texture.h
	${CC} ${CFLAGS} -c -o texture.o $(INCLUDE) texture.cpp

material.o: material.cpp material.h
	${CC} ${CFLAGS} -c -o material.o $(INCLUDE) material.cpp

clean:
	rm -f viewer *.o
