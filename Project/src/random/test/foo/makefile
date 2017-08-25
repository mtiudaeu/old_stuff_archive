TARGET = prog
LIBS = 
CC = g++
CPPFLAGS = -g -Wall
OBJECTS = main.o foo.o
PREREQUISITES = main.d foo.d

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
	-rm -f *.d
	-rm -f *.o
	-rm -f $(TARGET)
