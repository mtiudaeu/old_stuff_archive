#Reserved keyword
.PHONY: all clean defaultIgnore network json test random_http

defaultIgnore:
	@echo "Invalid target"

# -------------------- Build options --------------------

# Build path
BUILD_PATH = ../bin/debug/

# Compile flags
# MDTMP separate library...
LIBS = -lboost_system -lboost_thread -lpthread -lgtest -lcurl
CC = clang++
#MDTMP Should not be default UNIT_TEST_ACTIVATED
CPPFLAGS = -std=c++0x -g -Wall -MD -MP -I. -I../third_party/include -DUNIT_TEST_ACTIVATED

# -------------------- Functions --------------------
#MDTMP add comment...
define OBJECTS_TEMPLATE
  $(1) = $(addprefix $(BUILD_PATH), $(2:.cpp=.o))
  ALL_OBJECTS += $(addprefix $(BUILD_PATH), $(2:.cpp=.o))
endef

#MDTMP add comment...
define TARGET_TEMPLATE
ALL_TARGET += $(1)
$(1) : $(2)
	$(CC) $(2) $(LIBS) -o $(1)
endef

# -------------------- All sources list --------------------
# common

# base
# logger
CURRENT_OBJECTS_NAME = BASE_LOGGER_OBJECTS
include common/base/logger/log_src.mk

# init
CURRENT_OBJECTS_NAME = BASE_INIT_OBJECTS
include common/base/init/init_src.mk
BASE_INIT_OBJECTS += $(BASE_LOGGER_OBJECTS)

# network
# MDTMP Move to another directy.
CURRENT_OBJECTS_NAME = NETWORK_OBJECTS
include common/server/network_src.mk
# server
CURRENT_OBJECTS_NAME = SERVER_OBJECTS
include common/server/server_src.mk
# client
CURRENT_OBJECTS_NAME = CLIENT_OBJECTS
include common/client/client_src.mk

# json
CURRENT_OBJECTS_NAME = JSON_OBJECTS
include common/json/json_src.mk
JSON_OBJECTS += $(BASE_INIT_OBJECTS)

# -------------------- Targets --------------------
# common
# test_server
CURRENT_DEPENDANCY = $(NETWORK_OBJECTS) $(SERVER_OBJECTS)
TEST_SERVER = $(BUILD_PATH)test_server
CURRENT_TARGET = $(TEST_SERVER)
include common/server/server_exec.mk
# test_client
CURRENT_DEPENDANCY = $(NETWORK_OBJECTS) $(CLIENT_OBJECTS)
TEST_CLIENT = $(BUILD_PATH)test_client
CURRENT_TARGET = $(TEST_CLIENT)
include common/client/client_exec.mk
# hello_world_unit_test
CURRENT_DEPENDANCY = 
TEST_HELLO_WORLD = $(BUILD_PATH)hello_world_test
CURRENT_TARGET = $(TEST_HELLO_WORLD)
include common/unit_test/hello_world_exec.mk
# test_json
CURRENT_DEPENDANCY =  $(JSON_OBJECTS)
TEST_JSON_UTILS = $(BUILD_PATH)test_json_utils
CURRENT_TARGET = $(TEST_JSON_UTILS)
include common/json/json_exec.mk

#base init
CURRENT_DEPENDANCY =  $(BASE_INIT_OBJECTS)
TEST_BASE_INIT = $(BUILD_PATH)test_base_init
CURRENT_TARGET = $(TEST_BASE_INIT)
include common/base/init/init_exec.mk

# tools
CURRENT_DEPENDANCY = $(JSON_OBJECTS)
TOOLS_GET_JSON_REQUEST = $(BUILD_PATH)getJsonRequest
CURRENT_TARGET = $(TOOLS_GET_JSON_REQUEST)
include common/tools/getJsonRequest/json_request_exec.mk

# random
# http test
CURRENT_DEPENDANCY = 
HTTP_TEST = $(BUILD_PATH)http_test
CURRENT_TARGET = $(HTTP_TEST)
include random/test/asio/http/http_exec.mk

# -------------------- Targets rules --------------------
# All target
all: $(ALL_TARGET)

network: $(TEST_SERVER) $(TEST_CLIENT)

hello_world: $(TEST_HELLO_WORLD)

json: $(TEST_JSON_UTILS) $(TOOLS_GET_JSON_REQUEST)

test: $(TEST_JSON_UTILS) $(TEST_BASE_INIT)

random_http: $(HTTP_TEST)


# -------------------- Generic rules --------------------
# All dependency
PREREQUISITES = $(addprefix $(BUILD_PATH), $(ALL_OBJECTS:.o=.d))

$(BUILD_PATH)%.o: %.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@

-include $(PREREQUISITES)

# -------------------- Clean up --------------------
clean:
	@echo "Cleaning targets"
	-rm -f $(ALL_TARGET)
	@echo "Cleaning objects"
	-rm -f $(ALL_OBJECTS)
	@echo "Cleaning prerequisites"
	-rm -f $(PREREQUISITES)

# -------------------- FIXME --------------------
# common

# network
#MDTMP NETWORK_MESSAGE_SOURCES = common/server/test_network_message.cpp
#MDTMP NETWORK_MESSAGE_OBJECTS = $(addprefix $(BUILD_PATH), $(NETWORK_MESSAGE_SOURCES:.cpp=.o))
#MDTMP ALL_OBJECTS += $(NETWORK_MESSAGE_OBJECTS)

#MDTMP NETWORK_MESSAGE_TARGET = $(BUILD_PATH)test_message
#MDTMP ALL_TARGET += $(NETWORK_MESSAGE_TARGET)
#MDTMP CURRENT_DEPENDANCY = $(NETWORK_MESSAGE_OBJECTS)
#MDTMP $(NETWORK_MESSAGE_TARGET): $(CURRENT_DEPENDANCY)
	#MDTMP $(CC) $(CURRENT_DEPENDANCY) $(LIBS) -o $@


# server
#SERVER_MAIN_SOURCES = common/server/main.cpp
#CURRENT_OBJECTS_NAME = SERVER_MAIN_OBJECTS
#$(eval $(call OBJECTS_TEMPLATE, $(CURRENT_OBJECTS_NAME), $(SERVER_MAIN_SOURCES)))

#SERVER_MAIN_TARGET = $(BUILD_PATH)server
#CURRENT_DEPENDANCY = $(NETWORK_OBJECTS) $(SERVER_OBJECTS) $(SERVER_MAIN_OBJECTS) 
#$(eval $(call TARGET_TEMPLATE, $(SERVER_MAIN_TARGET), $(CURRENT_DEPENDANCY)))

