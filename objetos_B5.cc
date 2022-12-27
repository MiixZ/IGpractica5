//**************************************************************************
// Práctica 2 
//**************************************************************************

#include "objetos_B5.h"
#include "file_ply_stl.hpp"

//*************************************************************************
// _puntos3D
//*************************************************************************

_puntos3D::_puntos3D()
{
}
//*************************************************************************
// dibujar puntos
//*************************************************************************

void _puntos3D::draw_puntos(float r, float g, float b, int grosor)
{
    int i;
    glPointSize(grosor);
    glColor3f(r,g,b);
    glBegin(GL_POINTS);
    for (i=0;i<vertices.size();i++){
        glVertex3fv((GLfloat *) &vertices[i]);
        }
    glEnd();
}

//*************************************************************************
// _triangulos3D
//*************************************************************************

_triangulos3D::_triangulos3D(){
    ambiente_difuso =_vertex4f(0.4, 0.8, 0.0, 1.0);
    especular=_vertex4f(0.0, 0.5, 0.5, 1.0);
    brillo = 10;
}

void _triangulos3D::colors_random()
{
    int i, n_c;
    n_c=caras.size();
    colores_caras.resize(n_c);
    srand (time(NULL));
    for (i=0;i<n_c;i++) {
        colores_caras[i].r=rand()%1000/1000.0;
        colores_caras[i].g=rand()%1000/1000.0;
        colores_caras[i].b=rand()%1000/1000.0;
    }
}

//*************************************************************************
// dibujar en modo arista
//*************************************************************************

void _triangulos3D::draw_aristas(float r, float g, float b, int grosor)
{
int i;
glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
glLineWidth(grosor);
glColor3f(r,g,b);
glBegin(GL_TRIANGLES);
for (i=0;i<caras.size();i++){
	glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
	glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
	glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
	}
glEnd();
}

//*************************************************************************
// dibujar en modo sólido con un único color
//*************************************************************************

void _triangulos3D::draw_solido(float r, float g, float b)
{
    int i;
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    
    glColor3f(r,g,b);
    glBegin(GL_TRIANGLES);
    for (i=0; i<caras.size(); i++){
        glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
        glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
        glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
        }
    glEnd();
}

//*************************************************************************
// dibujar en modo sólido con colores diferentes para cada cara
//*************************************************************************

void _triangulos3D::draw_solido_colores(){
    int i;
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glBegin(GL_TRIANGLES);
    
    for (i=0;i<caras.size();i++){
        glColor3f(colores_caras[i].r, colores_caras[i].g ,colores_caras[i].b);
        glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
        glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
        glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
    }
    glEnd();
}

void _triangulos3D::draw_solido_plano(){
    int i;
    glEnable(GL_LIGHTING);

    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, (GLfloat *) &ambiente_difuso);
    glMaterialfv(GL_FRONT, GL_SPECULAR, (GLfloat *) &especular);
    glMaterialfv(GL_FRONT, GL_SHININESS, (GLfloat *) &brillo);

    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glBegin(GL_TRIANGLES);
    
    for(i=0; i<caras.size(); i++){
        glNormal3f(normales_caras[i].x, normales_caras[i].y, normales_caras[i].z);
        glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
        glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
        glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
    }
    glEnd();

    glDisable(GL_LIGHTING);
}

void _triangulos3D::draw_solido_suave() {
    glEnable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, (GLfloat *)&ambiente_difuso);
    glMaterialfv(GL_FRONT, GL_SPECULAR, (GLfloat*)&especular);
    glMaterialf(GL_FRONT, GL_SHININESS, brillo);  // este es solo float

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_TRIANGLES);

    int num_tot_caras = caras.size();

    for (int i = 0; i < num_tot_caras; i++) {
        glNormal3fv((GLfloat *)&normales_vertices[caras[i]._0]);
        glVertex3fv((GLfloat*)&vertices[caras[i]._0]);

        glNormal3fv((GLfloat *)&normales_vertices[caras[i]._1]);
        glVertex3fv((GLfloat*)&vertices[caras[i]._1]);

        glNormal3fv((GLfloat *)&normales_vertices[caras[i]._2]);
        glVertex3fv((GLfloat*)&vertices[caras[i]._2]);
    }
    glDisable(GL_LIGHTING);
    glEnd();
}

void _triangulos3D::draw_seleccion(int r, int g, int b){
    int i;

    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glColor3ub(r,g,b);
    glBegin(GL_TRIANGLES);
    for (i=0;i<caras.size();i++){
        glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
        glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
        glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
    }
    glEnd();
}

//*************************************************************************
// dibujar con distintos modos
//*************************************************************************

void _triangulos3D::draw(_modo modo, float r, float g, float b, float grosor){
    switch (modo){
        case POINTS:draw_puntos(r, g, b, grosor);break;
        case EDGES:draw_aristas(r, g, b, grosor);break;
        case SOLID:draw_solido(r, g, b);break;
        case SOLID_COLORS:draw_solido_colores();break;
        case SOLID_FLAT:draw_solido_plano();break;
        case SOLID_SMOOTH:draw_solido_suave();break;
        case SELECT:draw_seleccion(r, g, b);break;
    }
}

void _triangulos3D::colors_chess(float r1, float g1, float b1, float r2, float g2, float b2)
{
int i, n_c;
n_c=caras.size();
colores_caras.resize(n_c);
    for (i=0;i<n_c;i++)  {
        if (i%2==0) 
            {colores_caras[i].r=r1;
            colores_caras[i].g=g1;
            colores_caras[i].b=b1;
        }
        else {
            colores_caras[i].r=r2;
            colores_caras[i].g=g2;
            colores_caras[i].b=b2;
        } 
    }
}

void _triangulos3D::calcular_normales_caras() {
    int n_c;
    _vertex3f vector_a, vector_b,aux;

    n_c = caras.size();
    normales_caras.resize(n_c);
    float modulo;

    for (int i = 0; i < n_c; i++) {
        vector_a = vertices[caras[i]._1] - vertices[caras[i]._0];
        vector_b = vertices[caras[i]._2] - vertices[caras[i]._0];

        //calculo normal
        aux.x = vector_a.y * vector_b.z - vector_a.z * vector_b.y;
        aux.y = vector_a.z * vector_b.x - vector_a.x * vector_b.z;
        aux.z = vector_a.x * vector_b.y - vector_a.y * vector_b.x;

        //calculo del vector en si
        modulo = sqrt(aux.x * aux.x + aux.y * aux.y + aux.z * aux.z);
        normales_caras[i].x = aux.x / modulo;
        normales_caras[i].y = aux.y / modulo;
        normales_caras[i].z = aux.z / modulo;
    }
}

