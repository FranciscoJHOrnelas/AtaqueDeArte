#include "FSMJugador.hpp"
#include "../Func_aux.hpp"
#include<SDL.h>
#include<iostream>
#include <sstream>
#include "../../motor/fisica/MotorFisico.hpp"
#include "../../motor/Plotter.hpp"
#include "../../motor/camaras/ManejadorCamara.hpp"
#include <cmath>

/*
IDLE
*/
EstadoJugadorIDLE::EstadoJugadorIDLE()
{
    if(strnombre!="DERROTADO") {
        strnombre = "IDLE";
        frames_actual_ani=0;
        frames_maxim_ani=4;
        dir=1;
    }
    
};

FSMJugador* EstadoJugadorIDLE::input_handle(KeyOyente& input, MouseOyente& mouse)
{
    if(strnombre=="DERROTADO")        
        return NULL;   
    if(input.estaPresionado(SDL_SCANCODE_D))
        return new EstadoJugadorDerecha();
    if(input.estaPresionado(SDL_SCANCODE_A))
        return new EstadoJugadorIzquierda();
    if(input.estaPresionado(SDL_SCANCODE_SPACE))
        return new EstadoJugadorAtacar();
    /*if(input.estaPresionado(SDL_SCANCODE_T))
        return new EstadoJugadorDerrota();*/
    if(input.estaPresionado(SDL_SCANCODE_W))
        return new EstadoJugadorGravedadBrinco(-7);


    return NULL;
};
void EstadoJugadorIDLE::entrar(Jugador& player)
{
    frames_actual_ani=0;
    frames_maxim_ani=4;
    //DEBUGCOOR(player.get_piso());
    dir = (player.get_sprite()->get_flip())? -1 : 1;
    //printf("DIR: %d\n",dir);
};
void EstadoJugadorIDLE::salir(Jugador& player)
{

};
void EstadoJugadorIDLE::update(Jugador& player,double dt)
{
    player.get_sprite()->play_frame(0,frames_actual_ani%frames_maxim_ani);
    if(frame_dt>7)
    {
        frame_dt=0;
        frames_actual_ani++;
    }
    frame_dt++;

};

/*
MOVER
*/
EstadoJugadorIzquierda::EstadoJugadorIzquierda()
{
    if(strnombre!="DERROTADO") {
        frames_actual_ani=0;
        frames_maxim_ani=4;
        strnombre="IZQUIERDA";
    }

};
void EstadoJugadorIzquierda::entrar(Jugador &player)
{
    frames_actual_ani=0;
    frames_maxim_ani=4;
    player.get_sprite()->set_flip(true);
};

void EstadoJugadorIzquierda::salir(Jugador &player)
{
    //player.get_sprite()->set_flip(false);
};

FSMJugador* EstadoJugadorIzquierda::input_handle(KeyOyente& input, MouseOyente& mouse)
{
    if(strnombre=="DERROTADO")        
        return NULL;   
    if(input.estaPresionado(SDL_SCANCODE_D))
        return new EstadoJugadorDerecha();
    /*if(input.estaPresionado(SDL_SCANCODE_SPACE))
        return new EstadoJugadorAtacar();
    if(input.estaPresionado(SDL_SCANCODE_T))
        return new EstadoJugadorDerrota();*/
    if(input.estaPresionado(SDL_SCANCODE_W)) {
        return new EstadoJugadorGravedadBrinco(-7);
    }

    if(!(input.estaPresionado(SDL_SCANCODE_A)) && !(input.estaPresionado(SDL_SCANCODE_D)))
    {
        return new EstadoJugadorIDLE();
    }

    return NULL;
};

void EstadoJugadorIzquierda::update(Jugador &player, double dt)
{
    /*Coordenadas p = player.get_posicion_mundo();
    p.x +=(player.get_velocidad()*-1); 
    player.set_posicion_mundo(p);
    printf("[izquierda] (%d,%d)\n",p.x,p.y);*/
    player.get_sprite()->play_frame(1,frames_actual_ani%frames_maxim_ani);

    if(frame_dt>3)
    {
        frame_dt=0;
        frames_actual_ani++;
    }
    frame_dt++;
};

