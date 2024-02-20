#include <bits/stdc++.h>
using namespace std;
#define id first
#define weight second
#define MAX_N 1000005

int n, m;
// node, weight (init 1)
vector<pair<int, int>> adj[MAX_N];
set<int> child[MAX_N];
vector<int> dp(MAX_N);
vector<bool> visited(MAX_N);
vector<int> path(MAX_N);
set<int> check;
void dfs(int pos){
    visited[pos] = true;
    for (int i=0;i<adj[pos].size();i++){
        if (!visited[adj[pos][i].id]){
            dfs(adj[pos][i].id);  
        }
        dp[pos] = max(dp[pos], dp[adj[pos][i].id] + adj[pos][i].weight);
    }
}

void condense_loop(int pos){
    // cout<<"condense loop to "<<pos<<"\n";
    // exist loop
    if(check.count(pos) != 0){
        // cout<<"find loop at "<<pos<<"\n";
        int cnt = 0;
        int cur = 0;
        set<int> in_loop;
        in_loop.insert(pos);
        while(!path.empty() && path.back() != pos){
            cnt++;
            cur = path.back();   
            in_loop.insert(cur);  
            // reattach to loop start point
            for(int i=0;i<adj[cur].size();i++){
                // not in loop and not already a child of pos
                // [debug] return here is WA NOT RE 
                if(in_loop.count(adj[cur][i].id) == 0 && child[pos].count(adj[cur][i].id) == 0){
                    // [debug] return here is WA/RE
                    adj[pos].push_back({adj[cur][i].id, adj[cur][i].weight});                  
                    // cout<<"reattach way to "<<adj[cur][i].id<<"\n";
                }
            }
            // [debug] return here is RE
            adj[cur].clear();
            path.pop_back();
            check.erase(cur);
        }
        path.pop_back();
        check.erase(pos);
        for(auto i=adj[pos].begin();i!=adj[pos].end();i++){
            // remove loop
            if((*i).id == cur){
                adj[pos].erase(i);
                child[pos].erase(cur);
                break;
            }
        }
        int pre;
        if(path.back() != 0){
            pre = path.back();
            // cout<<"update weight from "<<pre<<" to "<<pos<<" : +"<<cnt<<"\n";
            for(int i=0;i<adj[pre].size();i++){
                if(adj[pre][i].id == pos){
                    adj[pre][i].weight += cnt;
                    break;
                }
            }
        }
        // loop is at start;
        else{
            // loop doesnt go outward
            if(adj[pos].size() == 0){
                dp[pos] += cnt;
            }
            else{
                // move the extra weight to child instead
                for(int i=0;i<adj[pos].size();i++){
                    adj[pos][i].weight += cnt;
                }
            }
        }
        
    }
    visited[pos] = true;
    path.push_back(pos);
    check.insert(pos);
    for (int i=0;i<adj[pos].size();i++){
        // not visited or potentially cycle
        if((!visited[adj[pos][i].id]) || visited[adj[pos][i].id] && check.count(adj[pos][i].id) != 0){
            condense_loop(adj[pos][i].id);  
        }
        
    }
    path.pop_back();
    check.erase(pos);
}

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    int u, v;
    cin>>n>>m;
    for(int i=0;i<m;i++){
        cin>>u>>v;
        adj[u].push_back({v, 1});
        child[u].insert(v);
    }

    for(int i=0;i<=n;i++){
        visited[i] = false;
        dp[i] = 1;
    }

    for(int i=1;i<=n;i++){
        if(!visited[i]){
            condense_loop(i);
        }
    }
    
    for(int i=0;i<=n;i++) visited[i] = false;

    for (int i=1;i<=n;i++) {
        if (!visited[i]){
            dfs(i);
        } 
    }
 
    int ans = 0;
    
    for (int i=1;i<=n;i++) {
        ans = max(ans, dp[i]);
    }
    
    cout<<ans<<"\n";
}

