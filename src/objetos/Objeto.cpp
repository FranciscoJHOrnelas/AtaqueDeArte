#include "Objeto.hpp"
#include "../motor/Pipeline.hpp"
#include <iostream>
#include "../utilidad/Func_aux.hpp"

void Objeto::render(SDL_Renderer* r)
{

    Pipeline temp(*r);
    if(!sprite && !tile)
    {    
        temp.figuras(avatar);
    }  
    else if(!sprite && tile)
    {
        temp.pintar_texturas(tile);
    }
    else 
    {
        temp.pintar_texturas(sprite);
        
    }
    
    /*if(render_colbox && col_box)
    {
        //printf("[%p] pintar col_box\n",this);
        temp.figuras(col_box);
    }*/

    /*if(area_plataforma)
    {
        //printf("[%p] pintar area_plataforma\n",this);
        temp.figuras(area_plataforma);
    }*/

    
};

void Objeto::set_posicion_camara(Coordenadas p)
{
    posicion_camara = p;
    
    if(avatar)
    {
        avatar->set_position(p.x,p.y);
        avatar->update_vertices();
    }
    
    if(sprite)
    {    
        sprite->set_sprite_position(p);
        if(col_box)
        {
            col_box->set_position(p.x,p.y);
            col_box->update_vertices();
        }
    }

    if(tile)
    {    
        tile->get_dst()->x=p.x-tile->get_sizes().x/2;
        tile->get_dst()->y=p.y-tile->get_sizes().y/2;
        if(col_box)
        {
            col_box->set_position(p.x-tile->get_sizes().x/2,p.y-tile->get_sizes().y/2);
            col_box->update_vertices();
        }
    }
    
};

void Objeto::set_posicion_mundo(Coordenadas p)
{
    this->posicion_mundo =p;
    
};
void Objeto::set_posx(int x)
{
    posicion_mundo.x=x;
    set_posicion_mundo(posicion_mundo);
};
void Objeto::set_posy(int y)
{
    posicion_mundo.y=y;
    set_posicion_mundo(posicion_mundo);
};