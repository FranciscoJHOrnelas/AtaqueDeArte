#include "ObjetoDinamico.hpp"
#include "../objetos/Figuras.hpp"
#include "../utilidad/Func_aux.hpp"
#include "../motor/camaras/ManejadorCamara.hpp"
#include "../motor/fisica/MotorFisico.hpp"
#include<iostream>
#include <cmath>

Jugador::Jugador(float vida, int x, int y , SDL_Color c)
:ObjetoDinamico{}
{
    hp = vida;
    posicion_mundo.x=x;
    posicion_mundo.y=y;
    avatar = new Rectangulo(x,y,75,75,c);
    c.a=150;
    color = c;
    avatar->set_rellenocolor(c);
    avatar->set_serellena(true);
    col_box = new Rectangulo(x,y,75+10,75+10,c);
    col_box->set_serellena(false);
    tiene_fisica = true;
    en_colision = false;
    estado_actual = new EstadoJugadorIDLE();
    piso = {x,y+100}; // definir el piso en general
    sprite = nullptr;
    tile=nullptr;
    velocidad=0;
    objetos=nullptr;
    area_plataforma=nullptr;
};

Jugador::Jugador(std::string path_sprite,float vida, int x, int y, int w, int h, int sw, int sh, SDL_Color c)
:ObjetoDinamico{}
{
    hp = vida;
    posicion_mundo.x=x;
    posicion_mundo.y=y;
    posicion_camara={0,0};
    avatar = new Rectangulo(x,y,sw,sh,c);
    c.a=150;
    color = c;
    avatar->set_rellenocolor(c);
    //avatar->set_serellena(false);
    col_box = new Rectangulo(x,y,sw-16,sh,c);
    col_box->set_serellena(false);
    tiene_fisica = true;
    en_colision = false;
    estado_actual = new EstadoJugadorIDLE();
    kills=0;
    c.a=255;
    col_piso = new Rectangulo(x-5,y+sh,sw/2+10,10,{150,200,0,255});

    piso = {x,y+sw}; // definir el piso en general
    sprite = new Sprite(path_sprite,posicion_mundo,w,h,sw,sh);
    tile = nullptr;
    objetos=nullptr;
    velocidad=5;
    dtgravedad=0;
    area_plataforma=col_piso;
    centro={posicion_camara.x+sprite->get_sprite_sizes().x/2,posicion_camara.y+sprite->get_sprite_sizes().y/2};
    
};


std::string Jugador::get_strEstado()
{
    return estado_actual->get_namestate();
};

void* Jugador::get_estado()
{
    return estado_actual;
};

void Jugador::set_estado(void* estado)
{
    estado_actual->salir(*this);
    delete estado_actual;
    estado_actual = (FSMJugador*)estado;
    estado_actual->entrar(*this);
};

void Jugador::set_onPlataforma(bool v)
{
    en_plataforma=v;
};

void Jugador::update(double dt)
{

    if(!estado_actual) //nulo
        return;

    centro={posicion_camara.x+sprite->get_sprite_sizes().x/2,posicion_camara.y+sprite->get_sprite_sizes().y/2};
    if(area_plataforma)
    {
        area_plataforma->set_position(posicion_camara.x+20,posicion_camara.y+sprite->get_sprite_sizes().y-10);
        area_plataforma->update_vertices();
    }

    estado_actual->update(*this,dt);
    
    //reset
    
    if(en_plataforma) 
    {
        set_dtgf(0);
    }
    set_onPlataforma(false);

};

void Jugador::input_handle(KeyOyente& input,MouseOyente& mouse)
{
    if(!estado_actual) //nulo
        return;
    
    FSMJugador* estado = estado_actual->input_handle(input,mouse);

    if(estado)
    {
        set_estado(estado);
    }

};

/*
---
*/

