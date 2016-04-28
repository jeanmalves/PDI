/*============================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "pdi.h"

/*============================================================================*/
#define INPUT_IMAGE "82.bmp"

#define NEGATIVO 0
#define THRESHOLD 0.5f
#define ALTURA_MIN 10
#define LARGURA_MIN 10
#define N_PIXELS_MIN 20

/*============================================================================*/


int main() {

  // Abre a imagem em escala de cinza, e mantem uma copia colorida dela para desenhar a saida.
  Imagem* img = abreImagem (INPUT_IMAGE, 1);
  if (!img)
  {
      printf ("Erro abrindo a imagem.\n");
      exit (1);
  }

  Imagem* img_out = criaImagem (img->largura, img->altura, 1);

  binariza (img, img_out, THRESHOLD);
  salvaImagem (img_out, "01 - binarizada-82.bmp");

  ComponenteConexo* componentes;
  int qtde1 = rotulaFloodFill (img_out, &componentes, LARGURA_MIN, ALTURA_MIN, N_PIXELS_MIN);
  printf("%d\n",qtde1 );
  salvaImagem (img_out, "01 - rotulada-82.bmp");

  Imagem* out = criaImagem (img->largura, img->altura, 1);

  Imagem* kernel = criaKernelCircular (3);
  Coordenada coordenada = criaCoordenada (1, 1);
  erode (img_out, kernel, coordenada, out);

  salvaImagem (out, "01 - erodida-82.bmp");

  int qtde2 = rotulaFloodFill (out, &componentes, LARGURA_MIN, ALTURA_MIN, N_PIXELS_MIN);

  printf("%d\n", qtde1 - qtde2 );
  Imagem* out2 = criaImagem (img->largura, img->altura, 1);
  erode (out, kernel, coordenada, out2);
  salvaImagem (out2, "02 - erodida-82.bmp");


  destroiImagem (img);

  return 0;
}
