NAME := rpn-x
BUILD_DIR ?= ./build
BUILD_DIR := $(patsubst %/,%,$(BUILD_DIR))

DEV_BUILD_SUFIX :=

BUILD ?= prod

FORCE ?=

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

prod:
	@rm -f .debug
	@if [ ! -f .prod ]; then \
		$(MAKE) --no-print-directory BUILD=prod FORCE='$(NAME)' all; \
	else \
		$(MAKE) --no-print-directory BUILD=prod all; \
	fi

debug:
	@rm -f .prod
	@if [ ! -f .debug ]; then \
		$(MAKE) --no-print-directory BUILD=debug FORCE='$(NAME)' all; \
	else \
		$(MAKE) --no-print-directory BUILD=debug all; \
	fi
		
all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)
	@touch '.$(BUILD)'

$(BUILD_DIR)/%.o: %.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@
$(BUILD_DIR)/%-dev.o: %.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPS)

clean:
	rm -f .debug .prod
	rm -rf $(BUILD_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re prod debug $(FORCE)
