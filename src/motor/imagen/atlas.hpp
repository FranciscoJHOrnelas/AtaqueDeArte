#pragma once
#include "../../utilidad/Estructuras.hpp"
#include "../../objetos_estaticos/ObjetoEstatico.hpp"
#include<string>
#include<SDL.h>

class Atlas
{
    public:
        Atlas(std::string atlas);
        ~Atlas();
        bool cargar_textura(SDL_Renderer *r);
        void generar_mapa(SDL_Renderer *r,int idflip,int notidobjfisicos);
        std::vector<Objeto*> get_objetos_fisicos() const{return objetos_fisicos;};

    private:
        AtlasInfo info;
        std::vector<Objeto*> objetos_fisicos;
        SDL_Texture *atlas_sheet;
        
};