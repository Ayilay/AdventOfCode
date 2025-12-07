reqs = main.c problem_solver.h sc_map.c sc_map.h

executables = part1 part2

# Force 32-bit compilation, it makes debugging easier
# You can see the pointer addresses more nicely
OPTS += -m32
OPTS += -ggdb3

UNITY_PATH=../../Unity/src
TEST_OPTS  += -I$(UNITY_PATH)

SRCS += sc_map.c

UNITY_SRCS   = $(wildcard $(UNITY_PATH)/*.c)

all: $(executables)

%: $(reqs) %.c
	gcc $(OPTS) -o $@ main.c $@.c $(SRCS) -lm 

test: test.c $(UNITY_SRCS)
	gcc $(OPTS) $(TEST_OPTS) -o $@ test.c $(UNITY_SRCS) -lm

clean:
	rm -f $(executables)
