/*============================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "pdi.h"

/*============================================================================*/
#define INPUT_IMAGE "2.bmp"
#define INPUT_IMAGE_BACKGROUND "paisagem.bmp"

#define NEGATIVO 0
#define THRESHOLD 0.7f
#define ALTURA_MIN 10
#define LARGURA_MIN 10
#define N_PIXELS_MIN 20

#define TRUE 1
#define FALSE 0

void chroma_key(Imagem* in, Imagem* bg, Imagem* out, float d)
{
    int row, column;
    for (row = 0; row < in->altura; row++)
    {
        for (column = 0; column < in->largura; column++)
        {
            float green = in->dados[1][row][column];
            float red = in->dados[0][row][column];
            float blue = in->dados[2][row][column];

            printf("linha = %d - coluna %d - red = %f - green = %f - blue = %f\n", row, column, red, green, blue);

            int eh_fundo = FALSE;
            
            // verifica se eh fundo
            if ((green > (red + d)) && (green > (blue + d)))
            {
                eh_fundo = TRUE;
            }
            
            int canal;
            for (canal = 0; canal < in->n_canais; canal++)
            {
                if (eh_fundo == TRUE)
                {
                    out->dados[canal][row][column] = bg->dados[canal][row][column];
                }
                else
                {
                    out->dados[canal][row][column] = in->dados[canal][row][column];
                }
            }
            
        }
    }
}

/*============================================================================*/
int main(int argc, char **argv)
{
    Imagem* img = abreImagem (INPUT_IMAGE, 3);
    if (!img)
    {
        printf("Erro abrindo a imagem.\n");
        exit(1);
    }

    Imagem* img_bg = abreImagem (INPUT_IMAGE_BACKGROUND, 3);
    if (!img_bg)
    {
        printf("Erro abrindo a imagem a ser aplicada no chromakey.\n");
        exit(1);
    }
    
    Imagem *img_bg_redimensionada = NULL;
    if (img_bg->altura != img->altura || img_bg->largura != img->largura)
    {
        img_bg_redimensionada = criaImagem(img->largura, img->altura, img->n_canais);
        if (!img_bg_redimensionada)
        {
            printf("Erro ao criar a imagem para redimensionar.\n");
            exit(1);
        }
        redimensionaNN(img_bg, img_bg_redimensionada);
    }
    
    if (img_bg_redimensionada == NULL)
    {
        img_bg_redimensionada = img_bg;
    }
    
    Imagem *img_final = criaImagem(img->largura, img->altura, img->n_canais);
    if (!img_final)
    {
        printf("Erro ao criar imagem para armazenar o resultado.\n");
        exit(1);
    }
    
    chroma_key(img, img_bg_redimensionada, img_final, 0.1);
    
    salvaImagem(img_final, "chroma_key.bmp");
    
    destroiImagem(img_final);
    destroiImagem(img_bg_redimensionada);
    destroiImagem(img_bg);
    destroiImagem(img);
    
    return 0;
}

