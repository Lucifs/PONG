#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#define WIDTH 1280
#define HIGH 720


void mouvement ( int y1 , int y2, int *v, int x);
int isCollide(SDL_Rect obj1, SDL_Rect obj2);

int main(int argc, char *argv[])
{
    int jeux = 1;
    int vx = 2; // obtenir une valeur alléatoire a chaque défaite + début
    int vy = 3;
    int vm1 = 0;
    int vm2 = 0;
    int pj1 = 0;
    int pj2 = 0;
    char score1_1 [1], score2_2 [1];


    SDL_Surface *ecran = NULL;
    //SDL_Surface *background = NULL;
    SDL_Surface *ball = NULL;
    SDL_Surface *mo1 = NULL;
    SDL_Surface *mo2 = NULL;
    SDL_Surface *score1 = NULL;
    SDL_Surface *score2 = NULL;

    TTF_Font *police = NULL;

    SDL_Color blanc = {255,255,255};



    SDL_Event event;

    SDL_Rect position;

    SDL_Rect b_position;

    b_position.x = (WIDTH + 10) / 2;
    b_position.y = (HIGH + 10 ) / 2;

    SDL_Rect ba_position;

    ba_position.x = 0;
    ba_position.y = 0;


    SDL_Rect mo_1_pos;

    mo_1_pos.x = WIDTH / 20;
    mo_1_pos.y = HIGH / 2;

    SDL_Rect mo_2_pos;

    mo_2_pos.x = (WIDTH / 20) * 19;
    mo_2_pos.y = HIGH / 2;


    if (SDL_Init(SDL_INIT_VIDEO) == -1)
    {
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE); // On quitte le programme
    }



    ecran = SDL_SetVideoMode(WIDTH, HIGH, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);

    if (ecran == NULL)
    {
        fprintf(stderr, "Impossible de charger le mode vidéo : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    if (TTF_Init() == -1)
    {
        fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }



    ball = SDL_CreateRGBSurface(SDL_HWSURFACE, 10, 10, 32, 0, 0, 0, 0);
    //background = SDL_CreateRGBSurface(SDL_HWSURFACE,WIDTH,HIGH,32,0,0,0,0);
    mo1 = SDL_CreateRGBSurface(SDL_HWSURFACE, 10, 80, 32, 0, 0, 0, 0);
    mo2 = SDL_CreateRGBSurface(SDL_HWSURFACE, 10, 80, 32, 0, 0, 0, 0);

    SDL_WM_SetCaption("Pong", NULL);
    SDL_FillRect(ball, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
    //SDL_FillRect(background, NULL, SDL_MapRGB(ecran->format, 0,0,0));
    SDL_FillRect(mo1,NULL, SDL_MapRGB(ecran->format, 255,255,255));
    SDL_FillRect(mo2,NULL, SDL_MapRGB(ecran->format, 255,255,255));

    police = TTF_OpenFont("police.ttf", 50);


    while(jeux){

    SDL_PollEvent(&event);
    switch(event.type)
    {
        case SDL_QUIT:
            jeux = 0;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
                case SDLK_ESCAPE: /* Appui sur la touche Echap, on arrête le programme */
                    jeux = 0;
                    break;
                case SDLK_UP:
                    vm1 = -3;
                    break;
                case SDLK_DOWN:
                    vm1 = 3;
                    break;
            }
            break;
        case SDL_KEYUP:
            switch (event.key.keysym.sym)
            {
                case SDLK_UP:
                    vm1 = 0;
                    break;
                case SDLK_DOWN:
                    vm1 = 0;
                    break;
            }
    }

    sprintf(score1_1,"%d", pj1);
    score1 = TTF_RenderText_Solid(police, score1_1, blanc);
    sprintf(score2_2,"%d", pj2);
    score2 = TTF_RenderText_Solid(police, score2_2, blanc);








    b_position.x+=vx;
    b_position.y+=vy;

    if (isCollide(b_position,mo_1_pos) || isCollide(b_position,mo_2_pos)){
        vx*=-1;
    }
    if (b_position.x>WIDTH){
        b_position.x = (WIDTH + 10) / 2;
        b_position.y = (HIGH + 10) / 2;
        pj1 += 1;
    }
    if (b_position.x<0){
        b_position.x = (WIDTH + 10) / 2;
        b_position.y = (HIGH + 10) / 2;
        pj2 += 1;
    }
    if ( b_position.y>HIGH || b_position.y<0){
        vy*=-1;
    }


    //mouvement(mo_1_pos.y, b_position.y, &vm1,b_position.x);

    mouvement(mo_2_pos.y, b_position.y, &vm2, b_position.x);


    mo_1_pos.y += vm1;
    mo_2_pos.y += vm2;
    if (mo_1_pos.y <0){
        mo_1_pos.y=0;
    }
    if (mo_1_pos.y>HIGH){
        mo_1_pos.y=HIGH-80;
    }
    if (mo_2_pos.y <0){
        mo_2_pos.y=0;
    }
    if (mo_2_pos.y>HIGH){
        mo_2_pos.y=HIGH-80;
    }






    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
   // SDL_BlitSurface(background,NULL, ecran, &ba_position);
    SDL_BlitSurface(mo1, NULL, ecran, &mo_1_pos);
    SDL_BlitSurface(mo2, NULL, ecran, &mo_2_pos);
    SDL_BlitSurface(ball, NULL, ecran, &b_position);

    SDL_Flip(ecran);



    SDL_Delay(5);

    }








    SDL_FreeSurface(ball);
    SDL_Quit();
    TTF_CloseFont(police);
    TTF_Quit();


    return EXIT_SUCCESS;
}






void mouvement ( int y1 , int y2, int *v, int x){

    int facile = 1000;
    int moyen = 950;
    int difficile = 700;
    int impossible = 0;



    if (x>(WIDTH + facile)/2){
    if (y1+40<y2) {
        *v = 3;
    }else{
        *v = -3;
    }
    }else{
        if( x > (WIDTH) / 2){
            if (y1+40<y2) {
                *v = 1;
            }else{
                *v = -1;
            }
        }else{
        *v = 0;
        }
    }

}






int isCollide(SDL_Rect obj1, SDL_Rect obj2)
{
    return
    (
        obj1.x          >= obj2.x && obj1.x         <= obj2.x+obj2.w &&
        obj1.y          >= obj2.y && obj1.y         <= obj2.y+obj2.h ||

        obj1.x+obj1.w   >= obj2.x && obj1.x+obj1.w  <= obj2.x+obj2.w &&
        obj1.y          >= obj2.y && obj1.y         <= obj2.y+obj2.h ||

        obj1.x          >= obj2.x && obj1.x         <= obj2.x+obj2.w &&
        obj1.y+obj1.h   >= obj2.y && obj1.y+obj1.h  <= obj2.y+obj2.h ||

        obj1.x+obj1.w   >= obj2.x && obj1.x+obj1.w  <= obj2.x+obj2.w &&
        obj1.y+obj1.h   >= obj2.y && obj1.y+obj1.h  <= obj2.y+obj2.h ||

        obj2.x          >= obj1.x && obj2.x         <= obj1.x+obj1.w &&
        obj2.y          >= obj1.y && obj2.y         <= obj1.y+obj1.h ||

        obj2.x+obj2.w   >= obj1.x && obj2.x+obj2.w  <= obj1.x+obj1.w &&
        obj2.y          >= obj1.y && obj2.y         <= obj1.y+obj1.h ||

        obj2.x          >= obj1.x && obj2.x         <= obj1.x+obj1.w &&
        obj2.y+obj2.h   >= obj1.y && obj2.y+obj2.h  <= obj1.y+obj1.h ||

        obj2.x+obj2.w   >= obj1.x && obj2.x+obj2.w  <= obj1.x+obj1.w &&
        obj2.y+obj2.h   >= obj1.y && obj2.y+obj2.h  <= obj1.y+obj1.h
    );
}
