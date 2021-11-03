#    fneotensor - Tensor manipulation program
#    Copyright (C) 2021 lieu02tard
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <https://www.gnu.org/licenses/>.
CCX		:=g++

NFLAGS	:=-Wall  -std=c++17
OFLAGS	:=-o 
CCFLAGS	:=-c
LFLAGS	:=-L /usr/lib 
BUILD	:=./build
TARGET	:=main
APP_DIR	:=.
DEB_DIR	:=$(BUILD)/debug
OBJ_DIR	:=$(BUILD)/obj
INC		:=-I . -I ~/lib
SRC_DIR	:=.
SRC		:=$(wildcard ./*.cpp)
OBJ		:=$(SRC:%.cpp=$(OBJ_DIR)/%.o)
EX 		:=./ex_obj
all: build $(APP_DIR)/$(TARGET)

$(OBJ_DIR)/%.o: %.cpp
		@mkdir -p $(@D)
		$(CCX) $(NFLAGS) $(INC) $(CCFLAGS) $< $(OFLAGS) $@

$(APP_DIR)/$(TARGET):$(OBJ)
		@mkdir -p $(@D)
		$(CCX) $(NFLAGS) $(OFLAGS) $(APP_DIR)/$(TARGET) $^  $(LFLAGS)
	 
.PHONY: all build debug execute clean
build:
		@mkdir -p $(APP_DIR)
		@mkdir -p $(OBJ_DIR)


debug: NFLAGS+=-g
debug: all

execute :all
execute: $(APP_DIR)/$(TARGET)

clean:
		-@rm -r $(OBJ_DIR)/*.o
