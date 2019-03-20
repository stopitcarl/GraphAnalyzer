#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <stack>

//#define EXISTS(a, n) (find(a.rbegin(), a.rend(), n) != a.end())
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
stack<int> stackL;
int *d;
int *low;
int *parent;
bool *ap;
bool *stackU;

// ###################### Node ################################################
class Node
{
    vector<int> _connects;

  public:
    Node() { _connects = vector<int>(); }
    void addConnection(int index) { _connects.push_back(index); }
    vector<int> getConnections() { return _connects; }
    ~Node() { _connects.clear(); }
};

// ###################### Input Handling ################################################
void readInput()
{
    if (!scanf("%u", &nodesNum) || !scanf("%u", &connectNum))
        exit(-1);
    nodes.resize(nodesNum);

    // Initialize the nodes
    for (int i = 0; i < nodesNum; i++)
        nodes[i] = new Node();

    int routes[2] = {0, 0};
    while (scanf("%u %u", &routes[0], &routes[1]) > 0)
    {
        --connectNum;
        // Update routes
        (*nodes[routes[0] - 1]).addConnection(routes[1] - 1);
        (*nodes[routes[1] - 1]).addConnection(routes[0] - 1);
    }
    /*// Initialize the nodes
    for (int i = 0; i < nodesNum; i++)
        (*nodes[i]).getConnections().shrink_to_fit();
    */

    if (connectNum != 0)
        exit(-1);
}

// ###################### Algorithms  ################################################

int tarjanVisit(int &visited, int &current)
{
    int weight = 0;

    Node curr = *(nodes[current]);
    vector<int> connections = curr.getConnections();
    d[current] = low[current] = visited;
    visited++; // ???
    stackL.push(current);
    stackU[current] = true;

    int children = 0;
    for (int a : connections)
    {
        //printf("Node %d checking connection %d\n", current + 1, a + 1);
        if (d[a] == -1 || stackU[a])
        {
            if (d[a] == -1)
            {
                children++;
                // printf("Node %d visiting %d\n", current + 1, a + 1);
                parent[a] = current;
                weight += tarjanVisit(visited, a);
                if (weight > biggestSCC)
                    biggestSCC = weight;
                //printf("Callback: Node %d visited %d\n", current + 1, a + 1);

                //printf("Updating low of %d with low[current]=%d or low[%d]=%d\n", current + 1, low[current], a + 1, low[a]);
                // (1) u is root of DFS tree and has two or more chilren.
                if (parent[current] == -1 && children > 1 && !ap[current] && !ap[current])
                {
                    ap[current] = true;
                    ++apCount;
                    // printf("(1)Found AP: %d\n", current + 1);
                }

                // (2) If u is not root and low value of one of its child is more
                // than discovery value of u.
                if (parent[current] != -1 && low[a] >= d[current] && !ap[current])
                {
                    ap[current] = true;
                    ++apCount;
                    //printf("(2)Found AP: %d\n", current + 1);
                }
                low[current] = MIN(low[current], low[a]);
            }
            else
            {
                low[current] = MIN(low[current], d[a]);
            }
        }
    }

    if (d[current] == low[current])
    {
        ++sccCount;
        int nodeCount = 0;
        int v = stackL.top();
        int max = v;
        while (current != v)
        {
            if (v > max)
                max = v;
            nodeCount++;
            stackL.pop();
            v = stackL.top();
        }

        SCCs.push_back(new int[3]{max, nodeCount});
    }
    //printf("Returning from %d with weight: %d\n", current + 1, weight);
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
            if (weight > biggestSCC)
                biggestSCC = weight;
        }
}

bool compare(const int *a, const int *b)
{
    return a[0] < b[0];
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
    stackU = new bool[nodesNum];

    SccTarjan(nodesNum, nodes);

    // Output
    // Number of SCCs
    printf("%d\n", sccCount);
    // Number of IDs
    sort(SCCs.begin(), SCCs.end(), compare);
    printf("%d", SCCs.front()[0] + 1);
    for (vector<int *>::iterator it = SCCs.begin() + 1; it != SCCs.end(); ++it)
        printf(" %d", (*it)[0] + 1);
    // Number of APs
    printf("\n%d\n", apCount);
    // Size of bigges SCC without APs
    printf("%d\n", biggestSCC);

    // Free allocs
    free(d);
    free(low);
    free(parent);
    free(ap);
    SCCs.clear();
    nodes.clear();

    return 0;
}