void _triangulos3D::calcular_normales_vertices() {
    int i, n_v;
    float norma;
    n_v = vertices.size();
    normales_vertices.resize(n_v);

    for(i=0; i < n_v; i++){
        normales_vertices[i].x = 0;
        normales_vertices[i].y = 0;
        normales_vertices[i].z = 0;
    }

    for(i = 0; i < caras.size(); i++){
        normales_vertices[caras[i]._0] += normales_caras[i];
        normales_vertices[caras[i]._1] += normales_caras[i];
        normales_vertices[caras[i]._2] += normales_caras[i];
    }

    for(i=0; i < n_v; i++){
        norma = sqrt(normales_vertices[i].x*normales_vertices[i].x +
                    normales_vertices[i].y*normales_vertices[i].y +
                    normales_vertices[i].z*normales_vertices[i].z);
        normales_vertices[i].x /= norma;
        normales_vertices[i].y /= norma;
        normales_vertices[i].z /= norma;
    }
}

void _triangulos3D::colors_lambert_c(float l_x, float l_y, float l_z, float r, float g, float b){       // l_x, l_y, l_z determina dónde está la luz.
    int n_c;
    n_c = caras.size();
    colores_caras.resize(n_c);
    _vertex3f luz, aux_luz;
    float modulo, p_escalar;
    aux_luz.x = l_x;
    aux_luz.y = l_y;
    aux_luz.z = l_z;

    for (int i = 0; i < n_c; i++) {
        luz = aux_luz - vertices[caras[i]._0];
        modulo = sqrt(luz.x * luz.x + luz.y * luz.y + luz.z * luz.z);

        luz.x = luz.x / modulo;
        luz.y = luz.y / modulo;
        luz.z = luz.z / modulo;

        p_escalar = luz.x * normales_caras[i].x + luz.y * normales_caras[i].y +
                    luz.z * normales_caras[i].z;

        if (p_escalar < 0) p_escalar = 0;

        colores_caras[i].r = r * 0.2 + r * p_escalar;         // La izquierda es una componente ambiental.
        colores_caras[i].g = g * 0.2 + g * p_escalar;
        colores_caras[i].b = b * 0.2 + b * p_escalar;
    }
}

//*************************************************************************
// clase cubo
//*************************************************************************

_cubo::_cubo(float tam){
//vértices
    vertices.resize(8); 
    vertices[0].x=-tam; vertices[0].y=0; vertices[0].z=tam;
    vertices[1].x=tam; vertices[1].y=0; vertices[1].z=tam;
    vertices[2].x=tam; vertices[2].y=0; vertices[2].z=-tam;
    vertices[3].x=-tam; vertices[3].y=0; vertices[3].z=-tam;
    vertices[4].x=-tam; vertices[4].y=tam*2; vertices[4].z=tam;
    vertices[5].x=tam; vertices[5].y=tam*2; vertices[5].z=tam;
    vertices[6].x=tam; vertices[6].y=tam*2; vertices[6].z=-tam;
    vertices[7].x=-tam; vertices[7].y=tam*2; vertices[7].z=-tam;
    
//colores vértices.
    /*colores_vertices.resize(8);    // valores entre 0 y 1.
    colores_vertices[0].r=0; colores_vertices[0].g=0.5; colores_vertices[0].b=0.5;
    colores_vertices[1].r=0.35; colores_vertices[1].g=0.15; colores_vertices[1].b=0.45;
    colores_vertices[2].r=0.6; colores_vertices[2].g=0.8; colores_vertices[2].b=0.11;
    colores_vertices[3].r=0,99; colores_vertices[3].g=0.99; colores_vertices[3].b=0.99;
    colores_vertices[4].r=0.1; colores_vertices[4].g=0.65; colores_vertices[4].b=0.29;
    colores_vertices[5].r=0.35; colores_vertices[5].g=0.84; colores_vertices[5].b=0.99;
    colores_vertices[6].r=0,99; colores_vertices[6].g=0.19; colores_vertices[6].b=0.79;
    colores_vertices[7].r=0.44; colores_vertices[7].g=0.41; colores_vertices[7].b=0.24;*/
 
// caras
    caras.resize(12);
    caras[0]._0=0; caras[0]._1=1; caras[0]._2=4;
    caras[1]._0=1; caras[1]._1=4; caras[1]._2=5;
    caras[2]._0=1; caras[2]._1=5; caras[2]._2=6;
    caras[3]._0=1; caras[3]._1=2; caras[3]._2=6;
    caras[4]._0=2; caras[4]._1=3; caras[4]._2=6;
    caras[5]._0=3; caras[5]._1=7; caras[5]._2=6;
    caras[6]._0=0; caras[6]._1=3; caras[6]._2=4;
    caras[7]._0=3; caras[7]._1=4; caras[7]._2=7;
    caras[8]._0=0; caras[8]._1=1; caras[8]._2=3;
    caras[9]._0=1; caras[9]._1=2; caras[9]._2=3;
    caras[10]._0=4; caras[10]._1=5; caras[10]._2=7;
    caras[11]._0=5; caras[11]._1=6; caras[11]._2=7;
    
//colores caras.
    colores_caras.resize(12);    // valores entre 0 y 1.
    /*colores_caras[0].r=0; colores_caras[0].g=0.5; colores_caras[0].b=0.5;
    colores_caras[1].r=0.2; colores_caras[1].g=1; colores_caras[1].b=0.4;
    colores_caras[2].r=0.6; colores_caras[2].g=0.8; colores_caras[2].b=0.8;
    colores_caras[3].r=0.8; colores_caras[3].g=0.6; colores_caras[3].b=0.6;
    colores_caras[4].r=1; colores_caras[4].g=0.4; colores_caras[4].b=0.2;
    colores_caras[5].r=0.1; colores_caras[5].g=0.2; colores_caras[5].b=1;
    colores_caras[6].r=0.2; colores_caras[6].g=1; colores_caras[6].b=0.4;
    colores_caras[7].r=0.6; colores_caras[7].g=0.8; colores_caras[7].b=0.8;
    colores_caras[8].r=0.8; colores_caras[8].g=0.6; colores_caras[8].b=0.6;
    colores_caras[9].r=0.11; colores_caras[9].g=0.23; colores_caras[9].b=0.2;
    colores_caras[10].r=0.1; colores_caras[10].g=0.2; colores_caras[10].b=1;
    colores_caras[11].r=0.19; colores_caras[11].g=0.85; colores_caras[11].b=0.33;
    */

    calcular_normales_caras();
    calcular_normales_vertices();

    colors_lambert_c(0, 20, 20, 1.0, 0.8, 0);
}

