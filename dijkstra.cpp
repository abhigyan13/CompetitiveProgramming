//Single source shortest paths

//Dijkstra Algorithm for non negative weighted edges.
//Pick minimum distanced node, mark it, relax other nodes.
//A marked node will never have a lesser distance again as edges are non negative.
//Minimum can be pcked with iterations, and relaxation is o(1). O( n^2 + m)
//Optimal for dense graphs, for sparse graphs, where m <<< n^2, min heap can be used to find minimum distanced node.
//Path can be constructed by storing the node that that relaxes that node in  a prev[] array.
//We have to find minimum among d[v] , and update values for other nodes.
//C++ STL provides set, priority_queue. In set we can access the element and update it, 
//while in priority queue we can add new extra vertices and be sure not to use the wrong one by checking if the distance is optimal.

//Set implementation::
//Since we need distance, vertex, set of pair ( dist , index of node) is better.

vector< vector< pair< int , int > > > adj;

void dijkstra(int s , vector< int > &d , vector< int > p )
{
    //d is distance vector, p is prev vector.
    int n = adj.size();
    d.assign(n,1e9);
    p.assign(n,-1);
    d[s] = 0;
    set< pair< int , int > > sol;
    
    sol.insert({0,s});
    
    while(!sol.empty())
    {
        pair< int , int > temp = sol.begin();
        sol.erase(sol.begin());
        int u = temp.second;
        
        for(auto edge : adj[u])
        {   
            int v = edge.first;
            if(d[u] + edge.second < d[v])
            {
                //A shorter path has been found. if the vertex is in set, update it by removing and adding new.
                sol.erase({d[v],v}); //Erases if present
                d[v] = d[u] + edge.second;
                prev[v] = u;
                sol.insert({d[v],v}); //Push latest. in a sense updated form set.
                
            }
            
        }
        
    }
    
    
}


//Priority Queue implementation

//Here we cannot remove as we did in set. 
//We keep old pairs too. Thus is priority queue has a vertex with multiple distances in the queue at the same time.
//Among these pairs we are only interested in the pairs where the first element is equal to the corresponding value in d[], all the other pairs are old
//We only continue with important pairs.


vector< vector< pair< int , int > > > adj;

void dijkstra(int s , vector<int> &d , vector<int> &p)
{
    int n = adj.size();
    d.assign(n,1e9);
    p.assign(n,-1);
    
    d[s] = 0;
    
    priority_queue< pair< int , int > , vector< pair< int , int > > , greater< pair< int , int > > > sol;
    sol.push({0,s});
    
    while(!sol.empty())
    {
        pair<int , int> temp = sol.top();
        sol.pop();
        int u = temp.second;
        if(d[u] < temp.first ) //Not the variable we want to use. An older pair.
        continue;
        
        for(auto edge: adj[u])
        {
            int v = edge.first;
            
            if(d[u] + edge.second < d[v])
            {
                d[v]= d[u] + edge.second;
                p[v] = u;
                sol.push({d[v],v});
                
            }
            
            
        }
        
    }
    
    
}




