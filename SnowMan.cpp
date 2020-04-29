/*
 * GLUT Shapes Demo
 *
 * Written by Nigel Stewart November 2003
 *
 * This program is test harness for the sphere, cone
 * and torus shapes in GLUT.
 *
 * Spinning wireframe and smooth shaded shapes are
 * displayed until the ESC or q key is pressed.  The
 * number of geometry stacks and slices can be adjusted
 * using the + and - keys.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <windows.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

// Angulo de rotacion para la direccion de la camara
float angle = 0.0f;

// Direccion inicial de la camara XZ
float lx=0.0f,lz=-1.0f;

// Posicion inicial de la camara  XZ
float x=0.0f, z=5.0f;

//Estados del angulo, movimiento y posicion cuando
//no se esta presionando ningun control
float deltaAngle = 0.0f;
float deltaMove = 0;
int xOrigin = -1;

//Colores para la nariz del muneco de nieve
float red = 1.0f, blue=0.5f, green=0.5f;

//Escala del muneco de nieve
float scale = 1.0f;

//Fuente por defecto para la escritura
void *font = GLUT_STROKE_ROMAN;

//Dimensiones de la ventana
int h,w;
//Cadena de modo actual
char currentMode[80];

// Configuracion optima (por defecto)
char gameModeString[40] = "640x480";

void init();

void reshape(int ww, int hh) {

	h = hh;
	w = ww;
	// Validacion cuando la ventana es muy pequena
	if (h == 0)
		h = 1;

	float ratio =  w * 1.0 / h;
	// La matriz de proyección se utiliza para crear su volumen de visualizacion
	glMatrixMode(GL_PROJECTION);
	// Resetea la matriz
	glLoadIdentity();
	// Establece la vista de la ventana
	glViewport(0, 0, w, h);
	//Configura una matriz de proyeccion en perspectiva
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);
	// La matriz modelview, se utiliza para realizar diversas transformaciones
    // en los modelos (objetos) en su mundo
	glMatrixMode(GL_MODELVIEW);
}

void drawSnowMan() {
    //Multiplica la matriz actual por una escala general
	glScalef(scale, scale, scale);
	//Establece el color actual
	glColor3f(1.0f, 1.0f, 1.0f);
// Cuerpo (esfera 1)
    //Multiplica la matriz actual por una de traslacion
	glTranslatef(0.0f ,0.75f, 0.0f);
	//Representa una esfera solida (radio,n_longitud,n_latitud)
	glutSolidSphere(0.75f,20,20);

// Cabeza (esfera 2)
	glTranslatef(0.0f, 1.0f, 0.0f);
	glutSolidSphere(0.25f,20,20);

    //Cuando se usa glTranslate o glRotate afecta a la matriz de vista de modelo
    //Establece la matriz actual en la fila
	glPushMatrix();

// Ojo derecho (Esfera 3)
	glColor3f(0.0f,0.0f,0.0f);
	glTranslatef(0.05f, 0.10f, 0.18f);
	glutSolidSphere(0.05f,10,10);
// Ojo izquierdo (Esfera 4)
	glTranslatef(-0.1f, 0.0f, 0.0f);
	glutSolidSphere(0.05f,10,10);
    //Toma la matriz anterior sin transformaciones
	glPopMatrix();

// Nariz (Cono 1)
	glColor3f(red, green, blue);
	//Multiplica la matriz actual por una de rotacion
	glRotatef(0.0f,1.0f, 0.0f, 0.0f);
	//Representa un cono solido (radio_base,cortes,pilas)
	glutSolidCone(0.08f,0.5f,10,2);
	glColor3f(1.0f, 1.0f, 1.0f);
}
//Renderiza cadena de mapa de bits
void renderBitmapString(
		float x,
		float y,
		float z,
		void *font,
		char *string) {

	char *c;
	//Especifica la posicion del raster para las operaciones de pixeles.
	glRasterPos3f(x, y,z);
	for (c=string; *c != '\0'; c++) {
	    //Representa un caracter de mapa de bits
		glutBitmapCharacter(font, *c);
	}
}
//Renderiza cadena con una fuente
void renderStrokeFontString(
		float x,
		float y,
		float z,
		void *font,
		char *string) {

	char *c;
	glPushMatrix();
	glTranslatef(x, y,z);
	//Multiplica la matriz actual por una escala general
	glScalef(0.002f, 0.002f, 0.002f);
	for (c=string; *c != '\0'; c++) {
		glutStrokeCharacter(font, *c);
	}
	glPopMatrix();
}
//Restaura la perspectiva de la proyeccion
void restorePerspectiveProjection() {
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void setOrthographicProjection() {
	// Cambia a modelo proyeccion
	glMatrixMode(GL_PROJECTION);
	// Guarda la matriz que continene las
	//configuraciones para la proyeccion de la perspectiva
	glPushMatrix();
	// Resetea la matriz
	glLoadIdentity();
	// Establece una proyeccion ortografica en 2D
	gluOrtho2D(0, w, h, 0);
	// Cambia a modelo vista
	glMatrixMode(GL_MODELVIEW);
}
//Funcion para actualizar posicion
void computePos(float deltaMove) {

	x += deltaMove * lx * 0.1f;
	z += deltaMove * lz * 0.1f;
}

void display(void) {

	if (deltaMove)
		computePos(deltaMove);
	//Borra los buffers para valores predeterminados
	//Constante GL_COLOR_BUFFER_BIT,
	//indica los buffers habilitados para la escritura en color
	//Constante GL_DEPTH_BUFFER_BIT,
	//indica el buffer de profundidad
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Restea las transformaciones
	glLoadIdentity();
	//Establece la vista de la camra
	gluLookAt(	x, 1.0f, z,
			x+lx, 1.0f,  z+lz,
			0.0f, 1.0f,  0.0f);
// Suelo
	glColor3f(0.9f, 0.9f, 0.9f);
	glBegin(GL_QUADS);
		glVertex3f(-100.0f, 0.0f, -100.0f);
		glVertex3f(-100.0f, 0.0f,  100.0f);
		glVertex3f( 100.0f, 0.0f,  100.0f);
		glVertex3f( 100.0f, 0.0f, -100.0f);
	glEnd();

// Dibujar 36 munecos de nieve
	char number[3];
	for(int i = -3; i < 3; i++)
		for(int j=-3; j < 3; j++) {
			glPushMatrix();
			glTranslatef(i*10.0f, 0.0f, j * 10.0f);
			drawSnowMan();
			//Imprimir enumeracion de los munecos
			sprintf(number,"%d",(i+3)*6+(j+3));
			//Renderiza la cadena
			renderStrokeFontString(0.0f, 0.5f, 0.0f, (void *)font ,number);
			glPopMatrix();
		}
	setOrthographicProjection();
	void *font= GLUT_BITMAP_8_BY_13;
	glPushMatrix();
	glLoadIdentity();
	renderBitmapString(30,15,0,font,(char *)"Munecos de nieve 3D");
	renderBitmapString(30,30,0,font,(char *)"Instrucciones");
	renderBitmapString(30,45,0,font,(char *)"F1 - 640x480 32 bits");
	renderBitmapString(30,60,0,font,(char *)"F2 - 800x600 32 bits");
	renderBitmapString(30,75,0,font,(char *)"F3 - 1024x768 32 bits");
	renderBitmapString(30,90,0,font,(char *)"F4 - Modo Ventana");
	renderBitmapString(30,105,0,font,(char *)"Esc - Quit");
	renderBitmapString(30,120,0,font,currentMode);
	glPopMatrix();
	restorePerspectiveProjection();
	glutSwapBuffers();
}

void processNormalKeys(unsigned char key, int xx, int yy) {
//TECLA ESC (Escape)
	switch (key) {
		case 27:
			if (glutGameModeGet(GLUT_GAME_MODE_ACTIVE) != 0)
				glutLeaveGameMode();
			exit(0);
			break;
	}
}

void pressKey(int key, int xx, int yy) {

	switch (key) {
    //TECLA FLECHA ARRIBA
		case GLUT_KEY_UP : deltaMove = 0.5f; break;
    //TECLA FLECHA ABAJO
		case GLUT_KEY_DOWN : deltaMove = -0.5f; break;
    //TECLA F1
		case GLUT_KEY_F1:
			glutGameModeString("640x480:32");
			if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)) {
				glutEnterGameMode();
				sprintf(gameModeString,"640x480:32");
				init();
			}
			else
				glutGameModeString(gameModeString);
			break;
    //TECLA F2
		case GLUT_KEY_F2:
			glutGameModeString("800x600:32");
			if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)) {
				glutEnterGameMode();
				sprintf(gameModeString,"800x600:32");
				init();
			}
			else
				glutGameModeString(gameModeString);
			break;
    //TECLA F3
		case GLUT_KEY_F3:
			glutGameModeString("1024x768:32");
			if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)) {
				glutEnterGameMode();
				sprintf(gameModeString,"1024x768:32");
				init();
			}
			else
				glutGameModeString(gameModeString);
			break;
    //TECLA F4
        case GLUT_KEY_F4:
			w = 800;h = 600;
			if (glutGameModeGet(GLUT_GAME_MODE_ACTIVE) != 0) {
				glutLeaveGameMode();
			}
			break;
	}
	if (glutGameModeGet(GLUT_GAME_MODE_ACTIVE) == 0)
		sprintf(currentMode,"Modo Acual: Ventana");
	else
		sprintf(currentMode,
			"Modo Actual: %dx%d",
			glutGameModeGet(GLUT_GAME_MODE_WIDTH),
			glutGameModeGet(GLUT_GAME_MODE_HEIGHT));
}

void releaseKey(int key, int x, int y) {

	switch (key) {
		case GLUT_KEY_UP :
		case GLUT_KEY_DOWN : deltaMove = 0;break;
	}
}

void mouseMove(int x, int y) {

	// Condicion para cuando el boton izquierdo este presionado
	if (xOrigin >= 0) {

		// update deltaAngle
		deltaAngle = (x - xOrigin) * 0.001f;

		// update camera's direction
		lx = sin(angle + deltaAngle);
		lz = -cos(angle + deltaAngle);
	}
}

void mouseButton(int button, int state, int x, int y) {

	// Solo empieza el movimiento si se presiona el boton izquierdo
	if (button == GLUT_LEFT_BUTTON) {
		// Cuando suelta el boton
		if (state == GLUT_UP) {
			angle += deltaAngle;
			xOrigin = -1;
		}
		else  {// state = GLUT_DOWN
			xOrigin = x;
		}
	}
}

void init() {
	//Establece la devolución de llamada de pantalla para la ventana actual
	glutDisplayFunc(display);
	//Establece la devolución de llamada de reforma para la ventana actual
	glutReshapeFunc(reshape);
	//Establece la devolución de llamada inactiva global.(tareas en 2do plano)
	glutIdleFunc(display);
	//Establece el estado de repeticion continua de teclas, distinto de 0 para activar
	glutIgnoreKeyRepeat(1);
	//Establece la devolucion de llamada del teclado para la ventana actual
	glutKeyboardFunc(processNormalKeys);
	//Registra la devolucion la funcion de devolucion de llamada
	glutSpecialFunc(pressKey);
	//Registra la devolucion la funcion de devolucion de llamada
	glutSpecialUpFunc(releaseKey);
	//Establece la devolución de llamada del mouse para la ventana actual
	glutMouseFunc(mouseButton);
	//Establecen las devoluciones de llamada de movimiento para la ventana actual
	glutMotionFunc(mouseMove);
    //---GL_DEPTH_TEST
	//Se activa para evitar artefactos "feos" que dependen
	//del angulo de vision y el orden del dibujo
	glEnable(GL_DEPTH_TEST);
	//---GL_CULL_FACE
	//Se activa por razones de rendimiento
	//y por buenas practicas
	glEnable(GL_CULL_FACE);
}

int main(int argc, char **argv) {
	//Inicializará la biblioteca GLUT y negociará una sesión con el sistema de ventanas
	glutInit(&argc, argv);
	//Establece el modo de visualización inicial
    //Mascaras de bits:
    //GLUT_DOUBLE - Para seleccionar una ventana de doble búfer
    //GLUT_DEPTH - Para seleccionar una ventana con un búfer de profundidad.
    //GLUT_RGBA - Para seleccionar una ventana de modo RGBA
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	//Establece la posicion de la ventana
	glutInitWindowPosition(100,100);
	//Establece el tamaño de la ventana
	glutInitWindowSize(800,600);
	//Crea una ventana de nivel superior y donde el nombre
	//se le proporcionará al sistema de ventanas
	glutCreateWindow("Muñeco de Nieve");
	init();
	//Bucle de procesamiento de eventos GLUT
	glutMainLoop();
	return 1;
}
