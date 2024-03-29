/* Texture functions for cs580 GzLib	*/
#include    "stdafx.h" 
#include	"stdio.h"
#include	"Gz.h"
#include	<string>

GzColor	*image = NULL;
int xs, ys;
int reset = 1;
int textureIndex = 0;
std::string fileType(".ppm");

/* Image texture function */
int tex_fun(float u, float v, GzColor color)
{
  unsigned char		pixel[3];
  unsigned char     dummy;
  char  		foo[8];
  int   		i, j;
  FILE			*fd;
  
  if (reset) {          /* open and load texture file */
    fd = fopen ((std::to_string(textureIndex) + fileType).c_str(), "rb");
    if (fd == NULL) {
      fprintf (stderr, "texture file not found\n");
      exit(-1);
    }
    fscanf (fd, "%s %d %d %c", foo, &xs, &ys, &dummy);
    image = (GzColor*)malloc(sizeof(GzColor)*(xs+1)*(ys+1));
    if (image == NULL) {
      fprintf (stderr, "malloc for texture image failed\n");
      exit(-1);
    }

    for (i = 0; i < xs*ys; i++) {	/* create array of GzColor values */
      fread(pixel, sizeof(pixel), 1, fd);
      image[i][RED] = (float)((int)pixel[RED]) * (1.0 / 255.0);
      image[i][GREEN] = (float)((int)pixel[GREEN]) * (1.0 / 255.0);
      image[i][BLUE] = (float)((int)pixel[BLUE]) * (1.0 / 255.0);
      }

    reset = 0;          /* init is done */
	fclose(fd);
  }

/* bounds-test u,v to make sure nothing will overflow image array bounds */
  if (u > 1)  u = 1;
  if (u < 0)  u = 0;
  if (v > 1)  v = 1;
  if (v < 0)  v = 0;

/* determine texture cell corner values and perform bilinear interpolation */
/* set color to interpolated GzColor value and return */
  float px, py;
  float s, t;
  px = u * (xs - 1);
  py = v * (ys - 1);
  s = px - floor(px);
  t = py - floor(py);

  GzColor colorA, colorB, colorC, colorD;
  //color at A
  for (i = 0; i < 3; i++)  colorA[i] = image[xs * (int)floor(py) + (int)floor(px)][i];
  //color at B
  for (i = 0; i < 3; i++)  colorB[i] = image[xs * (int)floor(py) + (int)ceil(px)][i];
  //color at C
  for (i = 0; i < 3; i++)  colorC[i] = image[xs * (int)ceil(py) + (int)ceil(px)][i];
  //color at D
  for (i = 0; i < 3; i++)  colorD[i] = image[xs * (int)ceil(py) + (int)floor(px)][i];
  //color at P
  for (i = 0; i < 3; i++)  color[i] = s * t * colorC[i] + (1 - s) * t * colorD[i] + s * (1 - t) * colorB[i] + (1 - s) * (1 - t) * colorA[i];
  return GZ_SUCCESS;
}

/* Procedural texture function */
int ptex_fun(float u, float v, GzColor color)
{
	for (int i = 0; i < 3; i++)  color[i] = (int(floor(u * 10)) % 2 == 1) && (int(floor(v * 10)) % 2 == 1);
	return GZ_SUCCESS;
}

/* Free texture memory */
int GzFreeTexture()
{
	if(image!=NULL)
		free(image);
	return GZ_SUCCESS;
}

