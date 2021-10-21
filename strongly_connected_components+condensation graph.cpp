//Strongly connected component

#include<bits/stdc++.h>
using namespace std;
#define pb push_back

vector< vector< int > > adj , adjr;
vector<bool> used(n);
vector<int> comp, order;

void dfs1(int v)
{
    used[v] = true;
    for(auto x : adj[v])
    if(!used[x])
    dfs1(x);
    order.push_back(u);  //Reverse of Topological Ordering, Vertex finished first gets pushed.
}

void dfs2(int v)
{
    used[v] = true;
    comp.push_back(v);
    for(auto x : adjr[v])
    if(!used[x])
    dfs2(x);
    
}

void scc()
{
    used.assign(n,false);
    for (int i = 0; i < n; i++)
        if (!used[i])
            dfs1(i);

    used.assign(n, false);
    reverse(order.begin(), order.end()); //Ordering wrt Descending finish time

    for (auto v : order)
        if (!used[v]) {
            dfs2 (v);
            //comp now contains the strongly connected component. Process as you desire

            comp.clear();
        }
}
    
//we can give a definition of condensation graph GSCC as a graph containing every strongly connected component as one vertex.
//Each vertex of the condensation graph corresponds to the strongly connected component of graph G.
//There is an oriented edge between two vertices Ci and Cj of the condensation graph
//if and only if there are two vertices u∈Ci,v∈Cj such that there is an edge in initial graph, i.e. (u,v)∈E.    
    

//Each node of condensed graph can be represented by 1 node of our choice.

vector< int > roots(n,0);
vector< int > root_nodes;
vector< vector< int > > adjscc(n);

for(auto v : order)
{
    if(!used[v])
    {
        dfs2(v);
        int root = comp.front(); //We are taking first node as SCC
        for(auto u: comp)
        roots[u] = root;
        root_nodes.push_back(root); //store root
        
        comp.clear();
        
    }
}

//Constructing edges of condensation graph

for(int u = 0 ; u < n ; ++u)
{
    for(auto v : adj[u] )
    {
        if(roots[v] != roots[u])
        adj[roots[u]].push_back(roots[v]); 
        
    }
    
}

//adjscc has edges of consensation graphs for root_nodes.



