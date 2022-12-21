//**************************************************************************
// Práctica 2 
//**************************************************************************

#include <GL/glut.h>
#include <ctype.h>
#include <math.h>
#include <vector>
#include "objetos_B5.h"

using namespace std;

// tipos
typedef enum{CUBO, PIRAMIDE, OBJETO_PLY, ROTACION, EXTRUSION, CILINDRO, CONO, ESFERA, ROTACION_PLY, EXCAVADORA, COCHE, PIRAMIDEXAMEN, ESFERADOBLE
            , MODELOJERARQUICO} _tipo_objeto;
_tipo_objeto t_objeto=CUBO;
_modo   modo=POINTS;

// variables que definen la posicion de la camara en coordenadas polares
GLfloat Observer_distance;
GLfloat Observer_angle_x;
GLfloat Observer_angle_y;

// variables que controlan la ventana y la transformacion de perspectiva
GLfloat Size_x,Size_y,Front_plane,Back_plane;

// variables que determninan la posicion y tamaño de la ventana X
int Window_x=50,Window_y=50,Window_width=650,Window_high=650, mov_camara = 0;
bool luz_2 = false;

// objetos
_cubo cubo;
_piramide piramide(0.85,1.3);
_objeto_ply ply; 
_rotacion rotacion; 
_extrusion *extrusion;
_cilindro cilindro(1,2,6);
_cono cono(0.5, 1, 10);
_esfera esfera(0.7, 8, 8);
_rotacion_PLY objetoPLY;
_excavadora excavadora;
_coche coche;
_piramideExamen piramide2(0.85,1.3);
_esferaDoble esfera2(0.4, 0.6, 25,25);
_ModeloJerarquico modelo;

// _objeto_ply *ply;
int estadoRaton, xc, yc;
float factor = 1.0;
int Ancho = Window_width, Alto = Window_high, cambio = 0;

//**************************************************************************
//
//***************************************************************************

void clean_window(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

//**************************************************************************
// Función para definir la transformación de proyección
//***************************************************************************

void change_projection(){

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // formato(x_minimo,x_maximo, y_minimo, y_maximo,plano_delantero, plano_traser)
    //  plano_delantero>0  plano_trasero>PlanoDelantero)
    glFrustum(-Size_x*factor,Size_x*factor,-Size_y*factor,Size_y*factor,Front_plane,Back_plane);
}

//**************************************************************************
// Función para definir la transformación*ply1 de vista (posicionar la cámara)
//***************************************************************************

void change_observer(){
    // posición del observador
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0,0,-Observer_distance);
    glRotatef(Observer_angle_x,1,0,0);
    glRotatef(Observer_angle_y,0,1,0);
}

//**************************************************************************
// Función que dibuja los ejes utilizando la primitiva grafica de lineas
//***************************************************************************

void draw_axis() {
    glDisable(GL_LIGHTING);
    glLineWidth(2);
    glBegin(GL_LINES);
    // eje X, color rojo
    glColor3f(1,0,0);
    glVertex3f(-AXIS_SIZE,0,0);
    glVertex3f(AXIS_SIZE,0,0);
    // eje Y, color verde
    glColor3f(0,1,0);
    glVertex3f(0,-AXIS_SIZE,0);
    glVertex3f(0,AXIS_SIZE,0);
    // eje Z, color azul
    glColor3f(0,0,1);
    glVertex3f(0,0,-AXIS_SIZE);
    glVertex3f(0,0,AXIS_SIZE);
    glEnd();
}

//**************************************************************************
// Función que dibuja los objetos
//**************************************************************************

void draw_objects(){
    switch (t_objeto){
        case CUBO: cubo.draw(modo,1.0,0.0,0.0,5);break;
        case PIRAMIDE: piramide.draw(modo,1.0,0.0,0.0,5);break;
        case OBJETO_PLY: ply.draw(modo,1.0,0.6,0.0,5);break;
        case ROTACION: rotacion.draw(modo,1.0,0.0,0.0,5);break;
        case EXTRUSION: extrusion->draw(modo,1.0,0.0,0.0,5);break;
        case ESFERA: esfera.draw(modo,1.0,0.0,0.0,5);break;
        case CONO: cono.draw(modo,1.0,0.0,0.0, 5);break;
        case CILINDRO: cilindro.draw(modo,1.0,0.0,0.0,5);break;
        case ROTACION_PLY: ply.draw(modo, 1.0, 0.0, 0.0, 5); break;
        case EXCAVADORA: excavadora.draw(modo,1.0,0.0,0.0,5);break;
        case COCHE: coche.draw(modo,1.0,0.0,0.0,5);break;
        case PIRAMIDEXAMEN: piramide2.draw(modo,1.0,0.0,0.0,5);break;
        case ESFERADOBLE: esfera2.draw(modo,1.0,0.0,0.0,5); break;
        case MODELOJERARQUICO: modelo.draw(modo,1.0,0.0,0.0,5); break;
	}
}

