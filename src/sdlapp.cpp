#include "sdlapp.hpp"
#include "utilidad/Tiempo.hpp"
#include "utilidad/Estructuras.hpp"
#include "utilidad/Func_aux.hpp"
#include "motor/KeyOyente.hpp"
#include "motor/MouseOyente.hpp"
#include <SDL_ttf.h>
#include "utilidad/RenderTexto.hpp"
#include "motor/fisica/MotorFisico.hpp"
#include "motor/Plotter.hpp"
#include "utilidad/FSMS/FSMJugador.hpp"
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <time.h>

//#define RECTANGULOS
//#define TRIANGULOS
//#define CIRCULOS

SDLApp* SDLApp::instancia = 0;

SDLApp::SDLApp()
{
    vnt = nullptr;
    vntsurf = nullptr;
    render = nullptr;
    ensamble= nullptr;
    bg_color.r = 255; //rojo
    bg_color.g = 255; //verde
    bg_color.b = 255; //azul
    bg_color.a = 255; //alpha

    esta_corriendo = true;
    WIDTH=1024;
    HEIGHT=720;
    maxFPS=60;
    fps=0;

    
};

//Como solo queremos una instancia del esta clase, entonces se hace una clase singleton
SDLApp& SDLApp::get()
{
    if(!instancia)
        instancia = new SDLApp();
    return *instancia;
};

bool SDLApp::on_init()
{
    srand (time(NULL));
    //revisar que SDL inicio bien
    if(SDL_Init(SDL_INIT_EVERYTHING)<0){return false;};
    //crear la ventana
    get().vnt = SDL_CreateWindow(
        "Juego",                // Titulo de la ventana
        SDL_WINDOWPOS_UNDEFINED,// posicion X de la pantalla
        SDL_WINDOWPOS_UNDEFINED,// posicion Y de la pantalla
        get().WIDTH,            // ancho de la ventana
        get().HEIGHT,           // alto de la ventana
        SDL_WINDOW_OPENGL);     // que use OPENGL
    
    TTF_Init();
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    //revisar que se creo bien la ventana
    if(get().vnt == NULL)
    {
        printf("No se creo la ventana por: %s",SDL_GetError());
        return false;
    }
    //la forma de procesar en GPU
    SDL_SetHint(SDL_HINT_RENDER_BATCHING,"opengl");
    //creamos el 'canvas'
    get().render = SDL_CreateRenderer(
        get().vnt,                  // la ventana
        -1,                         // driver
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);  // flags driver
    //revisamos si se creo correctamente
    if(get().render == NULL)
    {
        printf("No se creo el renderer por: %s",SDL_GetError());
        return false; 
    }

    Plotter::get().set_renderer(*get().render);
    // si se creo correcto lo agregamos al Pipeline
    get().ensamble = new Pipeline(*get().render);

    //06_Camaras

    get().camara_principal = new Camara(0,0,get().WIDTH,get().HEIGHT,*get().render);
    
    ManejadorCamaras::get().set_camara(*get().camara_principal);

    platspawn = new PlataformasSpawner(
        "assets/sprites/mundo/bg/Painted_Tiles.png",
        11,9,256,256,32,32,{0,255,0,255},
        *get().ensamble
    );
    
    platspawn->set_velocidad(5);
    //platspawn->set_plat_posibles();
    //08 tiles
    mapa = new Atlas("assets/sprites/mundo/ids/paint_ids.txt");
    mapa->generar_mapa(get().render,2,1);
    //05
    player = new Jugador("assets/sprites/heroe/Player_sheet.png",
                //      hp , x , y, sW,sH , vW,vH ,color
                        5, 120, 900, 512, 512, 64, 64, {255,0,255,255});
    get().ensamble->cargar_texturas(player->get_sprite());
    player->set_velocidad(6);
    printf("Se creo el player\n");

    plataformas = mapa->get_objetos_fisicos();

    /*PlataformasDinamicas * test = new PlataformasDinamicas(
        "assets/sprites/mundo/bg/Painted_Tiles.png",
        10,12,256,256,32,32,{0,255,0,255});

    test->set_velocidad(5);
    get().ensamble->cargar_texturas(test->get_sprite());
    plataformadinamicas.push_back(test);*/

    prot = new Protector("assets/sprites/heroe/Ataque_sheet.png",
                        -3000, -3000, 512, 512, 80, 80, {255,0,255,255});
    get().ensamble->cargar_texturas(prot->get_sprite());

    //lock camara
    ManejadorCamaras::get().lock_objeto(*player);
    ManejadorCamaras::get().set_estado(new EstadoCamaraTransicion());

    //09 parallax
    background = new BackGroundDinamico("assets/sprites/mundo/bg/Background.png",16384,2048);
    background->set_velocidad(2);
    get().ensamble->cargar_texturas(background->get_sprite());

    objetos.push_back(background);
    

    for(int i=0;i<plataformas.size();i++)
    {
        //agregar todos los objetos en una lista para la camara
        plataformas[i]->set_velocidad(5);
        objetos.push_back(plataformas[i]);
    }
    //3840, 336
    meta = new Meta("assets/sprites/mundo/bg/Flag_Sheet.png", 3840, 336, 256, 256, 128, 128, {255, 0, 255, 255});
    get().ensamble->cargar_texturas(meta->get_sprite());
    objetos.push_back(meta);

    //objetos.push_back(test);
    for(int i=0;i<10;i++) {
        enemy = new Enemigo("assets/sprites/enemigo/Enemy_Sheet.png",
                                10, rand()%4000+500, rand()%1200-500, 128, 128, 32, 32, {255, 0, 255, 255});
        get().ensamble->cargar_texturas(enemy->get_sprite());
        enemy->set_velocidad(1);
        enemies.push_back(enemy);
        objetos.push_back(enemy);
    }

    for(int i=0;i<5;i++) {
        hb = new HealthBar("assets/sprites/heroe/Hearts_Sheet.png", get().camara_principal->get_posicion_mundo().x+(i*72), 
            get().camara_principal->get_posicion_mundo().y, 128, 128, 64, 64, {255, 0, 255, 255});
        get().ensamble->cargar_texturas(hb->get_sprite());
        hb->set_velocidad(5);
        healthBar.push_back(hb);
        objetos.push_back(hb);
    }

    objetos.push_back(prot);
    player->set_objetos_mundo(objetos);
    objetos.push_back(player);

    
    game_timer=0;
    past_time=0;
    contador=0;
    delay = 1;
    game_timer = Tiempo::get_tiempo();
    printf("\nSe crearon los objetos exitosamente\n");

    //agregamos el color del background del frame
    SDL_SetRenderDrawColor(
        get().render,       // canvas
        get().bg_color.r,   // rojo
        get().bg_color.g,   // verde
        get().bg_color.b,   // azul
        SDL_ALPHA_TRANSPARENT);// como usar el alpha

    return true;
};

