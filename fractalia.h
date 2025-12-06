#include "raylib.h"
#include <time.h>
#include <stdlib.h>

typedef enum
{
    INICIO,
    TUTORIAL,
    NIVEL1,
    NIVEL2,
    NIVEL3
} Pantalla;

typedef enum
{
    TIPO_IMAGEN,
    TIPO_FRACCION
} TipoCarta;

typedef struct
{
    TipoCarta tipo;
    int numerador; // Para fracciones
    int denominador;
    int id_par;       // Para saber qué cartas son pareja
    Texture2D imagen; // Para imágenes (solo si tipo == TIPO_IMAGEN)
    int estado;       // 0=oculta, 1=visible, 2=acertada
} Carta;

void randomizar_tutorial(Carta tablero[], int num_celdas)
{
    tablero[0] = (Carta){TIPO_IMAGEN, 1, 2, 0, LoadTexture("assets/images/un_medio.png"), 0};
    tablero[1] = (Carta){TIPO_FRACCION, 1, 2, 0, {0}};

    tablero[2] = (Carta){TIPO_IMAGEN, 1, 3, 1, LoadTexture("assets/images/un_tercio.png"), 0};
    tablero[3] = (Carta){TIPO_FRACCION, 1, 3, 1, {0}};

    tablero[4] = (Carta){TIPO_IMAGEN, 2, 3, 2, LoadTexture("assets/images/dos_tercios.png"), 0};
    tablero[5] = (Carta){TIPO_FRACCION, 2, 3, 2, {0}};

    // MEZCLA LOS PARES
    for (int i = num_celdas - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);

        Carta temp = tablero[i];
        tablero[i] = tablero[j];
        tablero[j] = temp;
    }
}

void randomizar_nivel1(Carta tablero[], int num_celdas)
{
    tablero[0] = (Carta){TIPO_IMAGEN, 1, 2, 0, LoadTexture("assets/images/un_medio.png"), 0};
    tablero[1] = (Carta){TIPO_FRACCION, 1, 2, 0, {0}};

    tablero[2] = (Carta){TIPO_IMAGEN, 1, 3, 1, LoadTexture("assets/images/un_tercio.png"), 0};
    tablero[3] = (Carta){TIPO_FRACCION, 1, 3, 1, {0}};

    tablero[4] = (Carta){TIPO_IMAGEN, 2, 3, 2, LoadTexture("assets/images/dos_tercios.png"), 0};
    tablero[5] = (Carta){TIPO_FRACCION, 2, 3, 2, {0}};

    tablero[6] = (Carta){TIPO_IMAGEN, 1, 4, 3, LoadTexture("assets/images/un_cuarto.png"), 0};
    tablero[7] = (Carta){TIPO_FRACCION, 1, 4, 3, {0}};

    tablero[8] = (Carta){TIPO_IMAGEN, 4, 5, 4, LoadTexture("assets/images/cuatro_quintos.png"), 0};
    tablero[9] = (Carta){TIPO_FRACCION, 4, 5, 4, {0}};

    tablero[10] = (Carta){TIPO_IMAGEN, 7, 8, 5, LoadTexture("assets/images/siete_octavos.png"), 0};
    tablero[11] = (Carta){TIPO_FRACCION, 7, 8, 5, {0}};

    for (int i = num_celdas - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);

        Carta temp = tablero[i];
        tablero[i] = tablero[j];
        tablero[j] = temp;
    }
}

