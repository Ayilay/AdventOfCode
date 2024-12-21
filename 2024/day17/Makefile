reqs = cpu.c cpu.h main.c problem_solver.h

executables = part1 part2

# Force 32-bit compilation, it makes debugging easier
# You can see the pointer addresses more nicely
OPTS += -m32
OPTS += -ggdb3

OPTS += -I ./Unity/src

SRCS += ./cpu.c
SRCS += main.c

UNITY += ./Unity/src/unity.c

all: $(executables)

%: $(reqs) %.c
	gcc $(OPTS) -o $@ $(SRCS) $@.c -lm 

test: $(reqs) test.c
	gcc $(OPTS) -o $@ cpu.c test.c $(UNITY) -lm 

clean:
	rm -f $(executables)
