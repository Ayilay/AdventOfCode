from multiprocessing import Process, Queue

from collections import deque
import copy
import sys

# Each trail uniquely identified by START and END row-col coords
# Represent set of trails as set containing tuples:
#   Tuple of ( rowcol-begin, rowcol-end)
trails = set()

# DIM is set during ProcessLine
DIM = 0
nlines = 0
origNodes = None

class Node:
    def __init__(self):
        self.val = 0

        # BFS helpers
        self.prev = None
        self.isVisited  = False

    def __repr__(self):
        return str(self.val)

    def setVal( self, val ):
        self.val = val

def printGrid( grid ):
    for r in grid:
        print( r )

def getNode( grid, rctup ):
    (r, c) = rctup
    return grid[r][c]

def isValidNeigh( grid, orig, neigh ):
    origNode = getNode( grid, orig )
    neigNode = getNode( grid, neigh )

    if neigNode.isVisited:
        return False

    if neigNode.val == origNode.val+1:
        return True

def neighbors( grid, rctup: (int, int) ):
    neigh = []

    (r,c) = rctup

    if( r > 0 ):
        n = ( r-1, c )
        if isValidNeigh( grid, rctup, n ): neigh.append( n )

    if( c > 0 ):
        n = ( r, c-1 )
        if isValidNeigh( grid, rctup, n ): neigh.append( n )

    if( r < len(grid[0])-1 ):
        n = ( r+1,c )
        if isValidNeigh( grid, rctup, n ): neigh.append( n )

    if( c < len(grid[0])-1 ):
        n = ( r,c+1 )
        if isValidNeigh( grid, rctup, n ): neigh.append( n )

    return neigh

def doBFS( grid, startNode: (int,int) ):
    deadloop = DIM*DIM + DIM


    nodeq = deque()
    nodeq.append( startNode )

    trailheads = set()

    while( len(nodeq) > 0 ):
        node = nodeq.popleft()
        #print( f"exploring {node}" )
        if getNode(grid,node).val == 9:
            thead =  (startNode, node) 
            #print( f"FOUND end of trailhead; adding {thead}" )
            trailheads.add(thead)

        neigh = neighbors( grid, node )

        for n in neigh:
            getNode(grid,n).prev = node
            getNode(grid,n).isVisited = True
            nodeq.append( n )

        deadloop -= 1
        if deadloop == 0:
            raise RuntimeError( "Too many loop iterations" )

    #print( f"returning {trailheads}" )
    return trailheads

def solveMaze( grid, startNode ):
    deadloop = DIM*DIM

    trails = doBFS( grid, startNode )

    return trails

def SolverThread( origNodes, candidates, results ):

    trailheads = set()
    while not candidates.empty():
        cand = candidates.get()
        print( f"trying {cand}" )

        nodes = []
        nodes = copy.deepcopy( origNodes )

        temptrails = solveMaze( nodes, cand )
        for t in temptrails:
            trailheads.add( t )

    ntrail = len(trailheads)
    print( f"{ntrail} valid trailheads: {trailheads}\n")

    for t in trailheads:
        #print( f"adding {t} to {results}")
        results.put( t )


def SOLVER_Init( argv ):
    global candidates

    candidates = Queue()

def SOLVER_ProcessLine( line: str ):
    global DIM
    global nlines
    global origNodes
    global candidates

    line = line.strip()

    if origNodes is None:
        DIM = len( line )
        origNodes = [[Node() for _ in range(DIM)] for _ in range(DIM)]

    for i,c in enumerate( line ):

        # arbitrary val for dots; We parse them for the examples
        if c == '.':
            val = 5 
        else:
            val = int(c)

        origNodes[nlines][i].setVal( val )

        # Save the trailhead candidates; Each thread will try one trailhead
        if c == '0':
            candidates.put( (nlines, i) )

    nlines += 1

def SOLVER_PrintSolution():
    global origNodes
    global candidates

    results = Queue()

    NTHREADS = 8
    procs = []
    for i in range(NTHREADS):
        proc = Process(
                    target=SolverThread, 
                    args=( origNodes, candidates, results )
                    )

        procs.append(proc)
        proc.start()

    for proc in procs:
        proc.join()

    validTrailheads = set()
    while not results.empty():
        res = results.get()
        #print( f"adding {res}")
        validTrailheads.add( res )

    print( f"Num trailheads: {len(validTrailheads)}" )





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
