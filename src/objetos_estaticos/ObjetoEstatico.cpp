#include "ObjetoEstatico.hpp"

Plataformas::Plataformas(int x, int y, int w, int h, SDL_Color color)
:ObjetoEstatico{} //llamar constructor padre
{
    posicion_mundo.x=x;
    posicion_mundo.y=y;
    avatar = new Rectangulo(x,y,w,h,color);
    avatar->set_rellenocolor(color);
    avatar->set_serellena(false);
    col_box = new Rectangulo(x,y,w+5,h+5,{0,255,0,255});
    tiene_fisica=true;
    en_colision=false;
    sprite= nullptr;
    tile = nullptr;
};

Plataformas::Plataformas(TileInfo tiles, bool colision, bool columna)
{
    avatar=nullptr;
    posicion_mundo = tiles.pos;
    tile = new Tile(tiles);
    area_plataforma=nullptr;
    col_box=nullptr;
    if(colision)
    {
        col_box = new Rectangulo(posicion_mundo.x,posicion_mundo.y,tiles.width,tiles.height,{0,255,0,255});
        render_colbox=true;
         //col_box->set_serellena(true);
        area_plataforma = new Rectangulo(posicion_mundo.x-10-tile->get_sizes().x/2,posicion_mundo.y-tiles.height/2,tiles.width+20,5,{0,0,255,255});
        trigger_box = new Rectangulo(posicion_mundo.x-tile->get_sizes().x/2,posicion_mundo.y-tiles.height/2,tiles.width,5,{0,0,255,255});
    }
    if(columna){
        col_box = new Rectangulo(posicion_mundo.x,posicion_mundo.y,tiles.width,tiles.height,{0,255,0,255});
    }
    tiene_fisica=colision;
    en_colision=false;

    sprite = nullptr;
    
};

void Plataformas::update(double dt)
{
    if(col_box!=nullptr)
        col_box->update_vertices();
    if(area_plataforma!=nullptr)
    {
        area_plataforma->set_position(posicion_camara.x-10-tile->get_sizes().x/2,posicion_camara.y-tile->get_sizes().y/2);
        area_plataforma->update_vertices();
    }
};

HealthBar::HealthBar(std::string sprite_path, int x, int y, int w, int h, int sw, int sh, SDL_Color c){
    posicion_mundo.x=x;
    posicion_mundo.y=y;
    avatar = new Rectangulo(x,y,sw,sh,c);
    c.a=150;
    color = c;
    avatar->set_rellenocolor(c);
    avatar->set_serellena(true);
    col_box=nullptr;
    area_plataforma=nullptr;
    sprite = new Sprite(sprite_path,posicion_mundo,w,h,sw,sh);
    tile = nullptr;
    frames_actual_ani=0;
    frames_maxim_ani=7;
};

void HealthBar::update(double dt) {
    get_sprite()->play_frame(0,frames_actual_ani%frames_maxim_ani);
    if(frame_dt>7)
    {
        frame_dt=0;
        frames_actual_ani++;
    }
    frame_dt++;
};