CC := g++-4.9
CPP_FILES := $(wildcard src/*.cpp)
C_FILES := $(wildcard src/*.c)
ACPP_FILES := $(wildcard src/*/*.cpp)
AC_FILES := $(wildcard src/*/*.c)
CPP_OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))
C_OBJ_FILES := $(addprefix obj/,$(notdir $(C_FILES:.c=.o)))
ACPP_OBJ_FILES := $(addprefix obj/,$(notdir $(ACPP_FILES:.cpp=.o)))
AC_OBJ_FILES := $(addprefix obj/,$(notdir $(AC_FILES:.c=.o)))
INCLUDE := -I ./include/
CC_FLAGS :=  $(INCLUDE) -std=c++14
LIB := -lSDL2 -lz
LD_FLAGS := 
VERSION := 0.0.1
OUTPUT_DIR := ./libtcup-dev.$(VERSION)/
OUT := $(OUTPUT_DIR)libtcup.so

$(OUT): $(C_OBJ_FILES) $(CPP_OBJ_FILES) $(AC_OBJ_FILES) $(ACPP_OBJ_FILES)
	mkdir $(OUTPUT_DIR)
	$(CC) $(LD_FLAGS) $^  -shared -o $@
	cp -avr ./include $(OUTPUT_DIR)include
	
obj/%.o: src/%.cpp
	$(CC) $(CC_FLAGS) -c -fPIC -o$@ $< $(LIB)

obj/%.o: src/%.c
	$(CC) $(CC_FLAGS) -c -fPIC -o $@ $< $(LIB)
	
obj/%.o: src/*/%.cpp
	$(CC) $(CC_FLAGS) -c -fPIC -o $@ $< $(LIB)

obj/%.o: src/*/%.c
	$(CC) $(CC_FLAGS) -c -fPIC -o $@ $< $(LIB)


.Phony:clean gitpush
	
clean:
	rm -r obj/*.o
	rm -r $(OUTPUT_DIR)

gitpush: 
	git add --all
	git commit -m "$(msg)"
	git push
