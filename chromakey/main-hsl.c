/*============================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "pdi.h"

/*============================================================================*/
#define INPUT_IMAGE "2.bmp"
#define INPUT_IMAGE_BACKGROUND "jardim_botanico.bmp"

#define NEGATIVO 0
#define THRESHOLD 0.7f
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

  Imagem* img_back = abreImagem (INPUT_IMAGE_BACKGROUND, 3);
  if (!img_back)
  {
      printf ("Erro abrindo a imagem a ser aplicada no chromakey.\n");
      exit (1);
  }
  Imagem* img_back_out = criaImagem (img->largura, img->altura, 3);

  redimensionaNN (img_back, img_back_out);
  salvaImagem (img_back_out, "back_redimencionada.bmp");

  Imagem* imgHsl = abreImagem (INPUT_IMAGE, 3);
  RGBParaHSL (img, imgHsl);
  salvaImagem (imgHsl, "hsl.bmp");

  Imagem* img_out_hsl = criaImagem (img->largura, img->altura, 3);
  ajustaHSL (imgHsl, img_out_hsl, 60, 0, 0);
  salvaImagem (img_out_hsl, "hsl-justada.bmp");

Imagem* img_out = criaImagem (img->largura, img->altura, 3);
 binariza1(imgHsl, img_out, THRESHOLD);
salvaImagem (img_out, "hsl-binarizado.bmp");

  destroiImagem (img);
  destroiImagem (img_back);

  return 0;
}

void binariza1 (Imagem* in, Imagem* out, float threshold)
{
    if (in->largura != out->largura || in->altura != out->altura || in->n_canais != out->n_canais)
    {
        printf ("ERRO: binariza: as imagens precisam ter o mesmo tamanho e numero de canais.\n");
        exit (1);
    }

    int channel, row, col;
    for (channel = 0; channel < in->n_canais; channel++){
        for (row = 0; row < in->altura; row++){
            for (col = 0; col < in->largura; col++){

              if (in->dados [0][row][col] > 65 && in->dados [0][row][col] < 160) {
                out->dados [channel][row][col] =  0;
              } else {
                out->dados [channel][row][col] = in->dados [channel][row][col];
              }

            }
        }
    }
}
