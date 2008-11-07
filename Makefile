all: gl_code gl_circle
gl_code: gl_code.c
	gcc -g -o gl_code gl_code.c -O2 -march=nocona -lm -L/usr/lib -lGLU -lGL -lglut -L/usr/X11R6/lib -lX11 -lXext -lXi -lXmu
gl_circle: gl_circle.c
	gcc -g -o gl_circle gl_circle.c -O2 -march=nocona -lm -L/usr/lib -lGLU -lGL -lglut -L/usr/X11R6/lib -lX11 -lXext -lXi -lXmu
