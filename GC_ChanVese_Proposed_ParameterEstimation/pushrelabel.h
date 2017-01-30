/** @file pushrelabel.h
 *  @brief Decalaration of PushRelabel type object that performs the Push-Relabel variant of the Max-flow Algorithm on a Graph object.
 *  @author Ryan Naidoo (rai5395@gmail.com)
 *  @bug Bugs present.
 */

#ifndef PUSHRELABEL_H
#define PUSHRELABEL_H

#include <vector>
#include <cmath>
#include <iostream>
using namespace std;

#include "graph.h"
#include "edge.h"

#define MOD

class PushRelabel
{
    /**
     *  @class PushRelabel
     *
     *  Implements the Push-Relabel variant of the Max-flow algorithm with the Gap Relabel optmisation.
     */
public:
    /**
     *  @brief <constructor> Assigns a local variable the address of the Graph object on which the Push-Relabel algorithm
     *         will be performed.
     *
     *  @param _g Pointer to a Graph object.
     *
     *  @return None.
     *
     *  @bug No known bugs.
     */
    PushRelabel(Graph *_g);

    /**
     *  @brief Performs the Push-Relabel algorithm.
     *
     *  @return void
     *
     *  @bug No known bugs.
     */
    void calculateMaxFlow();

private:
    Graph *g; /**< Pointer to the Graph object */
    vector<bool> active; /**< The vector which holds all whether a node is known to be active or not. */
    vector< vector<long> > bucket; /**< bucket[i] holds a vector of all nodes with distance=i */
    vector<long> count; /**< count[i] holds the number of nodes whose distance is i */
    long highest_level; /**< A pointer to the highest non-empty bucket. */
    long n; /**< The number of nodes in the graph. */
#ifdef MOD
    vector<unsigned int> current_edge; /**< For each node hold the index of the edge which is to be processed next. */
#endif /* MOD */

    /**
     *  @brief The first step in the Push-Relabel algorithm.
     *
     *  Saturate all edges emanating from the source and update the excess of the relative end nodes. For all nodes in the
     *  grid assign a distance of 1. Mark the sink node as active. The bucket with the heighest level is bucket[1] with
     *  n-2 nodes in it.
     *
     *  @return void
     *
     *  @bug No known bugs.
     */
    void initialise();

    /**
     *  @brief Pushes the maximum amount of flow through the edge from the start node to the end node. Queues the end node
     *  if it is not already queued.
     *
     *  @param e The pointer of the edge on which the flow must be pushed.
     *
     *  @return void
     *
     *  @bug No known bugs.
     */
    void push(Edge *e);

    /**
     *  @brief If a node is not marked as active, if it has positive excess flow and the distance is less than n (the
     *  number of nodes in the graph) then queue it.
     *
     *  @param v The index of the node in the graph.
     *
     *  @return void
     *
     *  @bug No known bugs.
     */
    void enqueue(long v);

    /**
     *  @brief If a node is overflowing and it is less than a distance of n (number of nodes in the graph) from the sink, then
     *  the node's distance needs to be increased to allow the overflow to be pushed through to the connected nodes.
     *
     *  @param v The index of the node in the graph.
     *
     *  @return void
     *
     *  @bug No known bugs.
     */
    void relabel(long v);

    /**
     *  @brief If a bucket which holds nodes of distance k becomes empty then it it is not possible for nodes with a distance
     *  greater than or equal to k to reach the sink. This gap at height k seperates nodes which are capable of reaching the
     *  sink with those that definitely cannot reach the sink. This method assigns a minimum distance of n to all nodes whose
     *  distance is at least k so that it will be ignored from now on by the algorithm.
     *
     *  @param k The index of the bucket that just became empty.
     *
     *  @return void
     *
     *  @bug No known bugs.
     */
    void gap(long k);

    /**
     *  @brief Relieves an active node of the excess flow until it is no more active or it is ignored by the algorithm.
     *
     *  @param v The index of the node in the graph.
     *
     *  @return void
     *
     *  @bug No known bugs.
     */
    void discharge(long v);
};

#endif // PUSHRELABEL_H
