#ifndef MAP_ALGO_H
#define MAP_ALGO_H


#include <stdlib.h>
#include <iostream>

using namespace std;

/**********************************STRUCTURES*********************************************/

typedef struct Position{

private:
    int p_x, p_y;

public:
    Position();
    Position(int x, int y);
    ~Position();
    int get_p_x();
    int get_p_y();
}Position;

//we are using graphs to represent the paths and their nodes (i.e. adjacency list)
typedef struct Node{

private: 
    int index, nature, width, height, nb_successors;
    Node* linked_to;
    Position coordinates;

public:
    Node();
    Node(Position coordinates, int N_index, int N_nature, int N_nb_successors, Node* N_linked_to); //constructor
    ~Node(); //destructor
    void set_successors(Node* successors);
    int get_successors();
    Position get_coordinates();
    int get_nature();

}Node;

typedef struct Graph{

private:
    Node* node_array;
    int nb_nodes;

public:
    Graph(int G_nb_nodes);
    ~Graph();
    void add_node(Node N, int position);
    Node get_node(int i);
    int get_nb_nodes();

}Graph;

/**********************************FUNCTIONS********************************************/

bool load_map(const char* filename); //returns true if the map is valid
void verify_path (Graph graph); //tells us if the path is valid
bool is_parameter_valid(string parameter);
Graph create_graph();

#endif 