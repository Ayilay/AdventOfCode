import sys

neighbors = dict()

def addNeighbor( clusterSet, key, neighbor ):
    if not key in clusterSet:
        clusterSet[key] = set()

    clusterSet[key].add( neighbor )



def SOLVER_Init( argv ):
    pass

def SOLVER_ProcessLine( line: str ):
    # Remove newline
    line = line.strip()

    comps = line.split('-')
    #print( f"Parsed {comps[0]} and {comps[1]}")

    addNeighbor( neighbors, comps[0], comps[1] )
    addNeighbor( neighbors, comps[1], comps[0] )


def SOLVER_PrintSolution():
    allFound = set()
    for a in neighbors:
        for b in (neighbors):
            if a == b: continue
            for c in neighbors:
                if a == c: continue

                # Check 1: all 3 computers are neighbors
                if a in neighbors[b] and b in neighbors[c] and c in neighbors[a]:
                    # Check 2: At least one computer begins with 't'
                    if a[0] == 't' or b[0] == 't' or c[0] == 't':
                        foundSet = frozenset({a, b, c})
                        if not foundSet in allFound:
                            allFound.add( foundSet )

    for foundSet in allFound:
        print( f"3-group of {foundSet}" )

    print( f"total: {len(allFound)}")

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
