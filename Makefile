gl_demo_0: gl_code.c
	mpicc -g -o gl_code gl_code.c -O2 -march=nocona -lm -L/usr/lib -lGLU -lGL -lglut -L/usr/X11R6/lib -lX11 -lXext -lXi -lXmu