_cilindro::_cilindro(float radio, float altura, int num) {           // Añadir el enum, la tecla y en el main.
    vector<_vertex3f> perfil;
    _vertex3f vert_aux;
    
    vert_aux.x = radio;
    vert_aux.y = -altura/2.0;
    vert_aux.z = 0.0;
    
    perfil.push_back(vert_aux);
    
    vert_aux.x = radio;
    vert_aux.y = altura/2.0;
    vert_aux.z = 0.0;
    
    perfil.push_back(vert_aux);
    
    parametros(perfil, num, 0, 1, 1);
}

_cono::_cono(float radio, float altura, int num) {
    vector <_vertex3f> perfil;
    _vertex3f vert_aux;
    vert_aux.x = radio;
    vert_aux.y = 0.0;
    vert_aux.z = 0.0;
    perfil.push_back(vert_aux);
    vert_aux.x = 0.0;
    vert_aux.y = altura;
    vert_aux.z = 0.0;
    perfil.push_back(vert_aux);
    
    parametros(perfil, num, 1, 1, 1);
}

_esfera::_esfera(float radio, int num1, int num2) {
    vector <_vertex3f> perfil;
    _vertex3f vert_aux;
    
    for(int i = 1; i < num1; i++) {
        vert_aux.x = radio*cos(M_PI*i/num1-M_PI/2.0);
        vert_aux.y = radio*sin(M_PI*i/num1-M_PI/2.0);
        vert_aux.z = 0.0;
        perfil.push_back(vert_aux);
    }
    
    parametros(perfil, num2, 2, 1, 1);
}

_esferaDoble::_esferaDoble(float radio1, float radio2, int num1, int num2) {
    vector <_vertex3f> perfil;
    _vertex3f vert_aux;
    
    vert_aux.x=0.8;
    vert_aux.y=0;
    vert_aux.z=0;
    perfil.push_back(vert_aux);

    vert_aux.x=0.8;
    vert_aux.y=0.3;
    vert_aux.z=0;
    perfil.push_back(vert_aux);

    vert_aux.x=0.7;
    vert_aux.y=0.3;
    vert_aux.z=0;
    perfil.push_back(vert_aux);

    for(int i = num2/2+1; i < num1; i++){
        vert_aux.x = 0.2 + 0.5*cos(M_PI*i/num1-M_PI/2.0);
        vert_aux.y = 0.3 + 0.5*sin(M_PI*i/num1-M_PI/2.0);
        vert_aux.z = 0.0;
        perfil.push_back(vert_aux);
    }

    vert_aux.x=0.2;
    vert_aux.y=0.8;
    vert_aux.z=0;
    perfil.push_back(vert_aux);

    vert_aux.x=0.2;
    vert_aux.y=1;
    vert_aux.z=0;
    perfil.push_back(vert_aux);
    parametros(perfil, num2, 2, 0, 0);
}

//*************************************************************************
// clase pirámide
//*************************************************************************

_piramide::_piramide(float tam, float al){

//vértices 
    vertices.resize(5); 
    vertices[0].x=-tam;vertices[0].y=0;vertices[0].z=tam;
    vertices[1].x=tam;vertices[1].y=0;vertices[1].z=tam;
    vertices[2].x=tam;vertices[2].y=0;vertices[2].z=-tam;
    vertices[3].x=-tam;vertices[3].y=0;vertices[3].z=-tam;
    vertices[4].x=0;vertices[4].y=al;vertices[4].z=0;

    caras.resize(6);
    caras[0]._0=0;caras[0]._1=1;caras[0]._2=4;
    caras[1]._0=1;caras[1]._1=2;caras[1]._2=4;
    caras[2]._0=2;caras[2]._1=3;caras[2]._2=4;
    caras[3]._0=3;caras[3]._1=0;caras[3]._2=4;
    caras[4]._0=3;caras[4]._1=1;caras[4]._2=0;
    caras[5]._0=3;caras[5]._1=2;caras[5]._2=1;
    
    colores_caras.resize(6);
    srand(10);
    /*for(int i = 0; i < 6; i++){
        colores_caras[i].r = rand()%1000/1000.0;
        colores_caras[i].g = rand()%1000/1000.0;
        colores_caras[i].b = rand()%1000/1000.0;    
    }*/
    calcular_normales_caras();
    calcular_normales_vertices();

    colors_lambert_c(0, 20, 20, 1.0, 0.8, 0);
}

//*************************************************************************
// clase pirámide examen
//*************************************************************************

_piramideExamen::_piramideExamen(float tam, float al)
{

//vértices 
    vertices.resize(7);
    vertices[0].x=-tam;vertices[0].y=0;vertices[0].z=tam;
    vertices[1].x=tam;vertices[1].y=0;vertices[1].z=tam;
    vertices[2].x=tam;vertices[2].y=0;vertices[2].z=-tam;
    vertices[3].x=-tam;vertices[3].y=0;vertices[3].z=-tam;
    vertices[4].x=tam;vertices[4].y=al;vertices[4].z=tam;
    vertices[5].x=-tam;vertices[5].y=al;vertices[5].z=-tam;
    vertices[6].x=-tam;vertices[6].y=2*al;vertices[6].z=tam;

    caras.resize(9);
    caras[0]._0=0;caras[0]._1=4;caras[0]._2=1;
    caras[1]._0=0;caras[1]._1=4;caras[1]._2=6;
    caras[2]._0=1;caras[2]._1=4;caras[2]._2=2;
    caras[3]._0=4;caras[3]._1=2;caras[3]._2=6;
    caras[4]._0=2;caras[4]._1=3;caras[4]._2=5;
    caras[5]._0=5;caras[5]._1=2;caras[5]._2=6;
    caras[6]._0=5;caras[6]._1=0;caras[6]._2=6;
    caras[7]._0=0;caras[7]._1=1;caras[7]._2=2;
    caras[8]._0=0;caras[8]._1=2;caras[8]._2=3;
    
    colores_caras.resize(9);
    srand(10);
    for(int i = 0; i < 9; i++){
        colores_caras[i].r = rand()%1000/1000.0;
        colores_caras[i].g = rand()%1000/1000.0;
        colores_caras[i].b = rand()%1000/1000.0;    
    }
}