void SDLApp::on_evento(SDL_Event* evento)
{
    if(evento->type==SDL_QUIT)
    {
        get().esta_corriendo = false;
    }
    //eventos del mouse
    MouseOyente::get().evento_movimiento(evento);
    MouseOyente::get().evento_presionado(evento);
    MouseOyente::get().evento_scroll(evento);
    // que esta presionado
    KeyOyente::get().keyPresionado(evento);
    
};

void SDLApp::on_fisicaupdate(double dt)
{
    player->en_colision=false;
    if(player->get_posicion_mundo().x<0){
        player->set_posx(0);
    }
    if(player->get_posicion_mundo().y>1000)
    {
        player->set_posy(800);
    }

    if(player->get_strEstado()=="ATACAR"){
        prot->set_posicion_mundo({player->get_posicion_mundo().x-10, player->get_posicion_mundo().y});
    }
    else if(player->get_strEstado()!="VICTORIA")
    {
        prot->set_posicion_mundo({-3000, -3000});
    }
    else {
        prot->set_posicion_mundo({meta->get_posicion_mundo().x - 32, player->get_posicion_mundo().y-160});
    }

    if(player->get_hp()<=0 && player->get_strEstado()!="DERROTADO") {
        player->set_estado(new EstadoJugadorDerrota());
        for(auto &e:enemies) {
            e->en_colision = true;
        }
    }

    for(auto &e:enemies) {
        if(player->get_strEstado()!="VICTORIA") e->lock_objeto(*player);
        else {
            e->lock_objeto(*prot);
            e->set_velocidad(8);
            if(e->get_posicion_mundo().x-prot->get_posicion_mundo().x<20 && 
               e->get_posicion_mundo().x-prot->get_posicion_mundo().x>-20 &&
               e->get_posicion_mundo().y-prot->get_posicion_mundo().y<20 && 
               e->get_posicion_mundo().y-prot->get_posicion_mundo().y>-20 &&
               e->get_hp()!=1)
                {
                e->en_colision=true;
                }
        }
    }
    //enemy->lock_objeto(*player);

    player->input_handle(KeyOyente::get(),MouseOyente::get());
    
    platspawn->update(&objetos);

    bool pplat=false;
    for(auto &p:objetos)
    {  
        p->update(dt);
        if(!p->render_colbox && p->get_colbox())
            p->render_colbox=true;

        if(p!= player && p->get_colbox() && p->get_hp()!=1)
        {
            MotorFisico2D::get().diag_overlap(*player,*p);
            bool pc = MotorFisico2D::get().aabb_colision(*player->get_colbox(),*p->get_colbox());
            player->en_colision|=pc;
            p->en_colision=pc;
            if(p==meta && pc && player->get_strEstado()!="DERROTADO" && player->get_strEstado()!="VICTORIA") {
                player->set_estado(new EstadoJugadorVictoria());
                meta->del_col_box();
                for(auto &e:enemies) {
                    e->lock_objeto(*prot);
                }
            }
            
            if((std::find(enemies.begin(), enemies.end(), p)!=enemies.end()) && pc)
            {
                if(player->get_strEstado()!="ATACAR" && player->get_strEstado()!="VICTORIA" && player->get_strEstado()!="DERROTADO") 
                {
                    player->set_hp(player->get_hp()-1); //player->set_estado(new EstadoJugadorDerrota());
                }
                if(player->get_strEstado()=="ATACAR") 
                {
                    player->kills++;
                }
            } 
            
            if(p->area_plataforma)
            {
                pplat|=MotorFisico2D::get().aabb_colision(*player->area_plataforma,*p->area_plataforma);
                //printf("en plataforma [%d]\n",pplat);
                if(pplat && pc)
                {
                    MotorFisico2D::get().diag_overlap(*player,*p);
                    player->set_onPlataforma(pplat);
                    if(p->trigger_box) {
                        platspawn->spawn(&objetos, p->get_posicion_mundo());
                        //platspawn->set_plat_posibles(p->get_posicion_mundo());
                        p->trigger_box=nullptr;
                    }
                    //player->en_colision=false;
                }
            }
        }
    }
    MotorFisico2D::get().gravedad({player});

    /*CAMARA al final para actualizar la proyeción de los objetos*/
    ManejadorCamaras::get().input_handle(KeyOyente::get(),MouseOyente::get());
    ManejadorCamaras::get().update(objetos); 
};

