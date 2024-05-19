TARGET := app

SRC := $(wildcard src/*.cpp)
OBJ := $(patsubst src/%.cpp,build/%.o,$(SRC))
DEP := $(OBJ:.o=.d)

CC := g++
CC_FLAGS := -Wall -Wextra -pedantic -MMD
LD_FLAGS := -lSDL2 -lSDL2_image

WIN_SDL_LIB := C:\sdl2-dev
WIN := no

ifeq ($(WIN),yes)
	LD_FLAGS := -I$(WIN_SDL_LIB)\include -L$(WIN_SDL_LIB)\lib $(LD_FLAGS)
endif

all: $(TARGET) $(HEADERS)

-include $(DEP)

$(TARGET): build $(OBJ)
	$(CC) $(CC_FLAGS) -o $(TARGET) $(OBJ) $(LD_FLAGS)

build:
	mkdir -p build

build/%.o: src/%.cpp
	$(CC) $(CC_FLAGS) $(LD_FLAGS) -c $< -o $@

clean:
	rm -rf build $(TARGET)

run: all
	./$(TARGET)
