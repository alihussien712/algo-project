#include <iostream>
#include <vector>
#include <climits>
#include <queue>
using namespace std;

/*
 * FlowNetwork class for maximum flow calculation
 * using Edmonds-Karp algorithm (BFS-based Ford-Fulkerson)
 */
class FlowNetwork {
private:
    int nodeCount;  // Total number of nodes
    vector<vector<int>> capacity;  // Residual capacity matrix

    /*
     * BFS to find shortest augmenting path
     * Returns true if path exists from start to sink
     * parent[] stores the path
     */
    bool bfsFindPath(int start, int sink, vector<int>& parent) {
        vector<bool> visited(nodeCount, false);
        queue<int> q;

        q.push(start);
        visited[start] = true;
        parent[start] = -1;  // Source has no parent

        while (!q.empty()) {
            int current = q.front();
            q.pop();

            // Check all adjacent nodes
            for (int next = 0; next < nodeCount; next++) {
                // If node is unvisited and has capacity
                if (!visited[next] && capacity[current][next] > 0) {
                    // If we reached the sink, we're done
                    if (next == sink) {
                        parent[next] = current;
                        return true;
                    }

                    q.push(next);
                    parent[next] = current;
                    visited[next] = true;
                }
            }
        }
        return false;  // No path found
    }

public:
    // Initialize network with given number of nodes
    FlowNetwork(int nodes) : nodeCount(nodes), capacity(nodes, vector<int>(nodes, 0)) {}

    // Add directed edge with given capacity
    void addEdge(int from, int to, int cap) {
        capacity[from][to] = cap;
    }

    /*
     * Calculate maximum flow using Edmonds-Karp algorithm
     * Returns maximum flow value from source to sink
     */
    int getMaxFlow(int source, int sink) {
        int maxFlow = 0;
        vector<int> parent(nodeCount, -1);

        // While augmenting paths exist
        while (bfsFindPath(source, sink, parent)) {
            // Find minimum (bottleneck) capacity in path
            int pathFlow = INT_MAX;
            for (int node = sink; node != source; node = parent[node]) {
                pathFlow = min(pathFlow, capacity[parent[node]][node]);
            }

            // Update residual capacities
            for (int node = sink; node != source; node = parent[node]) {
                capacity[parent[node]][node] -= pathFlow;  // Forward edge
                capacity[node][parent[node]] += pathFlow;  // Reverse edge
            }

            maxFlow += pathFlow;  // Add this path's flow
        }

        return maxFlow;
    }
};

int main() {
    cout << "Maximum Flow Calculator (Edmonds-Karp Algorithm)\n";
    cout << "===============================================\n";

    // Get network size
    int nodes, edges;
    cout << "Enter number of nodes: ";
    cin >> nodes;
    cout << "Enter number of edges: ";
    cin >> edges;

    // Create network
    FlowNetwork network(nodes);

    // Add edges
    cout << "\nEnter edges (from to capacity):\n";
    for (int i = 0; i < edges; i++) {
        int from, to, cap;
        cin >> from >> to >> cap;
        network.addEdge(from, to, cap);
    }

    // Get flow endpoints
    int source, sink;
    cout << "\nEnter source and sink nodes: ";
    cin >> source >> sink;

    // Calculate and display result
    int result = network.getMaxFlow(source, sink);
    cout << "\nMaximum flow: " << result << endl;

    return 0;
}