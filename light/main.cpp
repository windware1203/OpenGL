#include <math.h>
#include <stdio.h>
#include <GL/glut.h>
#include <GL/glui.h>
#include <iostream>
#include <GL/tinyfiledialogs.h>

#define SIZE_1 1
#define SIZE_2 2
#define SIZE_3 3
#define MEXIT 4

#define WORLDAX 1
#define LOCALAX 2
#define FCUBE 3
#define CCUBE 4

#define TEAPOT 0
#define CUBE 1
#define DATAMODEL 2

#define smooth 2
#define WIRE 0
#define FLAT 1

using namespace std;

float backgroundGray = 0.3f;
int a = 3;
GLint object1 = TEAPOT;

GLfloat vertices[][3] =
{
	{-1.0,-1.0,-1.0},{1.0,-1.0,-1.0},
	{1.0,1.0,-1.0},{-1.0,1.0,-1.0},{-1.0,-1.0,1.0},
	{1.0,-1.0,1.0},{1.0,1.0,1.0},{-1.0,1.0,1.0}
};

GLfloat colors[][3] =
{
	{0.0,0.0,0.0},{1,0.0,0.0},
	{1.0,1.0,0.0},{0.0,1.0,0.0},{0.0,0.0,1.0},
	{1.0,0.0,1.0},{1.0,1.0,1.0},{0.0,1.0,1.0}
};

GLfloat normal[][3] = 
{
	{-1.0,-1.0,-1.0},{1.0,-1.0,-1.0},
	{1.0,1.0,-1.0},{-1.0,1.0,-1.0},{-1.0,-1.0,1.0},
	{1.0,-1.0,1.0},{1.0,1.0,1.0},{-1.0,1.0,1.0}
};

GLfloat rotationm[16] =
{
	1.0,0.0,0.0,0.0,
	0.0,1.0,0.0,0.0,
	0.0,0.0,1.0,0.0,
	0.0,0.0,0.0,1.0
};

GLfloat light0pr[16] =
{
	1.0,0.0,0.0,0.0,
	0.0,1.0,0.0,0.0,
	0.0,0.0,1.0,0.0,
	0.0,0.0,0.0,1.0
};

GLfloat light1pr[16] =
{
	1.0,0.0,0.0,0.0,
	0.0,1.0,0.0,0.0,
	0.0,0.0,1.0,0.0,
	0.0,0.0,0.0,1.0
};

GLfloat light2pr[16] =
{
	1.0,0.0,0.0,0.0,
	0.0,1.0,0.0,0.0,
	0.0,0.0,1.0,0.0,
	0.0,0.0,0.0,1.0
};

GLint drawT = smooth;

GLint timer = 16;
GLfloat roangle = 0.0f;
GLfloat rospeed = 20.0f;

GLfloat obg;

GLfloat backg = 0.0f;
GLint click;

GLfloat objpx = 0.0f;
GLfloat objpy = 0.0f;
GLfloat objpz = 0.0f;

GLint light0 = 1;
GLint light1 = 1;
GLint light2 = 1;

GLint x = 0;
GLint y = 0;
GLint height = 950;
GLint width = 650;

GLint unifScale = 0;

GLfloat aspect = (float)width / (float)height;
GLfloat objsx = 1.0f;
GLfloat objsy = 1.0f;
GLfloat objsz = 1.0f;
GLfloat objs = 1.0f;

GLfloat maxs = 10.0f;
GLboolean loadm = false;
GLint window;
GLUI* glui;
GLUI_Translation* xT, * yT, * zT;
GLUI_Translation* xS, * yS, * zS;
GLUI_Button* resetlight0r, * resetlight1r, * resetlight2r;
GLUI_Rotation* rotationUI, * light0r, * light1r, * light2r;
struct simpletri 
{
	GLint trinum;
	GLint vertexin[3];
};
struct colortri 
{
	GLint trinum;
	GLint vertexin[3];
	GLint fromcol[3];
	GLint backco[3];
};
struct vertex 
{
	GLint vertexin;
	GLfloat abc[3];
	GLfloat normalabc[3];
};
typedef struct simpletri simpletri;
typedef simpletri* simpletrip;
typedef struct colortri colortri;
typedef colortri* colortrip;
typedef struct vertex vertex;
typedef vertex* vertexp;
typedef GLfloat point[3];
typedef GLint color[3];
simpletrip simpltriarr;
colortrip colortriarr;
vertexp vertexarr;
GLint tricount = 0;
GLint vercount = 0;
GLfloat totalsf = 1.0f;
GLfloat clamp(float v, float min, float max);

