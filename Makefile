BUILD_DIR ?= ./build
BUILD_DIR := $(patsubst %/,%,$(BUILD_DIR))

BUILD ?= prod

ifeq ($(BUILD), prod)
    NAME := rpn-x
    CFLAGS += -Wall -Wextra -Werror -O2 -DNDEBUG
else
    NAME := rpn-x-debug
    CFLAGS += -Wall -Wextra -O0 -g3 -DDEBUG
endif

CFLAGS += -pedantic -I. -MP -MMD

SRCS := $(shell find . -type f -name '*.c')
OBJS := $(addprefix $(BUILD_DIR)/, $(SRCS:.c=.o))
DEPS := $(OBJS:.o=.d)

all: $(NAME)

$(NAME): $(OBJS) 
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

$(BUILD_DIR)/%.o: %.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@
$(BUILD_DIR)/internal/%.o: %.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@
$(BUILD_DIR)/internal/config/%.o: %.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@
$(BUILD_DIR)/internal/builtin/%.o: %.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPS)

clean:
	rm -rf $(BUILD_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
