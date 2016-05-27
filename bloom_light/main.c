/*============================================================================*/
/* Exemplo: segmenta��o de uma imagem em escala de cinza.                     */
/*----------------------------------------------------------------------------*/
/* Autor: Bogdan T. Nassu                                                     */
/* Universidade Tecnol�gica Federal do Paran�                                 */
/*============================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "pdi.h"

/*============================================================================*/

#define INPUT_IMAGE "./GT2.BMP"
//#define INPUT_IMAGE "./Wind Waker GC.bmp"

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

void filtro_media(Imagem* in, Imagem* out, int janela);
void filtro_media_ingenuo(Imagem* in, Imagem* out, int janela);
void binariza_custom(Imagem* in, Imagem* out, float threshold);
int rotula (Imagem* img, Componente** componentes, int largura_min, int altura_min, int n_pixels_min);

/*============================================================================*/

int main ()
{
    // Abre a imagem em escala de cinza, e mantem uma copia colorida dela para desenhar a saida.
    Imagem* img = abreImagem (INPUT_IMAGE, 3);
    if (!img)
    {
        printf ("Erro abrindo a imagem.\n");
        exit (1);
    }

    Imagem* img_out = criaImagem (img->largura, img->altura, 3);

    binariza_custom(img, img_out, THRESHOLD);

    salvaImagem (img_out, "limiarizacao.bmp");

    int sig = 2;
    Imagem *img_out2 = criaImagem(img_out->largura, img_out->altura, img_out->n_canais);
    filtroGaussiano(img_out, img_out2, sig, sig, NULL);

    salvaImagem (img_out2, "filtro_sig2.bmp");

    sig = sig * 2;

    Imagem *img_out3 = criaImagem(img_out->largura, img_out->altura, img_out->n_canais);
    filtroGaussiano(img_out, img_out3, sig, sig, NULL);

    salvaImagem (img_out3, "filtro_sig4.bmp");

    sig = sig * 2;

    Imagem *img_out4 = criaImagem(img_out->largura, img_out->altura, img_out->n_canais);
    filtroGaussiano(img_out, img_out4, sig, sig, NULL);

    salvaImagem (img_out4, "filtro_sig8.bmp");

    Imagem *img_soma1 = criaImagem(img_out->largura, img_out->altura, img_out->n_canais);
    Imagem *img_soma_final = criaImagem(img_out->largura, img_out->altura, img_out->n_canais);
    soma(img_out2, img_out3, 0.5, 0.5, img_soma1);
    soma(img_soma1, img_out4, 0.5, 0.5, img_soma_final);

    salvaImagem (img_soma_final, "filtro_soma.bmp");

    Imagem *img_final = criaImagem(img_out->largura, img_out->altura, img_out->n_canais);
    soma(img_soma_final, img, 0.4, 0.8, img_final);

    salvaImagem (img_final, "final.bmp");

    // Limpez4a.
    //free (c3omponentes);
    destroiImagem (img_out4);
    destroiImagem (img_out3);
    destroiImagem (img_out2);
    destroiImagem (img_out);
    destroiImagem (img);
    return (0);
}

/*----------------------------------------------------------------------------*/
/** Binariza��o simples por limiariza��o.
 *
 * A rotulagem obtém os pixels mais claros ( >= threshold) e adiciona na imagem
 * de saída, já os píxels >= threshold são rotulados com valor = 0.
 * Par�metros: Imagem* in: imagem de entrada. Se tiver mais que 1 canal,
 *               binariza cada canal independentemente.
 *             Imagem* out: imagem de sa�da. Deve ter o mesmo tamanho da
 *               imagem de entrada.
 *             float threshold: limiar.
 *
 * Valor de retorno: nenhum (usa a imagem de sa�da). */

void binariza_custom(Imagem* in, Imagem* out, float threshold)
{
    if (in->largura != out->largura || in->altura != out->altura || in->n_canais != out->n_canais)
    {
        printf ("ERRO: binariza: as imagens precisam ter o mesmo tamanho e numero de canais.\n");
        exit (1);
    }

    int canal, i, j;
    for (canal = 0; canal < in->n_canais; canal++) {
      for (i = 0; i < in->altura; i++) {
        for (j = 0; j < in->largura; j++){
          if (in->dados[canal][i][j] <= THRESHOLD) {
            out->dados[canal][i][j] = 0;
          }
          else {
            out->dados[canal][i][j] = in->dados[canal][i][j];
          }
        }
      }
    }
}

/*============================================================================*/
/* ROTULAGEM                                                                  */
/*============================================================================*/
/** Rotulagem usando flood fill. Marca os objetos da imagem com os valores
 * [0.1,0.2,etc].
 *
 * Par�metros: Imagem* img: imagem de entrada E sa�da.
 *             Componente** componentes: um ponteiro para um vetor de sa�da.
 *               Supomos que o ponteiro inicialmente � inv�lido. Ele ir�
 *               apontar para um vetor que ser� alocado dentro desta fun��o.
 *               Lembre-se de desalocar o vetor criado!
 *             int largura_min: descarta componentes com largura menor que esta.
 *             int altura_min: descarta componentes com altura menor que esta.
 *             int n_pixels_min: descarta componentes com menos pixels que isso.
 *
 * Valor de retorno: o n�mero de componentes conexos encontrados. */

int rotula (Imagem* img, Componente** componentes, int largura_min, int altura_min, int n_pixels_min)
{
    // TODO: escreva esta fun��o.
	// Use a abordagem com flood fill e uma pilha expl�cita, ou a abordagem em 2 passadas.
    // Se voc� pretende fazer a rotulagem usando flood fill, pode ser interessante criar uma outra fun��o apenas para a inunda��o.
	// Observe tamb�m que o par�metro 'componentes' � um ponteiro para um vetor, ent�o a aloca��o dele deve ser algo como:
	// *componentes = malloc (sizeof (Componente) * n);
	// Dependendo de como voc� fizer a sua implementa��o, pode ser tamb�m interessante alocar primeiro um vetor maior do que o necess�rio, ajustando depois o tamanho usando a fun��o realloc.
    return (0);
}

/*============================================================================*/
