#pragma once
#include "../../objetos/Objeto.hpp"
#include "../../motor/Pipeline.hpp"
#include <SDL.h>

class PlataformasSpawner
{
    public:
        PlataformasSpawner(std::string sprite_path, int x, int y, int w, int h,int sw, int sh,SDL_Color c,Pipeline &p);

        void spawn(std::vector<Objeto*>*lista, Coordenadas c);
        void despawn(std::vector<Objeto*>*lista);
        void update(std::vector<Objeto*>*lista);
        void set_velocidad(int v);

        std::vector<Coordenadas> get_plat_posibles(){return plat_posibles;};
        void set_plat_posibles(Coordenadas c);
        std::vector<int> get_lens(){return lens;};
        void set_lens(std::vector<int>);

    private:
        std::vector<Objeto*> objetos;
        std::string sprite_path;
        SDL_Color colordebug;
        int x; // pos x
        int y; // pos y
        int w; // ancho sprite
        int h; // alto sprite
        int sw;// ancho mostrar sprite
        int sh;// alto mostrar sprite
        int objetos_activos;
        int delay;
        double init_tiempo;
        int past_tiempo;
        bool check;
        Pipeline *pipeline;
        int velocidad;
        std::vector<Coordenadas> plat_posibles;
        std::vector<int> lens;
        int frames_actual_ani{0};
        int frames_maxim_ani;
        int frame_dt{0};
        //int[17] lens = {6,6,3,3,3,4,6,6,3,6,3,3,4,3,6,4,3};

};