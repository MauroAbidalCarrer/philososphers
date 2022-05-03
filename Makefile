# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: maabidal <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/05/03 17:23:21 by maabidal          #+#    #+#              #
#    Updated: 2022/05/03 17:30:21 by maabidal         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS 	=	main.c\
			philosophize.c\
			utils.c

NAME	=	philo 
CFLAGS	=	-Wall -Wextra -Werror
RM		=	rm -f
OBJS	=	$(SRCS:.c=.o)

all : $(NAME)

$(NAME) : $(OBJS) $(MAINOBJS)
	$(CC) -o $(NAME) $(OBJS) $(MAINOBJS) -pthread

.o : .c
	$(CC) $(CFLAGS) -c $< -o $@ -I ./

clean : 
	$(RM) $(OBJS) $(BNSOBJS) $(MAINOBJS)

fclean : clean
	$(RM) $(NAME)

re :    fclean all

.PHONY : all clean fclean re 
