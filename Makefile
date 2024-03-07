# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aducobu <aducobu@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/10 16:29:29 by dlu               #+#    #+#              #
#    Updated: 2024/03/07 14:06:43 by aducobu          ###   ########.fr        #
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
			Cgi.cpp \
			Config.cpp \
			Launcher.cpp \
			Utils.cpp \
			Response.cpp \
		}
OBJS = $(SRCS:sources/%.cpp=objects/%.o)
DEPS = $(SRCS:sources/%.cpp=objects/%.d)
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