//--
EstadoJugadorDerecha::EstadoJugadorDerecha()
{
    if(strnombre!="DERROTADO") {
        frames_actual_ani=0;
        frames_maxim_ani=4;
        strnombre="DERECHA";
    }

};
void EstadoJugadorDerecha::entrar(Jugador &player)
{
    frames_actual_ani=0;
    frames_maxim_ani=4;
    player.get_sprite()->set_flip(false);
    //ManejadorCamaras::unluck_objeto();
};

void EstadoJugadorDerecha::salir(Jugador &player)
{
    //player.get_sprite()->set_flip(false);
    //ManejadorCamaras::lock_objeto(player);
};

FSMJugador* EstadoJugadorDerecha::input_handle(KeyOyente& input, MouseOyente& mouse)
{
    if(strnombre=="DERROTADO")        
        return NULL;   
    if(input.estaPresionado(SDL_SCANCODE_A))
        return new EstadoJugadorIzquierda();

    /*if(input.estaPresionado(SDL_SCANCODE_SPACE))
        return new EstadoJugadorAtacar();
    if(input.estaPresionado(SDL_SCANCODE_T))
        return new EstadoJugadorDerrota();*/

    if(input.estaPresionado(SDL_SCANCODE_W))
        return new EstadoJugadorGravedadBrinco(-7);

    if(!(input.estaPresionado(SDL_SCANCODE_A)) && !(input.estaPresionado(SDL_SCANCODE_D)))
    {
        return new EstadoJugadorIDLE();
    }

    return NULL;
};

void EstadoJugadorDerecha::update(Jugador &player, double dt)
{
    /*FSMCamara* ec = (FSMCamara*)ManejadorCamaras::get_estado();
    if(ec->strestado =="transicion")
    {
        Coordenadas p = player.get_posicion_mundo();
        p.x +=(player.get_velocidad()); 
        player.set_posicion_mundo(p);
    }*/
    player.get_sprite()->play_frame(1,frames_actual_ani%frames_maxim_ani);

    if(frame_dt>3)
    {
        frame_dt=0;
        frames_actual_ani++;
    }
    frame_dt++;
};

/*
- BRINCO GRAVEDAD
*/
EstadoJugadorGravedadBrinco::EstadoJugadorGravedadBrinco(int fuerza)
{
    if(strnombre!="DERROTADO")
    {
        f=fuerza;
        frames_actual_ani=0;
        frames_maxim_ani=4;
        strnombre="BRINCAR";
        en_aire=true;
    }

};

FSMJugador * EstadoJugadorGravedadBrinco::input_handle(KeyOyente& input, MouseOyente& mouse)
{
    if(strnombre=="DERROTADO")        
        return NULL;    
    if(!en_aire)
        return new EstadoJugadorIDLE();
    
    if(input.estaPresionado(SDL_SCANCODE_D) && !en_colision)
    {
        //flip=true;
        P1.x+=4;
    }
    if(input.estaPresionado(SDL_SCANCODE_A) && !en_colision)
    {
        P1.x-=4;
        //flip=false;
    }
    return NULL;
};

void EstadoJugadorGravedadBrinco::entrar(Jugador& player)
{
    player.set_dtgf(f);
    P1 = player.get_posicion_mundo();//player.get_piso();
    en_aire=true;
    player.set_onPlataforma(false);
    //ManejadorCamaras::set_estado(new EstadoCamaraTransicion());

};
void EstadoJugadorGravedadBrinco::salir(Jugador& player)
{
    
};

