/*============================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "pdi.h"

/*============================================================================*/
#define INPUT_IMAGE "114.bmp"

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
  int qtde = rotulaFloodFill (img_out, &componentes, LARGURA_MIN, ALTURA_MIN, N_PIXELS_MIN);

  // Mostra os objetos encontrados.
  int i;
  for (i = 0; i < qtde; i++)
      desenhaRetangulo (componentes [i].roi, criaCor (255,0,0), img_out);
  salvaImagem (img_out, "02 - out-retangulo.bmp");


  int totalPixel = 0;
  int var = 0;
  int media = 0;
  int desvPadrao = 0;
  int totalArroz = 0;
  int menorComponente = 9999;
  int maiorComponente = 0;
  int min = 0;
  int max = 0;

  for (i = 0; i < qtde; i++){
     if(componentes[i].n_pixels < menorComponente)
        menorComponente = componentes[i].n_pixels;

    if (componentes[i].n_pixels > maiorComponente)
        maiorComponente = componentes[i].n_pixels;

     totalPixel += componentes[i].n_pixels;
      printf("%d - %d\n", i, componentes[i].n_pixels);
  }

  media = totalPixel/qtde;
  printf("\n media: %d\n",  media);
  printf("\n menor comp: %d\n",  menorComponente);
  printf("\n maior comp: %d\n",  maiorComponente);

  for (i = 0; i < qtde; i++){
    int sub = componentes[i].n_pixels - media;
    var = var + sub*sub;//pow(sub,2);
  }

  var = var / (qtde - 1);
  printf("\n variancia: %d\n",  var);

  desvPadrao = sqrt(var);
  printf("\n desvio padrao: %d\n",  (int)desvPadrao);

  // um arroz está entre 349 e 621, então menor que 349 é ruido e maior que 621 pode ser mais d eum arroz
  totalArroz = 0;
  min = menorComponente;
  max = media + desvPadrao;

  for (i = 0; i < qtde; i++){
    if(componentes[i].n_pixels <= media + desvPadrao){
      totalArroz = totalArroz + 1;

    }else{
      /*int j = 1;
      int result = componentes[i].n_pixels;

      while (result > max) {
        j++;
        result = result / j;
      }

      totalArroz = totalArroz + j;
    }*/

    float n =componentes [i].n_pixels / (float) media;
    totalArroz += (int) (n + 0.5f);
}

  }

  printf("total de arroz: %d\n", totalArroz );

  destroiImagem (img);

  return 0;
}