GLfloat light0p[] = { 200.0f,0.0f,0.0f,1.0f };
GLfloat light1p[] = { 0.0f,200.0f,0.0f,1.0f };
GLfloat light2p[] = { 0.0f,0.0f,200.0f,1.0f };

GLint materialt = 0;


void drawTri(point x, point y, point z, point ax, point ay, point az)
{
	glBegin(GL_TRIANGLES);
	glColor3f(0.3f, 0.3f, 0.3f);
	glNormal3fv(ax);
	glVertex3fv(x);
	glNormal3fv(ay);
	glVertex3fv(y);
	glNormal3fv(az);
	glVertex3fv(z);
	glEnd();
}
void drawTriwitcol(point x, point y, point z, point ax, point ay, point az, color xy, color yz)
{
	GLfloat abfloat[3] = { xy[0] / 255.0f , xy[1] / 255.0f , xy[2] / 255.0f };
	glBegin(GL_TRIANGLES);
	glColor3fv(abfloat);
	glNormal3fv(ax);
	glVertex3fv(x);
	glNormal3fv(ay);
	glVertex3fv(y);
	glNormal3fv(az);
	glVertex3fv(z);
	glEnd();
}
void vertexNormalize() {
	GLfloat xb = vertexarr[0].abc[0];
	GLfloat yb = vertexarr[0].abc[1];
	GLfloat zb = vertexarr[0].abc[2];
	GLfloat tolx = 0;
	GLfloat toly = 0;
	GLfloat tolz = 0;
	GLfloat maxx = INT_MIN;
	GLfloat maxy = INT_MIN;
	GLfloat maxz = INT_MIN;
	GLfloat minx = INT_MAX;
	GLfloat miny = INT_MAX;
	GLfloat minz = INT_MAX;
	for (size_t i = 0; i < vercount; i++)
	{
		tolx += vertexarr[i].abc[0] - xb;
		toly += vertexarr[i].abc[1] - yb;
		tolz += vertexarr[i].abc[2] - zb;
	}
	tolx /= vercount;
	toly /= vercount;
	tolz /= vercount;
	for (size_t i = 0; i < vercount; i++) 
	{
		vertexarr[i].abc[0] -= tolx + xb;
		vertexarr[i].abc[1] -= toly + yb;
		vertexarr[i].abc[2] -= tolz + zb;
	}
	for (size_t i = 0; i < vercount; i++) 
	{
		if (vertexarr[i].abc[0] > maxx) 
		{
			maxx = vertexarr[i].abc[0];
		}
		else if (vertexarr[i].abc[0] < minx) 
		{
			minx = vertexarr[i].abc[0];
		}
		if (vertexarr[i].abc[1] > maxy) 
		{
			maxy = vertexarr[i].abc[1];
		}
		else if (vertexarr[i].abc[1] < miny) 
		{
			miny = vertexarr[i].abc[1];
		}
		if (vertexarr[i].abc[2] > maxz) {
			maxz = vertexarr[i].abc[2];
		}
		else if (vertexarr[i].abc[2] < minz) 
		{
			minz = vertexarr[i].abc[2];
		}
	}
	xb = maxx - minx;
	yb = maxy - miny;
	zb = maxz - minz;
	if (xb > yb) {
		if (xb > zb) 
		{
			maxs = xb;
		}
		else 
		{
			maxs = zb;
		}
	}
	else {
		if (yb > zb) 
		{
			maxs = yb;
		}
		else 
		{
			maxs = zb;
		}
	}
	maxs = fabs(maxs);
	int digi = 0;
	while (maxs >= 1) 
	{
		maxs /= 10.0;
		digi++;
	}
	totalsf = powf(10.0, (digi - 1));
}

