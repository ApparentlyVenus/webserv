# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yitani <yitani@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/01/15 20:00:00 by yitani            #+#    #+#              #
#    Updated: 2026/01/16 17:41:52 by yitani           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRC_DIR = src
INC_DIR = inc
OBJ_DIR = obj

SRCS = $(SRC_DIR)/main.cpp \
       $(SRC_DIR)/Server.cpp \
       $(SRC_DIR)/Client.cpp \
       $(SRC_DIR)/Request.cpp \
       $(SRC_DIR)/Response.cpp \
       $(SRC_DIR)/ResonseStatus.cpp \
       $(SRC_DIR)/Handlers.cpp \
       $(SRC_DIR)/ServerConfig.cpp \
       $(SRC_DIR)/LocationConfig.cpp \
       $(SRC_DIR)/ConfigFactory.cpp \
       $(SRC_DIR)/Parser.cpp \
       $(SRC_DIR)/Tokenizer.cpp \
       $(SRC_DIR)/SessionManager.cpp \
       $(SRC_DIR)/StringUtils.cpp \
       $(SRC_DIR)/FileUtils.cpp \
       $(SRC_DIR)/Logger.cpp

OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

HEADERS = $(INC_DIR)/Server.hpp \
          $(INC_DIR)/Client.hpp \
          $(INC_DIR)/Request.hpp \
          $(INC_DIR)/Response.hpp \
          $(INC_DIR)/ResponseStatus.hpp \
          $(INC_DIR)/Handlers.hpp \
          $(INC_DIR)/ServerConfig.hpp \
          $(INC_DIR)/LocationConfig.hpp \
          $(INC_DIR)/ConfigFactory.hpp \
          $(INC_DIR)/Parser.hpp \
          $(INC_DIR)/Tokenizer.hpp \
          $(INC_DIR)/SessionManager.hpp \
          $(INC_DIR)/StringUtils.hpp \
          $(INC_DIR)/FileUtils.hpp \
          $(INC_DIR)/Logger.hpp

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS)
	mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re