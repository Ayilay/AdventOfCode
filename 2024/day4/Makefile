
# If these files change, then recompile the app
reqs = main.c problem_solver.h Makefile

executables = part1 part2

# Force 32-bit compilation, it makes debugging easier
# You can see the pointer addresses more nicely
OPTS += -m32
OPTS += -ggdb3

#OPTS += -O2


all: $(executables)

%: $(reqs) %.c
	gcc $(OPTS) -o $@ main.c $@.c -lm 

clean:
	rm -f $(executables)
