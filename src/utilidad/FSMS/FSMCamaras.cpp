#include "FSMCamaras.hpp"
#include "../Func_aux.hpp"
#include <iostream>
#include <cmath>

EstadoCamaraMover::EstadoCamaraMover(Coordenadas dir)
{
    direccion = dir;
    strestado = "mover";
};

FSMCamara* EstadoCamaraMover::input_handle(Camara &cam, Jugador &obj, KeyOyente &keys, MouseOyente& mouse)
{
    if(obj.get_strEstado()!="DERROTADO" && obj.get_strEstado()!="ATACAR" && obj.get_strEstado()!="VICTORIA") {
        if(keys.estaPresionado(SDL_SCANCODE_A))
        {
            return new EstadoCamaraMoverIzq();
        }
        if(keys.estaPresionado(SDL_SCANCODE_D))
        {
            return new EstadoCamaraMoverDer();
        }
    }
    

    /*if(!(keys.estaPresionado(SDL_SCANCODE_A)) && !(keys.estaPresionado(SDL_SCANCODE_D)))
    {
        return new EstadoCamaraMover({0,0});
    }*/
    
    return NULL;
};

void EstadoCamaraMover::on_entrar(Camara &cam)
{
    //Alumnos implementa
    vel =0;// cam.velocidad;
    obj = dynamic_cast<Jugador*>(cam.get_obj_lock());
    
    FSMJugador* e = (FSMJugador*)obj->get_estado();
    //if(e->get_namestate() != "BRINCAR")
        //obj->set_estado(new EstadoJugadorIDLE());
    
};
void EstadoCamaraMover::on_salir(Camara &cam){};
void EstadoCamaraMover::on_update(Camara &cam){};

/*DER*/
EstadoCamaraMoverDer::EstadoCamaraMoverDer()
{
    strestado = "mover";
};

FSMCamara* EstadoCamaraMoverDer::input_handle(Camara &cam, Jugador &obj, KeyOyente &keys, MouseOyente& mouse)
{
    
    if(keys.estaPresionado(SDL_SCANCODE_A))
    {
        return new EstadoCamaraMoverIzq();
    }

    if(!(keys.estaPresionado(SDL_SCANCODE_A)) && !(keys.estaPresionado(SDL_SCANCODE_D)))
    {
        return new EstadoCamaraMover({0,0});
    }
    
    return NULL;
};

void EstadoCamaraMoverDer::on_entrar(Camara &cam)
{
    vel = cam.velocidad;
    obj = dynamic_cast<Jugador*>(cam.get_obj_lock());
    FSMJugador* e = (FSMJugador*)obj->get_estado();
    if(e->get_namestate() != "BRINCAR")
        obj->set_estado(new EstadoJugadorDerecha());
    
};
void EstadoCamaraMoverDer::on_salir(Camara &cam){};
void EstadoCamaraMoverDer::on_update(Camara &cam)
{
    //Coordenadas pos = cam.get_posicion_mundo();
    Coordenadas obpos = obj->get_posicion_mundo();

    FSMJugador * estado = (FSMJugador*)obj->get_estado();
    if(estado->get_namestate() != "BRINCAR")
    {
        //pos.x += obj->get_velocidad();
        obpos.x+=obj->get_velocidad();

        
    }
    for(auto& o: obj->get_objetos_mundo())
    {
        if(o==obj) //si el elemnto es igual al player omitir 
            continue;
        Coordenadas po = o->get_posicion_mundo();
        if(o->get_velocidad()==2) po.x-=o->get_velocidad();
        o->set_posicion_mundo(po);
    }
    
    //cam.set_posicion_mundo(pos);//mover la cámara con jugador
    obj->set_posicion_mundo(obpos);

};

/*IZQ*/

EstadoCamaraMoverIzq::EstadoCamaraMoverIzq()
{
    strestado = "mover";
};

FSMCamara* EstadoCamaraMoverIzq::input_handle(Camara &cam, Jugador &obj, KeyOyente &keys, MouseOyente& mouse)
{
    

    if(keys.estaPresionado(SDL_SCANCODE_D))
    {
        return new EstadoCamaraMoverDer();
    }

    if(!(keys.estaPresionado(SDL_SCANCODE_A)) && !(keys.estaPresionado(SDL_SCANCODE_D)))
    {
        return new EstadoCamaraMover({0,0});
    }
    
    return NULL;
};

