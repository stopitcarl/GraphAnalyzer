#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <deque>

#define EXISTS(a, n) (find(begin(a), end(a), n) != end(a))
#define MIN(a, b) (a < b ? a : b)

using namespace std;

/*
* Number of SCCs
* Graph IDs (1 for each)
* Number articulation points
* Size of biggest SCC if all artic. points decide to collectively fuck off to a better place
*/

// ###################### Global variables ################################################
class Node;
int nodesNum;
int connectNum;
vector<Node *> nodes;
vector<int *> SCCs; // {max ID of graph, number of nodes}
int sccCount;
int apCount;
int biggestSCC;
deque<int> dequeL;
int *d;
int *low;
int *parent;
bool *ap;

// ###################### Node ################################################
class Node
{
    int _id;
    vector<int> _connects;
    bool _visited = false;

  public:
    Node(int id) : _id(id) { _connects = vector<int>(); }
    void addConnection(int index) { _connects.push_back(index); }
    int getId() { return _id; }
    bool isVisited() { return _visited; }
    void visit() { _visited = true; }
    vector<int> getConnections() { return _connects; }

    string toString()
    {
        string a = "Id:";
        a += to_string(_id) + "\nvisited:" + (_visited ? "true" : "false") + "\nconnections: " + to_string(_connects.size());
        return a;
    }

    ~Node() {}
};

// ###################### Input Handling ################################################
void readInput()
{
    if (!scanf("%u", &nodesNum) || !scanf("%u", &connectNum))
        exit(-1);
    nodes.resize(nodesNum);

    // Initialize the nodes
    for (int i = 0; i < nodesNum; i++)
        nodes[i] = new Node(i + 1);

    int routes[2] = {0, 0};
    while (scanf("%u %u", &routes[0], &routes[1]) > 0)
    {
        --connectNum;
        // Update routes
        (*nodes[routes[0] - 1]).addConnection(routes[1] - 1);
        (*nodes[routes[1] - 1]).addConnection(routes[0] - 1);
    }

    if (connectNum != 0)
        exit(-1);
}

// ###################### Algorithms  ################################################

void tarjanVisit(int &visited, int &current)
{
    Node curr = *(nodes[current]);
    vector<int> connections = curr.getConnections();

    d[current] = low[current] = visited;
    visited++; // ???
    dequeL.push_back(current);

    int children = 0;
    for (int a : connections)
    {
        if (d[a] == -1 || EXISTS(dequeL, a))
        {
            ++children;
            if (d[a] == -1)
            {
                parent[a] = current;
                tarjanVisit(visited, a);
            }

            printf("Node %d: parent[%d]=%d, children=%d, low[a]=%d\n", d[current], current, parent[current], children, low[a]);

            // (1) u is root of DFS tree and has two or more chilren.
            if (parent[current] == -1 && children > 1 && !ap[current])
            {
                ap[current] = true;
                ++apCount;
                printf("(1)Found AP: %d\n", current + 1);
            }

            // (2) If u is not root and low value of one of its child is more
            // than discovery value of u.
            if (parent[current] != -1 && low[a] >= d[current])
            {
                ap[current] = true;
                ++apCount;
                printf("(2)Found AP: %d\n", current + 1);
            }
            low[current] = MIN(low[current], low[a]);
        }
    }

    if (d[current] == low[current])
    {
        ++sccCount;
        int nodeCount = 0, nodeCountWithDiscount = 0;
        int v = dequeL.back();
        int max = v;
        while (current != v)
        {
            if (v > max)
                max = v;
            if (!ap[v])
                ++nodeCountWithDiscount;
            nodeCount++;

            dequeL.pop_back();
            v = dequeL.back();
        }

        SCCs.push_back(new int[3]{max, nodeCount, nodeCountWithDiscount});
    }
}

void SccTarjan(int nodesNum, vector<Node *> routers)
{
    int visited = 0;
    dequeL = deque<int>();

    int i;

    // Initiliaze d, parent and low arrays to -1
    for (i = 0; i < nodesNum; i++)
    {
        d[i] = low[i] = parent[i] = -1;
        ap[i] = false;
    }

    // Run DFS
    for (i = 0; i < nodesNum; i++)
        if (d[i] == -1)
            tarjanVisit(visited, i);
}

int main()
{
    // Create array of router pointers
    nodes = vector<Node *>();
    nodesNum = 0;
    connectNum = 0;
    biggestSCC = 0;
    sccCount = 0;
    apCount = 0;

    readInput();
    d = new int[nodesNum];
    low = new int[nodesNum];
    parent = new int[nodesNum];
    ap = new bool[nodesNum];

    SccTarjan(nodesNum, nodes);

    // Output
    printf("Scc: %d\n", sccCount);
    printf("IDS:");
    int maxDiscountedGraph = 0;
    for (int *scc : SCCs)
    {
        printf(" %d", scc[0] + 1);
        if (scc[2] > maxDiscountedGraph)
            maxDiscountedGraph = scc[2];
    }

    printf("\nAPs: %d\n", apCount);
    printf("Max Graph: %d\n", maxDiscountedGraph);

    // Free allocs
    free(d);
    free(low);
    free(parent);
    SCCs.clear();

    return 0;
}