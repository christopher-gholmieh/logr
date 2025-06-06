# Written by: Christopher Gholmieh
# Include:
COMPILER_INCLUDE=-Iinclude/

# Source:
COMPILER_SOURCE=source/logr_arguments.c source/logr_hashmap.c source/logr_logger.c source/logr_json.c

# Output:
COMPILER_OUTPUT=-o logr

# Flags:
COMPILER_FLAGS=-Wall -Wextra -Wpedantic -Werror

all:
	gcc $(COMPILER_INCLUDE) $(COMPILER_SOURCE) $(COMPILER_FLAGS) main.c $(COMPILER_OUTPUT)