void SDLApp::on_frameupdate(double dt)
{
    // limpiar frame
    SDL_RenderClear(get().render);

    //Renderizar todo a través de la camara
    ManejadorCamaras::get().renderizar(objetos);
    camara_principal->render_cross();
    if(player->get_hp()>0) {
        for(int i=0;i<player->get_hp();i++) {
            healthBar[i]->set_posicion_mundo({camara_principal->get_posicion_mundo().x+(72*i), camara_principal->get_posicion_mundo().y});
        }
        for(int i=player->get_hp();i<5;i++) {
            healthBar[i]->set_posicion_mundo({-3000,-3000});
        }
    }
    else {
        for(int i=0;i<5;i++) {
            healthBar[i]->set_posicion_mundo({-3000,-3000});
        }
    }
    
    //Timer
    if(player->get_strEstado()!="VICTORIA") {
        game_timer += Tiempo::get_tiempo() - game_timer;
        if((int)game_timer%delay == 0 && (int)game_timer!=0 && (int)game_timer > past_time) {
            contador++;
            past_time = game_timer;
            on_fisicaupdate(dt);
        }
    }
    if(player->get_strEstado()=="DERROTADO") {
        RenderTexto::get().render_texto(get().render, WIDTH/2-150, HEIGHT/2, "GAME OVER", 300, 50, SDL_Color{200,0,100,255});
    }

    int comp = platspawn->get_plat_posibles().size();
    /*
    
    RenderTexto::get().render_texto(get().render, 500, 0, "Tiempo: " + std::to_string(contador), 120, 30, SDL_Color{0,0,0,255});
    RenderTexto::get().render_texto(get().render, 500, 60, "Kills: " + std::to_string(player->kills), 120, 30, SDL_Color{0,0,0,255});
    */
    //Achievements
    if(player->get_strEstado()=="VICTORIA") {
        std::string cpm = " " + std::to_string(comp*100/198) + "%"; 
        RenderTexto::get().render_texto(
        get().render,WIDTH/2-190,0,
        cpm,
        150,50,{255-(int)comp*1.275,(int)comp*1.275,0,255}
        );
        if(comp==198) {
            std::string hundred = "Clean Map!";
            RenderTexto::get().render_texto(
            get().render,50,60,
            hundred,
            150,50,{100, 250, 50, 255}
            );
        }
        if(comp<=40) {
            std::string low = "Low%!";
            RenderTexto::get().render_texto(
            get().render,50,60,
            low,
            150,50,{100, 250, 50, 255}
            );
        }
        if(player->get_hp()==5) {
            std::string fullhp = "Flawless!";
            RenderTexto::get().render_texto(
            get().render,50,120,
            fullhp,
            150,50,{100, 250, 50, 255}
            );
        }
        if(player->get_hp()==1) {
            std::string oneHit = "OneHit!";
            RenderTexto::get().render_texto(
            get().render,50,120,
            oneHit,
            150,50,{100, 250, 50, 255}
            );
        }
        if(contador<=40) {
            std::string theFlash = "The Flash!";
            RenderTexto::get().render_texto(
            get().render,50,180,
            theFlash,
            150,50,{100, 250, 50, 255}
            );
        }
        if(player->kills==0) {
            std::string pacifist = "Pacifist!";
            RenderTexto::get().render_texto(
            get().render,50,240,
            pacifist,
            150,50,{100, 250, 50, 255}
            );
        }
        if(player->kills>=50) {
            std::string theDoomSlayer = "The Doom Slayer!";
            RenderTexto::get().render_texto(
            get().render,50,240,
            theDoomSlayer,
            150,50,{100, 250, 50, 255}
            );
        }
    }
    

    //}
    /*
    Coordenadas pm = player->get_posicion_mundo();
    Coordenadas pc = player->get_posicion_camara();
    std::string spm = "mundo ( "+std::to_string(pm.x)+", "+std::to_string(pm.y)+")";

    RenderTexto::get().render_texto(
        get().render,100,100,
        spm,
        150,50,{0,0,0,255}
    );

    std::string spc = "camara ( "+std::to_string(pc.x)+", "+std::to_string(pc.y)+")";

    RenderTexto::get().render_texto(
        get().render,800,100,
        spc,
        150,50,{0,0,0,255}
    );*/

    

    //get().ensamble->figuras(player->get_col_piso());
    /*
    FSMJugador* e = (FSMJugador*)player->get_estado();
    RenderTexto::get().render_texto(
        get().render,600,600,
        e->get_namestate(),
        100,50,{255,0,255,255}
    );

    FSMCamara* ce = (FSMCamara*)ManejadorCamaras::get().get_estado();
    RenderTexto::get().render_texto(
        get().render,100,600,
        ce->strestado,
        100,50,{255,0,255,255}
    );*/

    //Actualizar
    SDL_RenderPresent(get().render);

    //resetear color del frame
    SDL_SetRenderDrawColor(
        get().render,
        get().bg_color.r,
        get().bg_color.g,
        get().bg_color.b,
        SDL_ALPHA_OPAQUE);
};

