CC	?= gcc
RM	= rm -f
AR	= ar

CFLAGS	+= -ansi -pedantic -Wall -Werror -Wextra -Imulib -fpic
LDFLAGS	+= -shared

TARGET	= libmu

SRCS	= map.c \
			vec.c
OBJS	= $(addprefix mulib/, $(SRCS:.c=.o))

all: $(TARGET).a $(TARGET).so

$(TARGET).so: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

$(TARGET).a: $(OBJS)
	$(AR) rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(TARGET).a $(TARGET).so

re: fclean all

.PHONY: all clean fclean re
