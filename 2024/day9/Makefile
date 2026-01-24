# Rebuild if any of these change
reqs = main.c problem_solver.h Makefile

executables = part1 part2

# If using MORE source files, add them here
EXTRA_SRC = utils.c sc_list.c
reqs += $(EXTRA_SRC)

# Always use these options
OPTS += -Wuninitialized

# If we ARE debugging:
# - Force 32-bit compilation, it makes debugging easier
# - You can see the pointer addresses more nicely
#OPTS += -m32
#OPTS += -O2
#OPTS += -ggdb3

# Unless we are debugging, we want optimized build
OPTS += -O2

UNITY_PATH=../../Unity/src
TEST_OPTS  += -I$(UNITY_PATH)

UNITY_SRCS   = $(wildcard $(UNITY_PATH)/*.c)

all: $(executables)

# Recipe for part1 and part2
%: $(reqs) %.c
	gcc $(OPTS) -o $@ main.c $@.c $(EXTRA_SRC) -lm 

test: test.c $(UNITY_SRCS) $(reqs)
	gcc $(OPTS) $(TEST_OPTS) -o $@ test.c $(UNITY_SRCS) $(EXTRA_SRC) -lm

clean:
	rm -f $(executables)