void randomizar_nivel2(Carta tablero[], int num_celdas)
{
    tablero[0] = (Carta){TIPO_IMAGEN, 0, 0, 0, LoadTexture("assets/images/sucesion1_1.png"), 0};
    tablero[1] = (Carta){TIPO_IMAGEN, 0, 0, 0, LoadTexture("assets/images/sucesion1_2.png"), 0};

    tablero[2] = (Carta){TIPO_IMAGEN, 0, 0, 1, LoadTexture("assets/images/sucesion2_1.png"), 0};
    tablero[3] = (Carta){TIPO_IMAGEN, 0, 0, 1, LoadTexture("assets/images/sucesion2_2.png"), 0};

    tablero[4] = (Carta){TIPO_IMAGEN, 0, 0, 2, LoadTexture("assets/images/sucesion3_1.png"), 0};
    tablero[5] = (Carta){TIPO_IMAGEN, 0, 0, 2, LoadTexture("assets/images/sucesion3_2.png"), 0};

    tablero[6] = (Carta){TIPO_IMAGEN, 0, 0, 3, LoadTexture("assets/images/sucesion4_1.png"), 0};
    tablero[7] = (Carta){TIPO_IMAGEN, 0, 0, 3, LoadTexture("assets/images/sucesion4_2.png"), 0};

    tablero[8] = (Carta){TIPO_IMAGEN, 0, 0, 4, LoadTexture("assets/images/sucesion5_1.png"), 0};
    tablero[9] = (Carta){TIPO_IMAGEN, 0, 0, 4, LoadTexture("assets/images/sucesion5_2.png"), 0};

    tablero[10] = (Carta){TIPO_IMAGEN, 0, 0, 5, LoadTexture("assets/images/sucesion6_1.png"), 0};
    tablero[11] = (Carta){TIPO_IMAGEN, 0, 0, 5, LoadTexture("assets/images/sucesion6_2.png"), 0};

    tablero[12] = (Carta){TIPO_IMAGEN, 0, 0, 6, LoadTexture("assets/images/sucesion7_1.png"), 0};
    tablero[13] = (Carta){TIPO_IMAGEN, 0, 0, 6, LoadTexture("assets/images/sucesion7_2.png"), 0};

    tablero[14] = (Carta){TIPO_IMAGEN, 0, 0, 7, LoadTexture("assets/images/sucesion8_1.png"), 0};
    tablero[15] = (Carta){TIPO_IMAGEN, 0, 0, 7, LoadTexture("assets/images/sucesion8_2.png"), 0};

    for (int i = num_celdas - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);

        Carta temp = tablero[i];
        tablero[i] = tablero[j];
        tablero[j] = temp;
    }
}

void randomizar_nivel3(Carta tablero[], int num_celdas)
{
    tablero[0] = (Carta){TIPO_IMAGEN, 0, 0, 0, LoadTexture("assets/images/sucesion1_1.png"), 0};
    tablero[1] = (Carta){TIPO_IMAGEN, 0, 0, 0, LoadTexture("assets/images/sucesion1_2.png"), 0};

    tablero[2] = (Carta){TIPO_IMAGEN, 0, 0, 1, LoadTexture("assets/images/sucesion2_1.png"), 0};
    tablero[3] = (Carta){TIPO_IMAGEN, 0, 0, 1, LoadTexture("assets/images/sucesion2_2.png"), 0};

    tablero[4] = (Carta){TIPO_IMAGEN, 0, 0, 2, LoadTexture("assets/images/sucesion3_1.png"), 0};
    tablero[5] = (Carta){TIPO_IMAGEN, 0, 0, 2, LoadTexture("assets/images/sucesion3_2.png"), 0};

    tablero[6] = (Carta){TIPO_IMAGEN, 0, 0, 3, LoadTexture("assets/images/sucesion4_1.png"), 0};
    tablero[7] = (Carta){TIPO_IMAGEN, 0, 0, 3, LoadTexture("assets/images/sucesion4_2.png"), 0};

    tablero[8] = (Carta){TIPO_IMAGEN, 0, 0, 4, LoadTexture("assets/images/suma1_1.png"), 0};
    tablero[9] = (Carta){TIPO_IMAGEN, 0, 0, 4, LoadTexture("assets/images/suma1_2.png"), 0};

    tablero[10] = (Carta){TIPO_IMAGEN, 0, 0, 5, LoadTexture("assets/images/suma2.png"), 0};
    tablero[11] = (Carta){TIPO_FRACCION, 5, 7, 5, {0}, 0};

    tablero[12] = (Carta){TIPO_IMAGEN, 0, 0, 6, LoadTexture("assets/images/suma3.png"), 0};
    tablero[13] = (Carta){TIPO_FRACCION, 2, 5, 6, {0}, 0};

    tablero[14] = (Carta){TIPO_IMAGEN, 0, 0, 7, LoadTexture("assets/images/suma4.png"), 0};
    tablero[15] = (Carta){TIPO_FRACCION, 5, 10, 7, {0}, 0};

    tablero[16] = (Carta){TIPO_IMAGEN, 0, 0, 8, LoadTexture("assets/images/suma5.png"), 0};
    tablero[17] = (Carta){TIPO_FRACCION, 4, 6, 8, {0}, 0};

    tablero[18] = (Carta){TIPO_IMAGEN, 0, 0, 9, LoadTexture("assets/images/un_decimo.png"), 0};
    tablero[19] = (Carta){TIPO_FRACCION, 1, 10, 9, {0}, 0};

    for (int i = num_celdas - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);

        Carta temp = tablero[i];
        tablero[i] = tablero[j];
        tablero[j] = temp;
    }
}

