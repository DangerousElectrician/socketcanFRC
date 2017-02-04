IDIR	= ./include
INCLUDES = -I"include"
CXX=g++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES= $(wildcard *.cpp)
OBJECTS= $(SOURCES:.cpp=.o)
EXECUTABLE=test

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CXX) $(CFLAGS) $(INCLUDES) $< -o $@

