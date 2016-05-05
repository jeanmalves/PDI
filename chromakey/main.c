/*============================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "pdi.h"

/*============================================================================*/
#define INPUT_IMAGE "1.bmp"

#define NEGATIVO 0
#define THRESHOLD 0.8f
#define ALTURA_MIN 10
#define LARGURA_MIN 10
#define N_PIXELS_MIN 20

/*============================================================================*/

void binariza1 (Imagem* in, Imagem* out, float threshold);
int main() {

  // Abre a imagem em escala de cinza, e mantem uma copia colorida dela para desenhar a saida.
  Imagem* img = abreImagem (INPUT_IMAGE, 3);
  if (!img)
  {
      printf ("Erro abrindo a imagem.\n");
      exit (1);
  }

  Imagem* img_out = criaImagem (img->largura, img->altura, 3);

  //binariza (img, img_out, THRESHOLD);
 binariza1(img, img_out, THRESHOLD);
  salvaImagem (img_out, "02 - binarizada.bmp");

  destroiImagem (img);

  return 0;
}

void binariza1 (Imagem* in, Imagem* out, float threshold)
{
    if (in->largura != out->largura || in->altura != out->altura || in->n_canais != out->n_canais)
    {
        printf ("ERRO: binariza: as imagens precisam ter o mesmo tamanho e numero de canais.\n");
        exit (1);
    }

    float limiar_red = in->dados[0][0][0];
    float limiar_green = in->dados[1][0][0];
    float limiar_blue = in->dados[2][0][0];
    int threshold1;
    int channel, row, col, limiar;
    //for (channel = 0; channel < in->n_canais; channel++){
        for (row = 0; row < in->altura; row++){
            for (col = 0; col < in->largura; col++) {

              if (threshold1 <10) {
                limiar_red = in->dados[0][row][col];
                limiar_green = in->dados[1][row][col];
                limiar_blue = in->dados[2][row][col];
                threshold1++;
              } else{
              threshold1 =0;
}
                if (in->dados[1][row][col]  == limiar_green ) {
                  out->dados[0][row][col] = 0;
                  out->dados[1][row][col] = 0;
                  out->dados[2][row][col] = 0;
                }else{
                  out->dados[0][row][col] = in->dados [0][row][col];
                  out->dados[1][row][col] = in->dados [1][row][col];
                  out->dados[2][row][col] = in->dados [2][row][col];
                }

            }
        }
    //}
}
