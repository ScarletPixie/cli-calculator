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

CFLAGS += -I. -I./core -MP -MMD

SRCS := main.c core/builtin.c
OBJS := $(addprefix $(BUILD_DIR)/, $(SRCS:.c=.o))
DEPS := $(OBJS:.o=.d)

all: $(NAME)

$(NAME): LDLIBS += $(addprefix -l, $(PLUGIN_LIBS)) 
$(NAME): $(OBJS) 
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

$(BUILD_DIR)/core/%.o: %.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPS)

clean:
	rm -rf $(BUILD_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
