#include <bits/stdc++.h>
using namespace std;
#define MAX_N 300005
#define max_dis 300005

struct HeapNode{
    int element;
    int key;
    HeapNode():element(0),key(0){};
    HeapNode(int node, int key):element(node), key(key){};
};

class BinaryHeap{
private:
    vector<HeapNode> heap;
    vector<int> pos;

    void Heap_swap(struct HeapNode &p1, struct HeapNode &p2){
        struct HeapNode temp = p1;
        p1 = p2;
        p2 = temp;
    }

public:
    BinaryHeap(int n){
        heap.resize(n+1);
        pos.resize(n+1, 0);
    }
    bool IsHeapEmpty(){
        return (heap.size() <= 1);
    };

    // Min-Priority Queue
    void MinHeapify(int node, int length);
    void BuildMinHeap(vector<int> vec);
    void DecreaseKey(int node, int newKey);
    void MinHeapInsert(int node, int key);
    int ExtractMin();
};


class Graph{ 
private:
    int num_vertex;
    vector< list< pair<int, int> > > AdjList;
    vector<int> distance; 
    vector<bool> visited;
public:
    Graph(int n){
        num_vertex = n;
        AdjList.resize(num_vertex+1);
    }
    void AddEdge(int from, int to, int weight){
        AdjList[from].push_back({to, weight});
    }

    void Init(int start){
        distance.resize(num_vertex+1);
        for (int i=1;i<=num_vertex;i++) {
            distance[i] = max_dis;
        }
        distance[start] = 0;
    }
    void Relax(int from, int to, int weight){
        if (distance[to] > distance[from] + weight) {
            distance[to] = distance[from] + weight;
        }
    }

    void Dijkstra(int start);  
    long long int Minus_Pair(int dest, int n, set<int> done_set);

    set<int> Done(){
        set<int> done_set;
        for(int i=1;i<=num_vertex;i++){
            if(distance[i] != max_dis) done_set.insert(i);
        }
        return done_set;
    }

    friend class BinaryHeap;
};

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    long long int m;
    int n, s, t, u, v;
    cin>>n>>m>>s>>t;
    Graph campus(n);
    Graph reverse_campus(n);
    for(long long int i=0;i<m;i++){
        cin>>u>>v;
        campus.AddEdge(u, v, 1);
        reverse_campus.AddEdge(v, u, 1);
    }

    set<int> done_set;
    campus.Dijkstra(s);
    long long int minus1 = campus.Minus_Pair(t, n, done_set);

    done_set = campus.Done();

    reverse_campus.Dijkstra(t);
    long long int minus2 = reverse_campus.Minus_Pair(s, n, done_set);  

    long long int minus3 = 0;
    // s cannot reach t
    if(done_set.count(t) == 0){
        set<int> reverse_done_set = reverse_campus.Done();
        long long int start_size = done_set.size();
        long long int dest_size = reverse_done_set.size();
        minus3 = start_size * dest_size;
    }

    long long int ans = (long long int)n*(long long int)n - m - minus1 - minus2 - minus3;
    /*
    cout<<"in order minus "<<minus1<<"\n";
    cout<<"reverse order minus "<<minus2<<"\n";
    cout<<"cant reach minus "<<minus3<<"\n";
    */
    cout<<ans<<"\n";
    
    return 0;
}

long long int Graph::Minus_Pair(int dest, int n, set<int> done_set){
    long long int cant_add = 0;
    if(distance[dest] == max_dis) return cant_add;
    for(int i=1;i<=n;i++){
        if(done_set.count(i)) continue;
        cant_add += max(0, distance[dest] - distance[i] - 1);
    }
    
    return cant_add;
}

void Graph::Dijkstra(int start){
    Init(start);
    BinaryHeap minQueue(num_vertex);   
    minQueue.BuildMinHeap(distance);
    visited.resize(num_vertex+1, false);

    while (!minQueue.IsHeapEmpty()) {
        // get minimum in min_queue
        int u = minQueue.ExtractMin();
        for (auto it = AdjList[u].begin();it != AdjList[u].end();it++) {
            // relax edge and update
            Relax(u, (*it).first, (*it).second);
            minQueue.DecreaseKey((*it).first, distance[(*it).first]);
        }
    }
}

void BinaryHeap::BuildMinHeap(vector<int> vec){
    for (int i=1;i<vec.size(); i++) {     
        heap[i].element = i; 
        heap[i].key = vec[i];
        pos[i] = i;
    }
    for (int i = (int)heap.size()/2; i >= 1 ; i--) {
        MinHeapify(i, (int)heap.size()-1);
    }
}

void BinaryHeap::MinHeapify(int node, int length){

    int left = node*2;     
    int right = node*2 + 1;     
    int smallest = (left <= length && heap[left].key < heap[node].key) ? left : node;

    if (right <= length && heap[right].key < heap[smallest].key) smallest = right;

    if (smallest != node) { 
        // update position
        swap(pos[heap[smallest].element], pos[heap[node].element]);
        Heap_swap(heap[smallest], heap[node]);
        MinHeapify(smallest, length);
    }
}

int BinaryHeap::ExtractMin(){
    int min_node = heap[1].element;
    // delete first element
    heap[1] = heap[heap.size() - 1];
    pos[heap[1].element] = 1;
    heap.erase(heap.begin() + heap.size() - 1);
    MinHeapify(1, (int)heap.size());

    return min_node;
}

void BinaryHeap::DecreaseKey(int node, int newKey){

    int index_node = pos[node]; 
    // no need to decrease
    if (newKey > heap[index_node].key) return;
    
    heap[index_node].key = newKey;
    // parent node = floor(index_node/2)
    // parent key should be smaller than child key
    while (index_node > 1 && heap[floor((index_node/2))].key > heap[index_node].key) { 
        swap(pos[heap[index_node].element], pos[heap[floor((index_node/2))].element]);
        Heap_swap(heap[index_node], heap[floor(index_node/2)]);
        index_node = floor(index_node/2);
    }
}

void BinaryHeap::MinHeapInsert(int node, int key){
    heap.push_back(HeapNode(node, key));
    pos[node] = heap.size()-1;
    DecreaseKey(node, key);
}