//*************************************************************************
// clase objeto ply
//*************************************************************************

_objeto_ply::_objeto_ply() {
   // leer lista de coordenadas de vértices y lista de índices de vértices
 
}

void _objeto_ply::parametros(char *archivo) {                                           // OBJETO TECLA "o"     
    int n_ver,n_car;

    vector<float> ver_ply ;
    vector<int>   car_ply ;
    
    _file_ply::read(archivo, ver_ply, car_ply );

    n_ver=ver_ply.size()/3;
    n_car=car_ply.size()/3;

    printf("Number of vertices=%d\nNumber of faces=%d\n", n_ver, n_car);

    vertices.resize(n_ver);
    caras.resize(n_car);

// vértices                                 EN EL FICHERO .ply ESTÁN TODOS LOS DATOS NECESARIOS PARA CONSTRUIR EL OBJETO.
    for(int i = 0; i < n_ver; i++){
        vertices[i].x = ver_ply[i*3];
        vertices[i].y = ver_ply[i*3 + 1];
        vertices[i].z = ver_ply[i*3 + 2];
    }

// caras
    for(int i = 0; i < n_car; i++){
        caras[i].x = car_ply[i*3];
        caras[i].y = car_ply[i*3 + 1];
        caras[i].z = car_ply[i*3 + 2];
    }

// Calcular normales.
    calcular_normales_caras();

    calcular_normales_vertices();
// Colores

    colors_lambert_c(0, 20, 20, 0.2, 0.8, 0);
}

//************************************************************************
// objeto por revolución
//************************************************************************

_rotacion::_rotacion() {

}

void _rotacion::parametros(vector<_vertex3f> perfil, int num, int tipo, int tapa_in, int tapa_su) {
    int i,j;
    _vertex3f vertice_aux;
    _vertex3i cara_aux;
    int num_aux;
    float radio;

    // tratamiento de los vértice
    radio=sqrt(perfil[0].x*perfil[0].x+perfil[0].y*perfil[0].y);

    num_aux=perfil.size();
    if (tipo==1) num_aux=num_aux-1;
    vertices.resize(num_aux*num+2);
    for (j=0;j<num;j++){
        for (i=0;i<num_aux;i++){
        vertice_aux.x=perfil[i].x*cos(2.0*M_PI*j/(1.0*num))+
                        perfil[i].z*sin(2.0*M_PI*j/(1.0*num));
        vertice_aux.z=-perfil[i].x*sin(2.0*M_PI*j/(1.0*num))+
                        perfil[i].z*cos(2.0*M_PI*j/(1.0*num));
        vertice_aux.y=perfil[i].y;
        vertices[i+j*num_aux]=vertice_aux;
        }
    }

    caras.resize(2*(num_aux-1)*num+2*num);
    int c=0;
    for (j=0;j<num;j++){
        for (i=0;i<num_aux-1;i++){
            caras[c]._0=i+j*num_aux;
            caras[c]._1=((j+1)%num)*num_aux+i;
            caras[c]._2=1+i+j*num_aux;
            c+=1;
            caras[c]._0=((j+1)%num)*num_aux+i;
            caras[c]._1=((j+1)%num)*num_aux+1+i;
            caras[c]._2=1+i+j*num_aux;
            c+=1;
        }
    }
    
    //tapa inferior
    if(tapa_su == 1){
        int total=num_aux*num;
        vertices[total].x=0.0;
        if (tipo==2) vertices[total].y=-radio;
        else vertices[total].y=perfil[0].y;
        vertices[total].z=0.0;

        for (j=0;j<num;j++){
            caras[c]._0=j*num_aux;
            caras[c]._1=((j+1)%num)*num_aux;
            caras[c]._2=total;
            c+=1;
        } 
        
        //tapa superior
        vertices[total+1].x=0.0;
        if (tipo==1) vertices[total+1].y=perfil[1].y;
        if (tipo==0) vertices[total+1].y=perfil[num_aux-1].y;
        if (tipo==2) vertices[total+1].y=radio;
        vertices[total+1].z=0.0;

        for (j=0;j<num;j++){
            caras[c]._0=total+1;
            caras[c]._1=((j+1)%num)*num_aux+num_aux-1;
            caras[c]._2=num_aux-1+j*num_aux;
            c+=1;
        }
    }

    int n_c = 2*(num_aux-1)*num+2*num;
    colores_caras.resize(n_c);
    //colors_random();
     calcular_normales_caras();
     calcular_normales_vertices();

     colors_lambert_c(0, 20, 20, 1.0, 0.8, 0);
}

//************************************************************************
// objeto por extrusión
//************************************************************************

_extrusion::_extrusion(vector<_vertex3f> poligono, float x, float y, float z) {
    int i;
    _vertex3f vertice_aux;
    _vertex3i cara_aux;
    int num_aux;

// tratamiento de los vértices.
//...
    num_aux=poligono.size();
    vertices.resize(num_aux*2);
    for (i=0;i<num_aux;i++) {
        vertices[2*i]=poligono[i];
        vertices[2*i+1].x=poligono[i].x+x;
        vertices[2*i+1].y=poligono[i].y+y;
        vertices[2*i+1].z=poligono[i].z+z;
    }
    
// tratamiento de las caras 

    int num_Caras = num_aux * 2;
    caras.resize(num_Caras);
    int c=0;
    for (i=0;i<num_aux;i++){
        caras[c]._0=i*2;
        caras[c]._1=(i*2+2)%(num_Caras);
        caras[c]._2=i*2+1;    
        c=c+1;
        caras[c]._0=(i*2+2)%(num_Caras);
        caras[c]._1=(i*2+2)%(num_Caras)+1;
        caras[c]._2=i*2+1;    
        c=c+1;    
    }   
    
    colores_caras.resize(num_Caras);
    /*srand(10);
    for(int i = 0; i < num_Caras; i++){
        colores_caras[i].r = rand()%1000/1000.0;
        colores_caras[i].g = rand()%1000/1000.0;
        colores_caras[i].b = rand()%1000/1000.0;
    }*/
    calcular_normales_caras();

    colors_lambert_c(0, 20, 20, 1.0, 0.8, 0);
}

