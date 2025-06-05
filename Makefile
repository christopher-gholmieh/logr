# Written by: Christopher Gholmieh
# Include:
COMPILER_INCLUDE=-Iinclude/

# Source:
COMPILER_SOURCE=source/json.c

# Output:
COMPILER_OUTPUT=-o logr

# Flags:
COMPILER_FLAGS=-Wall -Wextra -Wpedantic -Werror

all:
	gcc $(COMPILER_INCLUDE) $(COMPILER_SOURCE) $(COMPILER_FLAGS) main.c $(COMPILER_OUTPUT)