void SDLApp::on_limpiar()
{
    SDL_DestroyRenderer(get().render);
    SDL_FreeSurface(get().vntsurf);
    SDL_DestroyWindow(get().vnt);

    for(std::vector<Objeto*>::iterator it = objetos.begin();it!=objetos.end();it++)
    {
        Objeto *p=*it;
        delete p;
    }
    //objetos.clear();
    delete ensamble;
    //delete player;
    delete platspawn;
    SDL_Quit();
};

int SDLApp::on_correr()
{
    //revisar que todo se inicializo bien
    if(get().on_init()==false){return -1;}

    SDL_Event eventos;
    double dt=0;
    double frecuencia = 1/get().maxFPS; // 1 frame a 60fps
    get().msfrecuencia = frecuencia*1000;

    while(get().estaCorriendo())
    {
        //double start = SDL_GetTicks();
        double inicio = Tiempo::get_tiempo();

        //printf("%lf <> %d\n",Tiempo::get_tiempo(),SDL_GetTicks());
        //captura eventos
        while(SDL_PollEvent(&eventos))
        {
            get().on_evento(&eventos);
        }

        //actualizamos si inicia o hay una diferencia de tiempo
        
        get().on_fisicaupdate(dt);
        get().on_frameupdate(dt);

        //calculamos el tiempo del frame
        dt = (Tiempo::get_tiempo() - inicio)/(frecuencia*1000);
        
        inicio=dt; //el nuevo frame inicia en este tiempo
        //printf("<%d>[%lf][%d]\n",get().fps,dt,(int)Tiempo::get_tiempo());
        if(dt>get().msfrecuencia)
        {
            SDL_Delay(floor(get().msfrecuencia-dt));
        }
        //get().fps++;
    }
    //liberamos memoria
    get().on_limpiar();
    return 0;
};