void vista_orto(){
    // ALZADO
    glViewport(Ancho/2, Alto/2, Ancho/2, Alto/2);       // Definir alto y ancho.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-5*factor, 5*factor, -5*factor, 5*factor, -100, 100);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    draw_axis();
    draw_objects();

    // PLANTA
    glViewport(0, Alto/2, Ancho/2, Alto/2);       // Definir alto y ancho.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-5*factor, 5*factor, -5*factor, 5*factor, -100, 100);

    glRotatef(90, 1, 0, 0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    draw_axis();
    draw_objects();

    // PERFIL.
    glViewport(0, 0, Ancho/2, Alto/2);       // Definir alto y ancho.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-5*factor, 5*factor, -5*factor, 5*factor, -100, 100);

    glRotatef(90, 0, 1, 0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    draw_axis();
    draw_objects();
}

//**************************************************************************
//  LUCES
//***************************************************************************

void luces(float alpha){
    GLfloat luz_ambiente[]  = {0.2, 0.2, 0.2, 1.0},
            luz_difusa[]  = {1.0, 1.0, 1.0, 1.0},
            luz_especular[] = {1.0, 0.0, 1.0, 1.0},
            luz_posicion[] = {0.0, 20.0, 20.0, 1.0},
            luz_ambiente2[]  = {0.6, 0.6, 0.6, 1.0},
            luz_difusa2[]  = {0.5, 0.5, 0.5, 1.0},
            luz_especular2[] = {0.2, 0.1, 0.2, 1.0};

    if(!luz_2){
        glLightfv(GL_LIGHT1, GL_AMBIENT, luz_ambiente);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, luz_difusa);
        glLightfv(GL_LIGHT1, GL_SPECULAR, luz_especular);
        glLightfv(GL_LIGHT1, GL_POSITION, luz_posicion);
    }
    else {
        glLightfv(GL_LIGHT1, GL_AMBIENT, luz_ambiente2);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, luz_difusa2);
        glLightfv(GL_LIGHT1, GL_SPECULAR, luz_especular2);

        glPushMatrix();

        glRotatef(alpha, 0, 1, 0);
        glLightfv(GL_LIGHT1, GL_POSITION, luz_posicion);

        glPopMatrix();
    }

    glEnable(GL_LIGHT1);
    glDisable(GL_LIGHT0);
} 

//**************************************************************************
//
//***************************************************************************

void draw(void) {
    glDrawBuffer(GL_FRONT);
    clean_window();
    //change_observer();
    luces(mov_camara);
    //draw_axis();
    //draw_objects();
    // glutSwapBuffers();

    if (cambio == 0) {
        glViewport(0, 0, Ancho, Alto);

        change_projection();
        change_observer();
        draw_axis();
        draw_objects();
    }
    else
        vista_orto();

    if (t_objeto == COCHE) {
        glDrawBuffer(GL_BACK);
        clean_window();

        if(cambio == 0) {
            change_projection();
            change_observer();
            coche.seleccion();
        }
    }
    glFlush();  
}

//***************************************************************************
// Función llamada cuando se produce un cambio en el tamaño de la ventana
//
// El evento manda a la función:
// nuevo ancho
// nuevo alto
//***************************************************************************

void change_window_size(int Ancho1,int Alto1) {
    float Aspect_ratio;
    Ancho = Ancho1;
    Alto = Alto1;

    Aspect_ratio=(float) Alto1/(float )Ancho1;
    Size_y=Size_x*Aspect_ratio;
    change_projection();
    glViewport(0,0,Ancho1,Alto1);
    glutPostRedisplay();
}

