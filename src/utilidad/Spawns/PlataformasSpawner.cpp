#include "PlataformasSpawner.hpp"
#include "../Tiempo.hpp"
#include "../Func_aux.hpp"
#include "../../objetos_dinamicos/ObjetoDinamico.hpp"
#include<iostream>
#include<cstdlib>
#include <cmath>
#include <vector>
#include <algorithm>

PlataformasSpawner::PlataformasSpawner(std::string sprite_path, int x, int y, int w, int h,int sw, int sh,SDL_Color c,Pipeline &p)
{
    this->sprite_path=sprite_path;
    colordebug=c;
    this->x=x; // pos x
    this->y=y; // pos y
    this->w=w; // ancho sprite
    this->h=h; // alto sprite
    this->sw=sw;// ancho mostrar sprite
    this->sh=sh;// alto mostrar sprite
    objetos_activos=0;
    delay=1; 
    init_tiempo=Tiempo::get_tiempo();
    check=false;
    pipeline = &p;
    velocidad=0;
};

void PlataformasSpawner::spawn(std::vector<Objeto*>*lista, Coordenadas c)
{
    /*
    Plataformas 72
    
    (10, 12)  - (10, 17)
    (10, 26)  - (10, 31)
    (12, 71)  - (12, 73)
    (12, 82)  - (12, 84)
    (14, 37)  - (14, 39)
    (15, 59)  - (15, 62)
    (16, 107) - (16, 112)
    (18, 25)  - (18, 30)
    (20, 71)  - (20, 73)
    (20, 95)  - (20, 100)
    (22, 17)  - (22, 19)
    (23, 79)  - (23, 81)
    (25, 107) - (25, 110)
    (27, 11)  - (27, 13)
    (27, 83)  - (27, 88)
    (28, 59)  - (28, 62)
    (28, 116) - (28, 118)
    */

    //random
    /*srand((unsigned) time(NULL));
    int sx = x+rand()%100;
    int sy = y+rand()%200;
    int nw = sw+rand()%100;
    int nh = sh+rand()%100;*/
    c.x = c.x/32;
    c.y = c.y/32;
    bool in=false;
    for(auto &cc:plat_posibles) {
        if(cc.x==c.x && cc.y==c.y){
            in=true;
        }
    }
    if(!in) {
        plat_posibles.push_back(c);
    }

    //int l=0;
    //for(auto &c:plat_posibles) {
    //    for(int i=0;i<lens[l];i++) {
    PlataformasDinamicas * nuevo = new PlataformasDinamicas(
        sprite_path,(c.x*32)-16,(c.y*32)-16,w,h,sw,sh,colordebug);
    nuevo->set_velocidad(velocidad);

    pipeline->cargar_texturas(nuevo->get_sprite());
    lista->push_back(nuevo);
    objetos.push_back(nuevo);
    objetos_activos++;
    DEBUGPRINT(" SPAWN "+std::to_string(objetos_activos))
    DEBUGCOOR(c);
    //    }
    //    l++;
    //}
    frames_maxim_ani=8;
};

void PlataformasSpawner::set_plat_posibles(Coordenadas c)
{
    c.x = c.x/32;
    c.y = c.y/32;
    bool in=false;
    for(auto &cc:plat_posibles) {
        if(cc.x==c.x && cc.y==c.y){
            in=true;
        }
    }
    if(!in) {
        plat_posibles.push_back(c);
    }
    /*plat_posibles.push_back({12, 9});
    lens.push_back(6);
    plat_posibles.push_back({26, 9});
    lens.push_back(6);
    plat_posibles.push_back({71, 11});
    lens.push_back(3);
    plat_posibles.push_back({82, 11});
    lens.push_back(3);
    plat_posibles.push_back({37, 13});
    lens.push_back(3);
    plat_posibles.push_back({59, 14});
    lens.push_back(4);
    plat_posibles.push_back({107, 15});
    lens.push_back(6);
    plat_posibles.push_back({25, 17});
    lens.push_back(6);
    plat_posibles.push_back({71, 19});
    lens.push_back(3);
    plat_posibles.push_back({95, 19});
    lens.push_back(6);
    plat_posibles.push_back({17, 21});
    lens.push_back(3);
    plat_posibles.push_back({79, 22});
    lens.push_back(3);
    plat_posibles.push_back({107, 24});
    lens.push_back(4);
    plat_posibles.push_back({11, 26});
    lens.push_back(3);
    plat_posibles.push_back({83, 26});
    lens.push_back(6);
    plat_posibles.push_back({59, 27});
    lens.push_back(4);
    plat_posibles.push_back({116, 27});
    lens.push_back(3);*/
}
void PlataformasSpawner::set_velocidad(int v)
{
    velocidad=v;
};

void PlataformasSpawner::despawn(std::vector<Objeto*>*lista)
{
    /*int id = lista->size()-objetos_activos;
    delete lista->at(id);
    objetos_activos--;
    lista->erase(std::next(lista->begin()+id-1));*/
};
void PlataformasSpawner::update(std::vector<Objeto*>*lista)
{
    double dt =  Tiempo::get_tiempo() - init_tiempo;
    
    /*for(auto &c: objetos) {
        c->get_sprite()->play_frame(0,frames_actual_ani%frames_maxim_ani);
        if(frame_dt>100)
        {
            frame_dt=0;
            frames_actual_ani++;
        }
    frame_dt++;
    }*/
    
    //DEBUGPRINT(dt)
    //DEBUGPRINT(past_tiempo)
    /*if((int)dt!=0 && ((int)dt)%delay == 0 && check==false && objetos_activos<72)
    {
        spawn(lista);
        past_tiempo=dt;
        check=true;
        DEBUGPRINT(std::to_string(dt)+" SPWAN "+std::to_string(objetos_activos))
        DEBUGCOOR(lista->at(lista->size()-1)->get_posicion_mundo());
    }*/
    
    /*if((int)dt!=0 && (int)dt%(delay*2) == 0 && !check)
    {
        DEBUGPRINT("DESPWAN")
        despawn(lista);
        //check=true;
    }*/
    
    //DEBUGPRINT(past_tiempo)
    //DEBUGPRINT((int)dt)
    /*if((int)dt>past_tiempo)
    {    
        check=false;
    }*/
};