char* filepath;
GLboolean dataload = false;
GLint modt = -1;
void readMod(void) 
{
	char const* fileEx[1] = { "*.tri" };
	filepath = tinyfd_openFileDialog("selecting", "", 0, fileEx, NULL, 0);
	if (filepath == NULL)
	{
		printf("D failure");
		return;
	}
	FILE* ifile = fopen(filepath, "r");//資料夾路徑不能有中文
	if (ifile == NULL) 
	{
		printf("open failure");
		return;
	}
	char typec[200] = { 0 };
	char tempc[200] = { 0 };
	fscanf(ifile, "%s", typec);
	modt = (strcmp("simple:", typec));
	if (modt != 0)
	{
		modt = 1;
	}
	if (!modt) 
	{
		fscanf(ifile, "%d", &tricount);
		fscanf(ifile, "%s %d", tempc, &vercount);
		simpltriarr = (simpletrip)calloc(tricount, sizeof(simpletri));
		vertexarr = (vertexp)calloc(vercount, sizeof(vertex));
		for (int i = 0; i < tricount; i++) 
		{
			fscanf(ifile, "%d %d %d %d", &simpltriarr[i].trinum, &simpltriarr[i].vertexin[0], &simpltriarr[i].vertexin[1], &simpltriarr[i].vertexin[2]);
		}
		for (int i = 0; i < vercount; i++)
		{
			fscanf(ifile, "%d %f %f %f %f %f %f", &vertexarr[i].vertexin, &vertexarr[i].abc[0], &vertexarr[i].abc[1], &vertexarr[i].abc[2], &vertexarr[i].normalabc[0], &vertexarr[i].normalabc[1], &vertexarr[i].normalabc[2]);
		}
	}
	else {
		fscanf(ifile, "%ud", &tricount);
		fscanf(ifile, "%s %ud", tempc, &vercount);
		colortriarr = (colortrip)calloc(tricount, sizeof(colortri));
		vertexarr = (vertexp)calloc(vercount, sizeof(vertex));
		for (int i = 0; i < tricount; i++) 
		{
			fscanf(ifile, "%d", &(colortriarr[i].trinum));
			for (int j = 0; j < 3; j++) 
			{
				fscanf(ifile, "%d", &(colortriarr[i].vertexin[j]));
			}
			for (int j = 0; j < 3; j++) 
			{
				fscanf(ifile, "%d", &(colortriarr[i].fromcol[j]));
			}
			for (int j = 0; j < 3; j++) 
			{
				fscanf(ifile, "%d", &(colortriarr[i].backco[j]));
			}
			puts("");
		}
		for (int i = 0; i < vercount; i++)
		{
			fscanf(ifile, "%d %f %f %f %f %f %f", &vertexarr[i].vertexin, &vertexarr[i].abc[0], &vertexarr[i].abc[1], &vertexarr[i].abc[2], &vertexarr[i].normalabc[0], &vertexarr[i].normalabc[1], &vertexarr[i].normalabc[2]);
		}
	}
	fclose(ifile);
	vertexNormalize();
	dataload = true;
}
void rendermod() 
{
	if (!modt) 
	{
		for (size_t i = 0; i < tricount; i++) 
		{
			drawTri(vertexarr[simpltriarr[i].vertexin[0]].abc, vertexarr[simpltriarr[i].vertexin[1]].abc, vertexarr[simpltriarr[i].vertexin[2]].abc, vertexarr[simpltriarr[i].vertexin[0]].normalabc, vertexarr[simpltriarr[i].vertexin[1]].normalabc, vertexarr[simpltriarr[i].vertexin[2]].normalabc);

		}
	}
	else 
	{
		for (size_t i = 0; i < tricount; i++) 
		{
			drawTriwitcol(vertexarr[colortriarr[i].vertexin[0]].abc, vertexarr[colortriarr[i].vertexin[1]].abc, vertexarr[colortriarr[i].vertexin[2]].abc, vertexarr[colortriarr[i].vertexin[0]].normalabc, vertexarr[colortriarr[i].vertexin[1]].normalabc, vertexarr[colortriarr[i].vertexin[2]].normalabc, colortriarr[i].fromcol, colortriarr[i].backco);

		}
	}
}
void matrixmult(float* matl, float* malt2, float* result)
{
	for (int i = 0; i < 4; i++)
	{
		result[i] = 0.0f;
		for (int j = 0; j < 4; j++)
		{
			result[i] += matl[j] * malt2[4 * j + i];
		}
	}
}
GLfloat psa[] = { 0.73125f,0.73125f,0.73125f,1.0f };
GLfloat psd[] = { 0.7775f,0.7775f,0.7775f,1.0f };
GLfloat psspec[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat psshin[] = { 256.0f };
GLfloat pse[] = { 0.069375f, 0.069375f , 0.069375f ,1.0f };

GLfloat pa[] = { 0.10588f,0.058824f,0.113725f,1.0f };
GLfloat pd[] = { 0.427451f,0.470588f,0.541176f,1.0f };
GLfloat pspec[] = { 0.3333f,0.3333f,0.521569f,1.0f };
GLfloat pshin[] = { 9.84615f };
GLfloat pe[] = { 0.0f,0.0f,0.0f,1.0f };

GLfloat ga[] = { 0.24725f,0.1995f,0.0745f,1.0f };
GLfloat gd[] = { 0.75164f,0.60648f,0.22648f,1.0f };
GLfloat gspec[] = { 0.628281f,0.555802f,0.366065f,1.0f };
GLfloat gshin[] = { 250.2f };
GLfloat ge[] = { 0.18791f,0.15162f,0.05662f,1.0f };

GLfloat da[] = { 0.2f,0.2f,0.2f,1.0f };
GLfloat dd[] = { 20.0f / 255,10.0f / 255,9.0f / 255,1.0f };
GLfloat dspec[] = { 20.0f / 255,10.0f / 255,9.0f / 255,1.0f };
GLfloat dshin[] = { 10.0f };
GLfloat de[] = { 0.0f,0.0f,0.0f,1.0f };
void material() {
	glEnable(GL_COLOR_MATERIAL);
	switch (materialt) 
	{
		case 0:
			glColorMaterial(GL_FRONT, GL_SPECULAR);
			glMaterialfv(GL_FRONT, GL_AMBIENT, ga);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, gd);
			glMaterialfv(GL_FRONT, GL_SPECULAR, gspec);
			glMaterialfv(GL_FRONT, GL_SHININESS, gshin);
			glMaterialfv(GL_FRONT, GL_EMISSION, ge);
			break;
		case 1:
			glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
			glMaterialfv(GL_FRONT, GL_AMBIENT, pa);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, pd);
			glMaterialfv(GL_FRONT, GL_SPECULAR, pspec);
			glMaterialfv(GL_FRONT, GL_SHININESS, pshin);
			glMaterialfv(GL_FRONT, GL_EMISSION, pe);
			break;
		case 2:
			glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, psa);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, psd);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, psspec);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, psshin);
			glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, pse);
			break;
		case 3:
			glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
			glMaterialfv(GL_FRONT, GL_AMBIENT, da);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, dd);
			glMaterialfv(GL_FRONT, GL_SPECULAR, dspec);
			glMaterialfv(GL_FRONT, GL_SHININESS, dshin);
			glMaterialfv(GL_FRONT, GL_EMISSION, de);
			break;
	}

}
void light()
{
	glEnable(GL_LIGHTING);
	GLfloat temp[] = { 0.0f,0.0f,0.0f,1.0f };
	GLfloat lightd[] = { 0.0f,0.0f,0.0f ,1.0f };
	GLfloat globala[] = { 0.2f,0.2f,0.2f,1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globala);
	GLfloat light0a[] = { 0.2f,0.2f,0.2f,1.0f };
	GLfloat light0d[] = { 0.3f,0.3f,0.3f,1.0f };
	GLfloat light0s[] = { 0.3f,0.3f,0.3f,1.0f };
	matrixmult(light0p, light0pr, temp);
	glLightfv(GL_LIGHT0, GL_POSITION, temp);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0a);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0d);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0s);
	light0 ? glEnable(GL_LIGHT0) : glDisable(GL_LIGHT0);

	GLfloat light1a[] = { 0.2f,0.2f,0.2f,1.0f };
	GLfloat light1d[] = { 0.2f,0.2f,0.2f,1.0f };
	GLfloat light1s[] = { 0.5f,0.5f,0.5f,1.0f };
	matrixmult(light1p, light1pr, temp);
	glLightfv(GL_LIGHT1, GL_POSITION, temp);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1a);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1d);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1s);
	light1 ? glEnable(GL_LIGHT1) : glDisable(GL_LIGHT1);

	GLfloat light2a[] = { 0.2f,0.2f,0.2f,1.0f };
	GLfloat light2d[] = { 0.2f,0.2f,0.2f,1.0f };
	GLfloat light2s[] = { 0.5f,0.5f,0.5f,1.0f };
	matrixmult(light2p, light2pr, temp);
	glLightfv(GL_LIGHT2, GL_POSITION, temp);
	glLightfv(GL_LIGHT2, GL_AMBIENT, light2a);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light2d);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light2s);
	light2 ? glEnable(GL_LIGHT2) : glDisable(GL_LIGHT2);
}


