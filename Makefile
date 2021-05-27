SRC_DIR := src
OBJ_DIR := obj
H_DIR := include
BIN_DIR := bin
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
FLAGS := -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -licuuc -licuio
CPPFLAGS := -Wall -g -std=c++17 -I./$(H_DIR)

all: $(OBJ_FILES)
	g++ -o $(BIN_DIR)/main $^ $(FLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
ifneq ("$(wildcard $($(H_DIR)/%.h))","")
	g++ $(CPPFLAGS) $(FLAGS) -c -o $@ $< $(H_DIR)/%.h
else
	g++ $(CPPFLAGS) $(FLAGS) -c -o $@ $<
endif

clean:
	rm $(wildcard $(OBJ_DIR)/*.o) $(wildcard $(BIN_DIR)/*)
