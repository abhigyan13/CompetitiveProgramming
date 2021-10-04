// Advance Seg Tree: Lazy Propogation + 2D seg tree 

#include<bits/stdc++.h>
using namespace std;

//Range Updates, Point Queries
//Addition on segment, Query a single point.
// We will store the values on segments ( at most log(n) segments ), and while queries sum the value from root till leaf.   

const int MAXN = 1e5;
int t[4*MAXN];

void build(int a[] , int v , int tl , int tr )
{
    if(tl==tr)  { t[v] = a[tl]; return; }
    int tm = (tl+tr)/2;
    build(a,2*v ,tl , tm );
    build(a,2*v+1 ,tm+1 , tr );
    t[v] = 0; //Ininitally no update queries.
    
}

void update(int v , int tl , int tr , int l , int r , int val)
{
    if(l>r) return;
    if( l == tl && r == tr ) { t[v] += val; return; }
    int tm = (tl+tr)/2;
    update(2*v , tl , tm , l , min(tm,r) , val);
    update(2*v+1 , tm+1 , tr , max(tm+1,l) , r, val );
    
}

int get(int v, int tl, int tr, int pos) {
    if (tl == tr)
        return t[v];
    int tm = (tl + tr) / 2;
    if (pos <= tm)
        return t[v] + get(v*2, tl, tm, pos);
    else
        return t[v] + get(v*2+1, tm+1, tr, pos);
}


//Assignment on segment, Quering a point. We will store in some values and lazily update the tree whenever we need to.

bool marked[4*MAXN];

void push(int v)
{
    if(marked[v])
    {
        marked[v] = false;
        marked[2*v] = true;
        marked[2*v+1] = true;
        t[2*v] = t[2*v+1] = t[v];
    }
}

void update(int v, int tl , int tr , int l , int r , int nval)
{
    if(l>r) return;
    if(l == tl && r == tr)
    {
        marked[v] = true;
        t[v] = nval;
        return;
    }
    
    push(v);
    int tm = (tl+tr)/2;
    update(2*v , tl , tm , l , min(tm,r), nval);
    update(2*v + 1 , tm+1 , tr , max(tm+1 , l) , r, nval);
    
}

int get(int v , int tl , int tr , int pos)
{
    if(tl == tr || marked[v])
    return t[v];
    
    int tm = (tl+tr)/2;
    if(pos<=tm)
    return get(2*v,tl,tm,pos);
    else return get(2*v+1,tm+1,tr,pos);
    
}

//You can also choose to push here also, and get the answer from leaf i.e dont delay updates.

//Example: Adding on segments, querying for maximum:
//Since adding in a range will increase the maximum by the same amount. We cn process with seg tree.
//For storing maximum, out t-array will be sufficient. For handling updates, we need to store values that have not yet been pushed to the range.
//Before calling update or query for child, we need to push. If the update range exactly covers that segment, we will add to the maximum.
//What is in lazy[v] is already added to t[v] i.e considred for the range of that node. but not for children. Thus content of lazy[v] has to be propoagted to children.

int lazy[4*MAXN]; //Storing updates lazily for the range. Propogating to children when in need.

void push(int v)
{
    t[2*v] += lazy[v];  //Pushing the add value to both children.
    t[2*v+1] += lazy[v];
    lazy[2*v] += lazy[v];
    lazy[2*v+1] += lazy[v];
    lazy[v] = 0;
}

void update( int v , int tl , int tr, int l , int r , int addval)
{
    if(l>r) return;
    if(tl == l && tr == r  ) { lazy[v]+= addval; t[v]+=addval; return; }
    int tm = (tl+tr)/2;
    push(v); //Need to push the value already in the range before going to the children.
    update(2*v , tl , tm , l , min(r,tm) , addval);
    update(2*v+1 , tm+1 , tr , max(tm+1,l) , r , addval);
    t[v] = max(t[2*v] , t[2*v+1]); //Any combine function.
}

int query( int v , int tl , int tr , int l , int r)
{
    if(l > r ) return -2e9;
    if(tl == l && tr == r ) { return t[v]; }
    push(v); 
    int tm = (tl+tr)/2;
    return max( query(2*v , tl , tm , l , min(tm, r)) , query(2*v+1 , tm+1 , tr , max(tm+1, l) , r ));
    
}



