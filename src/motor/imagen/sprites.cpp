#include "sprites.hpp"
#include "../../utilidad/Func_aux.hpp"
#include <iostream>

Sprite::Sprite(std::string file,Coordenadas pos, int w, int h, int sw, int sh)
{
    width = w;
    height = h;
    path = file;
    //SDL_Rect A. --- .B 
    //         c.     .d
    sprite_select ={0,0,width,height};
    sprite_frame = {pos.x,pos.y,width,height};
    sprite_size={sw,sh};
    estado_animacion = nullptr;
};


Sprite::~Sprite()
{
    SDL_DestroyTexture(hoja);
    //delete cuando sea del tipo FSMAnimacionIdle
    //delete estado_animacion;
};

bool Sprite::cargar_textura(SDL_Renderer* r)
{
    hoja = IMG_LoadTexture(r,path.c_str());
    if(!hoja)
    {
        std::cout<<SDL_GetError()<<"\n";
        return false;
    }
    return true;
};

void Sprite::set_sprite_position(Coordenadas p)
{
    
    sprite_frame.x = p.x;//-(sprite_size.x/2);
    sprite_frame.y = p.y;//-(sprite_size.y/2);
}

Coordenadas Sprite::get_sprite_position()
{
    return {sprite_frame.x,sprite_frame.y};
};

void Sprite::play_frame(int row, int frame)
{
    sprite_select.x = row+(width*frame);
    sprite_select.y = row*height; //(0,0) -> (500,500) etc..
    sprite_select.w = width;
    sprite_select.h = height;
};

void Sprite::set_estado_animacion(void *estado)
{
    /*
    estado_animacion->salir(*this);
    delete estado_animacion;
    estado_animacion = estado;
    estado_animacion->entrar(*this);
    */
};