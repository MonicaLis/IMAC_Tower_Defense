#ifndef MAP_GRAPHIC_H
#define MAP_GRAPHIC_H


#include <stdlib.h>
#include <iostream>

using namespace std;

/**********************************STRUCTURES*********************************************/


//we are using graphs to represent the paths and their nodes (i.e. adjacency list)
typedef struct Node{

private:
    int index, nature, width, height, nb_successors;
    Node* linked_to;

public:
    Node();
    Node(int N_index, int N_nature, int N_width, int N_height, int N_nb_successors, Node* N_linked_to); //constructor
    ~Node(); //destructor
    void set_successors(Node* successors);

}Node;

typedef struct Graph{

private:
    Node* node_array;
    int nb_nodes;

public:
    Graph(int G_nb_nodes);
    ~Graph();
    void add_node(Node N);

}Graph;

/**********************************FUNCTIONS********************************************/

bool load_map(const char* filename); //returns true if the map is valid
bool is_parameter_valid(string parameter);
Graph create_graph();


/*******************************FROM C PROJECT**********************************/


typedef struct Pixel{
    unsigned char red, green, blue;
}Pixel;

typedef struct Image{
    Pixel* data;
    int height;
    int width;
}Image;



//functions from our Minigimp project
Image* load(const char* filename);
int save(Image* I,const char* filename);



/* _____________________________________________WE PROBABLY DON'T NEED THIS____
Image* create_image(int w, int h);
void delete_image(Image* I);
Pixel get_pixel(int x, int y, Image* I);
void set_pixel(Image* I, Pixel p, int i, int j);
void fill_image(Image* I, unsigned char r, unsigned char g, unsigned char b);

void display_pixel(Pixel p);
Pixel create_pixel(unsigned char r, unsigned char g, unsigned char b); */

#endif 