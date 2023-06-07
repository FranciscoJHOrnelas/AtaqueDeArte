#include "Camaras.hpp"
#include "../../utilidad/Func_aux.hpp"
#include "../../objetos_dinamicos/ObjetoDinamico.hpp"
#include <iostream>

Camara::Camara(int x, int y, int w, int h, SDL_Renderer &view)
{
    // se considera que la camara empieza en 0,0 y es toda la pantalla
    // si se quiere centrar hay que desplazar w y h en todos los cálculos.
    width =w;
    height=h;
    pos_mundo = {x,y};
    pos_centro = {x+w/2,y+h/2};

    cruz.push_back({pos_centro.x-(w/4),y+h/2});
    cruz.push_back({pos_centro.x+(w/4),y+h/2});
    cruz.push_back({pos_centro.x+(w/4),y});
    cruz.push_back({pos_centro.x+(w/4),y+h});

    limder.push_back({pos_centro.x+(w/4),300});
    limder.push_back({pos_centro.x+(w/4),600});

    limizq.push_back({pos_centro.x-(w/4),300});
    limizq.push_back({pos_centro.x-(w/4),600});

    limsup.push_back({pos_centro.x-(w/4),250});
    limsup.push_back({pos_centro.x+(w/4),250});

    liminf.push_back({pos_centro.x-(w/4),650});
    liminf.push_back({pos_centro.x+(w/4),650});
    
    viewport = &view;
    objensamble = new Pipeline(view);
    velocidad =0;
    estado_actual = new EstadoCamaraMover({0,0});
};

Camara::~Camara()
{
    SDL_DestroyRenderer(viewport);
    delete objensamble;
    delete estado_actual;
    delete objeto_seguir;
};

void Camara::render_cross()
{
    //objensamble->lineas(cruz,{0,255,0,180});
    objensamble->lineas(limder,{0,0,0,180});
    objensamble->lineas(limizq,{0,0,0,180});
    objensamble->lineas(limsup,{0,0,0,180});
    objensamble->lineas(liminf,{0,0,0,180});
};

void Camara::lock_objeto(Objeto &obj)
{
    //Alumnos implementarla
    objeto_seguir = dynamic_cast<Jugador*>(&obj);
    lock =1;
    
};

void Camara::unluck_objeto()
{
    //Alumnos implementarla
    objeto_seguir = nullptr;
    lock=0;
};

void Camara::proyectar(std::vector<Objeto*> objetos)
{
    
    for(auto &o:objetos)
    {
        Coordenadas posM = o->get_posicion_mundo();
        posM.x -= pos_mundo.x;
        posM.y -= pos_mundo.y;
        //o->set_posicion_mundo(posM);
        o->set_posicion_camara(posM);

    }
    //DEBUGPRINT("_____")
    
};

void Camara::renderizar(std::vector<Objeto*>objetos)
{
    for(auto& o:objetos)
    {
        o->render(viewport);
    }
};

void Camara::set_posicion_mundo(Coordenadas np)
{
    pos_mundo = np;
};
void Camara::set_posicion_centro(Coordenadas np)
{
    pos_centro = np;
};
void Camara::input_handle(KeyOyente &input, MouseOyente &mouse)
{
    if(!estado_actual)
        return; //estado nulo al inicio
    
    FSMCamara* estado = estado_actual->input_handle(*this,*objeto_seguir,input,mouse);
    if(estado)
    {
        set_estado(estado);
    }
};
void Camara::set_estado(void* nuevo)
{
    estado_actual->on_salir(*this);
    delete estado_actual;
    estado_actual = (FSMCamara*)nuevo;
    estado_actual->on_entrar(*this);
};
void Camara::update()
{
    if(estado_actual)
        estado_actual->on_update(*this);
    FSMCamara*e = (FSMCamara*)get_estado();
    if(objeto_seguir->get_centro().x<(pos_centro.x/2))
    {
        if(e->strestado!="transicion")
            set_estado(new EstadoCamaraTransicion());
    }

    if(objeto_seguir->get_centro().x>pos_centro.x+(pos_centro.x/2))
    {
        if(e->strestado!="transicion")
            set_estado(new EstadoCamaraTransicion());
    }

    if(objeto_seguir->get_centro().x<(pos_centro.x/2))
    {
        if(e->strestado!="transicion")
            set_estado(new EstadoCamaraTransicion());
    }

    if(objeto_seguir->get_centro().y>450)
    {
        if(e->strestado!="transicion")
            set_estado(new EstadoCamaraTransicion());
    }

    if(objeto_seguir->get_centro().y<250)
    {
        if(e->strestado!="transicion")
            set_estado(new EstadoCamaraTransicion());
    }

    /*if(pos_mundo.x<0)
        pos_mundo.x=0;
    if(pos_mundo.x>width)
        pos_mundo.x=width;
    if(pos_mundo.y<0)
        pos_mundo.y=0;
    if(pos_mundo.y>height)
        pos_mundo.y=height;*/
};