BackGroundDinamico::BackGroundDinamico(std::string path_sprite,int w, int h)
{
    posicion_mundo.x=-w/2;
    posicion_mundo.y=-h/4;
    posicion_camara = posicion_mundo;
    avatar= nullptr;
    tile = nullptr;
    col_box = nullptr;
    //toda la imagen sprite?
    limites.x = w;
    limites.y = 0;
    sprite = new Sprite(path_sprite,posicion_mundo,w,h,w,h);
    tiene_fisica=false;
    en_colision=false;
    velocidad=0;
    area_plataforma=nullptr;
    //crear la camara antes del background
    centro_cam = ManejadorCamaras::get_camara().get_posicion_centro();
};
void BackGroundDinamico::set_estado(void *estado){};
void* BackGroundDinamico::get_estado()
{
    return NULL;
}
void BackGroundDinamico::update(double dt)
{   
    /*
    //parallax/scroller reset    
    float dist = std::sqrt(std::pow( posicion_mundo.x-limites.x/2,2)+std::pow( posicion_mundo.y-limites.y,2));
    //DEBUGPRINT(dist)
    if(dist < (limites.x/2)+centro_cam.x-velocidad) //izquierda
    {
        //reset
        posicion_mundo.x=-(limites.x/2)-centro_cam.x-velocidad;
    }
    else if(dist > limites.x+centro_cam.x+velocidad) //derecha
    {
         //reset
        posicion_mundo.x=-(limites.x/2)+centro_cam.x+velocidad;
    }*/
};

/*
Plataformas Dinámicas
*/

PlataformasDinamicas::PlataformasDinamicas(std::string sprite_path, int x, int y, int w, int h,int sw, int sh,SDL_Color c)
:ObjetoDinamico{}
{
    posicion_mundo.x=x;
    posicion_mundo.y=y;
    posicion_camara = {0,0};
    avatar= nullptr;
    tile = nullptr;
    //usar para la colision
    col_box = nullptr; 
    //render_colbox=true;

    //usar para el area de piso de la plataforma
    area_plataforma = nullptr;

    //toda la imagen sprite?
    sprite      = new Sprite(sprite_path,posicion_mundo,w,h,sw,sh);
    tiene_fisica=false;
    en_colision =false;
    velocidad=0;
    frames_actual_ani=0;
    frames_maxim_ani=8;
};

void PlataformasDinamicas::update(double dt)
{
    get_sprite()->play_frame(0,frames_actual_ani%frames_maxim_ani);
    if(frame_dt>7)
    {
        frame_dt=0;
        frames_actual_ani++;
    }
    frame_dt++;
    //area_plataforma->set_position(posicion_camara.x-10,posicion_camara.y);
    //area_plataforma->update_vertices();
    //DEBUGCOOR(area_plataforma->get_posicion());
};
void PlataformasDinamicas::set_estado(void* estado)
{

};
void* PlataformasDinamicas::get_estado()
{
    return NULL;
};

//Enemigo
Enemigo::Enemigo(float vida, int x, int y , SDL_Color c)
:ObjetoDinamico{}
{
    hp = vida;
    posicion_mundo.x=x;
    posicion_mundo.y=y;
    avatar = new Rectangulo(x,y,75,75,c);
    c.a=150;
    color = c;
    avatar->set_rellenocolor(c);
    avatar->set_serellena(true);
    col_box = new Rectangulo(x,y,75+10,75+10,c);
    col_box->set_serellena(false);
    tiene_fisica = false;
    en_colision = false;
    estado_actual = new EstadoEnemigoIDLE();
};

