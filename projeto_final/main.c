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
#define ALTURA_MIN 20
#define LARGURA_MIN 7
#define N_PIXELS_MIN 50

void binarizaInvert(Imagem* in, Imagem* out, float threshold);

/*============================================================================*/
int main() {

  // Abre a imagem em escala de cinza, e mantem uma copia colorida dela para desenhar a saida.
  Imagem* img = abreImagem (INPUT_IMAGE, 1);
  if (!img)
  {
      printf ("Erro abrindo a imagem.\n");
      exit (1);
  }

  Imagem* img_out =  criaImagem (img->largura, img->altura, 1);

  binarizaInvert (img, img_out, THRESHOLD);
  salvaImagem (img_out, "p_binarizada.bmp");

  Imagem* img_out2 = criaImagem (img->largura, img->altura, 1);
  Imagem* kernel = criaKernelCircular(7);
  erode (img_out, kernel, criaCoordenada(3,3), img_out2);
  salvaImagem (img_out2, "p_erodida.bmp");

  ComponenteConexo* componentes;
  int qtde = rotulaFloodFill (img_out2, &componentes, LARGURA_MIN, ALTURA_MIN, N_PIXELS_MIN);
printf("componentes:  %d\n",qtde );
  // Mostra os objetos encontrados.
  int i;
  int row, col;
  Cor cor = criaCor (255,0,0);
  for (i = 0; i < qtde; i++){

      int altura = componentes [i].roi.b - componentes [i].roi.c;
      int largura = componentes [i].roi.d - componentes [i].roi.e;

      printf ("nova largura %d", largura);
      printf ("nova altura %d", altura);

      Imagem* img_out = criaImagem(largura, altura, 1);

      for (row = 0; row < altura; row++)
          for (col = 0; col < largura; col++)
              //img_out->dados [0][row][col] =  obter valor do pixel da componentes[i];





      salvaImagem(img_out, "letra.bmp");

      desenhaRetangulo (componentes [i].roi, cor, img_out2);
  }

  salvaImagem (img_out2, "placa_out_rotulada.bmp");

  destroiImagem (img);

  return 0;
}

void binarizaInvert (Imagem* in, Imagem* out, float threshold)
{
    if (in->largura != out->largura || in->altura != out->altura || in->n_canais != out->n_canais)
    {
        printf ("ERRO: binariza: as imagens precisam ter o mesmo tamanho e numero de canais.\n");
        exit (1);
    }

    int channel, row, col;
    for (channel = 0; channel < in->n_canais; channel++)
        for (row = 0; row < in->altura; row++)
            for (col = 0; col < in->largura; col++)
                out->dados [channel][row][col] = (in->dados [channel][row][col] > threshold)? 0 : 1;
}
