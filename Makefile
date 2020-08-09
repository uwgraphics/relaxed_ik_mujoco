ROOT_DIR=$(dir $(realpath $(firstword $(MAKEFILE_LIST))))
LIB_PATH=$(ROOT_DIR)relaxed_ik_core/target/debug

$(info $(LIB_PATH))

COMMON=-O2 -I../include -L../bin -std=c++11 -mavx -pthread -Wl,-rpath,'$$ORIGIN'

all:
	g++ $(COMMON) -L$(LIB_PATH) -Wl,-rpath=${LIB_PATH} testRelaxedIK.cpp -o ../bin/testRelaxedIK -lmujoco200 -lGL -lglew ../bin/libglfw.so.3 -lrelaxed_ik_lib 
