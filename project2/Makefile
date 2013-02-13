###########################################################
# Project 1 Makefile

CC = g++
CFLAGS = -g
INCLUDE =
LIBDIR =
LIBS = -lglut -lGLU -lGL -lm

###########################################################
# Options if compiling on Mac
UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
CC = g++
CFLAGS = -D__MAC__
INCLUDE = 
LIBDIR = -L/lusr/X11/lib
LIBS = -framework OpenGL -framework GLUT
endif

###########################################################
# Uncomment the following line if you are using Mesa
#LIBS = -lglut -lMesaGLU -lMesaGL -lm


###########################################################

# To add a cpp file called myfile.cpp see comments below

billiards: main.o ball.o geom2.o # myfile.o 
	./cpplint.py --filter=-readability/todo,-readability/streams,-runtime/threadsafe_fn,-legal,-build/namespaces,-build/header_guard *.cpp *.h
#	${CC} ${CFLAGS} $(INCLUDE) -o billiards main.o ball.o ${LIBDIR} ${LIBS}
	${CC} ${CFLAGS} $(INCLUDE) -o billiards main.o ball.o geom2.o ${LIBDIR} ${LIBS} # replace the previous line with this line

main.o: main.cpp common.h geom2.h
	${CC} -c ${CFLAGS} $(INCLUDE) main.cpp

ball.o: ball.cpp ball.h common.h geom2.h
	${CC} -c ${CFLAGS} $(INCLUDE) ball.cpp

# add the following lines
# myfile.o: myfile.cpp myfile.h # any additional dependencies
#	 ${CC} -c ${CFLAGS} $(INCLUDE) myfile.cpp

geom2.o: geom2.cpp common.h geom2.h ball.h
	${CC} -c ${CFLAGS} $(INCLUDE) geom2.cpp


clean:
	rm -f billiards *.o
