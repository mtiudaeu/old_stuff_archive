LIBS = -lboost_system -lboost_thread -lpthread 
CC = g++
CPPFLAGS = -std=c++0x -g -Wall

SERVERDIR = network/server/
BUILD_PATH = ../../../bin/$(SERVERDIR)
TARGET = $(BUILD_PATH)/server

SOURCES += main.cpp 
SOURCES += server.cpp 
SOURCES += session_manager.cpp 
SOURCES += session.cpp
OBJECTS = $(addprefix $(BUILD_PATH), $(SOURCES:.cpp=.o))
PREREQUISITES = $(addprefix $(BUILD_PATH), $(SOURCES:.cpp=.d))

.PHONY: all clean
 
all: $(TARGET)

$(BUILD_PATH)%.d: %.cpp
	@mkdir -p $(dir $(PREREQUISITES))
	set -e; rm -f $@; \
	$(CC) -M $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

$(TARGET): $(PREREQUISITES) $(OBJECTS)
	$(CC) $(OBJECTS) $(LIBS) -o $@

include $(PREREQUISITES)

$(BUILD_PATH)%.o: %.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@


clean:
	-rm -f $(TARGET)
	-rm -f $(OBJECTS)
	-rm -f $(PREREQUISITES)
