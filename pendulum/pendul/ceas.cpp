///CEAS CU PENDUL

//(nu am mai pus glos.h pentru ca e inutil fisierul)
//sper sa te descurci, partea cu animatia am comentat-o cat de mult am inteles ce se intampla acolo
//ii spui la prof ca animatia si valorile la culori ai luat-o de pe web
//proiectul nu are textura, dar eu zic ca nu are rost sa mai pui ca si asa e stufos, cum crezi, daca e pun
//are iluminare, umbra, animatie, eu zic ca arata bine
//daca mai ai intrebari, imi zici

#include <stdlib.h>//pt eroarea de tip exit()
#include <glut.h>//echivalent cu bibliotecile gl.h si glu.h
#include <glaux.h>
#include <time.h>
#include <math.h>//pt sqrt din functia de umbre
#include <windows.h>//pt sunet
#include <mmsystem.h>//pt sunet

void myinit(void);
void CALLBACK myReshape(GLsizei w, GLsizei h);
void CALLBACK display(void);



///variabile iluminare
GLfloat mat_ambient[] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat mat_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_shininess[] = { 70.0 };
GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat lmodel_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
GLfloat light_position[] = { 1, 1, 0.4, 0.0 };//unde se afla sursa de lumina


///functii umbre
void calcCoeficientiPlan(float P[3][3], float coef[4]) {
	float v1[3], v2[3];
	float length;
	static const int x = 0, y = 1, z = 2;

	// calculeaza 2 vectori din 3 pct
	v1[x] = P[0][x] - P[1][x];
	v1[y] = P[0][y] - P[1][y];
	v1[z] = P[0][z] - P[1][z];
	v2[x] = P[1][x] - P[2][x];
	v2[y] = P[1][y] - P[2][y];
	v2[z] = P[1][z] - P[2][z];

	//se calc produsul vectorial al celor 2 vectori => al3lea vector cu componentele A,B,C chiar coef din ec. planului
	coef[x] = v1[y] * v2[z] - v1[z] * v2[y];
	coef[y] = v1[z] * v2[x] - v1[x] * v2[z];
	coef[z] = v1[x] * v2[y] - v1[y] - v2[x];

	//normalizare vector
	length = (float)sqrt(coef[x] * coef[x] + coef[y] * coef[y] + coef[z] * coef[z]);
	coef[x] /= length;
	coef[y] /= length;
	coef[z] /= length;
}

void MatriceUmbra(GLfloat puncte[3][3], GLfloat pozSursa[4], GLfloat mat[4][4]) {
	// creeaza matricea care da umbra cunoscandu-se coef planului + poz sursei
	// IN mat SE SALVEAZA MATRICEA

	GLfloat coefPlan[4], temp;

	//determina coef planului
	calcCoeficientiPlan(puncte, coefPlan);

	// determinam D
	coefPlan[3] = -(coefPlan[0] * puncte[2][0] + coefPlan[1] * puncte[2][1] + coefPlan[2] * puncte[2][2]);

	// temp= AxL + ByL + CzL + Dw
	temp = coefPlan[0] * pozSursa[0] + coefPlan[1] * pozSursa[1] + coefPlan[2] * pozSursa[2] + coefPlan[3] * pozSursa[3];

	//prima coloana
	mat[0][0] = temp - coefPlan[0] * pozSursa[0];
	mat[1][0] = 0.0f - coefPlan[1] * pozSursa[0];
	mat[2][0] = 0.0f - coefPlan[2] * pozSursa[0];
	mat[3][0] = 0.0f - coefPlan[3] * pozSursa[0];
	//a 2a coloana
	mat[0][1] = 0.0f - coefPlan[0] * pozSursa[1];
	mat[1][1] = temp - coefPlan[1] * pozSursa[1];
	mat[2][1] = 0.0f - coefPlan[2] * pozSursa[1];
	mat[3][1] = 0.0f - coefPlan[3] * pozSursa[1];
	//a 3a coloana
	mat[0][2] = 0.0f - coefPlan[0] * pozSursa[2];
	mat[1][2] = 0.0f - coefPlan[1] * pozSursa[2];
	mat[2][2] = temp - coefPlan[2] * pozSursa[2];
	mat[3][2] = 0.0f - coefPlan[3] * pozSursa[2];
	//a4a coloana
	mat[0][3] = 0.0f - coefPlan[0] * pozSursa[3];
	mat[1][3] = 0.0f - coefPlan[1] * pozSursa[3];
	mat[2][3] = 0.0f - coefPlan[2] * pozSursa[3];
	mat[3][3] = temp - coefPlan[3] * pozSursa[3];
}

