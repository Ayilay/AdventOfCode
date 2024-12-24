from collections import deque
import copy
import sys

INIT_NODE = None
FINAL_NODE = None

nlines = 0
origNodes = None
allNodes = None

class Node:
    def __init__(self):
        self.isObstacle = False

        # BFS helpers
        self.prev = None
        self.isVisited  = False

    def __repr__(self):
        if self.isObstacle:
            return '#'
        else:
            return '.'

def printGrid( grid ):
    for r in grid:
        print( r )

def neighbors( grid, rctup: (int, int) ):
    neigh = []

    (r,c) = rctup

    if( r > 0 ):
        e = grid[r-1][c]
        if not e.isObstacle and not e.isVisited: neigh.append( (r-1, c) )

    if( c > 0 ):
        e = grid[r][c-1]
        if not e.isObstacle and not e.isVisited: neigh.append( (r, c-1) )

    if( r < len(grid[0])-1 ):
        e = grid[r+1][c]
        if not e.isObstacle and not e.isVisited: neigh.append( (r+1, c) )

    if( c < len(grid[0])-1 ):
        e = grid[r][c+1]
        if not e.isObstacle and not e.isVisited: neigh.append( (r, c+1) )

    return neigh

def getNode( grid, rc ):
    try:
        return grid[rc[0]][rc[1]]
    except IndexError:
        print( f"Bad index {rc}" )

def doBFS( grid ):
    deadloop = DIM*DIM + DIM

    nodeq = deque()
    nodeq.append( INIT_NODE )

    while( len(nodeq) > 0 ):
        node = nodeq.popleft()
        if node == FINAL_NODE:
            return True

        neigh = neighbors( allNodes, node )

        for n in neigh:
            getNode(allNodes,n).prev = node
            getNode(allNodes,n).isVisited = True
            nodeq.append( n )

        deadloop -= 1
        if deadloop == 0:
            raise RuntimeError( "Too many loop iterations" )

    return False

def solveMaze( grid ):
    deadloop = DIM*DIM

    doBFS( grid )

    nsteps = 0
    node = FINAL_NODE
    while node != INIT_NODE:
        nsteps += 1
        node = getNode( allNodes, node ).prev

        deadloop -= 1
        if deadloop == 0:
            raise RuntimeError( "Too many loop iterations" )

    print( f"{nsteps=}")
    return nsteps


def SOLVER_Init( argv ):
    pass

def findGlitchCandidates( grid ):
    global DIM

    candidates = []

    for r in range( 1, DIM-1 ):
        for c in range( 1, DIM-1 ):
            if not getNode(grid, (r, c)).isObstacle:
                continue

            if getNode(grid, (r-1, c)).isObstacle and getNode(grid, (r+1, c)).isObstacle:
                candidates.append( (r,c) )
            elif getNode(grid, (r, c-1)).isObstacle and getNode(grid, (r, c+1)).isObstacle:
                candidates.append( (r,c) )

    return candidates


def SOLVER_ProcessLine( line: str ):
    global DIM
    global nlines
    global origNodes
    global INIT_NODE
    global FINAL_NODE

    line = line.strip()

    if origNodes is None:
        DIM = len( line )
        origNodes = [[Node() for _ in range(DIM)] for _ in range(DIM)]

    for i,c in enumerate( line ):
        if c == '#':
            origNodes[nlines][i].isObstacle = True
        elif c == 'S':
            INIT_NODE = (nlines, i)
        elif c == 'E':
            FINAL_NODE = (nlines, i)

    nlines += 1

def SOLVER_PrintSolution():
    global origNodes
    global allNodes

    # First, obtain the baseline time w/o glitches
    allNodes = copy.deepcopy( origNodes )
    baseline = solveMaze( allNodes )

    candidates = findGlitchCandidates( origNodes )

    # Only count glitches with significant time savings
    thresh = 100

    goodGlitches = []

    #for cand in candidates:

    #    allNodes = copy.deepcopy( origNodes )
    #    getNode( allNodes, cand ).isObstacle = False

    #    newTime = solveMaze( allNodes )
    #    savings = baseline - newTime
    #    if savings >= thresh:
    #        goodGlitches.append( cand )

    #print( goodGlitches )
    #print( f"{len(goodGlitches)} candidates that save {thresh} picoseconds")

    print( f"{len(candidates)=}")

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
