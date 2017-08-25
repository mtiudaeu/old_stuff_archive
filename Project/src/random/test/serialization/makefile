TARGET = demo
LIBS = -lboost_serialization
CC = g++
CPPFLAGS = -std=c++0x -g -Wall
SOURCES = demo.cpp
OBJECTS = $(SOURCES:.cpp=.o)
PREREQUISITES = $(SOURCES:.cpp=.d)

.PHONY: all clean
 
all: $(TARGET)

%.d: %.cpp
	set -e; rm -f $@; \
	$(CC) -M $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

$(TARGET): $(PREREQUISITES) $(OBJECTS)
	$(CC) $(CPPFLAGS) $(OBJECTS) $(LIBS) -o $@

include $(PREREQUISITES)

#%.o: %.cpp
#	$(CC) $(CFLAGS) -c $< -o $@


clean:
	-rm -f *.o
	-rm -f *.d
	-rm -f $(TARGET)