void polygon(int t, int a, int b, int c, int d)
{
	glBegin(GL_POLYGON);
	if (t == CCUBE)
	{
		glColor3fv(colors[a]);
		glNormal3fv(normal[a]);
		glVertex3fv(vertices[a]);

		glColor3fv(colors[b]);
		glNormal3fv(normal[b]);
		glVertex3fv(vertices[b]);

		glColor3fv(colors[c]);
		glNormal3fv(normal[c]);
		glVertex3fv(vertices[c]);

		glColor3fv(colors[d]);
		glNormal3fv(normal[d]);
		glVertex3fv(vertices[d]);
	}
	else 
	{
		glColor3fv(colors[a]);
		glNormal3fv(normal[a]);
		glVertex3fv(vertices[a]);
		glVertex3fv(vertices[b]);
		glVertex3fv(vertices[c]);
		glVertex3fv(vertices[c]);
	}
	glEnd();
}
void drawObject(void)
{

	switch (drawT) 
	{
		case WIRE:
			glPolygonMode(GL_FRONT, GL_LINE);
			glPolygonMode(GL_BACK, GL_LINE);
			glShadeModel(GL_FLAT);
			glLineWidth(1.0f);
			break;
		case FLAT:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glShadeModel(GL_FLAT);
			break;
		case smooth:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glShadeModel(GL_SMOOTH);
			break;
	}

	switch (a)
	{
		case 0:
			glColor3b(125, 0, 0);
			break;
		case 1:
			glColor3b(0, 125, 0);
			break;
		case 2:
			glColor3b(0, 0, 125);
			break;
		case 3:
			glColor3b(20, 10, 9);
			break;
		case 4:
			glColor3b(125, 125, 0);
			break;

	}

	switch (object1)
	{
		case TEAPOT:
			dataload = false;
			maxs = 10.0f;
			totalsf = 1.0f;
			glutSolidTeapot(objs);
			glCallList(LOCALAX);
			break;
		case CUBE:
			dataload = false;
			maxs = 10.0f;
			totalsf = 1.0f;
			if (drawT == FLAT)
				glCallList(FCUBE);
			else
				glCallList(CCUBE);
			break;
		case DATAMODEL:
			if (!dataload)
				readMod();
			glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
			rendermod();
			break;
	}
}
void drawWorldax() 
{
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(totalsf * 5, 0.0, 0.0);
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, totalsf * 5, 0.0);
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, totalsf * 5);
	glEnd();
	glEnable(GL_LIGHTING);

}

