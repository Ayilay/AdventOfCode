reqs = main.c problem_solver.h sc_heap.h sc_heap.c

executables = part1 part2

# Force 32-bit compilation, it makes debugging easier
# You can see the pointer addresses more nicely
OPTS += -m32
OPTS += -ggdb3


all: $(executables)

%: $(reqs) %.c
	gcc $(OPTS) -o $@ main.c sc_heap.c $@.c -lm 

clean:
	rm -f $(executables)
