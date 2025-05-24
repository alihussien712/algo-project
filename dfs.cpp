#include <iostream>
#include <vector>
#include <climits>  // For INT_MAX
using namespace std;

/*
 * FlowNetwork class represents a directed graph
 * where edges have flow capacities
 */
class FlowNetwork {
private:
    int nodeCount;  // Total number of nodes in the network
    vector<vector<int>> capacity;  // 2D matrix storing edge capacities

    /*
     * Helper function that uses DFS to find an augmenting path
     * from current node to sink node
     * Returns true if path exists, false otherwise
     * Also fills the parent array to track the path
     */
    bool findPath(int current, int sink, vector<int>& parent, vector<bool>& visited) {
        visited[current] = true;  // Mark current node as visited

        // Base case: reached the sink node
        if (current == sink) return true;

        // Check all adjacent nodes
        for (int next = 0; next < nodeCount; next++) {
            // If node isn't visited and has available capacity
            if (!visited[next] && capacity[current][next] > 0) {
                parent[next] = current;  // Track path
                // Recursively explore this path
                if (findPath(next, sink, parent, visited)) {
                    return true;  // Return if path to sink is found
                }
            }
        }
        return false;  // No path found from this node
    }

public:
    // Constructor - initializes network with given number of nodes
    FlowNetwork(int nodes) : nodeCount(nodes), capacity(nodes, vector<int>(nodes, 0)) {}

    /*
     * Adds a directed edge with given capacity
     * from: Source node
     * to: Destination node
     * cap: Maximum flow capacity of this edge
     */
    void addEdge(int from, int to, int cap) {
        capacity[from][to] = cap;
    }

    /*
     * Implements Ford-Fulkerson algorithm to calculate maximum flow
     * source: Starting node
     * sink: Target node
     * Returns: Maximum possible flow from source to sink
     */
    int getMaxFlow(int source, int sink) {
        int maxFlow = 0;  // Initialize result
        vector<int> parent(nodeCount, -1);  // Stores path

        // Keep finding augmenting paths until none exist
        while (true) {
            vector<bool> visited(nodeCount, false);  // Reset visited nodes

            // Stop if no augmenting path exists
            if (!findPath(source, sink, parent, visited)) break;

            // Find minimum (bottleneck) capacity in the path
            int pathFlow = INT_MAX;
            for (int node = sink; node != source; node = parent[node]) {
                pathFlow = min(pathFlow, capacity[parent[node]][node]);
            }

            // Update residual capacities
            for (int node = sink; node != source; node = parent[node]) {
                // Reduce capacity on forward edge
                capacity[parent[node]][node] -= pathFlow;
                // Increase capacity on reverse edge
                capacity[node][parent[node]] += pathFlow;
            }

            // Add this path's flow to total
            maxFlow += pathFlow;
        }

        return maxFlow;
    }
};

int main() {
    cout << "Maximum Flow Calculator (Ford-Fulkerson with DFS)\n";
    cout << "===============================================\n";

    // Get network size from user
    int nodes, edges;
    cout << "Enter number of nodes: ";
    cin >> nodes;
    cout << "Enter number of edges: ";
    cin >> edges;

    // Create network
    FlowNetwork network(nodes);
     
    // Add all edges
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
    cout << "\nMaximum flow from node " << source << " to node " << sink
        << " is: " << result << endl;

    return 0;
}