void procesar_color(unsigned char color[3]) {
    int i;

    for (i=0;i<coche.piezas;i++) {
        if(color[0]==coche.color_select[i].r && color[1]==coche.color_select[i].g && color[2]==coche.color_select[i].r){
            if (coche.activo[i]==0) 
                coche.activo[i]=1;
            else 
                coche.activo[i]=0;
            glutPostRedisplay();
        }
    }                
}

//***************************************************************************

void pick_color(int x, int y) {
    GLint viewport[4];
    unsigned char pixel[3];

    glGetIntegerv(GL_VIEWPORT, viewport);
    glReadBuffer(GL_BACK);
    glReadPixels(x,viewport[3]-y,1,1,GL_RGB,GL_UNSIGNED_BYTE,(GLubyte *) &pixel[0]);
    printf(" valor x %d, valor y %d, color %d, %d, %d \n",x,y,pixel[0],pixel[1],pixel[2]);

    procesar_color(pixel);
}

void clickRaton( int boton, int estado, int x, int y ) {
    if(boton==GLUT_RIGHT_BUTTON) {
        if(estado==GLUT_DOWN) {
            estadoRaton=1;
            xc=x;
            yc=y;
        }
    } 
    else estadoRaton=0;
    
    if(boton==GLUT_LEFT_BUTTON) {
        if(estado==GLUT_DOWN) {
            estadoRaton=2;
            xc=x;
            yc=y;
            pick_color(xc, yc);
        } 
    }

    if(boton==3){
        factor *= 1.1;
        glutPostRedisplay();
    }
    if(boton==4){
        factor *= 0.9;
        glutPostRedisplay();
    }
}

/*************************************************************************/

void RatonMovido( int x, int y ){ 
    if(estadoRaton==1) {
        Observer_angle_y=Observer_angle_y-(x-xc);
        Observer_angle_x=Observer_angle_x+(y-yc);
        xc=x;
        yc=y;
        glutPostRedisplay();
    }
}

//***************************************************************************
// Función llamada cuando se aprieta una tecla normal.
//
// El evento manda a la función:
// Código de la tecla.
// Posición x del ratón.
// Posición y del ratón.
//***************************************************************************

void normal_key(unsigned char Tecla1,int x,int y){
    switch (toupper(Tecla1)){
        case 'Q':exit(0);
        case '1':modo=POINTS;break;
        case '2':modo=EDGES;break;
        case '3':modo=SOLID;break;
        case '4':modo=SOLID_COLORS;break;
        case '5':modo=SOLID_FLAT;break;
        case '6':modo=SOLID_SMOOTH;break;
        case 'P':t_objeto=PIRAMIDE;break;
        case 'C':t_objeto=CUBO;break;
        case 'O':t_objeto=OBJETO_PLY;break;	
        case 'R':t_objeto=ROTACION;break;
        case 'F':t_objeto=CILINDRO;break;
        case 'D':t_objeto=CONO;break;
        case 'E':t_objeto=ESFERA;break;
        case 'X':t_objeto=EXTRUSION;break;
        case 'L':t_objeto=ROTACION_PLY;break;
        //case 'A':t_objeto=EXCAVADORA;break;
        case 'K':t_objeto=COCHE;break;
        case 'B':t_objeto=PIRAMIDEXAMEN;break;
        case 'N':t_objeto=ESFERADOBLE;break;
        case 'M':t_objeto=MODELOJERARQUICO;break;
        case 'V': 
            modelo.rota1+=5;
            break;

        case 'H':
            modelo.rota2+=0.1;
            if(modelo.rota2>=modelo.MAXIMO_CRECE)
                modelo.rota2=modelo.MAXIMO_CRECE;
            break;

        case 'J':
            modelo.rota2-=0.1;
            if(modelo.rota2<=0)
                modelo.rota2 = 0;
            break;

        case 'S':
            if(coche.anima == true)
                coche.anima = false;
            else
                coche.anima = true;
            break;

        case 'Z':
            if(cambio == 0)
                cambio++;
            else
                cambio--;

        case ',': mov_camara+=5; break;
        case '.': mov_camara-=5; break;
        case '-': if(!luz_2) luz_2 = true; else luz_2 = false; break;

	}
    glutPostRedisplay();
}

//***************************************************************************
// Función l-olamada cuando se aprieta una tecla especial.
//
// El evento manda a la función:
// Código de la tecla.
// Posición x del ratón.
// Posición y del ratón.

//***************************************************************************

