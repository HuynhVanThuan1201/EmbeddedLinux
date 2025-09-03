CUR_DIR := .
INC_DIR := $(CUR_DIR)/inc
SRC_DIR := $(CUR_DIR)/src
BIN_DIR := $(CUR_DIR)/bin
OBJ_DIR := $(CUR_DIR)/obj
LIB_DIR := $(CUR_DIR)/lib
LIB_STATIC_DIR := $(LIB_DIR)/static
LIB_SHARED_DIR := $(LIB_DIR)/shared

LIB_NAME := strutils
OBJ_SHARED_NAME = strutilsShared
OBJ_STATIC_NAME = strutilsStatic

create_obj:
	gcc -c $(SRC_DIR)/main.c -o $(OBJ_DIR)/main.o -I $(INC_DIR)
	gcc -c -fPIC $(SRC_DIR)/strutils.c -o $(OBJ_DIR)/$(OBJ_SHARED_NAME).o -I $(INC_DIR) 
	gcc -c $(SRC_DIR)/strutils.c -o $(OBJ_DIR)/$(OBJ_STATIC_NAME).o -I $(INC_DIR) 

create_lib:
	gcc -shared $(OBJ_DIR)/$(OBJ_SHARED_NAME).o -o $(LIB_SHARED_DIR)/lib$(LIB_NAME).so
	ar rcs $(LIB_STATIC_DIR)/lib$(LIB_NAME).a $(OBJ_DIR)/$(OBJ_STATIC_NAME).o 

all: create_obj create_lib
	gcc $(OBJ_DIR)/main.o -L$(LIB_SHARED_DIR) -l$(LIB_NAME) -o $(BIN_DIR)/main_shared
	gcc $(OBJ_DIR)/main.o -L$(LIB_STATIC_DIR) -l$(LIB_NAME) -o $(BIN_DIR)/main_static

clean:
	rm -rf $(OBJ_DIR)/*
	rm -rf $(LIB_SHARED_DIR)/*
	rm -rf $(LIB_STATIC_DIR)/*
	rm -rf $(BIN_DIR)/*

