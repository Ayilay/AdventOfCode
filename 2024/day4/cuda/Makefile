reqs = main.cu problem_solver.h

executables = part2

# Force 32-bit compilation, it makes debugging easier
# You can see the pointer addresses more nicely
#OPTS += -m32
#OPTS += -ggdb3
OPTS += -O2


all: $(executables)

%: $(reqs) %.cu
	nvcc $(OPTS) -o $@ main.cu $@.cu

clean:
	rm -f $(executables)
