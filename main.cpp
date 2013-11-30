/*This source code copyrighted by Lazy Foo' Productions (2004-2013)
and may not be redistributed without written permission.*/

//The headers
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"
#include <string>
#include <iostream>
#include <vector>
#include "Personaje.h"

using namespace std;

//Screen attributes
const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;
const int SCREEN_BPP = 32;
const int FRAMES_PER_SECOND = 20;
//The surfaces
SDL_Surface *background = NULL;
SDL_Surface *upMessage = NULL;
SDL_Surface *downMessage = NULL;
SDL_Surface *leftMessage = NULL;
SDL_Surface *rightMessage = NULL;
SDL_Surface *message = NULL;
SDL_Surface *screen = NULL;

//The event structure
SDL_Event event;

//The font
TTF_Font *font = NULL;

//The color of the font
SDL_Color textColor = { 0, 0, 0 };
class Timer
{
    private:
    //The clock time when the timer started
    int startTicks;

    //The ticks stored when the timer was paused
    int pausedTicks;

    //The timer status
    bool paused;
    bool started;

    public:
    //Initializes variables
    Timer();

    //The various clock actions
    void start();
    void stop();
    void pause();
    void unpause();

    //Gets the timer's time
    int get_ticks();

    //Checks the status of the timer
    bool is_started();
    bool is_paused();
};
Timer::Timer()
{
    //Initialize the variables
    startTicks = 0;
    pausedTicks = 0;
    paused = false;
    started = false;
}
void Timer::start()
{
    //Start the timer
    started = true;

    //Unpause the timer
    paused = false;

    //Get the current clock time
    startTicks = SDL_GetTicks();
}

void Timer::stop()
{
    //Stop the timer
    started = false;

    //Unpause the timer
    paused = false;
}
void Timer::pause()
{
    //If the timer is running and isn't already paused
    if( ( started == true ) && ( paused == false ) )
    {
        //Pause the timer
        paused = true;

        //Calculate the paused ticks
        pausedTicks = SDL_GetTicks() - startTicks;
    }
}
void Timer::unpause()
{
    //If the timer is paused
    if( paused == true )
    {
        //Unpause the timer
        paused = false;

        //Reset the starting ticks
        startTicks = SDL_GetTicks() - pausedTicks;

        //Reset the paused ticks
        pausedTicks = 0;
    }
}
int Timer::get_ticks()
{
    //If the timer is running
    if( started == true )
    {
        //If the timer is paused
        if( paused == true )
        {
            //Return the number of ticks when the timer was paused
            return pausedTicks;
        }
        else
        {
            //Return the current time minus the start time
            return SDL_GetTicks() - startTicks;
        }
    }

    //If the timer isn't running
    return 0;
}
bool Timer::is_started()
{
    return started;
}

bool Timer::is_paused()
{
    return paused;
}
SDL_Surface *load_image( std::string filename )
{
    return IMG_Load( filename.c_str() );
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL )
{
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
}

bool init()
{
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }

    //Set up the screen
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

    //If there was an error in setting up the screen
    if( screen == NULL )
    {
        return false;
    }

    //Initialize SDL_ttf
    if( TTF_Init() == -1 )
    {
        return false;
    }

    //Set the window caption
    SDL_WM_SetCaption( "Press an Arrow Key", NULL );

    //If everything initialized fine
    return true;
}

bool load_files()
{
    //Load the background image
    background = load_image( "background.png" );

    //Open the font
    font = TTF_OpenFont( "lazy.ttf", 72 );

    //If there was a problem in loading the background
    if( background == NULL )
    {
        return false;
    }

    //If there was an error in loading the font
    if( font == NULL )
    {
        return false;
    }

    //If everything loaded fine
    return true;
}

void clean_up()
{
    //Free the surfaces
    SDL_FreeSurface( background );
    SDL_FreeSurface( upMessage );
    SDL_FreeSurface( downMessage );
    SDL_FreeSurface( leftMessage );
    SDL_FreeSurface( rightMessage );

    //Close the font
    TTF_CloseFont( font );

    //Quit SDL_ttf
    TTF_Quit();

    //Quit SDL
    SDL_Quit();
}

