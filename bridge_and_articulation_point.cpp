//Find Bridges in graph, Finding articulation point
#include<bits/stdc++.h>
using namespace std;

//A bridge in an undirected graph is an edge which when removed increases the number of connected components.
//DFS: O(n+m)
//While we are in dfs at a vertex v, the edge (v,to) is a bridge iff to or any on its descandant has a back edge to vertex v or its ancestors.
//tin[]: Entry time of DFS. (id)
//low[]: Contains the lowes tin numbered node, which can be reached from any vertex.
//low[v] = min( tin[v] ,tin[p] such that (v,p) is back_edge, low[c] such that c is tree edge. ]
//Now, there is a back edge from vertex v or one of its descendants to one of its ancestors
//if and only if vertex v has a child to for which low[to] ≤ tin[v]. 
//If low[to]=tin[v], the back edge comes directly to v, otherwise it comes to one of the ancestors of v.

//Thus, the current edge (v,to) in the DFS tree is a bridge if and only if low[to]>tin[v].

int n;
vector< vector< int > > adj;

vector<int> tin(n) , low(n);

int tim = 0;

void dfs(int u , int par = -1)
{
    tin[u] = low[u] = tim++;
    
    for(int x : adj[u])
    {
        if(x == par) continue;
        if(tin[x]!=-1)
        {
            low[u] = min(low[u],tin[x]); //back edge
        }
        else
        {
            dfs(x,u);
            low[u] = min(low[u] , low[x]);
            if(low[x] > low[u] ) //No edge from dfs subtree of x to any u or its ancestor. thus a bridge.
             bridge(); //This edge is a bridge.
        }
        
    }
    
}

void find_bridge(int n)
{
    tim = 0 ;
    tin.assign(n,-1);
    low.assign(n,-1);
    for(int i = 0 ; i < n ; ++i)
    if(tin[i]==-1)
    dfs(i,-1);
}

//This implementation will not work with multiple edges. It will ignore multiple images. Multiple edges can never be bridge.
//The reported bridges van be checked effectively.


//Articulation points, same logic. if v != root, v is an articulation point if low[to]>=v.
//if v is root, it will be articulation point if it has more than one children in DFS tree.

int n;
vector< vector< int > > adj;

vector<int> tin(n) , low(n);

int tim = 0;

void dfs(int u , int par = -1)
{
    tin[u] = low[u] = tim++;
    int child = 0;
    for(int x : adj[u])
    {
        if(x == par) continue;
        if(tin[x]!=-1)
        {
            low[u] = min(low[u],tin[x]); //back edge
        }
        else
        {
            dfs(x,u);
            low[u] = min(low[u] , low[x]);
            if(low[x] >= low[u] && par != -1 ) 
            articulation(); 
            child++;
        }
        
    }
    
    if( par == -1 && child > 1 )
    articulation();
    
}


void find_articulation(int n)
{
    tim = 0 ;
    tin.assign(n,-1);
    low.assign(n,-1);
    for(int i = 0 ; i < n ; ++i)
    if(tin[i]==-1)
    dfs(i,-1);
}






