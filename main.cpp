#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// ###################### Grafos ################################################
typedef unsigned int Int;

class Router;

class Router
{
    int _id;
    vector<Int> _connects;

  public:
    Router(Int id) : _id(id) { _connects = vector<Int>(); }
    void addConnection(Int id)
    {
        _connects.push_back(id);
    }

    ~Router() {}
};

void readInput(Int &routers_num, Int &connect_num, vector<Router *> &routers)
{

    printf("Reading\n");

    if (!scanf("%u", &routers_num) || !scanf("%u", &connect_num))
        exit(-1);
    routers.resize(routers_num);

    printf("Routers: %d\nConnections: %d\n", routers_num, connect_num);

    // Initialize the fuckers

    for (Int i = 0; i < routers_num; i++)
        routers[i] = new Router(i + 1);

    Int routes[2] = {0};
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

int main()
{
    // Create array of router pointers
    vector<Router *> routers = vector<Router *>();
    Int routers_num = 0;
    Int connect_num = 0;
    readInput(routers_num, connect_num, routers);

    return 0;
}