//************************************************************************
// Clase rotación PLY
//************************************************************************

_rotacion_PLY::_rotacion_PLY(){}

void _rotacion_PLY::parametros_PLY(char *archivo, int num) {
	int n_ver, n_car;
	vector<_vertex3f> perfil;
	vector<float> ver_ply;
	vector<int>   car_ply;
	
	_file_ply::read(archivo, ver_ply, car_ply );

	n_ver=ver_ply.size()/3;

	printf("Number of vertices=%d\nNumber of faces=%d\n", n_ver, n_car);

	vertices.resize(n_ver);

	//vértices
	for(int i=0; i < n_ver; i++){
		vertices[i].x=ver_ply[3*i];
		vertices[i].y=ver_ply[3*i+1];
		vertices[i].z=ver_ply[3*i+2];
		perfil.push_back(vertices[i]);
	}

    parametros(perfil, num, 0, 1, 0);
}

//************************************************************************
// práctica 3, objeto jerárquico articulado
//************************************************************************

//************************************************************************
// piezas
//************************************************************************

//************************************************************************
// pala
//************************************************************************

_pala::_pala(float radio, float ancho, int num) {
    vector<_vertex3f> perfil;
    _vertex3f vertice_aux;
    _vertex3i cara_aux;
    int i, j;

    vertice_aux.x=radio; vertice_aux.y=0; vertice_aux.z=-ancho/2.0;
    perfil.push_back(vertice_aux);
    vertice_aux.z=ancho/2.0;
    perfil.push_back(vertice_aux);

// tratamiento de los vértices

    for (j=0;j<=num;j++){
            for (i=0;i<2;i++){
            vertice_aux.x=perfil[i].x*cos(M_PI*j/(1.0*num))-
                            perfil[i].y*sin(M_PI*j/(1.0*num));
            vertice_aux.y=perfil[i].x*sin(M_PI*j/(1.0*num))+
                            perfil[i].y*cos(M_PI*j/(1.0*num));
            vertice_aux.z=perfil[i].z;
            vertices.push_back(vertice_aux);
        }
    }
  
// tratamiento de las caras 

    for (j=0;j<num;j++){
        cara_aux._0=j*2;
        cara_aux._1=(j+1)*2;
        cara_aux._2=(j+1)*2+1;
        caras.push_back(cara_aux);
        
        cara_aux._0=j*2;
        cara_aux._1=(j+1)*2+1;
        cara_aux._2=j*2+1;
        caras.push_back(cara_aux);
    }
   
// tapa inferior
    vertice_aux.x=0;
    vertice_aux.y=0;
    vertice_aux.z=-ancho/2.0;
    vertices.push_back(vertice_aux); 

    for (j=0;j<num;j++){
        cara_aux._0=j*2;
        cara_aux._1=(j+1)*2;
        cara_aux._2=vertices.size()-1;
        caras.push_back(cara_aux);
    }
  
// tapa superior
    vertice_aux.x=0;
    vertice_aux.y=0;
    vertice_aux.z=ancho/2.0;
    vertices.push_back(vertice_aux); 

    for (j=0;j<num;j++){
        cara_aux._0=j*2+1;
        cara_aux._1=(j+1)*2+1;
        cara_aux._2=vertices.size()-1;
        caras.push_back(cara_aux);
    }
  
    colors_chess(1.0,1.0,0.0,0.0,0.0,1.0);
}

//************************************************************************
// brazo
//************************************************************************

_brazo::_brazo() {
    ancho=0.6;
    alto=0.1;
    fondo=0.1;
    colors_chess(1.0,1.0,0.0,0.0,0.0,1.0);
};

void _brazo::draw(_modo modo, float r, float g, float b, float grosor) {
    glPushMatrix();
    glScalef(ancho, alto, fondo);
    glTranslatef(0.5,0,0);
    cubo.draw(modo, r, g, b, grosor);
    glPopMatrix();
};

//************************************************************************
// cabina
//************************************************************************

_cabina::_cabina() {                                                       // En el constructor es donde se cambian los valores del objeto grande.
    ancho=0.4;
    alto=0.6;
    fondo=0.4;
    cubo.colors_chess(1.0,1.0,0.0,0.0,0.0,1.0);
};

void _cabina::draw(_modo modo, float r, float g, float b, float grosor) {
    glPushMatrix();
    glScalef(ancho, alto, fondo);
    cubo.draw(modo, r, g, b, grosor);
    glPopMatrix();
};

//************************************************************************
// sustentación
//************************************************************************

_sustentacion::_sustentacion() {
    ancho=1.2;
    alto=0.3;
    fondo=0.8;
    radio=0.15;
    base.colors_chess(1.0,1.0,0.0,0.0,0.0,1.0);
};

void _sustentacion::draw(_modo modo, float r, float g, float b, float grosor) {
    glPushMatrix();                                         // Cilindro 1. (Rueda)
    glTranslatef(2*ancho/6,-alto/2.0,0);
    glRotatef(90,1,0,0);
    glScalef(radio, fondo/2.2, radio);
    rueda.draw(modo, r, g, b, grosor);
    glPopMatrix();
    
    glPushMatrix();                                         // Cilindro 2. (Rueda)
    glTranslatef(-2*ancho/6,-alto/2.0,0);
    glRotatef(90,1,0,0);
    glScalef(radio, fondo/2.2, radio);
    rueda.draw(modo, r, g, b, grosor);
    glPopMatrix();

    glPushMatrix();
    glScalef(ancho, alto, fondo);
    base.draw(modo, r, g, b, grosor);
    glPopMatrix();
};

//************************************************************************
// excavadora (montaje del objeto final)
//************************************************************************

_excavadora::_excavadora() {
    giro_cabina = 0.0;
    giro_primer_brazo = 0.0;
    giro_primer_brazo_max = 0;
    giro_primer_brazo_min = -90;
    giro_segundo_brazo = 0.0;
    giro_segundo_brazo_max = 30;
    giro_segundo_brazo_min = 0;
    giro_pala = 0.0;
    giro_pala_max = 50.0;
    giro_pala_min = -90.0;

    tamanio_pala=0.15;
};

