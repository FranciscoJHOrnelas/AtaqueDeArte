#pragma once
#include "../objetos/Objeto.hpp"
#include "../utilidad/Estructuras.hpp"
#include "../motor/imagen/tiles.hpp"
#include <SDL.h>

class ObjetoEstatico : public Objeto
{
    public:
        virtual ~ObjetoEstatico(){};
        virtual void update(double dt) = 0;

};

class Plataformas : public ObjetoEstatico
{
    public:
        virtual ~Plataformas(){};
        Plataformas(int x, int y, int w, int h, SDL_Color color);
        Plataformas(TileInfo tiles,bool colision, bool columna);
        void update(double dt);
        
};

class HealthBar : public ObjetoEstatico
{
    public:
        virtual ~HealthBar(){};
        HealthBar(std::string sprite_path, int x, int y, int w, int h, int sw, int sh, SDL_Color c);
        void update(double dt);
        int frames_actual_ani;
        int frames_maxim_ani;
        int frame_dt{0};
};