//2D segment tree: First build with respect to x cordinate. Each node will contain a segment tree that covers the y cordinate for this node's x cordinate range.
//O(4*n*4*m) = O(16*n*m) Memory
//when the first cordinate is fixed, say node vx (tlx: trx), it will contain a seg tree corresponding to (0,m-1) of the yth cordinate.
//Example: A martix (n,m) is given. Sum/Max/Min/GCD/lcm queries on submatrixes with point upates.

//Build x cordinate which will call build y for that x segment.Thus building/updating/querying y ht will always take input about corresponding x-range.

int t[4*MAXN][4*MAXM]; //t-array
int n,m;

void build_y(int vx , int lx , int rx , int vy , int ly , int ry  )
{
    if(ly == ry)
    {
        //Two cases
        if(lx == rx )
        {
        //In the first case, we just take the corresponding value from the matrix,
           t[vx][vy] = a[lx][ly]; 
        }
        else
        {
        //We can combine the values of two Segment Trees from the left and the right child in the coordinate x.
           t[vx][vy] = t[vx*2][vy] + t[vx*2+1][vy]; //Combine. //Note that both these x children were already built with build_x recursive calls.   
        }
        return;
        
    }
    
    int my = (ly+ry)/2;
    build_y(vx,lx,rx , 2*vy , ly , my);
    build_y(vx,lx,rx , 2*vy+1 , my+1 , ry);
    t[vx][vy] = t[vx][2*vy] + t[vx][2*vy+1]; //Combine
    
}

void build_x( int vx , int tlx , int trx )
{
    //Recursively build children. 
    if( tlx != trx )
    {
        int tmx = (tlx + trx)/2;
        build_x(2*vx , tlx , tmx);
        build_x(2*vx+1 , tmx + 1 , trx);
    }
    
    build_y(vx , tlx , trx , 1 , 0 , m-1   ) //For each segment build y seg tree. The base case of build_y, builds the column for that y.
    
}

//query
//first break the query on the first coordinate, and then for every reached vertex, we call the corresponding Segment Tree of the second coordinate.

int sum_y(int vx, int vy, int tly, int try_, int ly, int ry) {
    if (ly > ry) 
        return 0;
    if (ly == tly && try_ == ry)
        return t[vx][vy];
        
    int tmy = (tly + try_) / 2;
    return sum_y(vx, vy*2, tly, tmy, ly, min(ry, tmy))
         + sum_y(vx, vy*2+1, tmy+1, try_, max(ly, tmy+1), ry); //Any other combine function.
}

int sum_x(int vx, int tlx, int trx, int lx, int rx, int ly, int ry) {
    if (lx > rx)
        return 0;
    if (lx == tlx && trx == rx) //Found the x segment. Now find y.
        return sum_y(vx, 1, 0, m-1, ly, ry);
    int tmx = (tlx + trx) / 2;
    return sum_x(vx*2, tlx, tmx, lx, min(rx, tmx), ly, ry)
         + sum_x(vx*2+1, tmx+1, trx, max(lx, tmx+1), rx, ly, ry);
}

//Update: a point. We will only go the segments that contain this point. o(log(n)*log(m)) segments.

void update_y( int vx , int lx , int rx , int vy , int ly , int ry , int x , int y , int nval )
{
    if(ly == ry)
    {
        if(lx == rx )
        t[vx][vy] = nval;
        else
        t[vx][vy] = t[vx*2][vy] + t[vx*2+1][vy];
        return;
    }
    
    int my = (ly+ry)/2;
    if(my <= y)
    update_y(vx , lx , rx , 2*vy , ly , my , x , y , nval);
    else update_y(vx , lx , rx , 2*vy+1 , my + 1 , ry , x ,y ,nval);
    t[vx][vy] = t[vx][2*vy] + t[vx][2*vy+1];
    
}

void update_x( int vx , int lx , int rx , int x , int y , int nval)
{
    if(lx != rx) 
    {
        int mx = (lx+rx)/2;
        if(mx <= x)
        update_x(2*vx , lx , mx , x , y , nval);
        else update_x(2*vx+1 , lx , mx+1 , x , y , nval);
        
    }
    
    update_y(vx , lx , rx , 1 , 0 , m-1 , x , y , nval);
    
    
}