void EstadoJugadorGravedadBrinco::update(Jugador& player,double dt)
{
    en_colision = !en_aire;
    en_colision = player.en_colision;
    player.get_sprite()->play_frame(1,frames_actual_ani%frames_maxim_ani);
    P1.y+=player.get_dtgf(); 
    //player.get_sprite()->set_flip(flip);

    player.set_posicion_mundo(P1);
    if(player.get_onPlataforma())
        en_aire=false;
    
    if(frame_dt>(int)13)
    {
        frame_dt=0;
        if(frames_actual_ani<frames_maxim_ani-1)
            frames_actual_ani++;
    }
    frame_dt++;
};


EstadoJugadorAtacar::EstadoJugadorAtacar()
{
    if(strnombre!="DERROTADO"){
        frames_actual_ani=0;
        frames_maxim_ani=4;
        strnombre="ATACAR";
        anim_comp=false;
    }        

};

FSMJugador * EstadoJugadorAtacar::input_handle(KeyOyente& input, MouseOyente& mouse)
{
    if(strnombre=="DERROTADO")        
        return NULL;
    /*if(input.estaPresionado(SDL_SCANCODE_D))
        return new EstadoJugadorDerecha();
    if(input.estaPresionado(SDL_SCANCODE_A))
        return new EstadoJugadorIzquierda();
    if(input.estaPresionado(SDL_SCANCODE_SPACE))
        return new EstadoJugadorAtacar();
    if(input.estaPresionado(SDL_SCANCODE_T))
        return new EstadoJugadorDerrota();
    if(input.estaPresionado(SDL_SCANCODE_W))
        return new EstadoJugadorGravedadBrinco(-7);*/
    if(anim_comp){
        return new EstadoJugadorIDLE();
    }
    
    return NULL;
};

void EstadoJugadorAtacar::entrar(Jugador& player)
{

};
void EstadoJugadorAtacar::salir(Jugador& player)
{

};

void EstadoJugadorAtacar::update(Jugador& player,double dt)
{
    player.get_sprite()->play_frame(3,frames_actual_ani%frames_maxim_ani);
    if(frames_actual_ani==0) 
    {
        //Invocar protector
    }
    if(frame_dt>(int)10)
    {
        frame_dt=0;
        frames_actual_ani++;
    }
    frame_dt++;
    if(frames_actual_ani==frames_maxim_ani) {
        player.set_estado(new EstadoJugadorIDLE());
    }
};


EstadoJugadorDerrota::EstadoJugadorDerrota()
{
    frames_actual_ani=0;
    frames_maxim_ani=4;
    strnombre="DERROTADO";
};

FSMJugador * EstadoJugadorDerrota::input_handle(KeyOyente& input, MouseOyente& mouse)
{
    return NULL;
};

void EstadoJugadorDerrota::entrar(Jugador& player)
{

};
void EstadoJugadorDerrota::salir(Jugador& player)
{

};

void EstadoJugadorDerrota::update(Jugador& player,double dt)
{
    player.get_sprite()->play_frame(2,frames_actual_ani%frames_maxim_ani);
    
    if(frame_dt>(int)3)
    {
        frame_dt=0;
        frames_actual_ani++;
        if(frames_actual_ani%frames_maxim_ani==0)
            frames_actual_ani++;
    }
    frame_dt++;
};

EstadoJugadorVictoria::EstadoJugadorVictoria()
{
    frames_actual_ani=0;
    frames_maxim_ani=4;
    strnombre="VICTORIA";
};

FSMJugador * EstadoJugadorVictoria::input_handle(KeyOyente& input, MouseOyente& mouse)
{
    return NULL;
};

void EstadoJugadorVictoria::entrar(Jugador& player)
{
    player.set_dtgf(0);
};
void EstadoJugadorVictoria::salir(Jugador& player)
{

};

void EstadoJugadorVictoria::update(Jugador& player,double dt)
{
    player.set_dtgf(0);
    player.get_sprite()->play_frame(4,frames_actual_ani%frames_maxim_ani);
    
    if(frame_dt>(int)3)
    {
        frame_dt=0;
        frames_actual_ani++;
    }
    frame_dt++;
};
