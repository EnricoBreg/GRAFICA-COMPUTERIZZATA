SOURCE_C 	 := main
TARGET 		 := rubikCube
SHADERS_H	 := shadersf
# functions for modelview matrices stack
MVS_FUNCS    := modelViewStack
TODAY        := $(shell date +%Y%m%d)
ARCHIVE      := $(TARGET)-$(TODAY).zip
FLAGS		 := -lGL -lglut -lGLU -lGLEW -lX11 -lm -g

.PHONY: all clean exec dist

all: $(SOURCE_C).o $(SHADERS_H).o $(MVS_FUNCS).o $(TARGET)

# shadersf.o
$(SHADERS_H).o: $(SHADERS_H).c
	gcc -c $(SHADERS_H).c $(FLAGS)

# model view matrices stack file object
$(MVS_FUNCS).o: $(MVS_FUNCS).c
	gcc -c $(MVS_FUNCS).c $(FLAGS)

# main.o
$(SOURCE_C).o: $(SOURCE_C).c $(SHADERS_H).h $(MVS_FUNCS).h
	gcc -c $(SOURCE_C).c $(FLAGS)

#create executable
$(TARGET): $(SOURCE_C).o $(SHADERS_H).o $(MVS_FUNCS).o
	gcc -o $(TARGET) $(SOURCE_C).o $(SHADERS_H).o $(MVS_FUNCS).o $(FLAGS)

# clean wd
clean:
	@rm -rf $(TARGET) *.zip *.o

# create zip file
dist:
	@zip -r -9 $(ARCHIVE) $(wildcard *.[ch]) Makefile  *.glsl */

# exec the program
exec:
	./$(TARGET)
