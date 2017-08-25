LDLIBS=-lglut -lGLEW -lGL

all: gcc emcc

gcc:
	gcc main.cpp $(LDLIBS) -o triangle

emcc:
	emcc main.cpp -s FULL_ES2=1 -o triangle.html

clean:
	rm -f triangle*

.PHONY: all clean gcc emcc
