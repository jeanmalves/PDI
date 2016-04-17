/*============================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "pdi.h"

/*============================================================================*/
#define INPUT_IMAGE "82.bmp"

#define NEGATIVO 0
#define THRESHOLD 0.7f
#define ALTURA_MIN 10
#define LARGURA_MIN 10
#define N_PIXELS_MIN 20

/*============================================================================*/

typedef struct
{
    float label;
    Retangulo roi;
    int n_pixels;

} Componente;

/*============================================================================*/


int main() {

  // Abre a imagem em escala de cinza, e mantem uma copia colorida dela para desenhar a saida.
  Imagem* img = abreImagem (INPUT_IMAGE, 3);
  if (!img)
  {
      printf ("Erro abrindo a imagem.\n");
      exit (1);
  }

  Imagem* img_out = criaImagem (img->largura, img->altura, 3);

  binariza (img, img_out, THRESHOLD);
  salvaImagem (img, "01 - binarizada-82.bmp");

  //antes é preciso criar o kernel. O kernel é o circular?
  //Imagem* kernel = criarKernel(largura);
  Imagem* out = criaImagem (img->largura, img->altura, 3);
  //erode (img_out, kernel, centro, out);

  destroiImagem (img);

  return 0;
}

void criarKernel(int largura){

}
