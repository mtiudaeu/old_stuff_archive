LIBS = -lboost_system -lboost_thread -lpthread
CC = g++
INCDIRS = -I.
CPPFLAGS = -std=c++0x -g -Wall $(INCDIRS) -MD -MP

BUILD_PATH = 
ALL_TARGET = main

SOURCES += main.cpp
OBJECTS = $(addprefix $(BUILD_PATH), $(SOURCES:.cpp=.o))
PREREQUISITES = $(addprefix $(BUILD_PATH), $(SOURCES:.cpp=.d))


.PHONY: all clean

all: $(ALL_TARGET)

$(UNIT_TEST_TARGET) : CPPFLAGS += -DUNIT_TEST_ACTIVATED
$(UNIT_TEST_TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(LIBS) -o $@

$(BUILD_PATH)%.o: %.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@

-include $(PREREQUISITES)

clean:
	-rm -f $(ALL_TARGET)
	-rm -f $(OBJECTS)
	-rm -f $(PREREQUISITES)

