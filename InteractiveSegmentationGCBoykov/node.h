/**
 *  @file node.h
 *  @brief Decalaration of a Node type structure for a node in a network.
 *  @author Ryan Naidoo (rai5395@gmail.com)
 *  @bug No known bugs.
 */

#ifndef NODE_H
#define NODE_H

#include <vector>
#include "edge.h"

struct Node
{
    /**
     *  @struct Node
     *  @brief Holds all the basic information necessary for a network node for the Push-Relabel Max-flow/Min-cut algorithm.
     *
     *  Each node hold it's own excess, height from the sink (distance) and set of edges emanating from itself.
     */

    double excess; /**< Holds the net amount of flow going into the node. */
    long distance; /**< Holds the distance of how far away the node is from the sink node. Consistent with the graphical
                        representation of only being able to push flow to nodes lower that itself. */
    std::vector<Edge> edge; /**< The set of edges emanating from this node. */

    /**
     *  @brief Initialiser for an instance of the Node structure.
     *
     *  No excess, same level as the sink node, i.e. distance of 0 from the sink node, and an empty edge set.
     *
     *  @return None.
     *
     *  @bug No known bugs.
     */
    Node() : excess(), distance(){}

    /**
     *  @brief Initialiser for an instance of the Node structure.
     *
     *  Sets the initial excess flow into the node and distance from the sink node. Edge set is empty.
     *
     *  @param _excess The initial amount of net flow into the node.
     *  @param _distance The initial distance from the sink node.
     *
     *  @return None.
     *
     *  @bug No know bugs.
     */
    Node(double _excess, long _distance) : excess(_excess), distance(_distance){}
};

#endif // NODE_H
