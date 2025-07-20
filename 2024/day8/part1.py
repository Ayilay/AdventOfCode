from itertools import combinations
import sys

row = 0
freqs = {}

antinodes = set()

def SOLVER_Init( argv ):
    pass

def SOLVER_ProcessLine( line: str ):
    global row

    line = line.strip()


    for col,freq in enumerate(line):
        if freq == '.': continue
        
        if freq not in freqs:
            freqs[freq] = []

        freqs[freq].append( (row,col) )
    
    row += 1

def findAntinode( antinodes, pair ):
    dim = row

    f0, f1 = pair

    # Dim-access helpers
    X = 1
    y = 0

    dy = abs(f0[X] - f1[X])  # Row = X (dim 0)
    dx = abs(f0[Y] - f1[Y])  # Col = Y (dim 1)

    minx = min( f0[X], f1[X] )
    maxx = max( f0[X], f1[X] )
    miny = min( f0[Y], f1[Y] )
    maxy = max( f0[Y], f1[Y] )




def SOLVER_PrintSolution():
    #print( freqs )

    for freq, freqList in freqs.items():
        #print( freqList )
        for pair in combinations( freqList, 2 ):
            print( f"trying antinodes of {freq} at nodes {pair}")
            findAntinode( antinodes, pair )



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
