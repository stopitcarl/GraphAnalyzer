#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <stack>

using namespace std;

// ###################### Node ################################################
class Node
{
    int _id;
    vector<int> _connects;
    bool _visited = false;

  public:
    Node(int id) : _id(id) { _connects = vector<int>(); }
    void addConnection(int id) { _connects.push_back(id); }
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

void visit(Node *router)
{
    (*router).visit();
    printf("Node %d has been visited\n", (*router).getId());
}

void DFS(Node *router, vector<Node *> routers)
{
    Node curr = *router;
    vector<int> connections = curr.getConnections();
    visit(router);
    for (int i : connections)
    {
        printf("DFS loop %d\n", i);

        Node *r = routers[i - 1];
        if (!(*r).isVisited())
        {
            printf("DFS start on %d\n", i);
            DFS(r, routers);
        }
    }
}

void readInput(int &routers_num, int &connect_num, vector<Node *> &routers)
{

    printf("Reading\n");

    if (!scanf("%u", &routers_num) || !scanf("%u", &connect_num))
        exit(-1);
    routers.resize(routers_num);

    printf("Nodes: %d\nConnections: %d\n", routers_num, connect_num);

    // Initialize the nodes
    for (int i = 0; i < routers_num; i++)
        routers[i] = new Node(i + 1);

    int routes[2] = {0};
    while (scanf("%u %u", &routes[0], &routes[1]) > 0)
    {
        --connect_num;

        // Update routes
        (*routers[routes[0] - 1]).addConnection(routes[1]);
        (*routers[routes[1] - 1]).addConnection(routes[0]);
    }

    if (connect_num != 0)
        exit(-1);
}

void Tarjan_Visit(int &visited, int* d, int* low, Node *router, int current, std::stack<Node*> &L) {
    Node curr = *router;
    vector<int> connections = curr.getConnections();

    d[current] = low[current] = visited;
    visited++;
    L.push(router);
    // TO BE CONTINUED...
}

void SCC_Tarjan(int routers_num, vector<Node *> routers) {
    int visited = 0;
    std::stack<Node*> L;
    int d[routers_num];
    int low[routers_num];
    int i;

    for (i = 0; i < routers_num; i++) 
        d[i] = -1;
    for (i = 0; i < routers_num; i++)
        low[i] = -1;;
    for (int i = 0; i < routers_num; i++) 
        if (d[i] == -1) {
            Tarjan_Visit(visited, d, low, routers[i], i, L);
        }
}


int main()
{
    // Create array of router pointers
    vector<Node *> nodes = vector<Node *>();
    int routers_num = 0;
    int connect_num = 0;
    readInput(routers_num, connect_num, nodes);

    SCC_Tarjan(routers_num, nodes);

    // for (Node *node : nodes)
    // {
    //     cout << (*node).toString() << endl;
    // }
    // //DFS(nodes[0], nodes);

    return 0;
}
