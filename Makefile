CXX = em++
RANLIB = emranlib
AR = emar

EXCEPTIONS ?= 1
DEBUG ?= 0
RELDEBUG ?= 0
PIC ?= 0
LTO ?= 1

BUILD_DIR := build/$(notdir $(CXX))

ifeq ($(PIC), 1)
	CXXFLAGS += -fPIC
	BUILD_DIR := $(BUILD_DIR)-pic
endif

ifeq ($(LTO), 1)
	CXXFLAGS += -flto
	BUILD_DIR := $(BUILD_DIR)-lto
endif

ifeq ($(RELDEBUG), 1)
	CXXFLAGS += $(DEBUG_CXXFLAGS) $(OPT_CXXFLAGS)
	CPPFLAGS += $(DEBUG_CPPFLAGS) $(OPT_CPPFLAGS)
	BUILD_DIR := $(BUILD_DIR)-reldebug
else ifeq ($(DEBUG), 1)
	CXXFLAGS += $(DEBUG_CXXFLAGS)
	CPPFLAGS += $(DEBUG_CPPFLAGS)
	BUILD_DIR := $(BUILD_DIR)-debug
else
	CXXFLAGS += $(OPT_CXXFLAGS)
	CPPFLAGS += $(OPT_CPPFLAGS)
	BUILD_DIR := $(BUILD_DIR)-release
endif

ifeq ($(EXCEPTIONS), 0)
	CXXFLAGS += -fno-exceptions
	BUILD_DIR := $(BUILD_DIR)-no-exceptions
endif

-include $(BUILD_DIR)/conandeps.mk

DEBUG_CXXFLAGS = -O0 -ggdb
OPT_CXXFLAGS = -O3
OPT_CPPFLAGS = -DNDEBUG

CXXFLAGS += \
	-std=c++20 -Wall -Wextra -Wconversion -Wshadow -Wcast-qual -Wformat=2 \
	-pedantic -pipe
CPPFLAGS += \
	-MMD -MP -DFIPS_APP_WINDOWED=1 -DSOKOL_GLES3 -DSOKOL_NO_DEPRECATED \
	$(addprefix -isystem, $(CONAN_INCLUDE_DIRS))
LDFLAGS += -fuse-ld=gold $(addprefix -L, $(CONAN_LIB_DIRS))
LDLIBS += -lsokol -lwebsocket.js $(addprefix -l, $(CONAN_LIBS))

EMSCRIPTEN_FLAGS = \
	-s STACK_SIZE=512KB \
	-s INITIAL_MEMORY=33554432 \
	-s ERROR_ON_UNDEFINED_SYMBOLS=1 \
	-s NO_EXIT_RUNTIME=1 \
	-s ALLOW_MEMORY_GROWTH=1 \
	-s USE_WEBGL2=1 \
	-s "MALLOC='emmalloc'" \
	-s WASM=1 \
	--preload-file fonts \
	--shell-file html/shell.html \
	--closure 1 \
	-s ASSERTIONS=0

WASM_LDFLAGS = -L $(BUILD_DIR) $(EMSCRIPTEN_FLAGS)

TARGET = $(BUILD_DIR)/index.html
SRCS := \
	src/settings_window.cpp src/gui.cpp src/main.cpp \
	src/query_window.cpp src/websocket.cpp src/window.cpp
OBJS = $(SRCS:%=$(BUILD_DIR)/%.o)

SOKOL_TARGET = $(BUILD_DIR)/libsokol.a
SOKOL_SRCS := src/sokol.cpp
SOKOL_OBJS = $(SOKOL_SRCS:%=$(BUILD_DIR)/%.o)

DEPS = $(SOKOL_OBJS:.o=.d) $(OBJS:.o=.d)

$(TARGET): $(SOKOL_TARGET) $(OBJS)
	$(LINK.cc) -o $@ $(WASM_LDFLAGS) $(OBJS) $(LDLIBS)

$(SOKOL_TARGET): $(SOKOL_OBJS)
	$(AR) rc $@ $^
	$(RANLIB) $@

$(SOKOL_OBJS): $(BUILD_DIR)/%.cpp.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) -o $@ -c $(CPPFLAGS) $(CXXFLAGS) $<

$(OBJS): $(BUILD_DIR)/%.cpp.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) -o $@ -c $(CPPFLAGS) $(CXXFLAGS) $<

all: $(SOKOL_TARGET) $(TARGET)

deps: clean
	@mkdir -p $(BUILD_DIR)
	conan install --build=missing --output-folder=$(BUILD_DIR) --profile:host=emsdk --profile:build=default .

httpd: $(TARGET)
	python3 -m http.server -d $(BUILD_DIR)

clean:
	$(RM) $(SOKOL_TARGET) $(TARGET) \
	$(SOKOL_OBJS) $(OBJS) \
	$(DEPS)

-include $(DEPS)

.PHONY: all conan_install httpd clean
