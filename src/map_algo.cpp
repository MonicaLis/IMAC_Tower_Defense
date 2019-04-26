#include "map_algo.h"

#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <list>

using namespace std;


/**********************************STRUCTURE METHODS*********************************************/

Position::Position()
{
    p_x = 0;
    p_y = 0;
}

Position::Position(int x, int y)
{
    p_x = x;
    p_y = y;
}

Position::~Position()
{
    p_x = 0;
    p_y = 0;
}

int Position::get_p_x()
{
    return p_x;
}

int Position::get_p_y()
{
    return p_y;
}

Node::Node()
{
    index = 0;
    nature = 0;
    height = 0;
    width = 0;
    nb_successors = 0;
    linked_to = NULL;
}

Node::Node(Position N_coordinates, int N_index, int N_nature, int N_width, int N_height, int N_nb_successors, Node* N_linked_to)
{
    index = N_index;
    nature = N_nature;
    height = N_height;
    width = N_width;
    nb_successors = N_nb_successors;
    linked_to = N_linked_to;
    coordinates = N_coordinates;
}

Node::~Node()
{
    index = 0;
    nature = 0;
    height = 0;
    width = 0;
    nb_successors = 0;
    linked_to = NULL;
}

void Node::set_successors(Node* successors)
{
    linked_to = successors;
}

int Node::get_successors()
{
    return nb_successors;
}

Position Node::get_coordinates()
{
    return coordinates;
}

int Node::get_nature()
{
    return nature;
}

Graph::Graph(int G_nb_nodes)
{
    nb_nodes = G_nb_nodes;
    node_array = new Node[nb_nodes];
}

Graph::~Graph()
{
    nb_nodes = 0;
}

//careful: you can add nodes in an empty graph but not add a 6th node to a 5-node graph
void Graph::add_node(Node N, int position)
{
    node_array[position] = N;
}

Node Graph::get_node(int i)
{
    return node_array[i];
}


/**********************************LOAD MAP*********************************************/

Graph create_graph()
{
    Node* succ_N0;
    Node* succ_N1;
    Node* succ_N2;
    Node* succ_N3;
    Node* succ_N4;

    Position P0(10,20);
    Position P1(454,103);
    Position P2(300,103);
    Position P3(200,103);
    Position P4(200,206);

    //first we create the nodes independently
    Node N0(P0,0,1,10,20,3,succ_N0);
    Node N1(P1,1,2,454,103,2,succ_N1);
    Node N2(P2,2,4,300,103,3,succ_N2);
    Node N3(P3,3,3,200,103,2,succ_N3);
    Node N4(P4,4,4,200,206,3,succ_N4);

    //then we link them together
    //N0 has 3 successors:
    succ_N0 = new Node[3]; 
    succ_N0[0] = N3;
    succ_N0[1] = N2;
    succ_N0[2] = N4;
    //N1 has 2 successors:
    succ_N1 = new Node[2]; 
    succ_N1[0] = N2;
    succ_N1[1] = N4;
    //N2 has 3 successors:
    succ_N2 = new Node[3]; 
    succ_N2[0] = N3;
    succ_N2[1] = N1;
    succ_N2[2] = N0;
    //N3 has 2 successors:
    succ_N3 = new Node[2]; 
    succ_N3[0] = N0;
    succ_N3[1] = N2;
    //N4 has 3 successors:
    succ_N4 = new Node[3]; 
    succ_N4[0] = N0;
    succ_N4[1] = N2;
    succ_N4[2] = N1;

    //reupdate successors
    N0.set_successors(succ_N0);
    N1.set_successors(succ_N1);
    N2.set_successors(succ_N2);
    N3.set_successors(succ_N3);
    N4.set_successors(succ_N4);

    //create main graph
    Graph graph_game(5);
    graph_game.add_node(N0,0);
    graph_game.add_node(N1,1);
    graph_game.add_node(N2,2);
    graph_game.add_node(N3,3);
    graph_game.add_node(N4,4);

    return graph_game;
}


