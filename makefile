CC=g++
INCLUDEPATHS=-I$(SDKSTAGE)/opt/vc/include/ -I/opt/vc/include/interface/vcos/pthreads -I$(SDKSTAGE)/opt/vc/include/interface/vmcs_host/linux -I./include -I./src
CFLAGS=-std=gnu++11 -c -Wall -fPIC $(INCLUDEPATHS) -fmax-errors=3 -g
CFLAGS+=-DSTANDALONE -D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS -DTARGET_POSIX -D_LINUX -fPIC -DPIC -D_REENTRANT -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -U_FORTIFY_SOURCE -Wall -g -DHAVE_LIBOPENMAX=2 -DOMX -DOMX_SKIP64BIT -ftree-vectorize -pipe -DUSE_EXTERNAL_OMX -DHAVE_LIBBCM_HOST -DUSE_EXTERNAL_LIBBCM_HOST -DUSE_VCHIQ_ARM -Wno-psabi
SOURCES=src/onut.cpp src/Random.cpp src/Vector2.cpp src/Vector3.cpp src/Vector4.cpp src/Matrix.cpp src/Color.cpp src/ThreadPool.cpp src/Dispatcher.cpp src/Timing.cpp src/Settings.cpp src/Updater.cpp src/Window.cpp src/WindowX11.cpp src/Maths.cpp src/Renderer.cpp src/RendererGLES2.cpp src/Shader.cpp src/Texture.cpp src/VertexBuffer.cpp src/IndexBuffer.cpp src/ContentManager.cpp src/Resource.cpp src/Files.cpp src/Strings.cpp src/Curve.cpp src/ShaderGLES2.cpp src/TextureGLES2.cpp src/VertexBufferGLES2.cpp src/IndexBufferGLES2.cpp src/Log.cpp src/lodepng/LodePNG.cpp src/SpriteBatch.cpp src/PrimitiveBatch.cpp src/Tween.cpp
OBJECTS=$(SOURCES:.cpp=.o)
LIBRARY=lib/linux/onut.a

all: $(SOURCES) $(LIBRARY)

$(LIBRARY): $(OBJECTS)
	ar qc $(LIBRARY) $(OBJECTS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
