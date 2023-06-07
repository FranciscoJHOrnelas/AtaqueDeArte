#pragma once
#include "../objetos/Objeto.hpp"
#include "../motor/KeyOyente.hpp"
#include "../motor/MouseOyente.hpp"
#include "../motor/Pipeline.hpp"

#include "../utilidad/FSMS/FSMJugador.hpp"
#include "../utilidad/FSMS/FSMEnemigo.hpp"
#include<SDL.h>
#include<string>
class FSMJugador;//forward declaration
class FSMEnemigo;

class ObjetoDinamico : public Objeto
{
    public:
        virtual ~ObjetoDinamico(){};
        virtual void update(double dt)=0;
        float get_dtgf()const{return dtgravedad;};
        void set_dtgf(float v){dtgravedad=v;};

        //forma general de convertir cualquier puntero si se conoce la clase
        virtual void set_estado(void* estado)=0;
        virtual void* get_estado()=0;

        Coordenadas offsetoverlap;
        void lock_objeto(Objeto &obj);
        //void unluck_objeto();
        Objeto* get_obj_lock()const{return objeto_seguir;};
    protected:
        float dtgravedad{0};
        Objeto* objeto_seguir;
        
};


class Jugador : public ObjetoDinamico
{
    public:
        virtual ~Jugador(){};
        Jugador(float vida, int x, int y,SDL_Color c );
        Jugador(std::string path_sprite,float vida, int x, int y, int w, int h,int sw,int sh, SDL_Color c );
        void update(double dt);
        void input_handle(KeyOyente& input,MouseOyente& mouse);
        std::string get_strEstado();
        Coordenadas get_piso()const{return piso;};
        void set_piso(Coordenadas p){piso = p;};

        void set_estado(void* estado);
        void* get_estado();

        std::vector<Objeto*> get_objetos_mundo()const{return *objetos;};
        void set_objetos_mundo(std::vector<Objeto*> &o){objetos = &o;};
        Figura* get_col_piso()const{return col_piso;};
        bool get_onPlataforma()const{return en_plataforma;};
        void set_onPlataforma(bool v);
        Coordenadas get_centro() const{return centro;};
        int kills;

    private:
        FSMJugador* estado_actual;
        Coordenadas piso;
        std::vector<Objeto*> *objetos;
        Figura *col_piso;
        bool en_plataforma{false};
        Coordenadas centro;
};

class BackGroundDinamico : public ObjetoDinamico
{
    public:
        BackGroundDinamico(std::string path_sprite,int w, int h);
        virtual ~BackGroundDinamico(){};
        void update(double dt);
        void set_estado(void* estado);
        void* get_estado();
    private:
        Coordenadas limites;
        Coordenadas centro_cam;
};

class PlataformasDinamicas : public ObjetoDinamico
{
    public:
        PlataformasDinamicas(std::string sprite_path, int x, int y, int w, int h,int sw, int sh,SDL_Color c);
        virtual ~PlataformasDinamicas(){};
        void update(double dt);
        void set_estado(void* estado);
        void* get_estado();
        int frames_actual_ani;
        int frames_maxim_ani;
        int frame_dt{0};

};

class Enemigo : public ObjetoDinamico
{
    public:
        virtual ~Enemigo(){};
        Enemigo(float vida, int x, int y, SDL_Color c);
        Enemigo(std::string path_sprite, float vida, int x, int y, int w, int h, int sw, int sh, SDL_Color c);
        void update(double dt);
        std::string get_strEstado();
        void set_estado(void *estado);
        void* get_estado();
        void set_velocidad(int v);
        void lock_objeto(Objeto &obj);
    private:
        FSMEnemigo* estado_actual;
};

class Protector : public ObjetoDinamico{
    public:
        virtual ~Protector(){};
        Protector(std::string path_sprite, int x, int y, int w, int h, int sw, int sh, SDL_Color c);
        void update(double dt);
        void set_estado(void *estado);
        void* get_estado();
        void set_faa(int faa);
        int frames_actual_ani;
        int frames_maxim_ani;
        int frame_dt{0};
};

class Meta : public ObjetoDinamico
{
    public:
        Meta(std::string sprite_path, int x, int y, int w, int h, int sw, int sh, SDL_Color c);
        virtual ~Meta(){};
        void update(double dt);
        void set_estado(void *estado);
        void* get_estado();
        int frames_actual_ani;
        int frames_maxim_ani;
        int frame_dt{0};
        void del_col_box();
};