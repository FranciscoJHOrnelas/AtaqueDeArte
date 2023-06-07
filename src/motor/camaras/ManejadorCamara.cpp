#include "ManejadorCamara.hpp"

ManejadorCamaras * ManejadorCamaras::instancia = 0;

ManejadorCamaras::ManejadorCamaras()
{
    camara_actual=nullptr;
};

ManejadorCamaras& ManejadorCamaras::get()
{
    if(!instancia)
        instancia = new ManejadorCamaras();
    return *instancia;
};


void ManejadorCamaras::set_camara(Camara &cam)
{
    get().camara_actual = &cam;
};
Camara& ManejadorCamaras::get_camara()
{
    return *get().camara_actual;
};
void ManejadorCamaras::lock_objeto(Objeto& obj)
{
    get().camara_actual->lock_objeto(obj);
};
void ManejadorCamaras::unluck_objeto()
{
    get().camara_actual->unluck_objeto();
};

void ManejadorCamaras::input_handle(KeyOyente &input, MouseOyente &mouse)
{
    get().camara_actual->input_handle(input,mouse);
};
void ManejadorCamaras::update(std::vector<Objeto*> objetos)
{
    get().camara_actual->update();
    get().camara_actual->proyectar(objetos);
};
void ManejadorCamaras::set_estado(void *nuevo)
{
    get().camara_actual->set_estado(nuevo);
};

void* ManejadorCamaras::get_estado()
{
    return get().camara_actual->get_estado();
};

void ManejadorCamaras::renderizar(std::vector<Objeto*> objetos)
{
    get().camara_actual->renderizar(objetos);
};