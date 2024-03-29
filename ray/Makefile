#
#  Makefile for fltk based ray tracer on Lion
#

LOCAL = /usr/local
OPT = /opt/local

CC = clang++ -std=c++11 -stdlib=libc++

INCLUDE = -I$(LOCAL)/include

LDLIBS = -L$(LOCAL)/lib -framework Carbon -framework ApplicationServices -framework Cocoa

GLDLIBS = -framework AGL -framework OpenGL

LIBS  = $(LDLIBS) $(GLDLIBS) -lfltk_gl -lfltk -lfltk_images -lfltk_forms -lfltk_jpeg -lpng -lz -lm

CFLAGS = -O3

.SUFFIXES: .o .cpp .cxx

.o: 
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $*.o $<

.cpp.o: 
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $*.o $<

.cxx.o: 
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $*.o $<

ALL.O = src/main.o src/getopt.o src/RayTracer.o \
	src/ui/CommandLineUI.o src/ui/GraphicalUI.o src/ui/TraceGLWindow.o \
	src/ui/debuggingView.o src/ui/glObjects.o src/ui/debuggingWindow.o \
	src/ui/ModelerCamera.o \
	src/fileio/bitmap.o src/fileio/buffer.o \
	src/fileio/pngimage.o \
	src/parser/Token.o src/parser/Tokenizer.o \
	src/parser/Parser.o src/parser/ParserException.o \
	src/scene/camera.o src/scene/light.o\
	src/scene/material.o src/scene/ray.o src/scene/scene.o \
	src/SceneObjects/Box.o src/SceneObjects/Cone.o \
	src/SceneObjects/Cylinder.o src/SceneObjects/trimesh.o \
	src/SceneObjects/Sphere.o src/SceneObjects/Square.o

ray: $(ALL.O)
	$(CC) $(CFLAGS) -o $@ $(ALL.O) $(INCLUDE) $(LIBDIR) $(LIBS)

clean:
	rm -f $(ALL.O)

clean_all:
	rm -f $(ALL.O) ray

