################################################################################
#Shell variables:                                                              #
################################################################################
CPP_FILES=./src/NCon/main.cpp ./src/headers/*.cpp
EXTRA_LIBRARIES=-lpthread
BUILD_DIR=./build
TMP_DIR=/tmp/bins

################################################################################
#makefile targets:                                                             #
################################################################################
all: cd clear echo env help ls md rm
	mkdir $(BUILD_DIR)
	g++ -ggdb -Wall $(CPP_FILES) -Xlinker $(EXTRA_LIBRARIES) -o $(BUILD_DIR)/ncon;
	$(BUILD_DIR)/ncon --install
	mv $(TMP_DIR)/* $(BUILD_DIR)/ncon/bin
	rm -rf $(TMP_DIR)

#############################
#Variables for bins         #
#############################
BIN_DIR=./src/bins
BIN_CPP_FILES=$(BIN_DIR)/NCon.cpp ./src/headers/*.cpp

#############################
cd:
#############################
	mkdir -p $(TMP_DIR)
	g++ -ggdb -Wall $(BIN_DIR)/cd/main.cpp $(BIN_CPP_FILES) -Xlinker $(EXTRA_LIBRARIES) -o $(TMP_DIR)/cd


#############################
clear:
#############################
	mkdir -p $(TMP_DIR)
	g++ -ggdb -Wall $(BIN_DIR)/clear/main.cpp $(BIN_CPP_FILES) -Xlinker $(EXTRA_LIBRARIES) -o $(TMP_DIR)/clear
	
	
#############################
echo:
#############################
	mkdir -p $(TMP_DIR)
	g++ -ggdb -Wall $(BIN_DIR)/echo/main.cpp $(BIN_CPP_FILES) -Xlinker $(EXTRA_LIBRARIES) -o $(TMP_DIR)/echo
	
	
#############################
env:
#############################
	mkdir -p $(TMP_DIR)
	g++ -ggdb -Wall $(BIN_DIR)/env/main.cpp $(BIN_CPP_FILES) -Xlinker $(EXTRA_LIBRARIES) -o $(TMP_DIR)/environ
	
	
#############################
help:
#############################
	mkdir -p $(TMP_DIR)
	g++ -ggdb -Wall $(BIN_DIR)/help/main.cpp $(BIN_CPP_FILES) -Xlinker $(EXTRA_LIBRARIES) -o $(TMP_DIR)/help


#############################
ls:
#############################
	mkdir -p $(TMP_DIR)
	g++ -ggdb -Wall $(BIN_DIR)/ls/main.cpp $(BIN_CPP_FILES) -Xlinker $(EXTRA_LIBRARIES) -o $(TMP_DIR)/dir
	
	
#############################
md:
#############################
	mkdir -p $(TMP_DIR)
	g++ -ggdb -Wall $(BIN_DIR)/md/main.cpp $(BIN_CPP_FILES) -Xlinker $(EXTRA_LIBRARIES) -o $(TMP_DIR)/md
	
	
#############################
rm:
#############################
	mkdir -p $(TMP_DIR)
	g++ -ggdb -Wall $(BIN_DIR)/rm/main.cpp $(BIN_CPP_FILES) -Xlinker $(EXTRA_LIBRARIES) -o $(TMP_DIR)/rm


#############################
#Utilities:                 #
#############################

#############################
clean:
#############################
	rm -rf $(BUILD_DIR)
	rm -rf $(TMP_DIR)

#############################
cleanbuild: clean all
#############################
