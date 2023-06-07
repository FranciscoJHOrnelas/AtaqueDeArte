#pragma once
#include "../../utilidad/Estructuras.hpp"
#include "../../objetos/Objeto.hpp"
#include "../Pipeline.hpp"
#include "../KeyOyente.hpp"
#include "../MouseOyente.hpp"
#include "../../utilidad/FSMS/FSMCamaras.hpp"
#include "../../objetos_dinamicos/ObjetoDinamico.hpp"

#include <SDL.h>
#include <vector>

class FSMCamara; //declaración circular

class Camara
{
    public:
        Camara(int x, int y, int w, int h, SDL_Renderer &view);
        ~Camara();
        void renderizar(std::vector<Objeto*> objetos);
        void proyectar(std::vector<Objeto*> objetos);
        void render_cross();

        Coordenadas get_posicion_mundo() const {return pos_mundo;};
        void set_posicion_mundo(Coordenadas np);
        void set_posicion_centro(Coordenadas np);
        Coordenadas get_posicion_centro() const{return pos_centro;};

        void lock_objeto(Objeto &obj);
        void unluck_objeto();
        Objeto* get_obj_lock()const{return objeto_seguir;};

        /*FSM métodos*/
        void input_handle(KeyOyente &input, MouseOyente &mouse);
        void update();
        void set_estado(void* nuevo);
        void *get_estado()const{return estado_actual;};
    
    public:
        int lock{0};
        float velocidad;
        int en_transicion{0};
    
    private:
        int width;
        int height;
        Coordenadas pos_mundo;
        Coordenadas pos_centro;
        SDL_Renderer *viewport;
        Pipeline * objensamble;
        std::vector<Coordenadas> cruz;
        std::vector<Coordenadas> limder;
        std::vector<Coordenadas> limizq;
        std::vector<Coordenadas> limsup;
        std::vector<Coordenadas> liminf;
        Jugador* objeto_seguir;
        FSMCamara* estado_actual;
};