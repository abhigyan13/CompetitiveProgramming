//Simple Seg Tree Implementations

#include<bits/stdc++.h>
using namespace std;



//Write own struct data function if multiple things about segment needs to be stored.
//If you write your own struct, its better to write combine function as it will be needed to call in all three.
//You can write one or two querry functions.
// Example for finding maximum subarray sum for a range (l-r) with update querries.

struct data
{
    int sum , pref , suff, ans;
};

const int N = (int)2e3+1;

data t[4*N];  //4* Array Length

data make_data(int val)
{
    data obj;
    obj.sum = val;
    obj.pref = obj.suff = obj.ans = max(0,val);
    return obj;
}

data combine(data l , data r)
{
    data fin;
    fin.sum = l.sum + r.sum;
    fin.pref = max(l.pref , l.sum+ r.pref);
    fin.suff = max(r.suff , l.suff+ r.sum);
    fin.ans =  max(l.ans , max(r.ans , l.suff + r.pref));
    return fin;
}



void build(int ar[] , int v , int tl , int tr  ) 
{   
//Call by build(ar,1,0,n-1) i.e 1 indexed for nodes in t-array, and 0 indexed for base array.
//Left child: 2*v , Right child: 2*v + 1
    if(tr==tl)
    {
        t[v] = make_data(ar[tl]);
    }
    else
    {
       int tm = (tr+tl)/2;
       build(ar,2*v, tl , tm ); //Recursively build left child.
       build(ar,2*v + 1, tm+1 , tr ); //Recursively build right child.
       t[v] = combine(t[2*v] , t[2*v+1]); //Combine both into this vertex.
       
    }
    
}

void update(int v , int tl , int tr , int pos , int& val )
{
//tl , tr, pos are 0 indexed for the base array.
    if(tl == tr )
    t[v] = make_data(val);
    else
    {   
        int tm = (tl+tr)/2;
        if(pos <= tl)
        update(2*v , tl , tm , pos , val);
        else update(2*v+1 , tm+1 , tr , pos , val);
        t[v] = combine(t[2*v] , t[2*v+1]);
        
    }
    
}

data querry(int v , int tl , int tr , int l , int r )
{
// tl,tr: 0 indexed, v= 1 intially, l and r: 0 indexed, must be in range (0,n-1)
    if(l>r) return make_data(0);
    if( l == tl && r == tr) return t[v]; //Fits exactly
    

    int tm = (tl+tr)/2;  //Return combination from both child. 
    return combine( querry(2*v , tl , tm , l , min(tm,r)) , querry(2*v+1 , tm+1 ,tr , max(tm+1,l) , r));
    
}

//You can use the same underlying t-array to write more than 1 querry function.
//For example: Counting the no. of zeroes and finding the index of kth zero is a range:





int main()
{  
    int n;
    cin>>n;
    
    int ar[n];
    
    cout<<"elements"<<endl;
    
    for(int i = 0 ; i< n ; ++i)
    cin>>ar[i];
    
    cout<<"Querries "<<endl;
    
    build(ar,1,0,n-1);
    
    
    
    char x = 'Y';
    while(x == 'Y')
    {
        cout<<"querry type: 0 for query, 1 for update"<<endl;
        int z;
        cin>>z;
        
        if(z == 0)
        {
            cout<<"Enter l and r"<<endl;
            int l,r;
            cin>>l>>r;
            data temp = querry(1,0,n-1, l-1 , r-1);
            cout<<temp.ans<<endl;
            
            
        }
        else
        {
            cout<<"Enter pos and value:"<<endl;
            int l,r;
            cin>>l>>r; 
            update(1,0,n-1,l-1,r);
            cout<<"update succesful"<<endl;
        }
        
        cout<<"MORE query? (Y/N)"<<endl;
        cin>>x;
        
        
    }
    
    
}

/////////////////////////////////////////////////////////////////////////////////////


// Find the first index i in (l,r) such that x >= ar[i]
//i.e lower_bound of x in range (l,r) with update querries;

//We can do this for (1-n) effectively with segment tree in the sense that check max of left subtree.
//If max < x, Thus ans lies in right subtree. else left. Simple binary search can do this.
//For (l-r) range, 

struct data { int val; };

int get_first(int v , int tl , int tr , int l , int r , int x )
{
    if( l > tr || t < tl || t[v].val < x )  //No intersection of the range
    return -1;
    
    
    if( tl >= l && tr <= r ) //Completely inside the range. No more children needed to call recrusively. Find by binary search
    {
        if(t[v].val < x) return -1;
        
        while(tl!= tr)
        {
            int tmid = (tl+tr)/2;
            if(t[2*v].val <= x )
            {
                v = 2*v;
                tr = tmid;
                
            }
            else
            {
                v = 2*v+1;
                tl = tmid+1;
            }
            
        }
        
        return tl;  //tl is the left most index in this range (tl-tr) that is an answer.
    }
    
    //Not outside and not completely inside. Thus answer may be possible and found recursively.
    //First left subtree is called, thus if returned answer is not -1, it will be the ans.
    
    int tm = (tl+tr)/2;
    int temp = get_first(2*v , tl ,tm , l ,r ,x );
    if(temp!= -1)
    return temp;
    return get_first(2*v+1, tm+1 , tr, l , r , x);
    
}

//Saving entire subtrees in each node. We can call build once. 
//But while updating, we cannot call combine arrays. We have to update only the arrays.
//In the t-array, store vectors/maps/multisets belonging to that segment.

//Example: Find the smallest number greater or equal to a specified number. No modification queries.

const int MAXN = 1e4;
vector< int > t[4*MAXN];

vector<int> t[4*MAXN];

void build(int a[], int v, int tl, int tr) {
    if (tl == tr) {
        t[v] = vector<int>(1, a[tl]);
    } else { 
        int tm = (tl + tr) / 2;
        build(a, v*2, tl, tm);
        build(a, v*2+1, tm+1, tr);
        merge(t[v*2].begin(), t[v*2].end(), t[v*2+1].begin(), t[v*2+1].end(),
              back_inserter(t[v])); //STL merge function #back_inserter: The vector to be inserted into
    }
}

//Here the range (l,r) can be broken into (max log(n)) sub ranges, and two range can be comnbined in O(1) i.e minimuum of returned value.

int query(int v, int tl, int tr, int l, int r, int x) {
    
    if (l > r)
        return 2e9;
    if (l == tl && r == tr) {
        auto pos = lower_bound(t[v].begin(), t[v].end(), x);
        if (pos != t[v].end())
            return *pos;
        return 2e9;
    }
    int tm = (tl + tr) / 2;
    return min(query(v*2, tl, tm, l, min(r, tm), x), 
               query(v*2+1, tm+1, tr, max(l, tm+1), r, x));
}


//Find lower_bound in a range (l,r) with update querries. Here t-array needs to store multiset, to remove and update.

multiset<int> t[4*MAXN];

//Go down the tree and modify all the multiset. Do not merge as in other update queries as then it will be TLE.

void update(int v , int tl , int tr , int pos , int nval)
{   
    t[v].erase(t[v].find(a[pos])); //Remove old value
    t[v].insert(nval);
    if(tl == tr)
    {
        a[pos] = nval; 
    }
    else
    {
        int tm = (tl+tr)/2;
        if(pos<= tm)
        update(2*v  , tl ,tm , pos , nval);
        else update(2*v + 1 , tm+1 , tr , pos , nval);
    }
}






