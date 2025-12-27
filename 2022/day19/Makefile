reqs = main.c problem_solver.h Makefile common.h

executables = part1 part2

# Unless we are debugging, we want optimized build
#OPTS += -O2
OPTS += -Wuninitialized

# If we ARE debugging:
# - Force 32-bit compilation, it makes debugging easier
# - You can see the pointer addresses more nicely
#OPTS += -O0
#OPTS += -m32
#OPTS += -ggdb3
OPTS += -std=c2x

UNITY_PATH=../../Unity/src
TEST_OPTS  += -I$(UNITY_PATH)

UNITY_SRCS   = $(wildcard $(UNITY_PATH)/*.c)

all: $(executables)

%: $(reqs) %.c
	gcc $(OPTS) -o $@ main.c $@.c -lm 

test: test.c $(UNITY_SRCS) $(reqs)
	gcc $(OPTS) $(TEST_OPTS) -o $@ test.c $(UNITY_SRCS) -lm

clean:
	rm -f $(executables)
