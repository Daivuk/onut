CC=g++
INCLUDEPATHS=-I$(SDKSTAGE)/opt/vc/include/ -I/opt/vc/include/interface/vcos/pthreads -I$(SDKSTAGE)/opt/vc/include/interface/vmcs_host/linux -I../../include/
CFLAGS=-std=c++11 -c -Wall $(INCLUDEPATHS) -fmax-errors=3
LDFLAGS=-L$(SDKSTAGE)/opt/vc/lib/ -lGLESv2 -lEGL -lopenmaxil -lbcm_host -lvcos -lvchiq_arm -lpthread -lrt -lm ../../lib/linux/onut.a
SOURCES=src/EmptySample.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=EmptySample

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
