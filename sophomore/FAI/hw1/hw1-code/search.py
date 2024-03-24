# search.py
# ---------------
# Licensing Information:  You are free to use or extend this projects for
# educational purposes provided that (1) you do not distribute or publish
# solutions, (2) you retain this notice, and (3) you provide clear
# attribution to the University of Illinois at Urbana-Champaign
#
# Created by Michael Abir (abir2@illinois.edu) on 08/28/2018
# Modified by Shang-Tse Chen (stchen@csie.ntu.edu.tw) on 03/03/2022

"""
This is the main entry point for HW1. You should only modify code
within this file -- the unrevised staff files will be used for all other
files and classes when code is run, so be careful to not modify anything else.
"""
# Search should return the path.
# The path should be a list of tuples in the form (row, col) that correspond
# to the positions of the path taken by your search algorithm.
# maze is a Maze object based on the maze from the file specified by input filename
# searchMethod is the search method specified by --method flag (bfs,dfs,astar,astar_multi,fast)


from maze import Maze
import queue
MAXSIZE = 200
MAXDIS = 1000000000


def search(maze, searchMethod):
    return {
        "bfs": bfs,
        "astar": astar,
        "astar_corner": astar_corner,
        "astar_multi": astar_multi,
        "fast": fast,
    }.get(searchMethod)(maze)

def astar_multi(maze):
    """
    Runs A star for part 3 of the assignment in the case where there are
    multiple objectives.
    @param maze: The maze to execute the search on.
    @return path: a list of tuples containing the coordinates of each state in the computed path
    """
    # TODO: Write your code here
    def myhash(lista,b):
        return lista[0]+MAXSIZE*lista[1]+MAXSIZE*MAXSIZE*b

    def getstr(newnode):
        strtmp = ''
        idx = 0
        for n in newnode:
            strtmp += str(n)
            if idx < 2: strtmp += '-'
            idx += 1
        return strtmp

    def get_dis_Objs(Objs):
        dis_Objs = {}
        q = queue.Queue()
        for idx1 in range(len(Objs)):
            Obj1 = Objs[idx1]
            q.put(Obj1)
            dis_Objs[myhash(Obj1,idx1)] = 0
            while not q.empty():
                prev = q.get()
                for next in maze.getNeighbors(prev[0],prev[1]):
                    if myhash(next,idx1) not in dis_Objs:
                        dis_Objs[myhash(next,idx1)] = dis_Objs[myhash(prev,idx1)] + 1
                        q.put(next)
        return dis_Objs
    
    def get_Objs_id(Objs):
        Objs_id = {}
        for idx1 in range(len(Objs)):
            Objs_id[tuple(Objs[idx1])] = idx1
        return Objs_id

    # init
    Start = maze.getStart()
    Objs = maze.getObjectives()
    Objs.append(Start)
    Objs_id = get_Objs_id(Objs)   # [tuple(Obj)] = Obj's own id
    dis_Objs = get_dis_Objs(Objs) # [myhash(Obj,i)] = distance between Obj and Objs[i]
    # struct of node:[x, y, (0 if Obj[i] is visited else 1) for i in len(Objs) ]
    parent = {}                   # [getstr(node)] = parent node
    visited = {}                  # [getstr(node)] = True/None
    g = {}                        # [getstr(node)] = g
    mst_dic = {}                  # [getstr(node)] = mst

    # copy from ADA P5mini
    def FIND_SET_k(node,a):
        if node[a][0] != node[a][1]:
            node[a][1] = FIND_SET_k(node,node[a][1])
        return node[a][1]

    def UNION_k(node,a,b):
        a = FIND_SET_k(node,a)
        b = FIND_SET_k(node,b)
        if(a == b):
            return
        newr = node[a][2] + node[b][2]
        if node[a][2] >= node[b][2]:
            node[b][1] = node[a][0]
            node[a][2] = newr
        else:
            node[a][1] = node[b][0]
            node[b][2] = newr

    def kruskal(check):
        lk = len(Objs)
        for i in range(len(Objs)):
            if check[i+2] == 0: lk-=1
        node = [[0 for i in range(3)] for j in range(len(Objs))]
        edge = [[0 for i in range(3)] for j in range(lk*(lk-1))]      
        i1 = 0
        j1 = 0
        for i in range(len(Objs)):
            if check[i+2] == 0: continue
            node[i1] = i1,i1,1  # id, p, r
            j2 = 0
            for j in range(len(Objs)):
                if check[j+2] == 0: 
                    continue
                if i == j:
                    j2 += 1
                    continue
                edge[j1] = i1,j2,dis_Objs[myhash(Objs[j],i)]
                j1 += 1
                j2 += 1
            i1 += 1
        node = node[0:i1]
        edge = sorted(edge, key = lambda x: x[2])
        edge = [list(e) for e in edge]
        node = [list(n) for n in node]

        mst = 0  # might be improved
        for e in edge:
            if(FIND_SET_k(node,e[0]) != FIND_SET_k(node,e[1])):
                mst += e[2]
                UNION_k(node,e[0],e[1])
        return mst

    def f(node, node_p):
        # check if it's start
        is_start = False
        need_put = False
        if node == node_p:
            is_start = True

        nodestr = getstr(node)
        node_pstr = getstr(node_p)
        # get mst
        if tuple(node[0:2]) in Objs_id:
            checkObj = Objs_id[tuple(node[0:2])]
            if node[checkObj+2] == 1:
                node[checkObj+2] = 0
                nodestr = getstr(node)
                if nodestr not in mst_dic:
                    mst_dic[nodestr] = kruskal(node)
            else : mst_dic[nodestr] = mst_dic[node_pstr]
        else : mst_dic[nodestr] = mst_dic[node_pstr]       

        # get mi
        mi = MAXDIS
        done = True
        for i in range(len(Objs)):
            if node[i+2] == 1:
                mi = min(mi,dis_Objs[myhash(node,i)])
                done = False
        if done: mi = 0
        
        # get parent{} and g{}, and check if it's needed to be put
        if is_start:
            g[nodestr] = 0
            parent[nodestr] = node
            need_put = True
        elif nodestr not in g:
            g[nodestr] = g[node_pstr]+1
            parent[nodestr] = node_p
            need_put = True
        elif g[nodestr] > g[node_pstr]+1:
            g[nodestr] = g[node_pstr]+1
            parent[nodestr] = node_p
            need_put = True
        return mst_dic[nodestr]+g[nodestr]+mi, need_put

    def addnode(child,prev):
        newnode = [1 for i in range(len(Objs)+2)]
        newnode[0:2] = child
        newnode[2:] = prev[2:]
        return newnode
    
    def mission_complete(cur):
        welldone = True
        for i in range(2,len(cur)):
            if cur[i] == 1:
                welldone = False
                break
        return welldone
    
    # function done


    # main program
    pq = queue.PriorityQueue()
    
    newnode = addnode(Start,[1 for i in range(len(Objs)+2)])
    fv, need_put = f(newnode,newnode)
    pq.put((fv,newnode))
    while not pq.empty():
        tmp = 1
        cur = []
        tmp, cur = pq.get()
        if getstr(cur) in visited: continue
        visited[getstr(cur)] = True

        if mission_complete(cur):
            path = []
            ans = cur
            while parent[getstr(ans)] != ans:
                path.append(ans[0:2])
                ans = parent[getstr(ans)]
            path.append(ans[0:2])
            return path[::-1]
        
        for nei in maze.getNeighbors(cur[0],cur[1]):
            newnode = addnode(nei,cur)
            fv, need_put = f(newnode,cur)
            if (getstr(newnode) not in visited) and need_put:
                pq.put((fv,newnode))

    return []

