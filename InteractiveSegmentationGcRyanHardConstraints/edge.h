/**
 *  @file edge.h
 *  @brief Decalaration of an Edge type structure to connect nodes in a network.
 *  @author Ryan Naidoo (rai5395@gmail.com)
 *  @bug Bugs present.
 */

#ifndef EDGE_H
#define EDGE_H

struct Edge
{
    /**
     *  @struct Node
     *  @brief Holds all the basic information necessary for a network node for the Push-Relabel Max-flow/Min-cut algorithm.
     *
     *  Each node hold it's own excess, height from the sink (distance) and set of edges emanating from itself.
     */

    long from; /**< The index of the node where the edge is emanating from. */
    long to; /**< The index of the node where the edge ends at. */
    double capacity; /**< The maximum amount of flow the edge is able to push. */
    double flow; /**< The current amount of flow the edge is pushing. */
    long back_edge; /**< The index of the corresponding back edge at the end node. */

    /**
     *  @brief Initialiser for an instance of the Edge structure.
     *
     *  The edge goes from Node[0] to Node[0], self-loop at Node[0]. The edge cannot accomodate any flow, capacity is 0.
     *  There is no flow currently going through the edge, flow is 0. The back edge index is 0.
     *
     *  @return None.
     *
     *  @bug No know bugs.
     */
    Edge() : from(), to(), capacity(), flow(), back_edge() {}

    /**
     *  @brief Initialiser for an instance of the Edge structure.
     *
     *  The edge goes from Node[_from] to Node[_to]. The edge can accomodate a maximum flow of _capacity.
     *  The current flow through the edge is _flow. The back edge index is _back_edge.
     *
     *  @param _from The index of the node where the edge emanates from.
     *  @param _to The index of the node where the edge ends at.
     *  @param _capacity The maximum amount of flow the edge can push.
     *  @param _flow The current amount of flow being pushed through the edge.
     *  @param _back_edge The index of the corresponding back_edge at the end node's edge list.
     *
     *  @return None.
     *
     *  @bug Does not check it the _from and _to parameters are non-negative.
     *  @bug Does not check if the capacity is strictly non-negative, i.e. capacity>0
     *  @bug Does not check if the flow is less than or equal to the capacity
     */
    Edge(long _from, long _to, double _capacity, double _flow, long _back_edge) :
        from(_from), to(_to), capacity(_capacity), flow(_flow), back_edge(_back_edge) {}
};

#endif // EDGE_H
