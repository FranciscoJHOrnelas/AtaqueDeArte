#include "FSMEnemigo.hpp"
#include "../Func_aux.hpp"
#include "../Tiempo.hpp"
#include<SDL.h>
#include<iostream>
#include <sstream>
#include<cmath>
#include <stdlib.h>
#include <time.h>
/*
IDLE
*/
EstadoEnemigoIDLE::EstadoEnemigoIDLE()
{
    strnombre = "IDLE";
    frames_actual_ani=0;
    frames_maxim_ani=4;
};

void EstadoEnemigoIDLE::entrar(Enemigo& enemy)
{
    frames_actual_ani=0;
    frames_maxim_ani=4;
};
void EstadoEnemigoIDLE::salir(Enemigo& enemy)
{

};
void EstadoEnemigoIDLE::update(Enemigo& enemy,double dt)
{

    enemy.get_sprite()->play_frame(0,frames_actual_ani%frames_maxim_ani);
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
EstadoEnemigoMOVER::EstadoEnemigoMOVER(Coordenadas dir)
{
    strnombre = "MOVER";
    direccion = dir;
    velocidad = 4;
    frames_actual_ani=0;
    frames_maxim_ani=4;
    
};

void EstadoEnemigoMOVER::entrar(Enemigo& enemy)
{
    frames_actual_ani=0;
    frames_maxim_ani=4;
};
void EstadoEnemigoMOVER::salir(Enemigo& enemy){};

void EstadoEnemigoMOVER::update(Enemigo& enemy, double dt) 
{
    //Derecha
    Coordenadas inicio = enemy.get_posicion_mundo();
    Coordenadas final = enemy.get_obj_lock()->get_posicion_mundo();
    Coordenadas n = inicio;
    //inicio.x += (enemy.get_velocidad());
    enemy.set_posicion_mundo(inicio);
    enemy.get_sprite()->play_frame(1, frames_actual_ani%frames_maxim_ani);
    if(final.x>inicio.x) n.x += enemy.get_velocidad();
    else n.x -= enemy.get_velocidad();
    if(final.y>inicio.y) n.y += enemy.get_velocidad();
    else n.y -= enemy.get_velocidad();

    enemy.set_posicion_mundo(n);


    /*for(auto& obj : player.get_objetos_mundo())
    {
        Coordenadas po = obj->get_posicion_mundo();
        po.x -= obj->get_velocidad();
        obj->set_posicion_mundo(po);
    }*/
    enemy.get_sprite()->play_frame(0,frames_actual_ani%frames_maxim_ani);
    if(frame_dt>20)
    {
        frame_dt=0;
        frames_actual_ani++;
    }
    frame_dt++;
};

/*
DERROTA
*/
EstadoEnemigoDerrota::EstadoEnemigoDerrota()
{
    strnombre = "DERROTADO";
    frames_actual_ani=0;
    frames_maxim_ani=4;
    recuperation_time=rand()%7+4;
    init_tiempo=Tiempo::get_tiempo();

};

void EstadoEnemigoDerrota::entrar(Enemigo& enemy)
{
    frames_actual_ani=0;
    frames_maxim_ani=4;
};
void EstadoEnemigoDerrota::salir(Enemigo& enemy){};

void EstadoEnemigoDerrota::update(Enemigo& enemy, double dt) 
{
    dt =  Tiempo::get_tiempo() - init_tiempo;

    enemy.get_sprite()->play_frame(1,frames_actual_ani%frames_maxim_ani);
    if(frame_dt>20)
    {
        frame_dt=0;
        frames_actual_ani++;
    }
    frame_dt++;
    if((int)dt>recuperation_time)
    {
        int xflip=1;
        if(enemy.get_posicion_mundo().x>3000) xflip=-1;
        enemy.set_posicion_mundo({enemy.get_posicion_mundo().x + (rand()%2000+500)*xflip, enemy.get_posicion_mundo().y + rand()%1700-500});
        enemy.set_hp(10);
        enemy.set_estado(new EstadoEnemigoMOVER({0,0}));
    }
};