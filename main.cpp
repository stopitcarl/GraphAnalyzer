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
* Biggest SCC if all artic. points decide to collectively fuck off to a better place
*/

// ###################### Global variables ################################################
class Node;
int nodesNum;
int connectNum;
vector<Node *> nodes;
vector<int *> SCCs; // {max ID of graph, number of nodes}
int SccCount;
int biggestSCC;
deque<int> dequeL;
int *d;
int *low;
int *parent;

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

/*
class Scc
{
    int id;
    int root; // TODO: optional?
    vector<int> nodes;
};
*/

// ###################### Input Handling ################################################
void readInput()
{

    printf("Reading\n");

    if (!scanf("%u", &nodesNum) || !scanf("%u", &connectNum))
        exit(-1);
    nodes.resize(nodesNum);

    printf("Nodes: %d\nConnections: %d\n", nodesNum, connectNum);

    // Initialize the nodes
    for (int i = 0; i < nodesNum; i++)
        nodes[i] = new Node(i + 1);

    int routes[2] = {0};
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

    for (int a : connections)
    {
        if (d[a] == -1 || EXISTS(dequeL, a))
        {
            if (d[a] < 0)
                tarjanVisit(visited, a);
            low[current] = MIN(low[current], low[a]);
        }
    }

    if (d[current] == low[current])
    {
        ++SccCount;
        int nodeCount = 1;
        int v = dequeL.back();
        int max = v;
        while (current != v)
        {
            if (v > max)
                max = v;

            dequeL.pop_back();
            v = dequeL.back();

            nodeCount++;
        }

        SCCs.push_back(new int[2]{max, nodeCount});
    }
}

void SccTarjan(int nodesNum, vector<Node *> routers)
{
    int visited = 0;
    dequeL = deque<int>();

    int i;

    for (i = 0; i < nodesNum; i++) // TODO: find a cooler way initiliaze
    {
        d[i] = -1;
        low[i] = -1;
    }

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
    SccCount = 0;

    readInput();
    d = new int[nodesNum];
    low = new int[nodesNum];
    parent = new int[nodesNum];
    for (int i = 0; i < nodesNum; i++)
        printf("%d:%d\n", i, parent[i]);

    SccTarjan(nodesNum, nodes);

    // Output
    printf("Scc: %d\n", SccCount);
    printf("IDS:");

    for (int *scc : SCCs)
        printf(" %d", scc[0] + 1);

    // Free allocs
    free(d);
    free(low);
    free(parent);
    SCCs.clear();

    return 0;
}
