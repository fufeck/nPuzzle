#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ftaffore <ftaffore@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2013/11/20 15:15:50 by ftaffore          #+#    #+#              #
#    Updated: 2015/01/18 16:24:19 by chery            ###   ########.fr        #
#                                                                              #
#******************************************************************************#

NAME		=	puzzle

CC			=	g++

RM			=	rm -rf

INCLUDES	=	-Iincludes

LIBS		=	-std=c++0x -O2 -Wno-write-strings

CFLAGS		=	-Werror -Wall -Wextra -std=c++0x -O2 -Wno-write-strings

SRCS		=	srcs/main.cpp			\
				srcs/Npuzzle.cpp				
				
OBJS		=	$(SRCS:.cpp=.o)

all			:	$(NAME)

$(NAME)		:	$(OBJS)
				$(CC) -o $@ $(CFLAGS) $(OBJS) $(LIBS) $(INCLUDES)

.cpp.o		:
				$(CC) -o $@ $(CFLAGS) $(INCLUDES) -c $<

clean		:
				$(RM) $(OBJS)

fclean		:	clean
				$(RM) $(NAME)

re			:	fclean all

.PHONY		:	all clean fclean re
