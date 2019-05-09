#ifdef _WIN32
    #include <windows.h>
#endif
#define _USE_MATH_DEFINES
#include <cmath>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
using namespace std;
#include <GL/gl.h>
#include <GL/glu.h>
#include <stb_image/stb_image.h>
#include <vector>

#include "init.h"
#include "map_algo.h"
#include "map_graphic.h"
#include "towers_algo.h"
#include "towers_graphic.h"
#include "monsters_algo.h"
#include "monsters_graphic.h"
#include "player.h"

/* Nombre minimal de millisecondes separant le rendu de deux images */
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

int main(int argc, char **argv) {
    /* Init SDL, OpenGL, Debug and Glad */
    SDL_Window* window = init();
    if (window == nullptr) {
        cout << "Error window init" << endl;
    }

    /*loading and verifying the map*/
    bool verify_map = load_map("data/carte.itd");
    cout << verify_map << endl;

     /* Chargement de l'image */
    const char image_path[] = "images/space.ppm";
    int imgWidth, imgHeight, imgChannels;
    unsigned char *image = stbi_load(image_path, &imgWidth, &imgHeight, &imgChannels, STBI_rgb_alpha);
    if (nullptr == image)
    {
        return (EXIT_FAILURE);
    }
    // Autorisation de l'affichage des textures
    glEnable(GL_TEXTURE_2D);

    /* Initialisation de la texture */
    GLuint texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //display path
    GLuint texturePath=initTexturePath();

    /*==============PLAYER INIT==========================================*/
    Player player;
    int money = player.get_money();
    cout << "Argent disponbile : "<<money<<endl;

    GLuint textureTower=initTextureTower();
    GLuint textureMonster=initTextureMonster();

    std::vector<Tower*> towers;
    std::vector<Monster*> monsters;
    std::vector<Monster*> supr ;

    /* Boucle d'affichage */
    bool loop = true;
    bool wave=false;
    int numberWave=0;

    /*create the graph out of the given nodes and therefore the map*/
    Graph map = create_graph();
    Image* img_map = create_map_ppm(map); //will be used by drawPath(...)
    
    while (loop) {
        /* Recuperation du temps au debut de la boucle */
        Uint32 startTime = SDL_GetTicks();

        /* Placer ici le code de dessin */
        glClear(GL_COLOR_BUFFER_BIT);

        glBindTexture(GL_TEXTURE_2D, texture_id);
        glPushMatrix();
            glTranslated(350,350,0);
            glScalef(600,600,0);
            glBegin(GL_QUADS);
                glTexCoord2f(0, 1); glVertex2f(-0.5f, -0.5f);   // bas gauche
                glTexCoord2f(1, 1); glVertex2f(0.5f, -0.5f);    // bas droite
                glTexCoord2f(1, 0); glVertex2f(0.5f, 0.5f);     // haut droite
                glTexCoord2f(0, 0); glVertex2f(-0.5f, 0.5f);    // haut gauche
            glEnd();
        glPopMatrix();
        // Unbind texture
        glBindTexture(GL_TEXTURE_2D, 0); 


         /* Update des entités */
        for (Tower* tower : towers) {
            tower->drawTower();
        }
        for (Monster* monster : monsters) {
            monster->drawMonster();
            if (monsters.size() > 0){
                int numberX = rand() % 5;   
                int numberY = rand() % 5;      // v1 in the range 0 to 99
                int Posx=monster->get_x()+numberX;
                int Posy=monster->get_y()+numberY;
                monster->set_x(Posx);
                monster->set_y(Posy);
            }
            
        }
        
        
        int time=0;
       
        

       if(wave && monsters.size()>0){
            
            for (Tower* tower : towers) {
                int loopMonster=0; 
                for (Monster* monster : monsters) {
                    time+=1;
                    int compareX= tower->get_x()-monster->get_x();
                    int compareY= tower->get_y()-monster->get_y();
                    if(monsters.size()>0 && compareX<100 && compareY<100 && time>=3){
                        if(monster->get_life_points()<=0){
                            supr.push_back(monster);
                            monsters.erase(monsters.begin()+loopMonster);
                        }
                        if(monster->get_life_points()>0){
                        int life=monster->get_life_points()-2;
                        monster->set_life_points(life);
                        cout << "Monster life : "<<life<<endl;
                        time=0;
                        }
                    }
                    
                    loopMonster+=1;
                }
            
            }
        }
        if(monsters.size()<=0){
            wave=false;
        }
       
        if(!supr.empty()){
            for (Monster* toSupr : supr) {
            supr.erase(supr.begin(), supr.end());
            delete toSupr;
            cout << "Monstre tué" <<endl;
            }
        }

        drawPath(img_map);
   
        /* Echange du front et du back buffer : mise a jour de la fenetre */
        SDL_GL_SwapWindow(window);

        /* Boucle traitant les evenements */
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            /* L'utilisateur ferme la fenetre : */
            if (e.type == SDL_QUIT)
            {
                loop = false;
                break;
            }

            /* Quelques exemples de traitement d'evenements : */
            switch (e.type)
            {
                /* Clic souris */
                case SDL_MOUSEBUTTONUP:
                {
                    if(money>0 && !wave){
                        //CONTRUCT TOWER
                        Tower* newTower= new Tower(e.button.x,e.button.y, textureTower);
                        towers.push_back(newTower);

                        money=player.get_money()-newTower->get_cost();
                        player.set_money(money);
                        cout << "Argent disponbile : "<<money<<endl;
                    }
                    if(wave==true){
                        cout << "Pas de construction pendant une vague"<<endl;
                    }
                    if(money<=0){
                        cout << "Pas d'argent disponible"<<endl;
                    }
                
                    cout << "clic en "<< e.button.x<< " " << e.button.y<< endl;
                }
                    break;

                /* Touche clavier */
                case SDL_KEYDOWN:
                    if (e.key.keysym.sym == 'w') {
                        printf("New wave\n");
                        wave = true;
                        
                        numberWave+=1;
                        
                        for(int i=1; i<=numberWave; i++){
                            Monster* newMonster= new Monster(0,0, textureMonster);
                            monsters.push_back(newMonster);
                        }
                        

                    }
                    printf("touche pressee (code = %d)\n", e.key.keysym.sym);
                    break;

                default:
                    break;
            }
        }


        /* Calcul du temps ecoule */
        Uint32 elapsedTime = SDL_GetTicks() - startTime;
        /* Si trop peu de temps s'est ecoule, on met en pause le programme */
        if (elapsedTime < FRAMERATE_MILLISECONDS) {
            SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
        }
    }

    /* Liberation des ressources associees a la SDL */
    SDL_DestroyWindow(window);
    SDL_Quit();
    for (Tower* tower : towers) {
        delete tower;
    }
      for (Monster* monster : monsters) {
        delete monster;
    }

    return EXIT_SUCCESS;
}