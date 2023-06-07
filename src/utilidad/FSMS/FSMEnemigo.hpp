#pragma once
#include "../../objetos_dinamicos/ObjetoDinamico.hpp"
//#include "../../motor/KeyOyente.hpp"
//#include "../../motor/MouseOyente.hpp"
#include<string>

class Enemigo; //forward declaration
class FSMEnemigo
{
    protected:
        std::string strnombre;
    
    public:
        virtual ~FSMEnemigo(){};
        virtual void entrar(Enemigo& enemy)=0;
        virtual void salir(Enemigo& enemy)=0;
        virtual void update(Enemigo& enemy,double dt)=0;
        std::string get_namestate()const{return strnombre;};
        
};

class EstadoEnemigoIDLE : public FSMEnemigo
{
    public:
        EstadoEnemigoIDLE();
        virtual ~EstadoEnemigoIDLE(){};
        void entrar(Enemigo& enemy);
        void salir(Enemigo& enemy);
        void update(Enemigo& enemy,double dt);
        
    private:
        int frames_actual_ani;
        int frames_maxim_ani;
        int frame_dt{0};


};

class EstadoEnemigoMOVER : public FSMEnemigo
{
    public:
        EstadoEnemigoMOVER(Coordenadas dir);
        virtual ~EstadoEnemigoMOVER(){};
        void entrar(Enemigo& enemy);
        void salir(Enemigo& enemy);
        void update(Enemigo& enemy,double dt);
        
    private:
        Coordenadas direccion;
        int velocidad;
        int frames_actual_ani;
        int frames_maxim_ani;
        int frame_dt{0};
};

class EstadoEnemigoDerrota : public FSMEnemigo
{
    public:
        EstadoEnemigoDerrota();
        virtual ~EstadoEnemigoDerrota(){};
        void entrar(Enemigo& enemy);
        void salir(Enemigo& enemy);
        void update(Enemigo& enemy,double dt);
        int get_recuperation_time()const{return recuperation_time;};
        void set_recuperation_time(int rt);
    private:
        int frames_actual_ani;
        int frames_maxim_ani;
        int frame_dt{0};
        int recuperation_time;
        double init_tiempo;
};
