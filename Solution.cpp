#include <unordered_map>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

class Solution {
public:
    // Helper function to check if the graph is bipartite starting from node `curr`
    bool isBipartite(unordered_map<int, vector<int>> &adj, int curr, vector<int>& colors, int currColor) {
        // Assign the current color to the node
        colors[curr] = currColor;

        // Explore all the neighbors of the current node
        for(int &ngbr : adj[curr]) {
            // If a neighbor has the same color as the current node, it's not bipartite
            if(colors[ngbr] == colors[curr]) {
                return false;
            }

            // If the neighbor hasn't been visited yet, recurse with the opposite color
            if(colors[ngbr] == -1) {
                if(isBipartite(adj, ngbr, colors, 1 - currColor) == false) {
                    return false;
                }
            }
        }

        // If no conflict was found, the graph is bipartite for the current component
        return true;
    }

    // BFS to determine the maximum level or depth of each connected component
    int bfs(unordered_map<int, vector<int>> &adj, int currNode, int n) {
        queue<int> que;
        vector<bool> visited(n, false);
        
        // Start BFS from the current node
        que.push(currNode);
        visited[currNode] = true;

        int level = 1;  // Maximum groups (or levels) in this component
        while(!que.empty()) {

            int size = que.size();
            // Process all nodes at the current level
            while(size--) {
                int curr = que.front();
                que.pop();

                // Add unvisited neighbors to the queue
                for(int &ngbr : adj[curr]) {
                    if(visited[ngbr])
                        continue;
                    
                    que.push(ngbr);
                    visited[ngbr] = true;
                }
            }
            // Increase the level count after processing all nodes at the current level
            level++;
        }

        // Return the number of levels (connected components' depth)
        return level-1;
    }

    // Helper function to calculate the maximum level from each component
    int getMaxFromEachComp(unordered_map<int, vector<int>> &adj, int curr, vector<bool>& visited, vector<int>& levels) {
        int maxGrp = levels[curr];  // Initial maximum level of the current node
        visited[curr] = true;

        // Explore neighbors and calculate the maximum level in the connected component
        for(int &ngbr : adj[curr]) {
            if(!visited[ngbr]) {
                maxGrp = max(maxGrp, getMaxFromEachComp(adj, ngbr, visited, levels));
            }
        }

        // Return the maximum level found in this component
        return maxGrp;
    }

    // Main function to calculate the maximum number of levels in the graph
    int magnificentSets(int n, vector<vector<int>>& edges) {
        unordered_map<int,vector<int>> mp;

        // Construct adjacency list from the given edges
        for(auto& edge : edges) {
            int from = edge[0] - 1;  // Adjust to 0-based indexing
            int to = edge[1] - 1;    // Adjust to 0-based indexing
            mp[from].push_back(to);
            mp[to].push_back(from);
        }

        // Array to store colors for checking bipartiteness (initially uncolored)
        vector<int> visited1(n, -1);

        // Check if the graph is bipartite for every unvisited node
        for(int i = 0; i < n; i++) {
            if(visited1[i] == -1) {
                if(!isBipartite(mp, i, visited1, 1)) {
                    return -1;  // If any component is not bipartite, return -1
                }
            }
        }

        // Array to store the number of levels (max depth) for each component
        vector<int> nope(n, -1);

        // Perform BFS for each node to determine the maximum level of its component
        for(int i = 0; i < n; i++) {
            int pyo = bfs(mp, i, n);
            nope[i] = pyo;  // Store the result in the levels array
        }

        int opans = 0;  // Variable to store the final result
        vector<bool> visited(n, false);  // Array to track visited nodes during DFS

        // Perform DFS to compute the maximum levels from each component
        for(int i = 0; i < n; i++) {
            if(visited[i] == 0) {
                opans += getMaxFromEachComp(mp, i, visited, nope);
            }
        }

        // Return the final answer: the sum of the maximum levels from each component
        return opans;
    }
};
