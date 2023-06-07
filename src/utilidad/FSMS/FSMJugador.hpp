#pragma once
#include "../../objetos_dinamicos/ObjetoDinamico.hpp"
#include "../../motor/KeyOyente.hpp"
#include "../../motor/MouseOyente.hpp"
#include<string>

class Jugador; //forward declaration
class FSMJugador
{
    protected:
        std::string strnombre;
    
    public:
        virtual ~FSMJugador(){};
        virtual FSMJugador* input_handle(KeyOyente& input, MouseOyente& mouse)=0;
        virtual void entrar(Jugador& player)=0;
        virtual void salir(Jugador& player)=0;
        virtual void update(Jugador& player,double dt)=0;
        std::string get_namestate()const{return strnombre;};
    protected:
        int frames_actual_ani;
        int frames_maxim_ani;
        int frame_dt{0};
};

class EstadoJugadorIDLE : public FSMJugador
{
    public:
        EstadoJugadorIDLE();
        virtual ~EstadoJugadorIDLE(){};
        FSMJugador* input_handle(KeyOyente& input, MouseOyente& mouse);
        void entrar(Jugador& player);
        void salir(Jugador& player);
        void update(Jugador& player,double dt);
    private:
        int dir;

};

class EstadoJugadorIzquierda : public FSMJugador
{
    public:
        EstadoJugadorIzquierda();
        virtual ~EstadoJugadorIzquierda(){};
        FSMJugador* input_handle(KeyOyente& input, MouseOyente& mouse);
        void entrar(Jugador& player);
        void salir(Jugador& player);
        void update(Jugador& player,double dt);
};

class EstadoJugadorDerecha : public FSMJugador
{
    public:
        EstadoJugadorDerecha();
        virtual ~EstadoJugadorDerecha(){};
        FSMJugador* input_handle(KeyOyente& input, MouseOyente& mouse);
        void entrar(Jugador& player);
        void salir(Jugador& player);
        void update(Jugador& player,double dt);
};

class EstadoJugadorInitBrinco: public FSMJugador
{
    public:
        EstadoJugadorInitBrinco(int fbrinco);
        virtual ~EstadoJugadorInitBrinco(){};
        FSMJugador* input_handle(KeyOyente& input, MouseOyente& mouse);
        void entrar(Jugador& player);
        void salir(Jugador& player);
        void update(Jugador& player,double dt);

    private:
        Coordenadas fuerza;
        Coordenadas tope;
};

class EstadoJugadorSubeBrinco: public FSMJugador
{
    public:
        EstadoJugadorSubeBrinco(Coordenadas max);
        virtual ~EstadoJugadorSubeBrinco(){};
        FSMJugador* input_handle(KeyOyente& input, MouseOyente& mouse);
        void entrar(Jugador& player);
        void salir(Jugador& player);
        void update(Jugador& player,double dt);
    private:
        Coordenadas tope;
        Coordenadas P1,P2;
        Coordenadas temp;
        bool llego;
        int frames_actual;
        int frames_maximos;
};

class EstadoJugadorBajaBrinco: public FSMJugador
{
    public:
        EstadoJugadorBajaBrinco(Coordenadas tope);
        virtual ~EstadoJugadorBajaBrinco(){};
        FSMJugador* input_handle(KeyOyente& input, MouseOyente& mouse);
        void entrar(Jugador& player);
        void salir(Jugador& player);
        void update(Jugador& player,double dt);
    private:
        Coordenadas P1,P2;
        Coordenadas temp;
        bool llego;
        int frames_actual;
        int frames_maximos;
};

class EstadoJugadorLerpBrinco: public FSMJugador
{
    public:
        EstadoJugadorLerpBrinco(int max,int d);
        virtual ~EstadoJugadorLerpBrinco(){};
        FSMJugador* input_handle(KeyOyente& input, MouseOyente& mouse);
        void entrar(Jugador& player);
        void salir(Jugador& player);
        void update(Jugador& player,double dt);
    private:
        Coordenadas P1,P2,P3,temp;
        int pmax;
        bool en_aire;
        int dir;

        int frame_actual;
        int frame_maximo;
};

//GRAVEDAD
class EstadoJugadorGravedadBrinco: public FSMJugador
{
    public:
        EstadoJugadorGravedadBrinco(int fuerza);
        virtual ~EstadoJugadorGravedadBrinco(){};
        FSMJugador* input_handle(KeyOyente& input, MouseOyente& mouse);
        void entrar(Jugador& player);
        void salir(Jugador& player);
        void update(Jugador& player,double dt);
    private:
        int f;
        Coordenadas P1;
        bool en_aire;
        bool flip;
        bool en_colision;
};

class EstadoJugadorAtacar: public FSMJugador
{
    public:
        EstadoJugadorAtacar();
        virtual ~EstadoJugadorAtacar(){};
        FSMJugador* input_handle(KeyOyente& input, MouseOyente& mouse);
        void entrar(Jugador& player);
        void salir(Jugador& player);
        void update(Jugador& player,double dt);
    private:
        int dir;
        bool anim_comp;
};

class EstadoJugadorDerrota : public FSMJugador
{
    public:
        EstadoJugadorDerrota();
        virtual ~EstadoJugadorDerrota(){};
        FSMJugador* input_handle(KeyOyente& input, MouseOyente& mouse);
        void entrar(Jugador& player);
        void salir(Jugador& player);
        void update(Jugador& player,double dt);
};

class EstadoJugadorVictoria : public FSMJugador
{
    public:
        EstadoJugadorVictoria();
        virtual ~EstadoJugadorVictoria(){};
        FSMJugador* input_handle(KeyOyente& input, MouseOyente& mouse);
        void entrar(Jugador& player);
        void salir(Jugador& player);
        void update(Jugador& player,double dt);
};