bool puedoLLegar(char tablero[8][8],int x_actual, int y_actual,int pasos, int x_final, int y_final)
{
    //Casos base
    if(pasos<0)
        return false;

    if(x_actual>=8
       || y_actual>=8
       || x_actual<0
       || y_actual<0)
       return false;

    if(tablero[y_actual][x_actual]=='#')
       return false;

    if(x_actual==x_final
       && y_actual==y_final)
       return true;

    if(tablero[y_actual][x_actual]=='O')
        pasos--;

    if(tablero[y_actual][x_actual]=='V')
        pasos++;
      if(tablero[y_actual][x_actual]=='V'
           && pasos<=0)
            pasos=1;
    pasos--;



    return puedoLLegar(tablero,x_actual+1,y_actual,pasos,x_final,y_final)
            || puedoLLegar(tablero,x_actual-1,y_actual,pasos,x_final,y_final)
            || puedoLLegar(tablero,x_actual,y_actual+1,pasos,x_final,y_final)
            || puedoLLegar(tablero,x_actual,y_actual-1,pasos,x_final,y_final);
}
bool ataque(Personaje* p1, Personaje* p2){
    if(p1->x+1==p2->x&&p1->y==p2->y)
        return true;
    if(p1->x==p2->x&&p1->y-1==p2->y)
        return true;
    if(p1->x==p2->x&&p1->y+1==p2->y)
        return true;
    if(p1->x-1==p2->x&&p1->y==p2->y)
        return true;

    return false;
}
void marcar(char tablero[8][8],char tablero_de_pasos[8][8],char ataca[8][8],int x_actual, int y_actual,int pasos)
{
    //Casos base
    if(pasos<0)
        return;

    if(x_actual>=8
       || y_actual>=8
       || x_actual<0
       || y_actual<0)
       return;

    if(tablero[y_actual][x_actual]=='#')
       return;

    tablero_de_pasos[y_actual][x_actual]='P';
    if (pasos>=2&&tablero[y_actual][x_actual]==' ')
        ataca[y_actual][x_actual]='A';

    if(tablero[y_actual][x_actual]=='O')
        pasos--;

    if(tablero[y_actual][x_actual]=='V')
        pasos++;
    if(tablero[y_actual][x_actual]=='V'
           && pasos<=0)
            pasos=1;
    pasos--;



    marcar(tablero,tablero_de_pasos,ataca,x_actual+1,y_actual,pasos);
    marcar(tablero,tablero_de_pasos,ataca,x_actual-1,y_actual,pasos);
    marcar(tablero,tablero_de_pasos,ataca,x_actual,y_actual+1,pasos);
    marcar(tablero,tablero_de_pasos,ataca,x_actual,y_actual-1,pasos);
}




void limpiar(char tablero_de_pasos1[8][8],char ataca[8][8])
{

    for(int x=0;x<8;x++)
        for(int y=0;y<8;y++){
            tablero_de_pasos1[y][x]=' ';
            ataca[y][x]=' ';
        }
}
void limpiar2(char tablero_de_pasos1[8][8])
{

    for(int x=0;x<8;x++)
        for(int y=0;y<8;y++){
            tablero_de_pasos1[y][x]=' ';
        }
}

