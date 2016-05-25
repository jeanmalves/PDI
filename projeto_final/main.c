/*============================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "pdi.h"

/*============================================================================*/
#define INPUT_IMAGE "imagens/p_test1.bmp"

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
  salvaImagem (img_out, "p_binarizada.bmp");

  Imagem* img_out2 = criaImagem (img->largura, img->altura, 1);
  Imagem* kernel = criaKernelCircular(5);
  dilata (img_out, kernel, criaCoordenada(2,2), img_out2);
  salvaImagem (img_out2, "p_erodida.bmp");


  destroiImagem (img);

  return 0;
}