void _excavadora::draw(_modo modo, float r, float g, float b, float grosor) {
    glPushMatrix();

    sustentacion.draw(modo, r, g, b, grosor);

    glTranslatef(0,(cabina.alto+sustentacion.alto)/2.0,0);
    glRotatef(giro_cabina,0,1,0);
    cabina.draw(modo, r, g, b, grosor);

    glTranslatef(cabina.ancho/2.0,0,0);
    glRotatef(giro_segundo_brazo,0,0,1);
    brazo.draw(modo, r, g, b, grosor);

    glTranslatef(brazo.ancho,0,0);
    glRotatef(giro_primer_brazo,0,0,1);
    brazo.draw(modo, r, g, b, grosor);

    glTranslatef(brazo.ancho,0,0);
    glRotatef(giro_pala,0,0,1);
    glTranslatef(tamanio_pala,0,0);
    glScalef(tamanio_pala, tamanio_pala, tamanio_pala);
    pala.draw(modo, r, g, b, grosor);

    glPopMatrix();
};

/////////////////////////////////////////////////////////////////
//                          COCHE                              //
/////////////////////////////////////////////////////////////////

_coche::_coche() {
    giro_puerta1 = 0;
    giro_puerta2 = 0;
    giro_puerta3 = 0;
    giro_ruedas = 0;
    giro_ruedas_delanteras = 0;
    giro_antena = 0;
    luz_encendida = false;
    anima = false;
    der = true;
    time = 0;

//  SELECCIÓN

    int color=80;
    piezas=2;
    grosor_select=2;
    color_pick=_vertex3f(1.0,0.0,0.0); 
    color_select.resize(piezas);
    activo.resize(piezas);

    for (int i=0;i<piezas;i++){
        activo[i]=0;
        color_select[i].r=color_select[i].g=color_select[i].b=color;
        color+=20;
    }
}

_ruedas::_ruedas() {
    ancho = 4;
    alto = 2;
    fondo = 2;
    radio = 0.6;
    // rueda1.colors_chess(0.1,0.1,0.3,0.1,0.1,0.4);
    // rueda2.colors_chess(0.1,0.1,0.3,0.1,0.1,0.4);
    // rueda3.colors_chess(0.1,0.1,0.3,0.1,0.1,0.4);
    // rueda4.colors_chess(0.1,0.1,0.3,0.1,0.1,0.4);
    rueda1.colors_chess(0,0,0,0.5,0.5,0.5);
    rueda2.colors_chess(0,0,0,0.5,0.5,0.5);
    rueda3.colors_chess(0,0,0,0.5,0.5,0.5);
    rueda4.colors_chess(0,0,0,0.5,0.5,0.5);
    techo.colors_chess(0.7,0.7,0.7,0.7,0.7,0.7);
    luz.colors_chess(0,0,0,0,0,0);
    luz_trasera_derecha.colors_chess(1,0,0,1,0.1,0.1);
    luz_trasera_izquierda.colors_chess(1,0,0,1,0.1,0.1);
}

_sustentacioncoche::_sustentacioncoche() {
    ancho = 3;
    alto = 2;
    fondo = 2;
    radio = 2;
    base.colors_chess(0.6,0.6,0.6,0.5,0.5,0.5);
    puerta.colors_chess(0.1,0.4,0.1,0.1,0.4,0.1);
    ventanilla.colors_chess(0,0,1,0,0.2,1);
}

