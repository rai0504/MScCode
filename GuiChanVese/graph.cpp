#include "graph.h"

Graph::Graph(int _r, int _c)
{
    r = _r;
    c = _c;

    for (long i=0; i<r*c+2; i++) node.push_back(Node());

    s = node.size()-2; // source node index
    t = node.size()-1; // sink node index
}

void Graph::addEdge(long _from, long _to, double _capacity)
{
    // no self loops allowed
    if ((_from != _to) && (_from<node.size()) && (_to<node.size()) && _capacity>0)
    {
        node[_from].edge.push_back(Edge(_from, _to, _capacity, 0, node[_to].edge.size()));  // edge
        node[_to].edge.push_back(Edge(_to, _from, 0, 0, node[_from].edge.size()-1));        // anti-edge
    }
}

long Graph::toIndex(int _r, int _c)
{
    return _r*c + _c;
}

#ifdef DEBUG
void Graph::printGraph()
{
    cout << "S h=" << node[s].distance << ", e=" << node[s].excess << endl;
    for (unsigned int index=0; index<node[s].edge.size(); index++)
        cout << "  -- " << node[s].edge[index].flow << "/" << node[s].edge[index].capacity << " --> N[" << node[s].edge[index].to << "]" << endl;

    for (int r_=0; r_<r; r_++)
        for (int c_=0; c_<c; c_++)
        {
            cout << endl;
            cout << "N(" << r_ << ", " << c_ << ") h=" << node[r_*c + c_].distance << ", e=" << node[r_*c + c_].excess << endl;
            for (unsigned int i=0; i<node[r_*c + c_].edge.size(); i++)
            {
                cout << "  -- " << node[r_*c + c_].edge[i].flow << "/" << node[r_*c + c_].edge[i].capacity << " --> ";
                if (node[r_*c + c_].edge[i].to==t) cout << "T" << endl;
                else if (node[r_*c + c_].edge[i].to==s) cout << "S" << endl;
                else cout << "N[" << node[r_*c + c_].edge[i].to << "]" << endl;
            }
        }

    cout << endl;
    cout << "T h=" << node[t].distance << ", e=" << node[t].excess << endl;
    for (unsigned int index=0; index<node[s].edge.size(); index++)
        cout << "  -- " << node[t].edge[index].flow << "/" << node[t].edge[index].capacity << " --> N[" << node[t].edge[index].to << "]" << endl;
}
#endif /* DEBUG */
