from multiprocessing import Process, Queue

from collections import deque
import copy
import sys

INIT_NODE = None
FINAL_NODE = None

nlines = 0
origNodes = None
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

        neigh = neighbors( grid, node )

        for n in neigh:
            getNode(grid,n).prev = node
            getNode(grid,n).isVisited = True
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
        node = getNode( grid, node ).prev

        deadloop -= 1
        if deadloop == 0:
            raise RuntimeError( "Too many loop iterations" )

    print( f"{nsteps=}")
    return nsteps


def SOLVER_Init( argv ):
    pass

def findGlitchCandidates( grid ):
    global DIM

    candidates = Queue()

    for r in range( 1, DIM-1 ):
        for c in range( 1, DIM-1 ):
            if not getNode(grid, (r, c)).isObstacle:
                continue

            if getNode(grid, (r-1, c)).isObstacle and getNode(grid, (r+1, c)).isObstacle:
                candidates.put( (r,c) )
            elif getNode(grid, (r, c-1)).isObstacle and getNode(grid, (r, c+1)).isObstacle:
                candidates.put( (r,c) )

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

def SolverThread( origNodes, candidates, baseline, thresh ):

    goodGlitches = []

    while not candidates.empty():
        cand = candidates.get()
        print( f"trying {cand}" )

        nodes = []
        nodes = copy.deepcopy( origNodes )
        getNode( nodes, cand ).isObstacle = False

        newTime = solveMaze( nodes )
        savings = baseline - newTime
        if savings >= thresh:
            goodGlitches.append( cand )

    #print( goodGlitches )
    print( f"{len(goodGlitches)} candidates that save {thresh} picoseconds")

    return goodGlitches

def SOLVER_PrintSolution():
    global origNodes

    allNodes = []

    # First, obtain the baseline time w/o glitches
    allNodes = copy.deepcopy( origNodes )
    baseline = solveMaze( allNodes )

    candidates = findGlitchCandidates( origNodes )

    # Only count glitches with significant time savings
    thresh = 100

    goodGlitches = []

    NTHREADS = 8
    procs = []
    for i in range(NTHREADS):
        proc = Process(
                    target=SolverThread, 
                    args=( origNodes, candidates, baseline, thresh, )
                    )

        procs.append(proc)
        proc.start()

    for proc in procs:
        proc.join()

    #for cand in candidates:
    #while candidates.qsize() > 0:
    #    cand = candidates.get()

    #    allNodes = copy.deepcopy( origNodes )
    #    getNode( allNodes, cand ).isObstacle = False

    #    newTime = solveMaze( allNodes )
    #    savings = baseline - newTime
    #    if savings >= thresh:
    #        goodGlitches.append( cand )

    #print( goodGlitches )
    #print( f"{len(goodGlitches)} candidates that save {thresh} picoseconds")

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
