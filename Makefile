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
			Request.cpp \
			Response.cpp \
			Location.cpp \
			Upload.cpp \
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

fclean: clean clean_cgi
	$(RM) $(NAME)

clean_cgi:
	rm -f .cgi.txt

re: fclean 
	make all

.PHONY: all clean fclean re clean_cgi