void _ruedas::draw(_modo modo, float r, float g, float b, float grosor,
                    float giro_ruedas, float giro_ruedas_delanteras, float giro_antena, bool luz_encendida) {

    glPushMatrix();                                         // Rueda alante izquierda. (Rueda)
    glTranslatef(2*ancho/5,0,fondo/2);                      // Posición    
    glRotatef(90,1,0,0);
    glRotatef(giro_ruedas_delanteras,0,0,1);
    glRotatef(giro_ruedas,0,1,0);
    glScalef(radio, fondo/2.2, radio);                      // Tamaño
    rueda1.draw(modo, r, g, b, grosor);
    glPopMatrix();
    
    glPushMatrix();                                         // Rueda atrás derecha. (Rueda)
    glTranslatef(-2*ancho/5,0,fondo/2 + 0.6);
    glRotatef(90,1,0,0);
    glRotatef(giro_ruedas,0,1,0);
    glScalef(radio, fondo/2.2, radio);
    rueda1.draw(modo, r, g, b, grosor);
    glPopMatrix();

    glPushMatrix();                                         // Rueda alante derecha. (Rueda)
    glTranslatef(2*ancho/5,0,-fondo/2);                     // Posición    
    glRotatef(90,1,0,0);
    glRotatef(giro_ruedas_delanteras,0,0,1);
    glRotatef(giro_ruedas,0,1,0);
    glScalef(radio, fondo/2.2, radio);                      // Tamaño
    rueda1.draw(modo, r, g, b, grosor);
    glPopMatrix();
    
    glPushMatrix();                                         // Rueda atrás izquierda. (Rueda)
    glTranslatef(-2*ancho/5,0,-fondo/2 - 0.6);
    glRotatef(90,1,0,0);
    glRotatef(giro_ruedas,0,1,0);
    glScalef(radio, fondo/2.2, radio);
    rueda1.draw(modo, r, g, b, grosor);
    glPopMatrix();

    rueda1.ambiente_difuso = _vertex4f(0.5, 0.5, 0.5, 1.0);
    rueda1.especular= _vertex4f(0.5, 0.5, 0.5, 1.0);
    rueda1.brillo = 40;

// CILINDROS CABINA

    glPushMatrix();                                         // Cilindro 1 techo.
    glTranslatef(-2*ancho/9.305,alto,fondo/2);
    glRotatef(90,0,0,1);
    glScalef(radio, ancho*1.5, radio);
    techo.draw(modo, r, g, b, grosor);
    glPopMatrix();

    glPushMatrix();                                         // Cilindro 2 techo.
    glTranslatef(-2*ancho/9.305,alto,-fondo/2);
    glRotatef(90,0,0,1);
    glScalef(radio, ancho*1.5, radio);
    techo.draw(modo, r, g, b, grosor);
    glPopMatrix();

// CILINDROS CAPÓ

    glPushMatrix();                                         // Cilindro 1 capó.
    glTranslatef(2*ancho/7,alto-0.6,fondo/2 - 0.3);
    glRotatef(90,0,0,1);
    glScalef(radio/2, ancho, radio/2);
    techo.draw(modo, r, g, b, grosor);
    glPopMatrix();

    glPushMatrix();                                         // Cilindro 2 capó.
    glTranslatef(2*ancho/7,alto-0.6,-fondo/2 + 0.3);
    glRotatef(90,0,0,1);
    glScalef(radio/2, ancho, radio/2);
    techo.draw(modo, r, g, b, grosor);
    glPopMatrix();
    
    techo.ambiente_difuso = _vertex4f(0.8, 0.2, 0.1, 1.0);
    techo.especular= _vertex4f(0.0, 0.5, 0.5, 1.0);
    techo.brillo = 40;

// LUCES

    glPushMatrix();                                         // Luz izquierda.
    glTranslatef(ancho/2+0.2,alto-1,-fondo/4);
    glRotatef(90,0,0,1);
    glScalef(radio/4, 0.1, radio/4);
    if(luz_encendida){
        luz.colors_chess(1,1,1,1,1,1);
        luz.ambiente_difuso = _vertex4f(1.0, 1.0, 1.0, 1.0); 
        luz.especular = _vertex4f(1.0, 1.0, 1.0, 1.0); 
        luz.brillo = 100;
    }
    else{
        luz.colors_chess(0,0,0,0,0,0);
        luz.ambiente_difuso = _vertex4f(0.0, 0.0, 0.0, 1.0); 
        luz.especular = _vertex4f(0.0, 0.0, 0.0, 1.0); 
        luz.brillo = 0;
    }
    luz.draw(modo, r, g, b, grosor);
    glPopMatrix();

    glPushMatrix();                                         // Luz derecha.
    glTranslatef(ancho/2+0.2,alto-1,fondo/4);
    glRotatef(90,0,0,1);
    glScalef(radio/4, 0.1, radio/4);
    luz.draw(modo, r, g, b, grosor);
    glPopMatrix();

    glPushMatrix();                                         // Luz trasera izquierda.
    glTranslatef(-ancho/2 - 0.42,alto-1.5,-fondo/2 - 0.3);
    glRotatef(90,0,0,1);
    glScalef(radio/2, 0.1, radio/4);
    if(luz_encendida or giro_ruedas_delanteras < -20){
        luz_trasera_izquierda.colors_chess(1,0.5,0,1,0.5,0);
        luz_trasera_izquierda.ambiente_difuso = _vertex4f(1.0, 0.5, 0.0, 1.0); 
        luz_trasera_izquierda.especular = _vertex4f(1.0, 0.5, 0.0, 1.0); 
        luz_trasera_izquierda.brillo = 65;
    }
    else{
        luz_trasera_izquierda.colors_chess(1,0,0,1,0.1,0.1);
        luz_trasera_izquierda.ambiente_difuso = _vertex4f(1.0, 0.0, 0.0, 1.0); 
        luz_trasera_izquierda.especular = _vertex4f(1.0, 0.5, 0.5, 1.0); 
        luz_trasera_izquierda.brillo = 65;
    }
    luz_trasera_izquierda.draw(modo, r, g, b, grosor);
    glPopMatrix();

    glPushMatrix();                                         // Luz trasera derecha.
    glTranslatef(-ancho/2 - 0.42,alto-1.5,fondo/2 + 0.3);
    glRotatef(90,0,0,1);
    glScalef(radio/2, 0.1, radio/4);
    if(luz_encendida or giro_ruedas_delanteras > 20){
        luz_trasera_derecha.colors_chess(1,0.5,0,1,0.5,0);
        luz_trasera_derecha.ambiente_difuso = _vertex4f(1.0, 0.5, 0.0, 1.0); 
        luz_trasera_derecha.especular = _vertex4f(0.0, 0.5, 0.5, 1.0); 
        luz_trasera_derecha.brillo = 65;
    }
    else{
        luz_trasera_derecha.colors_chess(1,0,0,1,0.1,0.1);
        luz_trasera_derecha.ambiente_difuso = _vertex4f(1.0, 0.0, 0.0, 1.0); 
        luz_trasera_derecha.especular = _vertex4f(0.5, 0.5, 0.5, 1.0); 
        luz_trasera_derecha.brillo = 65;
    }
    luz_trasera_derecha.draw(modo, r, g, b, grosor);
    glPopMatrix();

// ANTENA

    glPushMatrix();                                         // Cilindro 1 capó.
    glTranslatef(-3.5*ancho/7,alto,fondo/2);
    glRotatef(giro_antena,0,0,1);
    glTranslatef(0,alto/2,0);                               // Cambia 
    glScalef(radio/11, ancho/2, radio/11);
    techo.draw(modo, r, g, b, grosor);
    glPopMatrix();

}