//iluminarea
void myinit(void)
{	
	///apelarea vectorilor de iluminare + adancime 
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
	glFrontFace (GL_CCW);
	glEnable(GL_AUTO_NORMAL);
    glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);// culoarea datã de coeficientul de reflexie difuzã
}

//pt animatie ceas
float pendulum_time = 0.0;

//pt umbra
GLfloat matUmbra[4][4];

//planul unde se pozitioneaza umbra
GLfloat puncte[3][3] = {
	{ 50.0f, -50.0f, -226.0f },
	{ -50.0f, 50.0f, -226.0f },
	{ -50.0f, -50.0f, -226.0f } };

void CALLBACK idle(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//determina matricea pt calcularea umbrei
	MatriceUmbra(puncte, light_position, matUmbra);


	///VARIABILE CE LE-AM PRELUAT DE PE WEB CA AM GASIT UN PROIECT UNDE MERGEA PARTEA DE ANIMATIE + SA RESPECTE ORA DIN SISTEMUL LAPTOP-ULUI
	pendulum_time += 0.0006;//timpul de miscare al penduluilui
	int pendulum_angle = 15*sin(sqrt(9.8/0.8)*pendulum_time);//unghi pendul
	time_t rawtime;
	struct tm * timeinfo;
	time (&rawtime);
	timeinfo = localtime(&rawtime);//isi preia timpul din sistemul laptop-ului (localtime e functie din biblioteca time.h)
	int minuteAngle = (float)(timeinfo->tm_min)/60*360 + (float)(timeinfo->tm_sec)/60*6;//ungiul pentru minutar (setat in functie variabila timeinfo, ce preia valoare orei/minut din sistem)
	int hourAngle = (float)(timeinfo->tm_hour % 12) / 12 * 360 + (float)minuteAngle/360*30;//unghiul pentru ac ora (setat in functie variabila timeinfo, ce preia valoare orei/minut din sistem)


///obiecte

	//perete
	glPushMatrix();
	glColor3f(1, 1, 1);
	glMaterialf(GL_FRONT, GL_SHININESS, 10.0);
	glTranslatef(730, 1050, -970);
	glRotatef(90, 1, 0, 0);
	glRotatef(45, 0, 0, 1);
	gluCylinder(gluNewQuadric(), 1050, 1050, 1050, 4, 1); 
	glPopMatrix();


	glPushMatrix();
	glTranslatef(730, 800, -210);
	glRotatef(45, 0, 0, 1);
	glColor3f(0.3, 0.1, 0.0);
	glMaterialf(GL_FRONT, GL_SHININESS, 120.0);
	gluDisk(gluNewQuadric(), 0, 200, 4, 1); //patratul cel mare(cel rsoiatic, din exterior)

	glTranslatef(0,0,2);
	glColor3f(0.6, 0.25, 0.0);
	gluDisk(gluNewQuadric(), 0, 180, 4, 1); //patratul cel mic (cel portocaliu, din interior)

	glTranslatef(0,0,2);
	glColor3f(0.3, 0.1, 0.0);
	gluDisk(gluNewQuadric(), 0, 120, 36, 5); // disc rosiatic din spatele discului alb

	glTranslatef(0,0,2);
	glColor3f(1,1,0.9);
	gluDisk(gluNewQuadric(), 0, 115, 36, 5); //disc din interior cel alb

	glTranslatef(0,0,-12);
	glRotatef(pendulum_angle-45, 0, 0, 1);///miscare pendul(este apelata doar pentru pendul si sfera pendul, doar alea doua obiecte se misca prin rotatie Z)
	glRotatef(90, 1, 0, 0);
	glColor3f(0.8, 0.34, 0);
	gluCylinder(gluNewQuadric(), 5, 5, 550, 16, 1); //pendul

	glTranslatef(0, 0, 550);
	glScalef(1, 0.3, 1);
	auxSolidSphere(50); //sfera pendul
	glPopMatrix();


	
	glPushMatrix();
	glTranslatef(730, 800, -202);
	for(int angle = 0; angle<360; angle+=30){
		glPushMatrix();
		glRotatef(angle, 0, 0, 1);
		glRotatef(90, 1, 0, 0);
		glTranslatef(0, 0, 90);
		glColor3f(0.1,0.1,0.1);
		gluCylinder(gluNewQuadric(), 3, 3, 20, 16, 1); /////numerele de pe disc
		glPopMatrix();
	}
	glPopMatrix();


	///minutar
	glPushMatrix();
	glTranslatef(730, 800, -200);
	glRotatef(-minuteAngle+180, 0, 0, 1);
	glRotatef(90, 1, 0, 0);
	glColor3f(0.1,0.1,0.1);
	gluCylinder(gluNewQuadric(), 2, 2, 65, 16, 1); //corp minutar

	glTranslatef(0, 0, 65);
	glScalef(1, 0.3, 1);
	gluCylinder(gluNewQuadric(), 5, 0, 40, 16, 1);// cap minutar
	glPopMatrix();


	///ac pentr ora
	glPushMatrix();
	glTranslatef(730, 800, -200);
	glRotatef(-hourAngle+180, 0, 0, 1);
	glRotatef(90, 1, 0, 0);
	glColor3f(0.1,0.1,0.1);
	gluCylinder(gluNewQuadric(), 2, 2, 50, 16, 1); //corp pt ac ora

	glTranslatef(0, 0, 50);
	glScalef(1, 0.3, 1);
	gluCylinder(gluNewQuadric(), 7, 0, 20, 16, 1);// cap pt ora
	glPopMatrix();

	///cerc unde se intersecteaza acele ceasului
	glPushMatrix();
	glTranslatef(730, 800, -198);
	glScalef(1, 1, 0.1);
	glColor3f(0.1,0.1,0.1);
	auxSolidSphere(10); 
	glPopMatrix();

	//deseneaza umbra
	//mai intai se dezactiveaza iluminarea si se salveaza starea matricei de proiectie
	glPushMatrix();
	glPushAttrib(GL_LIGHTING_BIT);
	glDisable(GL_LIGHTING);
	glPushMatrix();
	glMultMatrixf((GLfloat*)matUmbra);// se inmulteste matricea curenta cu matricea de umbrire


///obiectele

	glPushMatrix();
	glTranslatef(730, 800, -210);
	glRotatef(45, 0, 0, 1);
	glColor3f(0.05, 0.05, 0.0);
	gluDisk(gluNewQuadric(), 0, 200, 4, 1); //patratul cel mare(cel rsoiatic, din exterior)(doar umbra acestuia)
	glTranslatef(0,0,10);///sunt puse translatiile fiecarui obiect al ceasului in ordine chiar daca eu apelez pt umbra doar desenarea patratului mare
	glTranslatef(0,0,3);///nu si cea a patratului mic si a discurilor, pentru ca se vad urat ca umbre, am lasat restul translatiilor pt ca fiecare obiect
	glTranslatef(0,0,5);///desenat tine cont de informattile celuilalt (cum ar fi translatia, rotatia), asta e din cauza ca nu sa pus push si pop pt fiecare desenare
	glTranslatef(0,0,-25);///a unui obiect in parte 
	glRotatef(pendulum_angle-45, 0, 0, 1);
	glRotatef(90, 1, 0, 0);
	gluCylinder(gluNewQuadric(), 5, 5, 550, 16, 1); //pendulul
	glTranslatef(0, 0, 550);
	glScalef(1, 0.3, 1);
	auxSolidSphere(50); //sfera pendul
	glPopMatrix();

	//reseteaza starea variabilelor de iluminare
	glPopAttrib();
	glPopMatrix();

	auxSwapBuffers();

	glFlush();
	
}

//void CALLBACK display(void){
//}

void CALLBACK myReshape(GLsizei w, GLsizei h)
{
    if (!h) return;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h) 
        glOrtho (0, 1024, 0, 1024*(GLfloat)h/(GLfloat)w, 0, 1024);//pt dimensiunile viewport-ului(camerei)
    else 
        glOrtho (0, 1024*(GLfloat)w/(GLfloat)h, 0, 1024, 0, 1024);//pt dimensiunile viewport-ului(camerei)
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char** argv)
{
    auxInitDisplayMode (AUX_DOUBLE | AUX_RGB | AUX_DEPTH16);
    auxInitPosition (0, 0, 1024, 720);
    auxInitWindow ("Ceas cu pendul");
    myinit();//apel ilmuniarer
	PlaySound("clock.wav", NULL, SND_ASYNC|SND_FILENAME|SND_LOOP);//sunet
    auxReshapeFunc (myReshape);//apel pt dimensiuni viewport
	auxIdleFunc(idle);//apel animatie
    auxMainLoop(idle);//apel display(e facut tot in fct cu nume idle)
    return(0);
}
