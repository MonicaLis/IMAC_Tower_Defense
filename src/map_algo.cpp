#include "map_algo.h"

#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>

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
    ~coordinates;
}

void Node::set_successors(Node* successors)
{
    linked_to = successors;
}

Graph::Graph(int G_nb_nodes)
{
    nb_nodes = G_nb_nodes;
    node_array = new Node[nb_nodes];
}

Graph::~Graph()
{
    nb_nodes = 0;
    delete node_array;
}

void Graph::add_node(Node N)
{
    node_array[nb_nodes] = N;
    nb_nodes++;
}


/**********************************LOAD MAP*********************************************/

Graph create_graph()
{
    Node* succ_N0;
    Node* succ_N1;
    Node* succ_N2;
    Node* succ_N3;
    Node* succ_N4;
    //created the positions quite randomly, making sure they respect the nodes' natures
    Position P0(0,100);
    Position P1(200,190);
    Position P2(70,50);
    Position P3(150,15);
    Position P4(175,174);
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
    graph_game.add_node(N0);
    graph_game.add_node(N1);
    graph_game.add_node(N2);
    graph_game.add_node(N3);
    graph_game.add_node(N4);

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
    //check that there's a path between an IN and an OUT zone: i.e. check that there are nodes of natures 3 and 4
    bool is_there_a_path;
    //check that the path isn't crossing anything else: i.e. Bressenham code
}
/*

• Bonne validité des chemins : Le long d’un chemin entre 2 noeuds on doit rester sur le chemin (ne
pas croiser d’autre pixel que des pixels de type chemin). Pour ce faire, utilisez l’algorithme
de parcours de segment de Bressenham.
• Existence d’au moins un chemin entre la zone d’entrée et de sortie : c’est un parcours en
profondeur simple du graphe des chemin*/

/* ALGO BRESSENHAM EN PSEUDO CODE
procédure tracerSegment(entier x1, entier y1, entier x2, entier y2) est
  déclarer entier x, y, dx, dy ;
  déclarer rationnel e, e(1,0), e(0,1) ;  // valeur d’erreur et incréments
  dy ← y2 - y1 ;
  dx ← x2 - x1 ;
  y ← y1 ;  // rangée initiale
  e ← 0,0 ;  // valeur d’erreur initiale
  e(1,0) ← dy / dx ;
  e(0,1) ← -1.0 ;
  pour x variant de x1 jusqu’à x2 par incrément de 1 faire
    tracerPixel(x, y) ;
    si (e ←  e + e(1,0)) ≥ 0,5 alors  // erreur pour le pixel suivant de même rangée
      y ←  y + 1 ;  // choisir plutôt le pixel suivant dans la rangée supérieure
      e ←  e + e(0,1) ;  // ajuste l’erreur commise dans cette nouvelle rangée
    fin si ;
  fin pour ;
fin procédure ;
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


/**********************************FUNCTIONS FROM C PROJECT*********************************************/

Image* load(const char* filename)
{
    char buff[16];
    Image *img;
    FILE *fp;
    int c, rgb_comp_color;
    //open PPM file for reading
    fp = fopen(filename, "rb");
    if (!fp) {
        fprintf(stderr, "Unable to open file '%s'\n", filename);
        exit(1);
    }
    
    //read image format
    if (!fgets(buff, sizeof(buff), fp)) {
        perror(filename);
        exit(1);
    }
    
    //check the image format
    if (buff[0] != 'P' || buff[1] != '6') {
        fprintf(stderr, "Invalid image format (must be 'P6')\n");
        exit(1);
    }
    
    //alloc memory form image
    img = (Image *)malloc(sizeof(Image));
    if (!img) {
        fprintf(stderr, "Unable to allocate memory\n");
        exit(1);
    }
    
    //check for comments
    c = getc(fp);
    while (c == '#') {
        while (getc(fp) != '\n') ;
        c = getc(fp);
    }
    
    ungetc(c, fp);
    //read image size information
    if (fscanf(fp, "%d %d", &img->width, &img->height) != 2) {
        fprintf(stderr, "Invalid image size (error loading '%s')\n", filename);
        exit(1);
    }
    
    //read rgb component
    if (fscanf(fp, "%d", &rgb_comp_color) != 1) {
        fprintf(stderr, "Invalid rgb component (error loading '%s')\n", filename);
        exit(1);
    }
    
    //check rgb component depth
    if (rgb_comp_color!= 255) {
        fprintf(stderr, "'%s' does not have 8-bits components\n", filename);
        exit(1);
    }
    
    while (fgetc(fp) != '\n') ;
    //memory allocation for pixel data
    img->data = (Pixel*)malloc(img->width * img->height * 3 * sizeof(unsigned char));
    
    if (!img) {
        fprintf(stderr, "Unable to allocate memory\n");
        exit(1);
    }
    
    //read pixel data from file
    if (fread(img->data, 3 * img->width, img->height, fp) != img->height) {
        fprintf(stderr, "Error loading image '%s'\n", filename);
        exit(1);
    }
    
    fclose(fp);
    return img;
}
    





/*this function opens a file, prints:
    P6
    x y
    max
 and then all the other pixel information from image I into the file.
 */
int save(Image* I, const char* filename)
{
    FILE* fp = fopen(filename, "wb"); //write, binary
    if (!fp)
    {
        printf("failed to save image\n");
        return -1;
    }
    
    //write the header file
    //image format
    fprintf(fp, "P6\n");
    
    //comments
    //fprintf(fp, "# Created by an IMAC1 student\n");
    
    //image size
    fprintf(fp, "%d %d\n",I->width,I->height);
    
    // rgb component depth
    fprintf(fp, "255\n");
    
    //pixel data
    fwrite(I->data, 3 * I->width, I->height, fp);
    
    fclose(fp);
    printf("image saved\n");
    return 0;
}



/* _____________________________________________WE PROBABLY DON'T NEED THIS____
Image* create_image(int w, int h)
{
    Image* I = new Image;
    if (!I)
    {
        printf("allocation of memory for the image failed\n");
    }
    I->width = w;
    I->height = h;
    //3 times unsigned char because 3 RGB components per pixel
    I->data = (Pixel*)malloc(I->width * I->height * 3 * sizeof(unsigned char));
    if (!I->data)
    {
        printf("allocation of memory for image->data failed\n");
    }
    return I;
}


void delete_image(Image* I)
{
    if (I)
    {
        free(I->data);
        free(I);
        printf("image deleted\n");
    }
}

Pixel get_pixel(int x, int y, Image* I)
{
    return I->data[I->width*y+x]; //because we count pixels from the left to the right, and from the top to bottom
}


void set_pixel(Image* I, Pixel p, int i, int j)
{
    I->data[I->width*j+i].red= p.red;
    I->data[I->width*j+i].green= p.green;
    I->data[I->width*j+i].blue= p.blue;
}


void fill_image(Image* I, unsigned char r, unsigned char g, unsigned char b)
{
    int i,j;
    Pixel p = create_pixel(r,g,b);
    for (j=0; j<I->height; j++)
    {
        for (i=0; i<I->width; i++)
        {
            set_pixel(I,p,i,j);
        }
    }
}

Pixel create_pixel(unsigned char r, unsigned char g, unsigned char b)
{
    Pixel p;
    p.red = r;
    p.green = g;
    p.blue = b;
    return p;
}


void display_pixel(Pixel p)
{
    printf("red: %u green: %u blue: %u\n", p.red, p.green, p.blue);
}
*/