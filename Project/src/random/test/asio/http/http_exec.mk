CURRENT_SOURCES =
CURRENT_SOURCES += random/test/asio/http/connection.cpp
CURRENT_SOURCES += random/test/asio/http/connection_manager.cpp
CURRENT_SOURCES += random/test/asio/http/main.cpp
CURRENT_SOURCES += random/test/asio/http/mime_types.cpp
CURRENT_SOURCES += random/test/asio/http/reply.cpp
CURRENT_SOURCES += random/test/asio/http/request_handler.cpp
CURRENT_SOURCES += random/test/asio/http/request_parser.cpp
CURRENT_SOURCES += random/test/asio/http/server.cpp
CURRENT_OBJECTS_NAME = EXECUTABLE_OBJECTS
$(eval $(call OBJECTS_TEMPLATE, $(CURRENT_OBJECTS_NAME), $(CURRENT_SOURCES)))

CURRENT_DEPENDANCY = $(EXECUTABLE_OBJECTS)
$(eval $(call TARGET_TEMPLATE, $(CURRENT_TARGET), $(CURRENT_DEPENDANCY)))


