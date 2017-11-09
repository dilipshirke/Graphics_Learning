CC=gcc
CFLAGS=-I.

OBJ= main.o gl_egl_render.o shader.o x_window.o

%.o: %.c 
	$(CC) -c -o $@ $< $(CFLAGS) -lX11 -lEGL -lGLESv2

app_executable: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) -lX11 -lEGL -lGLESv2

.PHONY: clean

clean: 
	rm -f */.o app_executable
