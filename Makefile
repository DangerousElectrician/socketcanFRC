IDIR	= ./include
INCLUDES = -I"include"
CXX=g++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES= $(wildcard *.cpp)
OBJECTS= $(SOURCES:%.cpp=build/%.o)
EXECUTABLE=build/test

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

$(OBJECTS): build/%.o : %.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) $< -o $@