bool load_map(const char* filename)
{
    ifstream myfile;
    myfile.open (filename);

    //check if we can open the file correctly
    if (!myfile) 
    {
        cerr << "Unable to open file"<<endl;
    }
    else cout<<"File opened"<<endl;

    int i;
    string line;
    bool valid_itd = false;
    bool valid_comm = false;
    bool valid_carte = false;
    bool valid_chemin = false;
    bool valid_noeud = false;
    bool valid_construct = false;
    bool valid_in = false;
    bool valid_out = false;

    bool one_char_line = false; //when this is true we've reached the nodes part and we can get out the loop

    //loop that lasts as long as there's a line to read and that the boolean is set to true
    while (getline(myfile, line) && !one_char_line)
    {
        cout<<"line : "<<line<<endl;
        //check the first line is @ITD (technically this could be any line...)
        if((line.find("@ITD", 0) == 0))  //if found at position 0
        {
            valid_itd = true;
            cout<<"ITD present"<<endl;
        }

        //check that the second line starts with #
        if((line.find("#", 0) == 0)) 
        {
            valid_comm = true;
            cout<<"Comment present"<<endl;
        }

        if((line.find("carte", 0) == 0)) 
        {
            //substr(a, b) function returns a substring of b length starting from index a 
            //this will therefore return ".ppm" (hopefully)
            int size = line.size();
            string suffixe = line.substr(size-4, 4);
            if(suffixe.compare(".ppm") == 0)
            {
                valid_carte = true;
                cout<<"Correct image format"<<endl;
            } 
        }
        if((line.find("chemin", 0) == 0))
        {
            string para, para1, para2, para3;
            myfile >> para >> para1 >> para2 >> para3; //"para" is the word
            //if values are between 0 and 255
            if ( (is_parameter_valid(para1))
                && (is_parameter_valid(para2))
                && (is_parameter_valid(para3)) ) 
                {
                    valid_chemin = true;
                    cout<<"Correct path format"<<endl;
                }
        }
        //this is never read, the line is empty ----------------------------------> WEIRD
        if((line.find("noeud", 0) == 0))
        {
            string para, para1, para2, para3;
            //if values are all 0 (because the color is 0)
            if ( (para1 == "0")
                && (para2 == "0")
                && (para3 == "0") ) 
                {
                    valid_noeud = true;
                    cout<<"Correct noeud format"<<endl;
                }
        }
        if((line.find("construct", 0) == 0))
        {
            string para, para1, para2, para3;
            myfile >> para >> para1 >> para2 >> para3;
            //if values aren't between 0 and 255
            if ( (is_parameter_valid(para1))
                && (is_parameter_valid(para2))
                && (is_parameter_valid(para3)) ) 
                {
                    valid_construct = true;
                    cout<<"Correct constructible area format"<<endl;  
                }         
        }
        //this is never read, the line is empty ----------------------------------> WEIRD
        if((line.find("in", 0) == 0))
        {
            string para, para1, para2, para3;
            myfile >> para >> para1 >> para2 >> para3;
            //if values aren't between 0 and 255
            if ( (is_parameter_valid(para1))
                && (is_parameter_valid(para2))
                && (is_parameter_valid(para3)) ) 
                {
                    valid_in = true; 
                    cout<<"Correct entry format"<<endl;
                }         
        }
        if((line.find("out", 0) == 0))
        {
            string para, para1, para2, para3;
            myfile >> para >> para1 >> para2 >> para3;
            //if values aren't between 0 and 255
            if ( (is_parameter_valid(para1))
                && (is_parameter_valid(para2))
                && (is_parameter_valid(para3)) ) 
                {
                    valid_out = true;   
                    cout<<"Correct exit format"<<endl;     
                }   
        }

        if (line.size() == 1) one_char_line == true;
    }

    myfile.close();
    return  (valid_itd &&valid_comm && valid_carte && valid_chemin 
            && valid_noeud && valid_construct && valid_in && valid_out);
}

bool verify_path (Graph graph)
{
    /*
    //check that there's at least one path between an IN and an OUT zone
    //i.e. check that there are nodes of natures 3 and 4 between nodes 1 and 2
    bool is_there_a_path;

    //check that the path isn't crossing anything else: i.e. Djikstra code
    list<Node> visited_nodes;
    list<Node> list_nodes;
    list<Node> predecessors;
    list<int> distances; //minimum distances starting from first_node
    int i;
    for (i=0; i<5; i++) 
    {
        Node N = graph.get_node(i);
        list_nodes.push_back(N);
    }
    Node first_node = list_nodes.front();

    while (S != NULL)
    {
        visited_nodes.push_back(S);
        for (i=0; i<S.get_successors(); i++) //for each segment/edge of the node
        {

        }
    }
*/

    //delete all tabs
    return true;
}

/* ALGO BRESSENHAM EN PSEUDO CODE

*/

bool is_parameter_valid(string parameter)
{
    char first_char = parameter.at(0);
    //if the first character is a number then continue
    if ( (first_char >= '0') && (first_char <= '9'))
    {
        //convert the string to an int
        int parameter_int = stoi (parameter);
        if ( (parameter_int >= 0) && (parameter_int <= 255)) return true; 
    }
    //if it's not a number or it's not between 0 and 255
    return false;
}
