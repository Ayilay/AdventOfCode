
srcs = union_find.c sc_heap.c sc_map.c
reqs = $(srcs) problem_solver.h 
mainreqs = $(reqs) main.c 
mainsrcs = $(srcs) main.c 

executables = part1 part2

# Unless we are debugging, we want optimized build
OPTS += -O2

# If we ARE debugging:
# - Force 32-bit compilation, it makes debugging easier
# - You can see the pointer addresses more nicely
#OPTS += -m32
#OPTS += -ggdb3

UNITY_PATH=../../Unity/src
TEST_OPTS  += -I$(UNITY_PATH)

UNITY_SRCS   = $(wildcard $(UNITY_PATH)/*.c)

all: $(executables)

%: $(mainreqs) %.c
	gcc $(OPTS) -o $@ $(mainsrcs) $@.c -lm 

test: test.c $(mainreqs) $(UNITY_SRCS)
	gcc $(OPTS) $(TEST_OPTS) -o $@ sc_list.c sc_heap.c test.c $(UNITY_SRCS) -lm

clean:
	rm -f $(executables)
