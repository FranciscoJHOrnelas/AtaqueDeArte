#pragma once
#include "../../motor/KeyOyente.hpp"
#include "../../motor/MouseOyente.hpp"
#include "../../motor/camaras/Camaras.hpp"
#include "../../objetos_dinamicos/ObjetoDinamico.hpp"
#include "../Estructuras.hpp"
#include<string>

class Camara; //declaración circular

class FSMCamara
{
    protected:
        float vel;
        Coordenadas pos_final;
        Coordenadas pos_inicial;
        int frames_actual;
        int frames_maximo;
        bool is_pressed{0};
    public:
        std::string strestado;
    public:
        virtual ~FSMCamara(){};
        virtual FSMCamara* input_handle(Camara &cam, Jugador &obj, KeyOyente &keys, MouseOyente& mouse)=0;
        virtual void on_entrar(Camara &cam)=0;
        virtual void on_salir(Camara &cam)=0;
        virtual void on_update(Camara &cam)=0;
};

class EstadoCamaraMover : public FSMCamara
{
    public:
        EstadoCamaraMover(Coordenadas dir);
        virtual ~EstadoCamaraMover(){};
        FSMCamara* input_handle(Camara &cam,Jugador &obj, KeyOyente &keys, MouseOyente& mouse);
        void on_entrar(Camara &cam);
        void on_salir(Camara &cam);
        void on_update(Camara &cam);
    private:
        Coordenadas direccion;
        Jugador* obj;

};

class EstadoCamaraMoverDer : public FSMCamara
{
    public:
        EstadoCamaraMoverDer();
        virtual ~EstadoCamaraMoverDer(){};
        FSMCamara* input_handle(Camara &cam,Jugador &obj, KeyOyente &keys, MouseOyente& mouse);
        void on_entrar(Camara &cam);
        void on_salir(Camara &cam);
        void on_update(Camara &cam);
    private:
        Coordenadas direccion;
        Jugador* obj;

};

class EstadoCamaraMoverIzq : public FSMCamara
{
    public:
        EstadoCamaraMoverIzq();
        virtual ~EstadoCamaraMoverIzq(){};
        FSMCamara* input_handle(Camara &cam,Jugador &obj, KeyOyente &keys, MouseOyente& mouse);
        void on_entrar(Camara &cam);
        void on_salir(Camara &cam);
        void on_update(Camara &cam);
    private:
        Coordenadas direccion;
        Jugador* obj;

};



//EJEMPLO DE LERP
class EstadoCamaraTransicion : public FSMCamara
{
    public:
        EstadoCamaraTransicion();
        virtual ~EstadoCamaraTransicion(){};
        FSMCamara* input_handle(Camara &cam, Jugador &obj, KeyOyente &keys, MouseOyente& mouse);
        void on_entrar(Camara &cam);
        void on_salir(Camara &cam);
        void on_update(Camara &cam);
    private:
        Coordenadas check;
        Coordenadas ant_check;
        Coordenadas centro;
        float distancia;
};

class EstadoCamaraLock : public FSMCamara
{
    public:
        EstadoCamaraLock(Objeto &objlock);
        virtual ~EstadoCamaraLock(){};
        FSMCamara* input_handle(Camara &cam, Jugador &obj, KeyOyente &keys, MouseOyente& mouse);
        void on_entrar(Camara &cam);
        void on_salir(Camara &cam);
        void on_update(Camara &cam);
    private:
        Coordenadas centro;
        Objeto *obj;
        bool locked;

};