void drawLocalax() 
{
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(totalsf * 1.5f, 0.0, 0.0);
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, totalsf * 1.5f, 0.0);
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, totalsf * 1.5f);
	glEnd();
	glEnable(GL_LIGHTING);

}
void myInit() 
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glNewList(FCUBE, GL_COMPILE);
	polygon(FCUBE, 3, 2, 1, 0);
	polygon(FCUBE, 2, 3, 7, 6);
	polygon(FCUBE, 7, 3, 0, 4);
	polygon(FCUBE, 1, 2, 6, 5);
	polygon(FCUBE, 4, 5, 6, 7);
	polygon(FCUBE, 5, 4, 0, 1);
	glEnd();
	glEndList();
	glNewList(CCUBE, GL_COMPILE);
	polygon(CCUBE, 0, 3, 2, 1);
	polygon(CCUBE, 2, 3, 7, 6);
	polygon(CCUBE, 0, 4, 7, 3);
	polygon(CCUBE, 1, 2, 6, 5);
	polygon(CCUBE, 4, 5, 6, 7);
	polygon(CCUBE, 0, 1, 5, 4);
	glEnd();
	glEndList();
	glViewport(x, y, width, height);
}
void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(backg, backg, backg, 1.0f);
	glEnable(GL_LIGHTING);
	drawObject();
	material();
	GLUI_Master.get_viewport_area(&x, &y, &width, &height);
	aspect = (float)width / (float)height;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, aspect, 0.1f, 12.0f * totalsf);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(totalsf * 4.0, totalsf * 4.0, totalsf * 4.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	light();
	glDisable(GL_LIGHTING);
	drawWorldax();
	glEnable(GL_LIGHTING);
	glTranslatef(objpx, objpy, objpz);
	glRotatef(roangle, 0.0f, 1.0f, 0.0f);
	rotationUI->get_float_array_val(rotationm);
	glMultMatrixf(rotationm);

	(unifScale) ? glScalef(objsx, objsx, objsx) : glScalef(objsx, objsy, objsz);

	drawLocalax();

	glutSwapBuffers();
}
void myReshape(int width, int height)
{
	int x = 0, y = 0;
	GLUI_Master.get_viewport_area(&x, &y, &width, &height);
	aspect = width * 1.0f / height;
	glui->refresh();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(x, y, width, height);

	gluPerspective(60.0f, aspect, 0.1f, 10.0f);
	glutSwapBuffers();
}

