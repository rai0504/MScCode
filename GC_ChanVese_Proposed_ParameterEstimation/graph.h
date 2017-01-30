/**
 *  @file graph.h
 *  @brief Decalaration of Graph type object for network flow.
 *  @author Ryan Naidoo (rai5395@gmail.com)
 *  @bug Bugs present.
 */

#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <iostream>
using namespace std;

#include "node.h"

/** @brief The DEBUG define turns on all debug. */
#undef DEBUG

class Graph
{
/**
 *  @class Graph
 *
 *  This object holds all the nodes of a graph. It is specifically tailored for grid-type graphs.
 *  The grid dimensions are given by row and column parameters. In addition to the grid nodes, there
 *  are two additional nodes namely source, S, and sink T.
 */
public:
    long s; /**< The index of the source node on the vector of nodes. */
    long t; /**< The index of the sink node on the vector of nodes. */

    vector<Node> node; /**< The vector of nodes in the graph is held by the public access member variable node. */

    int r; /**< The number of rows on the grid. */
    int c; /**< The number of columns on the grid. */

    /**
     *  @brief <constructor> Contructs the vector of nodes, saves the number of rows and columns in the respective member variables,
     *  calculates and save the index of the source and sink nodes in the respective variables.
     *
     *  @param _r The number of rows in the grid of nodes.
     *  @param _c The number of columns in the grid of nodes.
     *
     *  @return None.
     *
     *  @bug No known bugs.
     */
    Graph(int _r, int _c);

    /**
     *  @brief Adds an edge between the node '_from' to the node '_to' with a maximum capable flow given by '_capacity'.
     *
     *  A forward edge will only be added between the nodes if the capacity is strictly non-negative (_capacity>0) and the
     *  index of the nodes do not exceed the bounds of the vector of nodes.
     *  If the forward edge is added then a corresponding back edge will be added from the node '_to' to the node '_from'
     *  with capacity equal to 0.
     *
     *  @param _from The index of the node where the edge emanates from.
     *  @param _to The index of the node where the edge ends at.
     *  @param _capacity The amount of flow the edge is able to push along the forward direction of the edge.
     *
     *  @return void
     *
     *  @bug No known bugs.
     */
    void addEdge(long _from, long _to, double _capacity);

    /**
     *  @brief Calculates the index of the node in the vector of nodes given the row and column placement of the node on the grid.
     *
     *  @param _r The row placement of the node on the grid.
     *  @param _c The column placement of the node on the grid.
     *
     *  @return Index of the node on the vector of nodes.
     *
     *  @bug Does not verify that if the row and column parameters are within the bounds of the grid of nodes.
     */
    long toIndex(int _r, int _c);

#ifdef DEBUG
    /**
     *  @brief Prints the structure of the graph to the screen.
     *
     *  @return void
     *
     *  @bug No known bugs.
     */
    void printGraph();
#endif /* DEBUG */
};

#endif // GRAPH_H
