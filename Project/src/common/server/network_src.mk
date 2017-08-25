CURRENT_SOURCES =
CURRENT_SOURCES += common/server/session_manager.cpp
CURRENT_SOURCES += common/server/session.cpp
$(eval $(call OBJECTS_TEMPLATE, $(CURRENT_OBJECTS_NAME), $(CURRENT_SOURCES)))

