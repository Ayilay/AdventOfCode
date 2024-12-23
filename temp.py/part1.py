import sys

def SOLVER_Init( argv ):
    pass

def SOLVER_ProcessLine( line: str ):
    pass

def SOLVER_PrintSolution():
    pass


def main( argv ):
    filename = "input.txt"

    SOLVER_Init( argv )

    if len(argv) > 1:
        filename = argv[1]

    with open(filename, "r") as infile:
        for line in infile:
            SOLVER_ProcessLine( line )

    SOLVER_PrintSolution()

if __name__ == "__main__":
    main( sys.argv )
