from collections import deque
import sys

nBytes = 0

DIM = 71
BYTE_LIM = 1024
INIT_NODE = (0,0)
FINAL_NODE = (DIM-1,DIM-1)

bytelist = []

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

allNodes = [[Node() for _ in range(DIM)] for _ in range(DIM)]

def prepGrid( grid, bytelist ):
    for r in grid:
        for node in r:
            node.prev = None
            node.isVisited = False
            node.isObstacle = False

    for i,b in enumerate( bytelist ):
        if i == BYTE_LIM:
            return

        grid[b[0]][b[1]].isObstacle = True


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


def SOLVER_Init( argv ):
    pass

def SOLVER_ProcessLine( line: str ):
    global nBytes

    line = line.strip()
    (x,y) = line.split(',')
    x = int(x)
    y = int(y)

    bytelist.append( (y,x) )

    nBytes += 1

def getNode( grid, rc ):
    try:
        return grid[rc[0]][rc[1]]
    except IndexError:
        print( f"Bad index {rc}" )


def doBFS( grid ):
    # O(N^2) steps taken to solve NxN maze + some margin
    deadloop = DIM*DIM

    nodeq = deque()
    nodeq.append( INIT_NODE )

    while( len(nodeq) > 0 ):
        node = nodeq.popleft()

        # Early-exit if we find node
        if node == FINAL_NODE:
            return True

        neigh = neighbors( allNodes, node )
        for n in neigh:
            getNode(allNodes,n).prev = node
            getNode(allNodes,n).isVisited = True
            nodeq.append( n )

        #print( f"visiting {node}" )

        deadloop -= 1
        if deadloop == 0:
            raise RuntimeError( f"Too many loop iterations, NBYTES={BYTE_LIM}" )
    
    # No solution found via early-exit condition
    print( f"lastnode: {bytelist[BYTE_LIM-1]}" )
    return False

def solveMaze( grid ):
    deadloop = 1000

    print( f"Trying {BYTE_LIM}" )
    isSolveable = doBFS( grid )
    if not isSolveable:
        # Hacky solution to abort program...
        print( f"SOLUTION FOUND: {BYTE_LIM}" )
        raise RuntimeError( "sol found" )


def SOLVER_PrintSolution():
    global BYTE_LIM

    while BYTE_LIM < len( bytelist ):
        
        print( f"Trying {BYTE_LIM} prep" )
        prepGrid( allNodes, bytelist )
        solveMaze( allNodes )

        BYTE_LIM += 1

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
