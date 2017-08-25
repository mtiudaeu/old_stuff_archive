OBJ_DIR=bin

all: $(OBJ_DIR) test_logger test_init

test_logger: include/mat-utils/logger/log.hpp include/mat-utils/unit_test/unit_test.hpp test/test_logger.cpp
	g++ -std=c++11 -Iinclude -lgtest -std=c++11 test/test_logger.cpp -o $(OBJ_DIR)/test_logger

test_init: include/mat-utils/logger/log.hpp include/mat-utils/init/init.hpp include/mat-utils/unit_test/unit_test.hpp test/test_logger.cpp
	g++ -std=c++11 -Iinclude -lgtest -std=c++11 test/test_init.cpp -o $(OBJ_DIR)/test_init

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean :
	rm $(OBJ_DIR)/test_logger


