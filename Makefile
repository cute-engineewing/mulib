DIRECTORY_GUARD=mkdir -p $(@D)

CC	?= gcc
RM	= rm -f
AR	= ar

CFLAGS	+= -ansi -pedantic -Wall -Werror -Wextra -fpic -Iinc
LDFLAGS	+=

test: CFLAGS	+=  -Iinc --coverage
test: LDFLAGS	+= -lcmocka --coverage
test: CC	= gcc

TARGET	= libmu

STATIC_TARGET = build/static/$(TARGET).a
SHARED_TARGET = build/shared/$(TARGET).so

SRCS	= map.c \
			vec.c \
			scan.c

OBJS	= $(addprefix build/, $(SRCS:.c=.o))

TEST_SRCS	= test.c
TEST_OBJS	= $(addprefix tests/, $(TEST_SRCS:.c=.o))

all: $(STATIC_TARGET) $(SHARED_TARGET)

$(SHARED_TARGET): $(OBJS)
	$(DIRECTORY_GUARD)
	$(CC) -o $@ $^ -shared $(LDFLAGS)

$(STATIC_TARGET): $(OBJS)
	$(DIRECTORY_GUARD)
	$(AR) rcs $@ $^

build/%.o: src/%.c
	$(DIRECTORY_GUARD)
	$(CC) $(CFLAGS) -c -o $@ $^

test: $(STATIC_TARGET) $(TEST_OBJS)
	$(DIRECTORY_GUARD)
	$(CC) -o $@ $^ $(LDFLAGS)
	@./$@

clean:
	$(RM) -r build
	$(RM) $(TEST_OBJS:.o=.gcda)
	$(RM) $(TEST_OBJS:.o=.gcno)
	$(RM) $(TEST_OBJS)

re: clean all

.PHONY: all clean clean re test
