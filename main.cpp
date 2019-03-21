#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <stack>

#define MIN(a, b) (a < b ? a : b)

using namespace std;

/*
Input:
* Number of vertices (nodesNum)
* Number of edges (connectNum)
* Edge (e.g. 2 4)
* Edge (e.g. 1 3)
* ...

Output
* Number of SCCs
* Graph IDs (1 for each)
* Number articulation points
* Size of biggest SCC if all artic. points decide to collectively fuck off to a better place
*/

// ###################### Global variables ####################################################
class Node;
int nodesNum;         // Number of vertices
int connectNum;       // Number of edges
vector<Node *> nodes; // Vector of vertices
vector<int *> SCCs;   // Vector of SCCs, each containing {max ID of graph, number of nodes}
int sccCount;         // Number of SCCs
int apCount;          // Number of articulation points
int biggestSCC;       // Size of the biggest SCC
stack<int> stackL;    // Stack for the Tarjan Algorithm
int *d;               // Array d for the Tarjan Algorithm
int *low;             // Array low for the Tarjan Algorithm
int *parent;          // Array that contains the direct parent of each vertex
bool *ap;             // Checklist if a point i an articulation point
bool *stackU;         // Checklist if a vertex is in the stack

// ###################### Node ################################################################
class Node
{
    vector<int> _connects; // Vector of adjacencies of the vertex

  public:
    Node() { _connects = vector<int>(); }
    void addConnection(int index) { _connects.push_back(index); }
    vector<int> getConnections() { return _connects; }
    ~Node() { _connects.clear(); }
};

// ###################### Input Handling ######################################################
void readInput()
{
    // Read the number of vertexes and edges
    if (!scanf("%u", &nodesNum) || !scanf("%u", &connectNum))
        exit(-1);
    // Shorten the nodes list
    nodes.resize(nodesNum);

    // Initialize the nodes
    for (int i = 0; i < nodesNum; i++)
        nodes[i] = new Node();

    // Update the adjacencies list of each vertex
    int routes[2] = {0, 0};
    while (scanf("%u %u", &routes[0], &routes[1]) > 0)
    {
        --connectNum;
        (*nodes[routes[0] - 1]).addConnection(routes[1] - 1);
        (*nodes[routes[1] - 1]).addConnection(routes[0] - 1);
    }

    if (connectNum != 0)
        exit(-1);
}

// ###################### Algorithms  #########################################################

int tarjanVisit(int &visited, int &current)
{
    int weight = 0;

    Node curr = *(nodes[current]);                   // The vertex being visited
    vector<int> connections = curr.getConnections(); // The adjancencies of the current vertex
    d[current] = low[current] = visited;             // Fill in the d and low arrays
    visited++;
    // Put the vertex in the stack
    stackL.push(current);
    stackU[current] = true;

    int children = 0; // Number of vertex's children
    for (int a : connections)
    {
        if (d[a] == -1 || stackU[a])
        {
            if (d[a] == -1)
            {
                children++;
                parent[a] = current;
                weight += tarjanVisit(visited, a);
                // Update the size of the biggest SCC
                if (weight > biggestSCC)
                    biggestSCC = weight;

                // Check if vertex is an articulation point:

                //  Check if vertex is root of DFS tree and has two or more chilren.
                if (parent[current] == -1 && children > 1 && !ap[current] && !ap[current])
                {
                    ap[current] = true;
                    ++apCount;
                }

                // Check if vertex is not root and low-link of one of its child is bigger
                // than discovery value.
                if (parent[current] != -1 && low[a] >= d[current] && !ap[current])
                {
                    ap[current] = true;
                    ++apCount;
                }
                low[current] = MIN(low[current], low[a]);
            }
            else
            {
                low[current] = MIN(low[current], d[a]);
            }
        }
    }

    // Store a new SCC
    if (d[current] == low[current])
    {
        ++sccCount;
        int nodeCount = 0;
        int v = stackL.top();
        int max = v;
        // Check the whole stack untill
        while (current != v)
        {
            // Find the biggest id in the SCC
            if (v > max)
                max = v;
            nodeCount++;
            stackL.pop();
            v = stackL.top();
        }

        SCCs.push_back(new int[3]{max, nodeCount});
    }

    if (ap[current])
        return 0;
    return weight + 1;
}

void SccTarjan(int nodesNum, vector<Node *> routers)
{
    int visited = 0;
    stackL = stack<int>();

    int i;

    // Initiliaze util arrays
    for (i = 0; i < nodesNum; i++)
    {
        d[i] = low[i] = parent[i] = -1;
        ap[i] = stackU[i] = false;
    }

    // Run DFS
    int weight = 0;
    for (i = 0; i < nodesNum; i++)
        if (d[i] == -1)
        {
            weight = tarjanVisit(visited, i);
            // Update the size of the biggest SCC
            if (weight > biggestSCC)
                biggestSCC = weight;
        }
}

// For the sorting algorithm
bool compare(const int *a, const int *b)
{
    return a[0] < b[0];
}

// ###################### Main ################################################################
int main()
{
    // Create array of router pointers
    nodes = vector<Node *>();
    nodesNum = 0;
    connectNum = 0;
    biggestSCC = 0;
    sccCount = 0;
    apCount = 0;

    // Input
    readInput();

    // Initialize all the arrays
    d = new int[nodesNum];
    low = new int[nodesNum];
    parent = new int[nodesNum];
    ap = new bool[nodesNum];
    stackU = new bool[nodesNum];
    SccTarjan(nodesNum, nodes);

    // Output:
    // (1) Number of SCCs
    printf("%d\n", sccCount);
    // (2) Number of IDs
    sort(SCCs.begin(), SCCs.end(), compare);
    printf("%d", SCCs.front()[0] + 1);
    for (vector<int *>::iterator it = SCCs.begin() + 1; it != SCCs.end(); ++it)
        printf(" %d", (*it)[0] + 1);
    // (3) Number of APs
    printf("\n%d\n", apCount);
    // (4) Size of bigges SCC without APs
    printf("%d\n", biggestSCC);

    // Free allocs
    delete[] d;
    delete[] low;
    delete[] parent;
    delete[] ap;
    delete[] stackU;

    for (Node *node : nodes)
        delete node;
    for (int *scc : SCCs)
        delete[] scc;

    return 0;
}