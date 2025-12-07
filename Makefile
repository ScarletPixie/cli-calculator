NAME := rpn-x
BUILD_DIR ?= ./build
BUILD_DIR := $(patsubst %/,%,$(BUILD_DIR))

DEV_BUILD_SUFIX :=

BUILD ?= prod

ifeq ($(BUILD), prod)
    CFLAGS += -Wall -Wextra -Werror -O2 -DNDEBUG
else
    CFLAGS += -Wall -Wextra -O0 -g3 -DDEBUG
    DEV_BUILD_SUFIX := -dev
endif

CFLAGS += -pedantic -I. -MP -MMD

SRCS := $(shell find . -type f -name '*.c')
OBJS := $(addprefix $(BUILD_DIR)/, $(SRCS:.c=$(DEV_BUILD_SUFIX).o))
DEPS := $(OBJS:.o=.d)

all: $(NAME)

$(NAME): $(OBJS) 
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

$(BUILD_DIR)/%.o: %.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@
$(BUILD_DIR)/%-dev.o: %.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPS)

clean:
	rm -rf $(BUILD_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
