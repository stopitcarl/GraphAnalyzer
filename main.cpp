#include <iostream>
#include <vector>
#include <algorithm>

using namespace std; // Bora tipo nao escrever 'std::' em tudo ya?

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

int main()
{
    Int routers_num = 0;
    Int connect_num = 0;

    printf("Reading\n");

    scanf("%ud", &routers_num);
    scanf("%ud", &connect_num);
    printf("Routers: %d\nConnections: %d\n", routers_num, connect_num);

    // Create array of router pointers
    vector<Router *> routers = vector<Router *>(routers_num);
    // Initialize the fuckers
    for (Int i = 0; i < routers_num; i++)
        routers[i] = new Router(i + 1);

    while (connect_num-- > 0)
    {
        Int routes[2] = {0};
        scanf("%d %d", &routes[0], &routes[1]);

        // Update routes
        (*routers[routes[0] - 1]).addConnection(routes[1]);
        (*routers[routes[1] - 1]).addConnection(routes[0]);
    }

    return 0;
}
