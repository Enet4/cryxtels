# Cryxtels Modern Version Makefile.am
bin_PROGRAMS = cryxtels
AM_CXXFLAGS = -I../include -Wall -std=c++11 $(shell sdl2-config --cflags)

INC = ../include

cryxtels_SOURCES = cryxtels.cpp fast3d.cpp text3d.cpp input.cpp global.cpp $(INC)/fast3d.h $(INC)/text3d.h $(INC)/input.h $(INC)/global.h $(INC)/scanline.h
cryxtels_LDFLAGS = $(shell sdl2-config --libs)

