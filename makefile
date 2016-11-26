CC=g++
INCLUDEPATHS=-I$(SDKSTAGE)/opt/vc/include/ -I/opt/vc/include/interface/vcos/pthreads -I$(SDKSTAGE)/opt/vc/include/interface/vmcs_host/linux -I./include -I./src
CFLAGS=-std=c++11 -c -Wall -static -fPIC $(INCLUDEPATHS) -fmax-errors=3
SOURCES=src/onut.cpp src/Random.cpp src/Vector2.cpp src/Vector3.cpp src/Vector4.cpp src/Matrix.cpp src/Color.cpp src/ThreadPool.cpp src/Dispatcher.cpp src/Timing.cpp src/Settings.cpp src/Updater.cpp src/Window.cpp src/WindowX11.cpp
OBJECTS=$(SOURCES:.cpp=.o)
LIBRARY=lib/linux/onut.a

all: $(SOURCES) $(LIBRARY)

$(LIBRARY): $(OBJECTS)
	ar rcs $@ $^

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