void animacioncoche(){
    if(coche.anima){
        int velocidad = 1;

        coche.giro_ruedas -= velocidad;

        coche.giro_antena += velocidad;

        if(coche.giro_antena >= coche.GIRO_MAXIMO_ANTENA)
            coche.giro_antena = coche.GIRO_MAXIMO_ANTENA;
        
        if(coche.der and coche.time > 400 and coche.time < 1000){
            coche.giro_ruedas_delanteras += velocidad;
            if(coche.giro_ruedas_delanteras >= coche.GIRO_MAXIMO_RUEDAS){
                coche.giro_ruedas_delanteras = coche.GIRO_MAXIMO_RUEDAS;
                coche.der = false;
            }
        }
        else if(coche.time > 1500){
            coche.giro_ruedas_delanteras -= velocidad;
            if(coche.giro_ruedas_delanteras <= -coche.GIRO_MAXIMO_RUEDAS){
                coche.giro_ruedas_delanteras = -coche.GIRO_MAXIMO_RUEDAS;
                coche.der = true;
                coche.time = 0;
            }
        }
    }
    coche.time++;
    glutPostRedisplay();
}

void special_key(int Tecla1,int x,int y){

switch (Tecla1){
        case GLUT_KEY_LEFT:Observer_angle_y--;break;
        case GLUT_KEY_RIGHT:Observer_angle_y++;break;
        case GLUT_KEY_UP:Observer_angle_x--;break;
        case GLUT_KEY_DOWN:Observer_angle_x++;break;
        case GLUT_KEY_PAGE_UP:Observer_distance*=1.2;break;             
        case GLUT_KEY_PAGE_DOWN:Observer_distance/=1.2;break;
            
        case GLUT_KEY_F1: coche.giro_ruedas_delanteras+=5;
                if (coche.giro_ruedas_delanteras > coche.GIRO_MAXIMO_RUEDAS)
                    coche.giro_ruedas_delanteras=coche.GIRO_MAXIMO_RUEDAS;break;
        case GLUT_KEY_F2: coche.giro_ruedas_delanteras-=5;
                if (coche.giro_ruedas_delanteras < -coche.GIRO_MAXIMO_RUEDAS)
                    coche.giro_ruedas_delanteras=-coche.GIRO_MAXIMO_RUEDAS;break;
        case GLUT_KEY_F3: coche.giro_ruedas-=5; break;
    
        case GLUT_KEY_F4:coche.giro_puerta1+=5;
                if (coche.giro_puerta1 > coche.GIRO_MAXIMO_PUERTAS)
                    coche.giro_puerta1 = coche.GIRO_MAXIMO_PUERTAS;break;
        case GLUT_KEY_F5:coche.giro_puerta1-=5;
                if (coche.giro_puerta1 < 0)
                    coche.giro_puerta1 = 0 ;break;
        case GLUT_KEY_F6:coche.giro_puerta2-=5;
                if (coche.giro_puerta2 < -coche.GIRO_MAXIMO_PUERTAS)
                    coche.giro_puerta2 = -coche.GIRO_MAXIMO_PUERTAS;break;
        case GLUT_KEY_F7:coche.giro_puerta2+=5;
                if (coche.giro_puerta2 > 0)
                    coche.giro_puerta2 = 0 ;break;
        case GLUT_KEY_F8:coche.giro_puerta3+=5;
                if (coche.giro_puerta3 > coche.GIRO_MAXIMO_PUERTAS)
                    coche.giro_puerta3 = coche.GIRO_MAXIMO_PUERTAS;break;
        case GLUT_KEY_F9:coche.giro_puerta3-=5;
                if (coche.giro_puerta3 < 0)
                    coche.giro_puerta3 = 0 ;break;
        case GLUT_KEY_F10:coche.giro_antena+=5;
                if (coche.giro_antena > coche.GIRO_MAXIMO_ANTENA)
                    coche.giro_antena = coche.GIRO_MAXIMO_ANTENA;break;
        case GLUT_KEY_F11:coche.giro_antena-=5;
                if (coche.giro_antena < -coche.GIRO_MAXIMO_ANTENA)
                    coche.giro_antena = -coche.GIRO_MAXIMO_ANTENA;break;
        case GLUT_KEY_F12:
            if (coche.luz_encendida)
                coche.luz_encendida = false;
            else
                coche.luz_encendida = true;       
            break;
    }
    glutPostRedisplay();
}

