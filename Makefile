COMPILADOR = g++

LIBRERIAS_PATH = C:/SDL2MINGW/x86_64-w64-mingw32
#LIBRERIAS_PATH = C:\SDL2MINGW\i686-w64-mingw32

SRC = src

CFLAGSG = -std=c++17 -O2 -pipe -I"$(LIBRERIAS_PATH)/include/SDL2"
LDFLAGS = -L"$(LIBRERIAS_PATH)/lib" -lmingw32 -mwindows -mconsole -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -lm

TARGET = main.exe

CPP_SOURCE = $(wildcard *.cpp) $(wildcard $(SRC)/*.cpp) $(wildcard $(SRC)/*/*.cpp) $(wildcard $(SRC)/*/*/*.cpp)
CPP_HEADERS = $(wildcard *.hpp) $(wildcard $(SRC)/*.hpp) $(wildcard $(SRC)/*/*.hpp) $(wildcard $(SRC)/*/*/*.hpp)

$(TARGET): $(CPP_SOURCE) $(CPP_HEADERS)
	$(COMPILADOR) $(CFLAGSG) -o $(TARGET) $(CPP_SOURCE) $(LDFLAGS)

correr:
	$(PROG)
 