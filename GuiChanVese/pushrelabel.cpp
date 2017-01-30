#include "pushrelabel.h"
#include <algorithm>

PushRelabel::PushRelabel(Graph *_g)
{
    g = _g;

    n = g->r*g->c+2;
    active = vector<bool>(n);

    vector<long> bucket_temp;
    bucket = vector< vector<long> >(n+1, bucket_temp);

    count = vector<long>(n+1, 0);

#ifdef MOD
    current_edge = vector<unsigned int>(n, 0);
#endif /* MOD */

    highest_level = 0;
}

void PushRelabel::initialise()
{
    g->node[g->s].distance = n;
    g->node[g->t].distance = 0;

    // saturate all edges from the source
    for(unsigned int i=0; i<g->node[g->s].edge.size(); i++)
    {
        g->node[g->s].excess    -= g->node[g->s].edge[i].capacity;
        long to = g->node[g->s].edge[i].to;
        g->node[to].excess      += g->node[g->s].edge[i].capacity;

        g->node[g->s].edge[i].flow  += g->node[g->s].edge[i].capacity;
        long be = g->node[g->s].edge[i].back_edge;
        g->node[to].edge[be].flow   -= g->node[g->s].edge[i].capacity;
    }

    for(long i=0; i<g->r*g->c; i++)
    {
        g->node[i].distance = 1;    // assign all nodes with correlation to image pixels a distance of 1
        bucket[1].push_back(i);     // place the node index in the bucket which corresponds to a distance of 1
        active[i] = true;           // mark node as active
    }
    active[g->t] = true;

    highest_level = 1;
    count[highest_level] = n-2;
}

void PushRelabel::push(Edge *e)
{
    double amount = min(g->node[e->from].excess, e->capacity-e->flow);
    if ((g->node[e->from].distance==g->node[e->to].distance+1) && amount>0)
    {
        e->flow                                 += amount;
        g->node[e->to].edge[e->back_edge].flow  -= amount;

        g->node[e->from].excess -= amount;
        g->node[e->to].excess   += amount;

        enqueue(e->to);
    }
}

void PushRelabel::enqueue(long v)
{
    if (!active[v] && (g->node[v].excess>0) && (g->node[v].distance<n))
    {
        active[v] = true;
        bucket[g->node[v].distance].push_back(v);
        highest_level = max(highest_level, g->node[v].distance);
    }
}

void PushRelabel::relabel(long v)
{
    count[g->node[v].distance]--;

    // find the relabel of v
    g->node[v].distance = n;
    for(unsigned long i=0; i<g->node[v].edge.size(); i++)
    {
        if ((g->node[v].edge[i].capacity-g->node[v].edge[i].flow)>0)
            g->node[v].distance = min(g->node[v].distance, g->node[g->node[v].edge[i].to].distance+1);
    }

    count[g->node[v].distance]++;
    enqueue(v);
}

void PushRelabel::gap(long k)
{
    for (long v=0; v<n; v++)
        if (g->node[v].distance>=k)
        {
            count[g->node[v].distance]--;
            g->node[v].distance = max(g->node[v].distance, n);
            count[g->node[v].distance]++;
            enqueue(v);
        }
}

void PushRelabel::discharge(long v)
{
#ifdef MOD
    while (current_edge[v]<g->node[v].edge.size())
    {
        if (g->node[v].excess>0)
        {
            push(&(g->node[v].edge[current_edge[v]]));
            current_edge[v]++;
        }
        else break;
    }
    current_edge[v] %= g->node[v].edge.size();
#else /* MOD */
    for (unsigned int i=0; i<g->node[v].edge.size(); i++)
    {
        if (g->node[v].excess>0) push(&(g->node[v].edge[i]));
        else break;
    }
#endif /* MOD */

    if (g->node[v].excess>0)
    {
        if (count[g->node[v].distance]==1) gap(g->node[v].distance);
        else relabel(v);
    }
}

void PushRelabel::calculateMaxFlow()
{
    initialise();

    while (highest_level>0)
    {
        if (!bucket[highest_level].empty())
        {
            long v = bucket[highest_level].back();
            bucket[highest_level].pop_back();
            active[v] = false;
            discharge(v);
        }
        else highest_level--;
    }
}
