#include "disp.h" /* include your own disp.h file (e.g. hw1)*/

/* Camera defaults */
#define	DEFAULT_FOV		35.0
#define	DEFAULT_IM_Z	(-10.0)  /* world coords for image plane origin */
#define	DEFAULT_IM_Y	(5.0)    /* default look-at point = 0,0,0 */
#define	DEFAULT_IM_X	(-10.0)

#define	DEFAULT_AMBIENT	{0.1, 0.1, 0.1}
#define	DEFAULT_DIFFUSE	{0.7, 0.6, 0.5}
#define	DEFAULT_SPECULAR	{0.2, 0.3, 0.4}
#define	DEFAULT_SPEC		32

#define	MATLEVELS	100		/* how many matrix pushes allowed */
#define	MAX_LIGHTS	10		/* how many lights allowed */

#define PI 3.1415926

#ifndef GZRENDER
#define GZRENDER
typedef struct {			/* define a renderer */
  GzDisplay		*display;
  GzCamera		camera;
  short		    matlevel;	        /* top of stack - current xform */
  GzMatrix		Ximage[MATLEVELS];	/* stack of xforms (Xsm) */
  GzMatrix		Xnorm[MATLEVELS];	/* xforms for norms (Xim) */
  GzMatrix		Xsp;		        /* NDC to screen (pers-to-screen) */
  GzColor		flatcolor;          /* color state for flat shaded triangles */
  int			interp_mode;
  int			numlights;
  GzLight		lights[MAX_LIGHTS];
  GzLight		ambientlight;
  GzColor		Ka, Kd, Ks;
  float		    spec;		/* specular power */
  GzTexture		tex_fun;    /* tex_fun(float u, float v, GzColor color) */
  float			shiftX;
  float			shiftY;
  GzNormalMap	normalmap_fun;
  GZHeightMap   heightmap_fun;
  GzMatrix      TBN;
  GzColor       tanViewPos;
  GzColor       tanFragPos;
  GzColor       viewDir;
}  GzRender;
#endif

// Function declaration
// HW2
int GzNewRender(GzRender **render, GzDisplay *display);
int GzFreeRender(GzRender *render);
int GzBeginRender(GzRender	*render);
int GzPutAttribute(GzRender	*render, int numAttributes, GzToken	*nameList, 
	GzPointer *valueList);
int GzPutTriangle(GzRender *render, int	numParts, GzToken *nameList,
	GzPointer *valueList);

// HW3
int GzPutCamera(GzRender *render, GzCamera *camera);
int GzPushMatrix(GzRender *render, GzMatrix	matrix);
int GzPopMatrix(GzRender *render);

// HW5
int GzFreeTexture();

// Object Translation
int GzRotXMat(float degree, GzMatrix mat);
int GzRotYMat(float degree, GzMatrix mat);
int GzRotZMat(float degree, GzMatrix mat);
int GzTrxMat(GzCoord translate, GzMatrix mat);
int GzScaleMat(GzCoord scale, GzMatrix mat);


//Added Functions
int setupXsp(GzRender *render);
int setupXpi(GzRender *render);
int setupXiw(GzRender *render);
int initCamera(GzRender *render);
int normalized(GzCoord vector);
void SetupTri(GzCoord* vertices, GzCoord* normals, GzTextureIndex* uvs);
void SwapCoord(float* v1, float* v2);
void LEE(GzRender* render, GzCoord* vertices, GzCoord* normals, GzTextureIndex* uvs);
void GetZPlane(const GzCoord* vertices, float* A, float* B, float* C, float* D);
float EdgeSide(const float* start, const float* end, int x, int y, bool right);
void ToScreen(const GzCoord* vert_world, GzMatrix Xsw, GzCoord* vert_screen);
void NormalToScreen(const GzCoord* normal_world, GzMatrix Xsw, GzCoord* normal_screen);
short ctoi(float color);
float triangleArea(GzCoord v0, GzCoord v1, GzCoord v2);
int CalculateColor(GzRender *render, GzColor color, GzCoord norm);
int CalculateColorTexture(GzRender *render, GzColor color, GzCoord norm);
void SwapUV(float* v1, float* v2);