# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rmeriau <rmeriau@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/10 16:29:29 by dlu               #+#    #+#              #
#    Updated: 2024/03/04 14:52:21 by rmeriau          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98 -MMD
RM = rm -rf
NAME = webserv

SRCS = ${addprefix sources/, \
			main.cpp \
			Server.cpp \
			User.cpp \
			Pages.cpp \
			Config.cpp \
			Launcher.cpp \
		}
OBJS = $(SRCS:sources/%.cpp=objects/%.o)
DEPS = $(SRCS:.cpp=.d)
INCLUDE = -I includes

all: $(NAME)

$(NAME): $(OBJS) 
	$(CC) $(CFLAGS) ${INCLUDE} $(OBJS) -o $(NAME)

-include $(DEPS)

objects/%.o: sources/%.cpp
	mkdir -p $(dir $@)
	${CC} ${CFLAGS} ${INCLUDE} -c $< -o $@

clean:
	$(RM) objects

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re