def bfs(maze):
    """
    Runs BFS for part 1 of the assignment.
    @param maze: The maze to execute the search on.
    @return path: a list of tuples containing the coordinates of each state in the computed path
    """
    # TODO: Write your code here
    return astar_multi(maze)


def astar(maze):
    """
    Runs A star for part 1 of the assignment.
    @param maze: The maze to execute the search on.
    @return path: a list of tuples containing the coordinates of each state in the computed path
    """
    # TODO: Write your code here
    return astar_multi(maze)

def astar_corner(maze):
    """
    Runs A star for part 2 of the assignment in the case where there are four corner objectives.
    @param maze: The maze to execute the search on.
    @return path: a list of tuples containing the coordinates of each state in the computed path
        """
    # TODO: Write your code here
    return astar_multi(maze)

def fast(maze): 
    """
    Runs suboptimal search algorithm for part 4.
    @param maze: The maze to execute the search on.
    @return path: a list of tuples containing the coordinates of each state in the computed path
    """
    # TODO: Write your code here
    Path = []
    visited_fast = [[False for i in range(MAXSIZE)] for j in range(MAXSIZE)]
    def dfs(x,y):
        visited_fast[x][y] = True
        Path.append((x,y))
        for nei in maze.getNeighbors(x,y):
            if not visited_fast[nei[0]][nei[1]]:
                dfs(nei[0],nei[1])
                Path.append(nei)

    Start = maze.getStart()
    dfs(Start[0],Start[1])

    return Path
