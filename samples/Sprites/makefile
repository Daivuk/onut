CC=g++
INCLUDEPATHS=-I$(SDKSTAGE)/opt/vc/include/ -I/opt/vc/include/interface/vcos/pthreads -I$(SDKSTAGE)/opt/vc/include/interface/vmcs_host/linux -I../../include/
CFLAGS=-std=c++11 -c -static -Wall $(INCLUDEPATHS) -fmax-errors=3 -g
CFLAGS+=-DSTANDALONE -D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS -DTARGET_POSIX -D_LINUX -fPIC -DPIC -D_REENTRANT -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -U_FORTIFY_SOURCE -Wall -g -DHAVE_LIBOPENMAX=2 -DOMX -DOMX_SKIP64BIT -ftree-vectorize -pipe -DUSE_EXTERNAL_OMX -DHAVE_LIBBCM_HOST -DUSE_EXTERNAL_LIBBCM_HOST -DUSE_VCHIQ_ARM -Wno-psabi
LDFLAGS=-L$(SDKSTAGE)/opt/vc/lib/ -lbcm_host -lvcos -lvchiq_arm -rdynamic ../../lib/linux/onut.a -lEGL -lGLESv2 -lEGL -lGLESv2 -lpthread -lrt -ldl
SOURCES=src/SpritesSample.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=SpritesSample.bin

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@


.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
