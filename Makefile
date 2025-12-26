NAME = webserv

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRC_DIR = src
INC_DIR = inc
OBJ_DIR = obj

SRCS = $(SRC_DIR)/main.cpp \
       $(SRC_DIR)/Tokenizer.cpp \
       $(SRC_DIR)/Parser.cpp \
       $(SRC_DIR)/ServerConfig.cpp \
       $(SRC_DIR)/LocationConfig.cpp \
       $(SRC_DIR)/StringUtils.cpp \
       $(SRC_DIR)/FileUtils.cpp \
       $(SRC_DIR)/Logger.cpp \
       $(SRC_DIR)/ConfigFactory.cpp

OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

INCLUDES = -I$(INC_DIR)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re