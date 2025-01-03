from collections import deque
import sys

nBytes = 0

DIM = 71
BYTE_LIM = 1024
INIT_NODE = (0,0)
FINAL_NODE = (DIM-1,DIM-1)

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
    if nBytes >= BYTE_LIM:
        return

    line = line.strip()
    (x,y) = line.split(',')
    x = int(x)
    y = int(y)
    allNodes[y][x].isObstacle = True

    nBytes += 1

def getNode( grid, rc ):
    try:
        return grid[rc[0]][rc[1]]
    except IndexError:
        print( f"Bad index {rc}" )


def doBFS( grid ):
    deadloop = 10000

    nodeq = deque()
    nodeq.append( INIT_NODE )

    while( len(nodeq) > 0 ):
        node = nodeq.popleft()
        neigh = neighbors( allNodes, node )

        for n in neigh:
            getNode(allNodes,n).prev = node
            getNode(allNodes,n).isVisited = True
            nodeq.append( n )

        #print( f"visiting {node}" )

        deadloop -= 1
        if deadloop == 0:
            raise RuntimeError( "Too many loop iterations" )

def solveMaze( grid ):
    deadloop = 1000

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

def SOLVER_PrintSolution():
    solveMaze( allNodes )

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
