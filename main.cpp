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
    vector<Router *> routers;

  public:
    Router(Int id) : _id(id) {}
};

int main()
{
    Int routers_num = 0;
    Int connect_num = 0;

    printf("Reading\n");

    scanf("%d", &routers_num);
    scanf("%d", &connect_num);
    printf("Routers: %d\nConnections: %d\n", routers_num, connect_num);

    while (connect_num-- > 0)
    {
        Int rout1 = 0, rout2 = 0;
        scanf("%d %d", rout1, rout2);
    }
    return 0;
}
