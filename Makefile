PYTHON_CONFIG=python2.7-config
PYTHON_CFLAGS=$(shell $(PYTHON_CONFIG) --includes)
PYTHON_LDFLAGS=$(shell $(PYTHON_CONFIG) --ldflags)
LDFLAGS=$(PYTHON_LDFLAGS) 
CFLAGS=$(PYTHON_CFLAGS) -fPIC -DNDEBUG -ggdb -fwrapv -Wall -fno-strict-aliasing -O1
SWIG=swig
CC=gcc
CXX=g++

%_wrap.cpp : %.i
	$(SWIG) -Wall -threads -c++ -python -o $@ $<

%.o: %.cpp
	$(CXX) -c $(CFLAGS) $< -o $@

_%.so: %.o
	$(CXX) -shared $(CFLAGS) $(LDFLAGS) $^ -o $@

default: _WS2801.so strip

strip: WS2801.cpp WS2801.h strip.cpp
	g++ WS2801.cpp strip.cpp -O3 -o strip

clean:
	rm -f strip _WS2801.so WS2801_wrap.cpp WS2801.py *.o *.pyc

_WS2801.so: WS2801_wrap.o WS2801.o
WS2801_wrap.o: WS2801_wrap.cpp
WS2801_wrap.cpp: WS2801.i WS2801.h
WS2801.o: WS2801.cpp WS2801.h
