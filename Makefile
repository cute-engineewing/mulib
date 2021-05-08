CC	?= gcc
RM	= rm -f
AR	= ar

CFLAGS	+= -ansi -pedantic -pedantic-errors -Wall -Werror -Wextra -fpic -Iinc \
			-Wno-variadic-macros
LDFLAGS	+=

test: CFLAGS	+=  -Iinc --coverage
test: LDFLAGS	+= -lcmocka --coverage
test: CC	= gcc

TARGET	= libmu

SRCS	= map.c \
			vec.c \
			scan.c \
			log.c

OBJS	= $(addprefix src/, $(SRCS:.c=.o))

TEST_SRCS	= test.c
TEST_OBJS	= $(addprefix tests/, $(TEST_SRCS:.c=.o))

all: $(TARGET).a $(TARGET).so

$(TARGET).so: $(OBJS)
	$(CC) -o $@ $^ -shared $(LDFLAGS)

$(TARGET).a: $(OBJS)
	$(AR) rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

test: $(TARGET).a $(TEST_OBJS)
	$(CC) -o $@ $(TEST_OBJS) $(LDFLAGS) -L. -l:libmu.a
	@./$@

clean:
	$(RM) $(OBJS)
	$(RM) $(TEST_OBJS:.o=.gcda)
	$(RM) $(TEST_OBJS:.o=.gcno)
	$(RM) $(TEST_OBJS)
	$(RM) $(TARGET).a $(TARGET).so

re: clean all

.PHONY: all clean clean re test
