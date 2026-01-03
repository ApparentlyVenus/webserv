# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: odana <odana@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/01/02 00:46:14 by yitani            #+#    #+#              #
#    Updated: 2026/01/04 01:35:42 by odana            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

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
       $(SRC_DIR)/ConfigFactory.cpp \
       $(SRC_DIR)/Server.cpp \       
       $(SRC_DIR)/Request.cpp \      
       $(SRC_DIR)/Response.cpp \     
       $(SRC_DIR)/Handlers.cpp \     
       $(SRC_DIR)/ResponseStatus.cpp \
       $(SRC_DIR)/SessionManager.cpp 


OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

INCLUDES = $(INC_DIR)/main.cpp \
       $(INC_DIR)/Tokenizer.hpp \
       $(INC_DIR)/Parser.hpp \
       $(INC_DIR)/ServerConfig.hpp \
       $(INC_DIR)/LocationConfig.hpp \
       $(INC_DIR)/StringUtils.hpp \
       $(INC_DIR)/FileUtils.hpp \
       $(INC_DIR)/Logger.hpp \
       $(INC_DIR)/ConfigFactory.hpp \
       $(INC_DIR)/Server.hpp \       
       $(INC_DIR)/Request.hpp \      
       $(INC_DIR)/Response.hpp \     
       $(INC_DIR)/Handlers.hpp \     
       $(INC_DIR)/ResponseStatus.hpp \
       $(INC_DIR)/SessionManager.hpp 

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