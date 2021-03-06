/*============================================================================*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "pdi.h"

/*============================================================================*/
#define INPUT_IMAGE "imagens/p_test7.bmp"

#define NEGATIVO 0
#define THRESHOLD 0.5f
#define ALTURA_MIN 20
#define LARGURA_MIN 7
#define N_PIXELS_MIN 50

void chamar_jar();
void binarizaInvert(Imagem* in, Imagem* out, float threshold);
int ordenarLetras(int coordenadaXAtual, ComponenteConexo* componentes, int qtdeCompo);
Imagem* tratarLetras(Imagem* img_letra);
Imagem* tratarComponentes(ComponenteConexo* componentes, int qtdeCompo);
void tratarComp(ComponenteConexo* componentes, int qtdeCompo);
/*============================================================================*/
// novo codigo
typedef struct
{
	int compID;
	int tamanhoX;
  int tamanhoY;

} ComponenteAux;

int main(int argc, char **argv) {

  char *img_path = NULL;

  if (argc == 2)
  {
    img_path = argv[1];
  }
  else {
    printf("Passe uma imagem por parametro");
    exit(1);
  }

  // Abre a imagem em escala de cinza, e mantem uma copia colorida dela para desenhar a saida.
  Imagem* img = abreImagem (img_path, 1);
  if (!img)
  {
      printf ("Erro abrindo a imagem.\n");
      exit (1);
  }

  Imagem* img_out =  criaImagem (img->largura, img->altura, 1);

  binarizaInvert (img, img_out, THRESHOLD);
  salvaImagem (img_out, "imagens/processada/p_binarizada.bmp");

  Imagem* img_out2 = criaImagem (img->largura, img->altura, 1);

  Imagem* kernel = criaKernelCircular(5);
  erode (img_out, kernel, criaCoordenada(2,2), img_out2);
  salvaImagem (img_out2, "imagens/processada/p_erodida.bmp");

  ComponenteConexo* componentes;
  int qtde = rotulaFloodFill (img_out2, &componentes, LARGURA_MIN, ALTURA_MIN, N_PIXELS_MIN);
  printf("componentes:  %d\n",qtde );

	//tratarComponentes(componentes ,qtde);
	tratarComponentes(componentes ,qtde);

  //Componetes auxiliar
  //ComponenteAux* compAux = (ComponenteAux *) malloc(sizeof(ComponenteAux) * qtde);

  //ordenarLetras(componentes);
  // Mostra os objetos encontrados.
  int i;
  int row, col;
  Cor cor = criaCor (255,0,0);
  for (i = 0; i < qtde; i++){

    int altura = componentes[i].roi.b - componentes[i].roi.c;
    int largura = componentes[i].roi.d - componentes[i].roi.e;
    //printf ("nova largura %d", largura);
    //printf ("nova altura %d", altura);

    Imagem* img_out = criaImagem(largura, altura, 1);

    // preencher imagem
    int row;
    int col;
    int channel;
    for (channel = 0; channel < img_out2->n_canais; channel++){
        int row_in = 0;
        int col_in = 0;
        int row_out = 0;
        int col_out = 0;
        for (row_in = componentes[i].roi.c; row_in < componentes[i].roi.b; row_in++) {
            for (col_in = componentes[i].roi.e; col_in < componentes[i].roi.d; col_in++) {
                img_out->dados[channel][row_out][col_out] = img_out2->dados[channel][row_in][col_in];
                col_out = col_out + 1;
            }
            col_out = 0;
            row_out = row_out + 1;
        }
    }

    char str1[15];
    sprintf(str1, "%d", ordenarLetras(componentes[i].roi.e, componentes, qtde));
    char str2[25] = "_letra.bmp";
    char caminho[30] = "imagens/processada/letras/";
    char *res = strcat(caminho, strcat(str1, str2));

    //binarizaInvert(img_out, img_out, 0.1f); // workaround ou gambiaround :P
    salvaImagem(tratarLetras(img_out), res);

    desenhaRetangulo (componentes [i].roi, cor, img_out2);
  }

  salvaImagem (img_out2, "imagens/processada/placa_out_rotulada.bmp");

  destroiImagem (img);
  destroiImagem (img_out2);
  destroiImagem (img_out);

  chamar_jar();

  return 0;
}

void chamar_jar(){
  system("java -jar rede/car-neural-network.jar -cp");
}

