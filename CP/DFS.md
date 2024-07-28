Undirected graph:
1. Vertex coloured: white and black
2. Edges are either 
    * tree edge
    * back edge: edges that go from node to its ancestor

Directed graph:
1. Vertex coloured: white, gray, and black
2. Edges are either 
    * tree edge
    * back edge: edges that go from node to its ancestor
    * forward edge: `(u, v)`, where `v` is a descendant of `u`
    * cross edge: go across different depth-first trees, or in the same tree where one vertex is not the ancestor of the other

DFS on a tree can be done without `seen` array, using the node and its parent.

#### Basic DFS

```c++
vector<vector<int>> adj;
vector<int> seen;

// On graph (undirected / directed)
auto dfs = [&] (auto self, int u) {
    seen[u] = true;
    for (int v: adj[u]) {
        if (!seen[v]) {
            dfs(dfs, v);
        }
    }
}

// On tree
auto dfs = [&] (auto self, int u, int p = -1) {
    for (int v: adj[u]) {
        if (v != p) {
            dfs(dfs, v, u);
        }
    }
}
```

#### DFS on directed graph with color & time

```c++
vector<int> color; // 0 - white, 1 - gray, 2 - black
vector<int> in_time, out_time;
int timer = 0;

auto dfs = [&] (auto self, int u) {
    in_time[u] = timer++;
    color[u] = 1;
    for (int v: adj[u]) {
        if (color[v] == 0) {
            dfs(dfs, v);
        } else if (color[v] == 1) {
            // detected cycle
        }
    }
    out_time[u] = timer++;
    color[u] = 2;
}
```

#### Binary Lifting and LCA on a Tree

```c++
int n;
vector<vector<int>> g(n);
int m = log2(n) + 1;
vector<vector<int>> par(m, vector<int> n);
vector<int> d(n);

auto dfs = [&] (auto dfs, int u, int p, int dep) {
    par[0][u] = p;
    d[u] = dep;
    for (int v: g[u]) {
        if (v == p) continue;
        dfs(dfs, v, u, dep+1);
    }
};

dfs(dfs, 0, 0, 0);
// binary lifting
for (int i = 1; i < m; i++) 
    for (int u = 1; u < n; u++)
        par[i][u] = par[i-1][par[i-1][u]];

auto lca = [&] (int u, int v) -> int {
    if (d[u] < d[v]) swap(u, v);
    // bring u and v to the same level
    for (int i = 0; (1<<i) <= (d[u] - d[v]); i++) {
        if ((1<<i) & (d[u] - d[v])) u = par[i][u];
    }
    // jump u and v together
    for (int i = m-1; i>=0; i--) {
        if (par[i][u] == par[i][v]) continue;
        u = par[i][u];
        v = par[i][v];
    }
    return u == v ? u : par[0][u];
};
```

#### Finding bridges and articulation points in undirected graph

`dp[u]` denotes number of back edges passing over `u` and its parent
`dp[u] = sum(dp[child])_{(u, child)} - back_edge_into[u] + back_edge_out_of[u]`
`(u,p)` is bridge iff `dp[u] = 0`

Bridges form a forest

`root` is articulation point iff it has at least two children in the dfs tree.
`leaf` is never an articulation point
`u` is articulation point iff `dp[u] = 0`

```c++
int n;
vector<vector<int>> g(n);
vector<int> back_into(n), back_out(n);
vector<int> dp(n);
vector<int> seen(n);
vector<vector<int>> bridges(n);

int dfs(int u, int p) {
    seen[u] = true;
    int child_dp_count = 0;
    for (int v: g[u]) {
        if (v == par) continue;
        if (seen[v]) {
            back_out[u]++;
            back_into[v]++;
        } else {
            child_dp_count += dfs(v, u);
        }
    }

    dp[u] = child_dp_count - back_into[u] + back_out[u];
    if (dp[u] == 0) {
        bridges[p].push_back(u);
        bridges[u].push_back(p);
    }
    return dp[u];
}
```

#### Strongly connected components and Condensation graph

1. Run DFS on `G` and order vertices according to their DFS exit times.
2. Reverse the order of vertices and run DFS on `G^T`.
3. Extract components.

```c++
vector<vector<int>> adj, adj_rev;
vector<bool> used;
vector<int> order, component;
vector<int> roots(n, 0);
vector<int> root_nodes;
vector<vector<int>> adj_scc(n);

void dfs1(int v) {
    used[v] = true;

    for (auto u : adj[v])
        if (!used[u])
            dfs1(u);

    order.push_back(v);
}

void dfs2(int v) {
    used[v] = true;
    component.push_back(v);

    for (auto u : adj_rev[v])
        if (!used[u])
            dfs2(u);
}

int main() {
    int n;
    // ... read n ...

    for (;;) {
        int a, b;
        // ... read next directed edge (a,b) ...
        adj[a].push_back(b);
        adj_rev[b].push_back(a);
    }

    used.assign(n, false);

    for (int i = 0; i < n; i++)
        if (!used[i])
            dfs1(i);

    used.assign(n, false);
    reverse(order.begin(), order.end());

    for (auto v : order)
        if (!used[v]) {
            dfs2 (v);

            // ... processing next component ...
            int root = component.front();
            for (auto u : component) roots[u] = root;
            root_nodes.push_back(root);

            component.clear();
        }

    for (int v = 0; v < n; v++)
        for (auto u : adj[v]) {
            int root_v = roots[v],
                root_u = roots[u];

            if (root_u != root_v)
                adj_scc[root_v].push_back(root_u);
        }
}
```