void myTimer(int val)
{
	glutPostRedisplay();
	glutTimerFunc(timer, myTimer, val);
	roangle += rospeed * timer * 0.001f;
}
void myMouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			obg = backg;
			click = x;
		}
	}
}
void mouseMoving(int x, int y)
{
	backg = (x - click) * 0.005f + obg;
	backg = clamp(backg, 0.0f, 1.0f);
}
void myKeyboard(unsigned char key, int x, int y)
{
	if (key == 'W' || key == 'w')
	{
		objpy += 0.05f;
	}
	else if (key == 'S' || key == 's')
	{
		objpy -= 0.05f;
	}
	else if (key == 'A' || key == 'a')
	{
		objpx -= 0.05f;
	}
	else if (key == 'D' || key == 'd')
	{
		objpx += 0.05f;
	}
	else if (key == 'Q' || key == 'q')
	{
		rospeed += 10.0f;
	}
	else if (key == 'E' || key == 'e')
	{
		rospeed -= 10.0f;
	}
	glutPostRedisplay();
}
void mySpecial(int key, int x, int y)
{
	if (key == GLUT_KEY_F1)
	{
		a = 0;
	}
	else if (key == GLUT_KEY_F2)
	{
		a = 1;
	}
	else if (key == GLUT_KEY_F3)
	{
		a = 2;
	}
	else if (key == GLUT_KEY_F4)
	{
		a = 4;

	}

	glutPostRedisplay();
}
void myMenu(int id)
{
	switch (id) 
	{
		case SIZE_1:
			objs = 1.0f;
			break;
		case SIZE_2:
			objs = 1.5f;
			break;
		case SIZE_3:
			objs = 2.0f;
			break;
		case MEXIT:
			exit(0);
	}

}
void idle() 
{
	glutSetWindow(window);
	glutPostRedisplay();
}
void lightcb(int id) 
{
	!light0 ? (light0r->disable(), resetlight0r->disable()) : (light0r->enable(), resetlight0r->enable());
	!light1 ? (light1r->disable(), resetlight1r->disable()) : (light1r->enable(), resetlight1r->enable());
	!light2 ? (light2r->disable(), resetlight2r->disable()) : (light2r->enable(), resetlight2r->enable());
}
void scaleCb(int id) 
{
	objsy = objsx = objsz;
	if (unifScale) 
	{
		yS->disable();
		zS->disable();
	}
	else
	{
		yS->enable();
		zS->enable();
	}

}
void resetCb(int id) 
{
	switch (id) 
	{
	case 0:
		light0r->reset();
		break;
	case 1:
		light1r->reset();
		break;
	case 2:
		light2r->reset();
		break;
	case 3:
		objpx = objpy = objpz = 0.0f;
		break;
	case 4:
		objsx = objsy = objsz = 1.0f;
		break;
	case 5:
		rotationUI->reset();
	}
}
void initUI() 
{
	GLUI_Panel* lightpanel = glui->add_panel("lighting");
	glui->add_checkbox_to_panel(lightpanel, "light0", &light0, 0, lightcb);
	light0r = glui->add_rotation_to_panel(lightpanel, "rotate", light0pr);
	resetlight0r = glui->add_button_to_panel(lightpanel, "reset", 0, resetCb);
	!light0 ? (light0r->disable(), resetlight0r->disable()) : (light0r->enable(), resetlight0r->enable());

	glui->add_column_to_panel(lightpanel, 1);
	glui->add_checkbox_to_panel(lightpanel, "light1", &light1, 1, lightcb);
	light1r = glui->add_rotation_to_panel(lightpanel, "rotate", light1pr);
	resetlight1r = glui->add_button_to_panel(lightpanel, "reset", 1, resetCb);
	!light1 ? (light1r->disable(), resetlight1r->disable()) : (light1r->enable(), resetlight1r->enable());

	glui->add_column_to_panel(lightpanel, 2);
	glui->add_checkbox_to_panel(lightpanel, "light2", &light2, 2, lightcb);
	light2r = glui->add_rotation_to_panel(lightpanel, "rotate", light2pr);
	resetlight2r = glui->add_button_to_panel(lightpanel, "reset", 2, resetCb);
	!light2 ? (light2r->disable(), resetlight2r->disable()) : (light2r->enable(), resetlight2r->enable());

	GLUI_Panel* objpanel = glui->add_panel("object");
	GLUI_Panel* drawtypepanel = glui->add_panel_to_panel(objpanel, "");
	GLUI_RadioGroup* drawTypegroup = glui->add_radiogroup_to_panel(drawtypepanel, &drawT);
	glui->add_radiobutton_to_group(drawTypegroup, "wire");
	glui->add_radiobutton_to_group(drawTypegroup, "flat");
	glui->add_radiobutton_to_group(drawTypegroup, "smooth");

	glui->add_column_to_panel(objpanel, 0);

	GLUI_Panel* objtpanel = glui->add_panel_to_panel(objpanel, "");
	GLUI_RadioGroup* objtgroup = glui->add_radiogroup_to_panel(objtpanel, &object1);
	glui->add_radiobutton_to_group(objtgroup, "teapot");
	glui->add_radiobutton_to_group(objtgroup, "colorcube");
	glui->add_radiobutton_to_group(objtgroup, "datamodel");

	GLUI_Panel* translationp = glui->add_panel("translation");
	GLUI_Panel* itr = glui->add_panel_to_panel(translationp, " ");
	xT = glui->add_translation_to_panel(itr, "X", GLUI_TRANSLATION_X, &objpx);
	xT->set_speed(0.1f);
	glui->add_column_to_panel(itr, 0);
	yT = glui->add_translation_to_panel(itr, "Y", GLUI_TRANSLATION_Y, &objpy);
	yT->set_speed(0.1f);
	glui->add_column_to_panel(itr, 0);
	zT = glui->add_translation_to_panel(itr, "Z", GLUI_TRANSLATION_Z, &objpz);
	zT->set_speed(0.1f);
	glui->add_separator_to_panel(translationp);
	glui->add_button_to_panel(translationp, "reset translation", 3, resetCb);

	GLUI_Panel* sp = glui->add_panel("scale");
	GLUI_Panel* spr = glui->add_panel_to_panel(sp, " ");
	xS = glui->add_translation_to_panel(spr, "X", GLUI_TRANSLATION_X, &objsx);
	xS->set_speed(0.1f);
	glui->add_column_to_panel(spr, 0);
	yS = glui->add_translation_to_panel(spr, "Y", GLUI_TRANSLATION_Y, &objsy);
	yS->set_speed(0.1f);
	glui->add_column_to_panel(spr, 0);
	zS = glui->add_translation_to_panel(spr, "Z", GLUI_TRANSLATION_Z, &objsz);
	zS->set_speed(0.1f);
	GLUI_Checkbox* unifcheck = glui->add_checkbox_to_panel(sp, "uniform scaling", &unifScale, 0, scaleCb);
	unifcheck->set_alignment(GLUI_ALIGN_CENTER);
	glui->add_button_to_panel(sp, "reset scale", 4, resetCb);
	GLUI_Panel* bp = glui->add_panel("");
	GLUI_Panel* rp = glui->add_panel("");
	rotationUI = glui->add_rotation_to_panel(bp, "rotation", rotationm);
	glui->add_button_to_panel(bp, "reset rotation", 5, resetCb);
	glui->add_column_to_panel(bp, 0);
	GLUI_Panel* materialp = glui->add_panel_to_panel(bp, "material");
	GLUI_RadioGroup* materialg = glui->add_radiogroup_to_panel(materialp, &materialt);
	glui->add_radiobutton_to_group(materialg, "gold");
	glui->add_radiobutton_to_group(materialg, "pewter");
	glui->add_radiobutton_to_group(materialg, "polishedsilver");
	glui->add_radiobutton_to_group(materialg, "clay");
}
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitDisplayString("rgba double depth hidpi samples=2048");
	glutInitWindowSize(1500, 800);
	glutInitWindowPosition(100, 100);
	window = glutCreateWindow("U10916024");
	myInit();
	int menuMain = glutCreateMenu(myMenu);
	int menuEntry = glutCreateMenu(myMenu);

	glutSetMenu(menuMain);
	glutAddSubMenu("object size", menuEntry);
	glutAddMenuEntry("exit", MEXIT);
	glutSetMenu(menuEntry);
	glutAddMenuEntry("1.0", SIZE_1);
	glutAddMenuEntry("1.5", SIZE_2);
	glutAddMenuEntry("2.0", SIZE_3);

	glutSetMenu(menuMain);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutDisplayFunc(myDisplay);
	glutMotionFunc(mouseMoving);


	glutTimerFunc(timer, myTimer, 0);
	GLUI_Master.set_glutIdleFunc(idle);
	GLUI_Master.set_glutKeyboardFunc(myKeyboard);
	GLUI_Master.set_glutSpecialFunc(mySpecial);
	GLUI_Master.set_glutMouseFunc(myMouse);
	GLUI_Master.set_glutReshapeFunc(myReshape);
	glui = GLUI_Master.create_glui_subwindow(window, GLUI_SUBWINDOW_RIGHT);
	initUI();
	glui->set_main_gfx_window(window);

	glutMainLoop();
	return 0;
}

GLfloat clamp(float v, float min, float max)
{
	if (v >= max)
	{
		return max;
	}
	else if (v <= min)
	{
		return min;
	}
	else
	{
		return v;
	}
}