void _sustentacioncoche::draw(_modo modo, float r, float g, float b, float grosor,
                              float giro_puerta1, float giro_puerta2, float giro_puerta3) {
    
//CABINA

    glPushMatrix();                                         // Cabina
    glTranslatef(-2*ancho/7,0,0);
    glScalef(ancho, alto, fondo);
    base.draw(modo, r, g, b, grosor);
    glPopMatrix();

    glPushMatrix();                                         // Cabina
    glTranslatef(-2*ancho/7, alto,0);
    glScalef(ancho, 0.6, fondo);
    base.draw(modo, r, g, b, grosor);
    glPopMatrix();

    glPushMatrix();                                         // Cabina
    glTranslatef(-2*ancho/7,0, -fondo + 0.7);
    glScalef(ancho, alto, -0.6);
    base.draw(modo, r, g, b, grosor);
    glPopMatrix();

    glPushMatrix();                                         // Cabina
    glTranslatef(-2*ancho/7,0, fondo - 0.7);
    glScalef(ancho, alto, 0.6);
    base.draw(modo, r, g, b, grosor);
    glPopMatrix();

    base.ambiente_difuso = _vertex4f(0.8, 0.3, 0.2, 1.0);
    base.especular= _vertex4f(0.0, 0.5, 0.5, 1.0);
    base.brillo = 40;

//VENTANILLAS

    glPushMatrix();                                         // Ventanilla izquierda.
    glTranslatef(-2*ancho/7,alto/1.5,fondo - 0.4);
    glRotatef(90,0,0,1);
    glScalef(ancho/6, alto/2, 0.05);
    ventanilla.draw(modo, r, g, b, grosor);
    glPopMatrix();

    glPushMatrix();                                         // Ventanilla derecha.
    glTranslatef(-2*ancho/7,alto/1.5,-fondo + 0.4);
    glRotatef(90,0,0,1);
    glScalef(ancho/6, alto/2, 0.05);
    ventanilla.draw(modo, r, g, b, grosor);
    glPopMatrix();

    glPushMatrix();                                         // Ventanilla delantera.
    glTranslatef(2*ancho/7-0.2,alto+0.05,-0.5);
    glRotatef(90,1,0,0);
    glScalef(0.05, alto/2, ancho/6);
    ventanilla.draw(modo, r, g, b, grosor);
    glPopMatrix();

    ventanilla.ambiente_difuso = _vertex4f(0.0, 0.5, 0.8, 1.0);
    ventanilla.especular= _vertex4f(0.5, 0.0, 0.5, 1.0);
    ventanilla.brillo = 40;

//CAPÓ

    glPushMatrix();                                         // Capó
    glTranslatef(2*ancho/7 + 0.55,0,0);
    glScalef(ancho/2, alto - 0.6, fondo -0.6);
    base.draw(modo, r, g, b, grosor);
    glPopMatrix();

    glPushMatrix();                                         // Capó
    glTranslatef(2*ancho/7 + 0.55,0,fondo - 1.15);
    glScalef(ancho/2, alto - 0.6, 0.3);
    base.draw(modo, r, g, b, grosor);
    glPopMatrix();

    glPushMatrix();                                         // Capó
    glTranslatef(2*ancho/7 + 0.55,0, -fondo + 1.15);
    glScalef(ancho/2, alto - 0.6, 0.3);
    base.draw(modo, r, g, b, grosor);
    glPopMatrix();

    glPushMatrix();                                         // Capó
    glTranslatef(2*ancho/7 + 0.55,alto - 0.6, 0);
    glScalef(ancho/2, 0.3, fondo -0.6);
    base.draw(modo, r, g, b, grosor);
    glPopMatrix();
    
//PUERTAS

    glPushMatrix();                                         // Puerta derecha
    glTranslatef(ancho/7 - 0.6,alto/6, fondo - 0.4);
    glRotatef(giro_puerta1, 1, 0, 0);
    glScalef(ancho/4, alto - 0.6, 0.05);
    puerta.draw(modo, r, g, b, grosor);
    glPopMatrix();

    glPushMatrix();                                         // Puerta izquierda
    glTranslatef(ancho/7 - 0.6,alto/6, -fondo + 0.4);
    glRotatef(giro_puerta2, 1, 0, 0);
    glScalef(ancho/4, alto - 0.6, 0.05);
    puerta.draw(modo, r, g, b, grosor);
    glPopMatrix();

    glPushMatrix();                                         // Puerta trasera
    glTranslatef(-ancho + 0.6,alto/6, 0);
    glRotatef(giro_puerta3, 0, 0, 1);
    glScalef(0.05, alto - 0.6, fondo/1.5);
    puerta.draw(modo, r, g, b, grosor);
    glPopMatrix();

    puerta.ambiente_difuso = _vertex4f(0.5, 0.5, 0.0, 1.0);
    puerta.especular= _vertex4f(0.5, 0.1, 0.1, 1.0);
    puerta.brillo = 40;
}

void _coche::draw(_modo modo, float r, float g, float b, float grosor) {
    float r_p,g_p,b_p;
    r_p=color_pick.r;
    g_p=color_pick.g;
    b_p=color_pick.b;

    glPushMatrix();
    if (activo[0]==1) sustentacion.draw(EDGES, r_p, g_p, b_p, grosor_select, giro_puerta1, giro_puerta2, giro_puerta3);
    else sustentacion.draw(modo, r, g, b, grosor, giro_puerta1, giro_puerta2, giro_puerta3);

    if (activo[1]==1) ruedas.draw(EDGES, r_p, g_p, b_p, grosor_select, giro_ruedas, giro_ruedas_delanteras, giro_antena, luz_encendida);
    else ruedas.draw(modo, r, g, b, grosor, giro_ruedas, giro_ruedas_delanteras, giro_antena, luz_encendida);

    glPopMatrix();
}

void _coche::seleccion() {

    _vertex3i color;
    float r_p,g_p,b_p;

    glPushMatrix();
    color = color_select[0];
    sustentacion.draw(SELECT, color.r, color.g, color.b, 1, giro_puerta1, giro_puerta2, giro_puerta3);

    color = color_select[1];
    ruedas.draw(SELECT, color.r, color.g, color.b, 1, giro_ruedas, giro_ruedas_delanteras, giro_antena, luz_encendida);

    glPopMatrix();
}

/////////////////////////////////////////////////////////////////
//                          EXAMEN 2018                        //
/////////////////////////////////////////////////////////////////

_ModeloJerarquico::_ModeloJerarquico() {
    ancho1 = 1;
    alto1 = 2;
    fondo1 = 1;
    radio1 = 0.1;
    rota1 = 0;
    rota2 = 0;
}

void _ModeloJerarquico::draw(_modo modo, float r, float g, float b, float grosor) {
    
    glPushMatrix();

    glPushMatrix();                         // NIVEL 1
    glScalef(ancho1, alto1, fondo1*2);
    nivel1.draw(modo, r, g, b, grosor);
    nivel1.colors_lambert_c(0, 20, 20, 1, 1, 1);
    glPopMatrix();

    glPushMatrix();                                     // NIVEL 2.1
    glTranslatef(ancho1+rota2/10, 5*alto1/6,0);
    glRotatef(90,0,0,1);
    glScalef(radio1, alto1+rota2, radio1);
    nivel2_1.draw(modo, r, g, b, grosor);
    glPopMatrix();

    glPushMatrix();                                                     // NIVEL 2.2
    glTranslatef(ancho1/2+alto1/2+rota2/3, 5*alto1/6,radio1);
    glRotatef(90, 1, 0, 0);
    glRotatef(rota1, 0, 1, 0);
    glScalef(radio1/2, 0.1, radio1*3);
    nivel2_2.draw(modo, r, g, b, grosor);
    glPopMatrix();

    glPopMatrix();
}