CURRENT_SOURCES =
CURRENT_SOURCES += common/unit_test/hello_world_test.cpp
CURRENT_OBJECTS_NAME = EXECUTABLE_OBJECTS
$(eval $(call OBJECTS_TEMPLATE, $(CURRENT_OBJECTS_NAME), $(CURRENT_SOURCES)))

CURRENT_DEPENDANCY = $(EXECUTABLE_OBJECTS)
$(eval $(call TARGET_TEMPLATE, $(CURRENT_TARGET), $(CURRENT_DEPENDANCY)))