//***************************************************************************
// Función de incialización.
//***************************************************************************

void initialize(void) {
    // Se inicalizan la ventana y los planos de corte.
    Size_x=0.5;
    Size_y=0.5;
    Front_plane=1;
    Back_plane=1000;

    // Se incia la posición del observador, en el eje z.
    Observer_distance=4*Front_plane;
    Observer_angle_x=0;
    Observer_angle_y=0;

    // Se indica el color para limpiar la ventana	(r,v,a,al)
    // blanco=(1,1,1,1) rojo=(1,0,0,1), ...
    glClearColor(1,1,1,1);

    // Se habilita el z-bufer.
    glEnable(GL_DEPTH_TEST);
    change_projection();
    glViewport(0,0,Window_width,Window_high);
}

//***************************************************************************
// Programa principal
//
// Se encarga de iniciar la ventana, asignar las funciones e comenzar el
// bucle de eventos.
//***************************************************************************

int main(int argc, char *argv[] ){
    // Perfil 

    vector<_vertex3f> perfil, poligono;
    _vertex3f aux;

    aux.x=1.0; aux.y=-1.0; aux.z=0.0;
    perfil.push_back(aux);
    aux.x=1.5; aux.y=0.0; aux.z=0.0;
    perfil.push_back(aux);
    aux.x=1.5; aux.y=1.2; aux.z=0.0;
    perfil.push_back(aux);
    aux.x=1.0; aux.y=1.0; aux.z=0.0;
    perfil.push_back(aux);

    rotacion.parametros(perfil,6, 1,1,1);

    aux.x=1.0; aux.y=0.0; aux.z=1.0;
    poligono.push_back(aux);
    aux.x=1.0; aux.y=0.0; aux.z=-1.0;
    poligono.push_back(aux);
    aux.x=-1.2; aux.y=0.0; aux.z=-1.0;
    poligono.push_back(aux);
    aux.x=-0.8; aux.y=0.0; aux.z=0.0;
    poligono.push_back(aux);
    aux.x=-1.2; aux.y=0.0; aux.z=1.0;
    poligono.push_back(aux);

    extrusion= new _extrusion(poligono, 0.25, 1.0, 0.25);

    // Se llama a la inicialización de glut.
    glutInit(&argc, argv);

    // Se indica las características que se desean para la visualización con OpenGL.
    // Las posibilidades son:
    // GLUT_SIMPLE -> memoria de imagen simple.
    // GLUT_DOUBLE -> memoria de imagen doble.
    // GLUT_INDEX -> memoria de imagen con color indizado.
    // GLUT_RGB -> memoria de imagen con componentes rojo, verde y azul para cada pixel.
    // GLUT_RGBA -> memoria de imagen con componentes rojo, verde, azul y alfa para cada pixel.
    // GLUT_DEPTH -> memoria de profundidad o z-bufer.
    // GLUT_STENCIL -> memoria de estarcido_rotation Rotation;
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    // Posición de la esquina inferior izquierdad de la ventana.
    glutInitWindowPosition(Window_x,Window_y);

    // Tamaño de la ventana (ancho y alto).
    glutInitWindowSize(Window_width,Window_high);

    // Llamada para crear la ventana, indicando el titulo (no se visualiza hasta que se llama
    // al bucle de eventos).
    glutCreateWindow("PRACTICA - 5");

    // Asignación de la función llamada "dibujar" al evento de dibujo.
    glutDisplayFunc(draw);
    // Asignación de la función llamada "change_window_size" al evento correspondiente.
    glutReshapeFunc(change_window_size);
    // Asignación de la función llamada "normal_key" al evento correspondiente.
    glutKeyboardFunc(normal_key);
    // Asignación de la función llamada "tecla_Especial" al evento correspondiente.
    glutSpecialFunc(special_key);

    // Función de inicialización.
    initialize();

	// eventos ratón.
	glutMouseFunc(clickRaton);
	glutMotionFunc(RatonMovido);

    glutIdleFunc(animacioncoche);

    // Creación del objeto ply.
    ply.parametros(argv[1]);
    objetoPLY.parametros_PLY(argv[2], 8);

    //  ply = new _objeto_ply(argv[1]);

    glutMouseFunc(clickRaton);
    glutMotionFunc(RatonMovido);

    // Inicio del bucle de eventos.
    glutMainLoop();
    return 0;
}