Enemigo::Enemigo(std::string path_sprite,float vida, int x, int y, int w, int h, int sw, int sh, SDL_Color c)
:ObjetoDinamico{}
{
    hp = vida;
    posicion_mundo.x=x;
    posicion_mundo.y=y;
    avatar = new Rectangulo(x,y,sw,sh,c);
    c.a=150;
    color = c;
    avatar->set_rellenocolor(c);
    avatar->set_serellena(true);
    col_box = new Rectangulo(x,y,sw,sh,c);
    col_box->set_serellena(false);
    tiene_fisica = false;
    en_colision = false;
    estado_actual = new EstadoEnemigoMOVER({0,0});
    //piso = {500,500}; // definir el piso en general

    sprite = new Sprite(path_sprite,posicion_mundo,w,h,sw,sh);
    tile = nullptr;
};
void Enemigo::set_velocidad(int v) {velocidad = v;};

std::string Enemigo::get_strEstado()
{
    return estado_actual->get_namestate();
};

void* Enemigo::get_estado()
{
    return estado_actual;
};

void Enemigo::set_estado(void* estado)
{
    estado_actual->salir(*this);
    delete estado_actual;
    estado_actual = (FSMEnemigo*)estado;
    estado_actual->entrar(*this);
};

void Enemigo::update(double dt)
{

    if(!estado_actual) //nulo
        return;

    if(en_colision) {
        set_hp(1);
        set_estado(new EstadoEnemigoDerrota());
        avatar->set_rellenocolor({255,0,0,255});
        en_colision=false;
        //set_posicion_mundo({rand()%4000+500, rand()%1200-500});
        //col_box = nullptr;
    }

    estado_actual->update(*this,dt);
    
    //reset
    //en_colision=false;


};

void Enemigo::lock_objeto(Objeto &obj)
{
    //Alumnos implementarla
    objeto_seguir = &obj;
    
};

Protector::Protector(std::string sprite_path, int x, int y, int w, int h,int sw, int sh,SDL_Color c)
:ObjetoDinamico{}
{
    posicion_mundo.x=x;
    posicion_mundo.y=y;
    avatar= nullptr;
    tile = nullptr;
    trigger_box = new Rectangulo(x,y,sw,sh,c);
    //usar para la colision
    //col_box = new Rectangulo(x,y,sw,sh,c);
    //col_box->set_serellena(false);
    //render_colbox=true;

    //toda la imagen sprite?
    sprite      = new Sprite(sprite_path,posicion_mundo,w,h,sw,sh);
    tiene_fisica=false;
    en_colision =false;
    frames_actual_ani=0;
    frames_maxim_ani=6;
};

void Protector::update(double dt) 
{
    if(posicion_mundo.x>-2000) {
        get_sprite()->play_frame(0,frames_actual_ani%frames_maxim_ani);
        if(frame_dt>10)
        {
            frame_dt=0;
            frames_actual_ani++;
        }
        frame_dt++;
    }
    else frames_actual_ani=0;

};

void Protector::set_faa(int faa){
    frames_actual_ani=0;
}
void* Protector::get_estado()
{
    return NULL;
};

void Protector::set_estado(void* estado){

};

Meta::Meta(std::string sprite_path, int x, int y, int w, int h,int sw, int sh,SDL_Color c)
:ObjetoDinamico{}
{
    posicion_mundo.x=x;
    posicion_mundo.y=y;
    posicion_camara = {0,0};
    avatar= nullptr;
    tile = nullptr;
    //usar para la colision
    col_box = new Rectangulo(x,y,sw,sh,c);
    //col_box->set_serellena(false);
    //render_colbox=true;

    //toda la imagen sprite?
    sprite      = new Sprite(sprite_path,posicion_mundo,w,h,sw,sh);
    tiene_fisica=false;
    en_colision =false;
    frames_actual_ani=0;
    frames_maxim_ani=4;
};

void Meta::del_col_box() {
    col_box=nullptr;
}
void Meta::update(double dt) 
{
    get_sprite()->play_frame(0,frames_actual_ani%frames_maxim_ani);
    if(frame_dt>7)
    {
        frame_dt=0;
        frames_actual_ani++;
    }
    frame_dt++;
};

void* Meta::get_estado()
{
    return NULL;
};

void Meta::set_estado(void* estado)
{

};