int ordenarLetras(int coordenadaXAtual, ComponenteConexo* componentes, int qtdeCompo){
  int posicao = 0;
  for (int i = 0; i < qtdeCompo; i++) {
      int coordenadaX1 = componentes[i].roi.e;
      if (coordenadaXAtual > coordenadaX1) {
        posicao = posicao + 1;
      }
  }
  return posicao;
}
/**
Par�metros: int c: posi��o y do lado superior.
*             int b: posi��o y do lado inferior.
*             int e: posi��o x do lado esquerdo.
*             int d: posi��o x do lado direito.

*/

void tratarComp(ComponenteConexo* componentes, int qtdeCompo){
  int tamanhoY= 0;
  int thresholdTamanhoY = 5;
	int aux = 0;
	ComponenteConexo *componentesAux =
			(ComponenteConexo *)malloc(sizeof(ComponenteConexo) * (qtdeCompo));
	//*componentes = (ComponenteConexo*) malloc (sizeof (ComponenteConexo*) * (qtdeCompo));
  for (int i = 0; i < qtdeCompo; i++) {
    tamanhoY =  componentes[i].roi.c - componentes[i].roi.b;
		if (i < (qtdeCompo -1) ) {
			int tamanhoYprox = componentes[i+1].roi.c - componentes[i+1].roi.b;
			aux = tamanhoY - tamanhoYprox;
			if (aux < 0) {
				aux = aux * - 1;
			}
	    if (aux > thresholdTamanhoY) {
				printf("compoente teste\n");
				componentesAux[i] = componentes[i];
				//ComponenteConexo* c = &((*componentes) [i]);
      	//free(c);
	      // desaloca componente, exclui, sei la....
	    }
		}
  }
	*componentes = *componentesAux;
}


Imagem* tratarComponentes(ComponenteConexo* componentes, int qtdeCompo) {
  int tamanhoY = 0;
  int thresholdTamanhoY = 20;
  int aux = 0;
  int i = 0;

  int indexes[qtdeCompo];
  int j = 0;
  for (i = 0; i < qtdeCompo; i++) {
    float tamanho = componentes[i].roi.b - componentes[i].roi.c;

		//mudanca
		tamanhoY =  componentes[i].roi.c - componentes[i].roi.b;
		printf("tamanho y %d\n", tamanhoY);
		if (i < (qtdeCompo -1) ) {
			int tamanhoYprox = componentes[i+1].roi.c - componentes[i+1].roi.b;
				printf("tamanho y %d\n", tamanhoYprox);
			aux = tamanhoY - tamanhoYprox;
			if (aux < 0) {
				aux = aux * - 1;
			}
	    if (aux > thresholdTamanhoY) {
				printf("compoente teste\n");
				indexes[j] = i;
	      j++;
	    }
		}

			/*

    if (tamanho > 70.0) {
      indexes[j] = i;
      j++;
    }*/
  }

  ComponenteConexo *componentesAux =
      (ComponenteConexo *)malloc(sizeof(ComponenteConexo) * (qtdeCompo - j));

  int finalIndex = 0;
  for (i = 0; i < qtdeCompo; i++) {
    int achou = 0;
    int z;
    for (z = 0; z < j; z++) {
      if (indexes[z] == i) {
        achou = 1;
        break;
      }
    }
    if (achou == 0) {
      componentesAux[finalIndex] = componentes[i];
      finalIndex++;
    }
  }

  *componentes = *componentesAux;
}


Imagem* tratarLetras(Imagem* img_letra) {
  binariza(img_letra, img_letra, 0.01f); // workaround ou gambiaround :P
  Imagem* img_out2 = criaImagem (img_letra->largura, img_letra->altura, 1);
  Imagem* kernel = criaKernelCircular(3);
  int qtde = 0;
  int x = 0;
  Imagem *img_previous;
  /*do {
    ComponenteConexo* componentes;
    qtde = rotulaFloodFill (img_letra, &componentes, 5, 5, 1);
    binariza(img_letra, img_letra, 0.01f); // workaround ou gambiaround :P
    erode (img_letra, kernel, criaCoordenada(1,1), img_out2);

    if (qtde == 1)
      img_letra = clonaImagem(img_out2);
    printf("componentes %d\n", qtde);

  } while(qtde == 1);*/

  binariza(img_letra, img_letra, 0.01f); // workaround ou gambiaround :P

  Imagem *img_final = NULL;
  img_final = criaImagem(20, 20, img_letra->n_canais); // 32, 56
  redimensionaNN(img_letra, img_final);

  return img_final;
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