void dibujar_tablero(int offsetX, int offsetY, Carta tablero[], int columnas, int filas)
{
    int index = 0;
    int card_size;
    int spacing = 15;

    if (columnas == 3 && filas == 2)
    {
        card_size = 300;
    }
    else
    {
        if (columnas == 4 && filas == 3)
        {
            card_size = 250;
        }
        else
        {
            if (columnas == 4 && filas == 4)
            {
                card_size = 220;
            }
            else
            {
                if (columnas == 5 && filas == 4)
                {
                    card_size = 200;
                }
            }
        }
    }

    for (int y = 0; y < filas; y++)
    {
        for (int x = 0; x < columnas; x++)
        {
            Rectangle cell = {
                (float)offsetX + x * (card_size + spacing),
                (float)offsetY + y * (card_size + spacing),
                (float)card_size,
                (float)card_size};

            // ... resto del código igual, solo cambia card_size donde dice 400
            // Ejemplo:
            DrawRectangleRounded(cell, 0.3, 12, (Color){126, 230, 255, 255});
            DrawRectangleRoundedLines(cell, 0.3f, 12, BLACK);

            Carta carta = tablero[index];

            if (carta.estado == 0)
            {
                DrawRectangleRounded(cell, 0.3, 12, (Color){126, 230, 255, 255});
                DrawRectangleRoundedLines(cell, 0.3f, 12, BLACK);
            }
            else
            {
                DrawRectangleRounded(cell, 0.3, 12, WHITE);
                DrawRectangleRoundedLines(cell, 0.3f, 12, BLACK);

                if (carta.tipo == TIPO_IMAGEN)
                {
                    float scale = (card_size * 0.75f) / carta.imagen.width;
                    float imgX = cell.x + (card_size - carta.imagen.width * scale) / 2;
                    float imgY = cell.y + (card_size - carta.imagen.height * scale) / 2;

                    DrawTextureEx(carta.imagen, (Vector2){imgX, imgY}, 0.0f, scale, WHITE);
                }
                else
                {
                    int font_size = card_size / 4;

                    const char *numText = TextFormat("%d", carta.numerador);
                    int numWidth = MeasureText(numText, font_size);
                    DrawText(numText, cell.x + card_size / 2 - numWidth / 2, cell.y + card_size / 4, font_size, BLACK);

                    DrawLineEx(
                        (Vector2){cell.x + card_size * 0.25f, cell.y + card_size / 2},
                        (Vector2){cell.x + card_size * 0.75f, cell.y + card_size / 2},
                        4, BLACK);

                    const char *denText = TextFormat("%d", carta.denominador);
                    int denWidth = MeasureText(denText, font_size);
                    DrawText(denText, cell.x + card_size / 2 - denWidth / 2, cell.y + card_size * 0.6f, font_size, BLACK);
                }
            }

            index++;
        }
    }
}