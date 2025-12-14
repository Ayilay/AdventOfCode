reqs = main.c problem_solver.h isIllegal.c

executables = part1 part2

# Force 32-bit compilation, it makes debugging easier
# You can see the pointer addresses more nicely
OPTS += -m32
OPTS += -ggdb3

UNITY_PATH=../../Unity/src
TEST_OPTS  += -I$(UNITY_PATH)

UNITY_SRCS   = $(wildcard $(UNITY_PATH)/*.c)

all: $(executables)

%: $(reqs) %.c
	gcc $(OPTS) -o $@ main.c $@.c isIllegal.c -lm 

test: test.c $(reqs) $(UNITY_SRCS)
	gcc $(OPTS) $(TEST_OPTS) -o $@ test.c $(UNITY_SRCS) isIllegal.c -lm

clean:
	rm -f $(executables)

