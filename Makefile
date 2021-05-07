CC	?= gcc
RM	= rm -f
AR	= ar

CFLAGS	+= -ansi -pedantic -Wall -Werror -Wextra -fpic -I.
LDFLAGS	+= -shared

test: CFLAGS	+=  -I. --coverage
test: LDFLAGS	= -lcmocka --coverage
test: CC	= gcc

TARGET	= libmu

SRCS	= map.c \
			vec.c \
			scan.c

OBJS	= $(addprefix mulib/, $(SRCS:.c=.o))

TEST_SRCS	= test.c
TEST_OBJS	= $(addprefix tests/, $(TEST_SRCS:.c=.o))

all: $(TARGET).a $(TARGET).so

$(TARGET).so: $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(TARGET).a: $(OBJS)
	$(AR) rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

test: $(TARGET).a $(TEST_OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)
	@./$@

clean:
	$(RM) $(OBJS)
	$(RM) $(TEST_OBJS:.o=.gcda)
	$(RM) $(TEST_OBJS:.o=.gcno)
	$(RM) $(TEST_OBJS)

fclean: clean
	$(RM) $(TARGET).a $(TARGET).so

re: fclean all

.PHONY: all clean fclean re
