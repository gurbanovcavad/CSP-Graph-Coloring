#include <bits/stdc++.h>

#define int long long
#define pi 3.14159265358979323846
using namespace std;
using ll = long long;
using ld = long double;
const int N = 2e5 + 5, M = 1e9 + 7, inf = 1e9;
const char nl = '\n';
set<int> graph[N];
set<int> domains[N];
// keeps track of prunes that are done during ac-3 function execution
vector<array<int, 2>> trail;
// number of nodes
int n = 0;

// to check if there is an empty domain 
bool emptyDomain() {
    for(int i = 1; i <= n; i++) if(domains[i].empty()) return 1;
    return 0;    
}

// to check if all of the nodes are colored
bool done() {
    for(int i = 1; i <= n; i++) if(domains[i].size() != 1) return 0;
    return 1;
}

void removeColor(int node, int color) {
    trail.push_back({node, color});
    domains[node].erase(color);
}

void undo(int sz) {
    while(trail.size() > sz) {
        int node = trail.back()[0], color = trail.back()[1]; trail.pop_back();
        domains[node].insert(color);
    }
}

int mrv() {
    int ans = -1, mn = inf;
    for(int i = 1; i <= n; i++) {
        int sz = domains[i].size();
        if(sz > 1 && sz < mn) {
            mn = sz;
            ans = i;
        }
    }
    return ans;
}

vector<int> lcv(int node) {
    // to hold the number of nodes that a color affects 
    vector<array<int, 2>> temp;
    for(auto &color: domains[node]) {
        int c = 0;
        for(auto &u: graph[node]) {
            if(domains[u].count(color)) c++;
        }
        temp.push_back({c, color});
    }
    sort(temp.begin(), temp.end(), [&](auto &i, auto &j) {
        if(i[0] != j[0]) return i[0] < j[0];
        return i[1] < j[1]; 
    });
    vector<int> res;
    for(auto &u: temp) res.push_back(u[1]);
    return res;
}

bool revise(int x, int y) {
    // to check that if we have removed any color
    bool ok = 0;
    // colors that have to be removed 
    vector<int> a;
    for(auto u: domains[x]) {
        // there is no support 
        if(domains[y].size() == 0) {
            a.push_back(u);
        } 
        // there can be support unless the element equals to the current color
        else if(domains[y].size() == 1) {
            if(*(domains[y].begin()) == u) {
                a.push_back(u);
            }
        }
        // if there are colors more than 1 then there is always a color to assign to y if we color x with color u 
    }
    if(!a.empty()) {
        for(auto &u: a) {
            if(domains[x].count(u)) {
                removeColor(x, u);
                ok = true;
            }
        }
    }
    return ok;
}   

bool ac3(queue<array<int, 2>> q) {
    while(!q.empty()) {
        int xi = q.front()[0], xj = q.front()[1]; q.pop();
        // if we have removed this arc then we need to add all the arcs where xi is the head
        if(revise(xi, xj)) {
            if(domains[xi].empty()) return 0;
            for(auto &u: graph[xi]) {
                if(u == xj) continue;
                q.push({u, xi});
            }
        }
    }
    return 1;
}

// this function checks if there is a way to color all the nodes before calling backtracking 
bool firstAc3() {
    queue<array<int, 2>> q;
    for(int i = 1; i <= n; i++) for(auto x: graph[i]) q.push({i, x});
    // if ac3 fails we will use the initial size of the trail to undo all the assignments we have done
    int cur = trail.size();
    bool ok = ac3(q);
    if(!ok) {
        undo(cur);
    }
    return ok;
}

bool backtracking() {
    if(emptyDomain()) return 0;
    if(done()) return 1;
    int node = mrv();
    if(node == -1) return 0;
    vector<int> colors = lcv(node);
    int initialSize = trail.size();
    for(auto &color: colors) {
        int cur = trail.size();
        vector<int> a;
        for(auto &u: domains[node]) if(u != color) a.push_back(u);
        for(auto &u : a) removeColor(node, u);
        queue<array<int, 2>> q;
        for(auto &u : graph[node]) q.push({u, node});
        bool ok = ac3(q);
        if(ok && !emptyDomain()) if(backtracking()) return 1;
        undo(cur);
    }
    undo(initialSize);
    return 0;
}

signed main() {
    ios_base::sync_with_stdio(false); cin.tie(nullptr); cout.tie(nullptr);
    
    // to check if there is a self-loop
    bool ok = 0;
    // get the input
    string l; cin >> l;
    int k = 0, i = 0, sz = l.size();
    for( ; i < sz; i++) {
        if(l[i] == '=') { i++; break;}
    }
    for( ; i < sz; i++) {
        k *= 10;
        k += (l[i] - '0');
    }
    while(cin >> l) {
        sz = l.size();
        int u = 0, v = 0;
        for(int i = 0; i < sz; i++) {
            if(l[i] == ',') {
                u = v;
                v = 0;
                continue;
            }
            v *= 10;
            v += (l[i] - '0');
        }
        // self-loop which leads to failure
        if(u == v) ok = 1;
        n = max(n, max(u, v));
        // add edeges to the graph
        graph[u].insert(v);
        graph[v].insert(u);
    }
    // craete a domain for each node 
    for(int u = 1; u <= n; u++) for(int i = 1; i <= k; i++) domains[u].insert(i);
    
    if(ok || !firstAc3()) {
        cout << "failure\n";
        return 0;
    }
    if(backtracking()) {
        cout << "SOLUTION: {";
        for(int i = 1; i <= n; i++) {
            cout << i << ": " << *(domains[i].begin());
            if(i != n) cout << ", ";
        }
        cout << "}\n";
    } else {
        cout << "failure\n";
    }
    return 0;
}