int main( int argc, char* args[] )
{

    //Quit flag
    bool quit = false;
    //Keep track of the current frame
    int frame = 0;

    //Whether or not to cap the frame rate
    bool cap = true;

    //The frame rate regulator
    Timer fps;
    //Initialize
    if( init() == false )
    {
        return 1;
    }

    //Load the files
    if( load_files() == false )
    {
        return 1;
    }

    //Generate the message surfaces
    //upMessage = TTF_RenderText_Solid( font, "Up was pressed.", textColor );
    //downMessage = TTF_RenderText_Solid( font, "Down was pressed.", textColor );
    //leftMessage = TTF_RenderText_Solid( font, "Left was pressed", textColor );
    //rightMessage = TTF_RenderText_Solid( font, "Right was pressed", textColor );



    SDL_Surface* pasillo = load_image("tablero/pasillo.png");
    SDL_Surface* muro = load_image("tablero/muro.png");
    SDL_Surface* cursor = load_image("tablero/cursor.png");
    SDL_Surface* obstaculo = load_image("tablero/obstaculo.png");
    SDL_Surface* ventaja = load_image("tablero/ventaja.png");
    SDL_Surface* ATACAR = load_image("tablero/ATACAR.png");
     SDL_Surface*Vida = load_image("life.png");
     SDL_Surface*Gandor1 = load_image("jugador1.png");
     SDL_Surface*Gandor2  = load_image("jugador2.png");
    SDL_Surface* vida1  = load_image("hp1.png");
    SDL_Surface* vida2  = load_image("hp2.png");
    SDL_Surface* vida3  = load_image("hp3.png");
    SDL_Surface* vida4  = load_image("hp4.png");
    SDL_Surface* vida5  = load_image("hp5.png");
        SDL_Surface* turn1  = load_image("turn1.png");
            SDL_Surface* turn2  = load_image("turn2.png");
    char tablero[8][8]={{' ','#',' ',' ','V','#','#','O',},
                        {' ','#',' ','#',' ',' ',' ','O',},
                        {' ','#','O','#','#',' ','O','O',},
                        {' ','#','O','#','#',' ','#',' ',},
                        {' ',' ','O',' ',' ',' ','#',' ',},
                        {' ',' ',' ','O',' ','V','#',' ',},
                        {'#',' ','#','#','O','O','#','O',},
                        {'#','V',' ',' ',' ','O',' ','O',}};

    char tablero_de_pasos1[8][8]={{' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '}};

    char tablero_de_pasos2[8][8]={{' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '}};
     char ataca[8][8]={{' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '}};

    SDL_Surface* pasos_surface = load_image("pasos.png");
    SDL_Surface* personaje_surface = load_image("Personaje.png");
    Personaje personaje(0,0,personaje_surface);

    SDL_Surface* personaje_surface2 = load_image("personaje2.png");
    Personaje personaje2(7,7,personaje_surface2);

    SDL_Surface* cursor_surface = load_image("cursor.png");
    SDL_Surface* turno1 = load_image("turno1.png");
    SDL_Surface* turno2 = load_image("turno2.png");
    int cursor_x=0;
    int cursor_y=0;
    int vida_jugador1=5;
    int vida_jugador2=5;
    bool termino=false;
    bool turno =true;
    Mix_Music *music = NULL;
    Mix_Chunk *wins = NULL;
    Mix_Chunk *atack1 = NULL;
    Mix_Chunk *atack2 = NULL;
    Mix_OpenAudio( 48000, MIX_DEFAULT_FORMAT, 2, 4096 );
    music = Mix_LoadMUS( "song.mp3" );
    wins = Mix_LoadWAV( "duelwin.mp3" );
    atack1 = Mix_LoadWAV( "activate.wav" );
    atack2 = Mix_LoadWAV( "summon.wav" );
    Mix_PlayMusic( music, -1 );
    //While the user hasn't quit
        while( quit == false )
    {
        fps.start();
        //If there's an event to handle
        if( SDL_PollEvent( &event ) )
        {
            //If a key was pressed
            if( event.type == SDL_KEYDOWN )
            {
                //Set the proper message surface

                switch( event.key.keysym.sym )
                {
                     cap = ( !cap );
                    case SDLK_m:
                        vida_jugador1--;
                    break;
                    case SDLK_UP:
                        cursor_y--;
                    break;
                    case SDLK_DOWN:
                        cursor_y++;
                    break;
                    case SDLK_LEFT:
                        cursor_x--;
                    break;
                    case SDLK_RIGHT:
                        cursor_x++;
                    break;
                    case SDLK_RETURN:
                        if(turno==true){
                        limpiar2(tablero_de_pasos2);
                        if(puedoLLegar(tablero,personaje.x,personaje.y,3,cursor_x,cursor_y)&&turno==true)
                        {   personaje.x=cursor_x;
                            personaje.y=cursor_y;
                            if(ataque(&personaje,&personaje2)!=false){
                            vida_jugador2--;
                            Mix_PlayChannel( -1, atack1, 0 );
                            personaje.x=0;
                            personaje.y=0;
                            personaje2.x=7;
                            personaje2.y=7;

                            }
                            limpiar(tablero_de_pasos1,ataca);
                            cursor_x=personaje2.x;
                            cursor_y=personaje2.y;
                            turno=false;
                        }
                        }else{
                         limpiar2(tablero_de_pasos1);
                         if(puedoLLegar(tablero,personaje2.x,personaje2.y,3,cursor_x,cursor_y)&&turno==false)
                        {   personaje2.x=cursor_x;
                            personaje2.y=cursor_y;
                            if(ataque(&personaje2,&personaje)!=false){
                            vida_jugador1--;
                            Mix_PlayChannel( -1, atack2, 0 );
                            personaje.x=0;
                            personaje.y=0;
                            personaje2.x=7;
                            personaje2.y=7;
                            }

                            limpiar(tablero_de_pasos2,ataca);
                            cursor_x=personaje.x;
                            cursor_y=personaje.y;
                            turno=true;

                        }
                        }

                    break;

                        }
            }

            //If the user has Xed out the window
            else if( event.type == SDL_QUIT )
            {
                //Quit the program
                quit = true;
            }
        }



        //Apply the background

        //If a message needs to be displayed
        if( message != NULL )
        {
            //Apply the background to the screen
            apply_surface( 0, 0, background, screen );

            //Apply the message centered on the screen
            apply_surface( ( SCREEN_WIDTH - message->w ) / 2, ( SCREEN_HEIGHT - message->h ) / 2, message, screen );

            //Null the surface pointer
            message = NULL;
        }

        if (termino==false){
        for(int x=0;x<10;x++)
            for(int y=0;y<10;y++)
            {
                if(tablero[y][x]==' ')
                    apply_surface(x*75,y*75,pasillo,screen);
                if(tablero[y][x]=='#')
                    apply_surface(x*75,y*75,muro,screen);
                if(tablero[y][x]=='O')
                    apply_surface(x*75,y*75,obstaculo,screen);
                if(tablero[y][x]=='V')
                    apply_surface(x*75,y*75,ventaja,screen);

            }

         for(int x=0;x<8;x++){
            for(int y=0;y<8;y++){
            if(tablero_de_pasos1[y][x]=='P'){
            apply_surface(x*75,y*75,pasos_surface,screen);
            }
             if(ataca[y][x]=='A'){
            apply_surface(x*75,y*75,ATACAR,screen);
             }
            }
         }
          for(int x=0;x<8;x++){
            for(int y=0;y<8;y++){
            if(tablero_de_pasos2[y][x]=='P'){
            apply_surface(x*75,y*75,pasos_surface,screen);
            }
             if(ataca[y][x]=='A'){
            apply_surface(x*75,y*75,ATACAR,screen);
             }
            }
         }
         if(vida_jugador1==5){
            apply_surface(0,0,vida1,screen);
         }
        if(vida_jugador1==4){
            apply_surface(0,0,vida2,screen);
         }
         if(vida_jugador1==3){
            apply_surface(0,0,vida3,screen);
         }
         if(vida_jugador1==2){
            apply_surface(0,0,vida4,screen);
         }
         if(vida_jugador1==1){
            apply_surface(0,0,vida5,screen);
         }
        if(vida_jugador2==5){
            apply_surface(450,0,vida1,screen);
         }
        if(vida_jugador2==4){
            apply_surface(450,0,vida2,screen);
         }
         if(vida_jugador2==3){
            apply_surface(450,0,vida3,screen);
         }
         if(vida_jugador2==2){
            apply_surface(450,0,vida4,screen);
         }
         if(vida_jugador2==1){
            apply_surface(450,0,vida5,screen);
         }

    if(turno==true)
        apply_surface(220,0,turn1,screen);
    else
        apply_surface(220,0,turn2,screen);


        apply_surface(cursor_x*75,cursor_y*75,cursor_surface,screen);
         personaje.dibujar(screen);
        personaje2.dibujar(screen);

        }
        //Update the screen
        if( SDL_Flip( screen ) == -1 )
        {
            return 1;
        }
    if (cursor_x>7)
        cursor_x=7;
     if (cursor_y>7)
        cursor_y=7;
    if (cursor_x<0)
        cursor_x=0;
     if (cursor_y<0)
        cursor_y=0;

       if (vida_jugador1==0){
        termino=true;
        apply_surface(0,0,Gandor2,screen);
        Mix_HaltMusic();
        }
        if (vida_jugador2==0){
        termino=true;
        apply_surface(0,0,Gandor1,screen);
        Mix_HaltMusic();
        }
        if(turno==true){
            marcar(tablero,tablero_de_pasos1,ataca,personaje.x,personaje.y,3);
        }
        else{
            marcar(tablero,tablero_de_pasos2,ataca,personaje2.x,personaje2.y,3);
        }
                //Increment the frame counter
        frame++;

        //If we want to cap the frame rate
        if( ( cap == true ) && ( fps.get_ticks() < 1000 / FRAMES_PER_SECOND ) )
        {
            //Sleep the remaining frame time
            SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks() );
        }
    }

    //Clean up
    clean_up();

    return 0;
}
