# makefile

INCLUDES = -Iincludes/fmt/include/ \
           -Iincludes/scnlib/include/ \
           -Iincludes/scnlib/src/ \
           -I./ \
           -I./vf3lib/include

#DEFINES = -DFMT_HEADER_ONLY=1 -DSCN_HEADER_ONLY=1

# by default do just a single trial (overrideable on the `make` CLI)
STRESS_TEST_COUNT ?= 1

DEFINES = \
    -DFMT_HEADER_ONLY=1 \
    -DSCN_HEADER_ONLY=1 \
    -DSTRESS_TEST_COUNT=$(STRESS_TEST_COUNT)

CXX := g++
LD := g++
LINKFLAGS := 
CPPFLAGS := -fdiagnostics-color=always -std=c++20 -O3 $(INCLUDES) $(DEFINES) -MMD -MP

BUILD_DIR := build

# Make does not offer a recursive wildcard function, so here's one:
# DO NOT ADD SPACES TO ARGS WHILE CALLING
# leave 2nd arg empty to search from this directory
rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

EXCLUDE := includes/%.cpp \
           vf3lib/%.cpp

SRC_CPP := $(filter-out $(EXCLUDE),$(call rwildcard,,*.cpp))
OBJ_CPP := $(patsubst %.cpp, $(BUILD_DIR)/objs/%.o, $(SRC_CPP))

DEPENDS := $(patsubst %.cpp,$(BUILD_DIR)/objs/%.d,$(SRC_CPP))

.PHONY: all build_out clean always

all: build_out

build_out: $(BUILD_DIR)/prog

$(BUILD_DIR)/prog: $(OBJ_CPP)
	@echo "Linking Project"
	@$(LD) $(LINKFLAGS) -o $@ $^ $(TARGET_LIBS)
	@echo "Created " $@

-include $(DEPENDS)

$(BUILD_DIR)/objs/%.o: %.cpp makefile
	@mkdir -p $(@D)
	@echo "Compiling: " $<
	@$(CXX) $(CPPFLAGS) -c -o $@ $<

clean:
	@rm -r $(BUILD_DIR)