void EstadoCamaraMoverIzq::on_entrar(Camara &cam)
{
    vel = cam.velocidad;
    obj = dynamic_cast<Jugador*>(cam.get_obj_lock());
    FSMJugador* e = (FSMJugador*)obj->get_estado();
    if(e->get_namestate() != "BRINCAR")
        obj->set_estado(new EstadoJugadorIzquierda());
    
};
void EstadoCamaraMoverIzq::on_salir(Camara &cam){};
void EstadoCamaraMoverIzq::on_update(Camara &cam)
{
    //Coordenadas pos = cam.get_posicion_mundo();
    Coordenadas obpos = obj->get_posicion_mundo();
    
    FSMJugador* e = (FSMJugador*)obj->get_estado();
    if(e->get_namestate() != "BRINCAR")
    {
        //pos.x  -= obj->get_velocidad();
        obpos.x-= obj->get_velocidad();

        
    }

    for(auto& o : obj->get_objetos_mundo())
    {
        if(o==obj)//si el elemnto es igual al player omitir 
            continue;
        Coordenadas po = o->get_posicion_mundo();
        if(o->get_velocidad()==2) po.x+=o->get_velocidad();
        o->set_posicion_mundo(po);
    }
    
    //cam.set_posicion_mundo(pos); //mover la cámara con jugador
    obj->set_posicion_mundo(obpos);

};

/*
TRANSICIÓN
*/
EstadoCamaraTransicion::EstadoCamaraTransicion()
{
    frames_actual =0;
    frames_maximo = 15;
    strestado = "transicion";
};

FSMCamara* EstadoCamaraTransicion::input_handle(Camara &cam, Jugador &obj, KeyOyente &keys, MouseOyente& mouse)
{
    if(frames_actual>frames_maximo)
    {
        /*if(distancia<10)
        {
            return new EstadoCamaraLock(*cam.get_obj_lock());
        }*/
        return new EstadoCamaraMover({0,0});//new EstadoCamaraMover({0,0});
    }

    return NULL;
};

void EstadoCamaraTransicion::on_entrar(Camara &cam)
{
    //Alumnos implementa
    cam.en_transicion = 1;

    vel = cam.velocidad;
    pos_inicial = cam.get_posicion_mundo();//(0,0) 

    //seguro decir que este estado siempre entra en lock object
    // no es necesario comprobar que lock object no es nulo
    pos_final = cam.get_obj_lock()->get_posicion_mundo(); //o.x,o.y
    
    //convertir en posicion relativo a las coordenads camara
    centro = cam.get_posicion_centro();
    pos_final.x -= centro.x;
    pos_final.y -= centro.y;

    //la diferencia es la cantidad de pixeles que se mueve
    ant_check={0,0};

    //checkar la distancia es pequeña para ajustar los frames
    distancia = std::sqrt(std::pow(centro.x-(pos_final.x+centro.x),2)+std::pow(centro.y-(pos_final.y+centro.y),2));
    float lim = cam.get_obj_lock()->get_avatar()->get_vertices()[3].x - cam.get_obj_lock()->get_avatar()->get_vertices()[0].x;
    if(distancia<lim)
    {
        //std::cout<<"LIM "<<lim<<"\n";
        frames_maximo = 10;
    }
};
void EstadoCamaraTransicion::on_salir(Camara &cam)
{
    //Alumnos implementa
    cam.en_transicion=0;
};
void EstadoCamaraTransicion::on_update(Camara &cam)
{

    //Alumnos implementa
    if(frames_actual>frames_maximo)
        return;

    float t = frames_actual/(float)frames_maximo;
    check= LERP(pos_inicial,pos_final,t);
    cam.set_posicion_mundo(check);
    frames_actual++;
    //DEBUGCOOR(cam.get_posicion_mundo())
    


};
/*
LOCK
*/
EstadoCamaraLock::EstadoCamaraLock(Objeto &objlock)
{
    strestado = "lock";
    obj = &objlock;

};

FSMCamara* EstadoCamaraLock::input_handle(Camara &cam, Jugador &obj, KeyOyente &keys, MouseOyente& mouse)
{   
    
    if(!cam.lock)
    {
        return new EstadoCamaraTransicion();
    }

    return NULL;
};

void EstadoCamaraLock::on_entrar(Camara &cam)
{
    //Alumnos implementa
    centro = cam.get_posicion_centro();
    
    
};
void EstadoCamaraLock::on_salir(Camara &cam)
{
    //Alumnos implementa
};
void EstadoCamaraLock::on_update(Camara &cam)
{
    //Alumnos implementa

    // simplemente la camara obtiene la posición del obj
    Coordenadas pos_mundo = obj->get_posicion_mundo();
    pos_mundo.x -= centro.x;
    pos_mundo.y -= centro.y;
    cam.set_posicion_mundo(pos_mundo);
    printf("LOCKCAMArA\n");
    DEBUGLINEA(pos_mundo,obj->get_posicion_mundo())

};