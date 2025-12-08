 #include "raylib.h"
#include "fractalia.h"
#include <time.h>
#include <stdlib.h>

#define STAR_COUNT 300

static float starX[STAR_COUNT];
static float starY[STAR_COUNT];
static float starSpeed[STAR_COUNT];
int estrellas_inicializadas = 0;
int randomizado = 0;
Pantalla Pantalla_Actual;
Pantalla nivel_actual;
Font fuenteLilita;
Texture2D Corazon;

void estrellas(float dt, int pantalla_ancho, int pantalla_alto);
void PantallaMenu(int pantalla_ancho, int pantalla_alto);
void tutorial();
void nivel1();
void nivel2();
void nivel3();
void pantalla_final(int tiempo_restante, int vida_rival, int nivel, bool *resetear);
void pantalla_creditos();

int main(void)
{
    srand(time(NULL));
    const int pantalla_ancho = 1920;
    const int pantalla_alto = 1080;

    InitWindow(pantalla_ancho, pantalla_alto, "FRACTALIA");
    InitAudioDevice();
    SetTargetFPS(60);

    Texture2D logo = LoadTexture("assets/images/LOGO FRACTALIA.png");
    Music musicaMenu = LoadMusicStream("assets/music/musica_menu.mp3");
    Music musicaTuto = LoadMusicStream("assets/music/tutorial.mp3");
    Music musicaLvl1 = LoadMusicStream("assets/music/level_1.mp3");
    Music musicaLvl2 = LoadMusicStream("assets/music/level_2.mp3");
    Music musicaLvl3 = LoadMusicStream("assets/music/level_3.mp3");
    fuenteLilita = LoadFont("assets/fonts/LilitaOneRegular.ttf");
    Corazon = LoadTexture("assets/images/heart.png");
    PlayMusicStream(musicaMenu);

    Pantalla_Actual = INICIO;

    while (!WindowShouldClose())
    {
        UpdateMusicStream(musicaMenu);
        UpdateMusicStream(musicaTuto);
        UpdateMusicStream(musicaLvl1);
        UpdateMusicStream(musicaLvl2);
        UpdateMusicStream(musicaLvl3);

        float dt = GetFrameTime();

        BeginDrawing();
        ClearBackground(DARKBLUE);

        switch (Pantalla_Actual)
        {
        case INICIO:
            StopMusicStream(musicaTuto);
            StopMusicStream(musicaLvl1);
            StopMusicStream(musicaLvl2);
            StopMusicStream(musicaLvl3);

            if (!IsMusicStreamPlaying(musicaMenu))
                PlayMusicStream(musicaMenu);

            estrellas(dt, pantalla_ancho, pantalla_alto);
            PantallaMenu(pantalla_ancho, pantalla_alto);
            break;

        case TUTORIAL:
            StopMusicStream(musicaMenu);
            StopMusicStream(musicaLvl1);
            StopMusicStream(musicaLvl2);
            StopMusicStream(musicaLvl3);

            if (!IsMusicStreamPlaying(musicaTuto))
                PlayMusicStream(musicaTuto);

            estrellas(dt, pantalla_ancho, pantalla_alto);
            tutorial();
            break;

        case NIVEL1:
            StopMusicStream(musicaMenu);
            StopMusicStream(musicaTuto);
            StopMusicStream(musicaLvl2);
            StopMusicStream(musicaLvl3);

            if (!IsMusicStreamPlaying(musicaLvl1))
                PlayMusicStream(musicaLvl1);

            estrellas(dt, pantalla_ancho, pantalla_alto);
            nivel1();
            break;

        case NIVEL2:
            StopMusicStream(musicaMenu);
            StopMusicStream(musicaTuto);
            StopMusicStream(musicaLvl1);
            StopMusicStream(musicaLvl3);

            if (!IsMusicStreamPlaying(musicaLvl2))
                PlayMusicStream(musicaLvl2);

            estrellas(dt, pantalla_ancho, pantalla_alto);
            nivel2();
            break;

        case NIVEL3:
            StopMusicStream(musicaMenu);
            StopMusicStream(musicaTuto);
            StopMusicStream(musicaLvl1);
            StopMusicStream(musicaLvl2);

            if (!IsMusicStreamPlaying(musicaLvl3))
                PlayMusicStream(musicaLvl3);

            estrellas(dt, pantalla_ancho, pantalla_alto);
            nivel3();
            break;
        }

        EndDrawing();
    }

    UnloadTexture(logo);
    UnloadMusicStream(musicaMenu);
    UnloadFont(fuenteLilita);
    UnloadTexture(Corazon);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}

void PantallaMenu(int screenWidth, int screenHeight)
{
    static int cargado = 0;
    static Texture2D logo;

    if (cargado == 0)
    {
        logo = LoadTexture("assets/images/LOGO FRACTALIA.png");
        cargado = 1;
    }

    float targetWidth = screenWidth * 0.40f;
    float scale = targetWidth / logo.width;

    float posX = (screenWidth - logo.width * scale) / 2;
    float posY = -20;

    Vector2 mouse = GetMousePosition();
    bool click = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    DrawTextureEx(logo, (Vector2){posX, posY}, 0.0f, scale, WHITE);

    int btnWidth = 420;
    int btnHeight = 100;
    float x = screenWidth / 2 - btnWidth / 2;

    Rectangle btnJugar = {x, 650, (float)btnWidth, (float)btnHeight};
    Rectangle btnSalir = {x, 800, (float)btnWidth, (float)btnHeight};

    Color colorJugar = CheckCollisionPointRec(mouse, btnJugar) ? YELLOW : GOLD;
    Color colorSalir = CheckCollisionPointRec(mouse, btnSalir) ? VIOLET : PURPLE;

    DrawRectangleRounded(btnJugar, 0.3f, 12, colorJugar);
    DrawRectangleRounded(btnSalir, 0.3f, 12, colorSalir);

    DrawText("JUGAR", x + 110, 665, 65, BLACK);
    DrawText("SALIR", x + 110, 815, 65, BLACK);

    if (CheckCollisionPointRec(mouse, btnSalir) && click)
    {
        CloseWindow();
    }

    if (CheckCollisionPointRec(mouse, btnJugar) && click)
    {
        Pantalla_Actual = TUTORIAL;
    }
}

void tutorial()
{
    nivel_actual = TUTORIAL;
    static Carta tablero[6];
    static int randomizado = 0;
    Vector2 mouse = GetMousePosition();
    bool click = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    int index = 0;
    static int primera_carta = -1;
    static int segunda_carta = -1;
    static float tiempo_espera = 0;
    bool click_procesado = false;
    static int cargado = 0;
    static Texture2D fract;
    static int clicks = 0;
    static bool inicio = 0;
    static int pares_encontrados = 0;
    static int vida_rival = 3;
    static float tiempo_restante = 90.0f;
    static bool turno_jugador = true;
    static float tiempo_turno_rival = 0;
    static bool debe_resetear = false;
    static int intentos_fallidos = 0; // CAMBIAR NOMBRE
    static float tiempoMensaje = 0;   // Para los mensajes
    static const char frases[14][20] = {"MUY BIEN!", "EXCELENTE!", "GENIAL!", "PERFECTO!", "WOW!", "INCREIBLE!", "FANTASTICO!", "SUPER!", "IMPACTANTE!", "ASOMBROSO!", "BRILLANTE!", "SORPRENDENTE!", "LO LOGRASTE!", "MAGNIFICO!"};
    static const char *fraseActual;

    if (debe_resetear)
    {
        randomizado = 0;
        primera_carta = -1;
        segunda_carta = -1;
        tiempo_espera = 0;
        clicks = 0;
        inicio = 0;
        pares_encontrados = 0;
        vida_rival = 3;
        tiempo_restante = 90.0f;
        turno_jugador = true;
        tiempo_turno_rival = 0;
        debe_resetear = false;
        intentos_fallidos = 0; // RESETEAR
    }

    if (cargado == 0)
    {
        fract = LoadTexture("assets/images/sprite_fract.png");
        cargado = 1;
    }

    if (!randomizado)
    {
        randomizar_tutorial(tablero, 6);
        randomizado = 1;
    }

    if (inicio == 0)
    {
        DrawTextureEx(fract, (Vector2){760, 680}, 0.0f, 0.4, WHITE);
        DrawCircle(1065, 835, 12, WHITE);
        DrawCircle(1080, 820, 18, WHITE);
        DrawRectangleRounded((Rectangle){790, 400, 380, 220}, 0.3f, 10, WHITE);

        DrawCircleLines(1065, 835, 12, BLACK);
        DrawCircleLines(1080, 820, 18, BLACK);
        DrawRectangleRoundedLines((Rectangle){790, 400, 380, 220}, 0.3f, 10, BLACK);

        if (click)
        {
            clicks++;
            if (clicks >= 4)
            {
                inicio = true;
            }
        }

        switch (clicks)
        {
        case 0:
            DrawText("BIENVENIDO AL", 980 - MeasureText("BIENVENIDO AL", 25) / 2, 420, 25, BLACK);
            DrawText("REINO DE FRACTALIA!", 980 - MeasureText("REINO DE FRACTALIA!", 25) / 2, 450, 25, BLACK);
            DrawText("NECESITO TU AYUDA PARA", 980 - MeasureText("NECESITO TU AYUDA PARA", 22) / 2, 490, 22, BLACK);
            DrawText("RESOLVER LOS MEMORAMAS", 980 - MeasureText("RESOLVER LOS MEMORAMAS", 22) / 2, 515, 22, BLACK);
            DrawText("Y ASI SALVAR EL REINO", 980 - MeasureText("Y ASI SALVAR EL REINO", 22) / 2, 540, 22, BLACK);
            break;

        case 1:
            DrawText("ES UN JUEGO POR TURNOS", 980 - MeasureText("ES UN JUEGO POR TURNOS", 23) / 2, 440, 23, BLACK);
            DrawText("TU INTENTAS HACER UN PAR", 980 - MeasureText("TU INTENTAS HACER UN PAR", 22) / 2, 475, 22, BLACK);
            DrawText("SI ACIERTAS, QUITAS 1 VIDA", 980 - MeasureText("SI ACIERTAS, QUITAS 1 VIDA", 22) / 2, 510, 22, BLACK);
            break;

        case 2:
            DrawText("SI FALLAS 2 VECES, EL RIVAL", 980 - MeasureText("SI FALLAS 2 VECES, EL RIVAL", 23) / 2, 450, 23, BLACK);
            DrawText("MEZCLA LAS CARTAS", 980 - MeasureText("MEZCLA LAS CARTAS", 23) / 2, 485, 23, BLACK);
            break;

        case 3:
            DrawText("TU PUEDES!", 980 - MeasureText("TU PUEDES!", 30) / 2, 460, 30, BLACK);
            break;
        }

        DrawText("CLICK PARA CONTINUAR", 980 - MeasureText("CLICK PARA CONTINUAR", 18) / 2, 580, 18, (Color){100, 100, 100, 180});
        return;
    }

    // ACTUALIZAR TIEMPO
    if (tiempo_restante > 0)
    {
        tiempo_restante -= GetFrameTime();
    }

    float margenX = 1450.0f;
    float margenY = 50.0f;
    float corazonSize = 40.0f;

    // MOSTRAR INFO
    DrawTextEx(fuenteLilita, TextFormat("Tiempo: %.0f", tiempo_restante), (Vector2){margenX, margenY}, 50, 2, WHITE);

    // TURNO
    if (turno_jugador)
    {
        DrawTextEx(fuenteLilita, "TU TURNO", (Vector2){margenX, margenY + 70}, 40, 2, GREEN);
        DrawTextEx(fuenteLilita, TextFormat("Fallos: %d/2", intentos_fallidos), (Vector2){margenX, margenY + 120}, 35, 2, YELLOW);
    }
    else
    {
        DrawTextEx(fuenteLilita, "TURNO RIVAL", (Vector2){margenX, margenY + 70}, 40, 2, RED);
    }

    // VIDA RIVAL
    DrawTextEx(fuenteLilita, "VIDA RIVAL:", (Vector2){margenX, margenY + 180}, 35, 2, WHITE);
    for (int i = 0; i < vida_rival; i++)
    {
        DrawTextureEx(Corazon, (Vector2){margenX + i * (corazonSize + 5), margenY + 215}, 0.0f, corazonSize / (float)Corazon.width, WHITE);
    }

    // MENSAJES SI ACERTASTE
    if (tiempoMensaje > 0)
    {
        DrawTextEx(fuenteLilita, TextFormat(fraseActual), (Vector2){margenX, margenY + 270}, 35, 2, GREEN);
        tiempoMensaje -= GetFrameTime();
    }

    // TURNO DEL RIVAL
    if (!turno_jugador)
    {
        if (tiempo_turno_rival == 0)
        {
            tiempo_turno_rival = 2.0f;
        }

        tiempo_turno_rival -= GetFrameTime();
        DrawTextEx(fuenteLilita, "EL RIVAL ESTA MEZCLANDO...", (Vector2){margenX, margenY + 300}, 30, 2, YELLOW);

        if (tiempo_turno_rival <= 0)
        {
            int cartas_ocultas = 0;
            for (int i = 0; i < 6; i++)
            {
                if (tablero[i].estado == 0)
                    cartas_ocultas++;
            }

            int cartas_a_mezclar = (cartas_ocultas + 1) / 4;
            int mezcladas = 0;

            for (int i = 5; i > 0 && mezcladas < cartas_a_mezclar; i--)
            {
                if (tablero[i].estado == 0)
                {
                    int j = rand() % (i + 1);
                    if (tablero[j].estado == 0)
                    {
                        Carta temp = tablero[i];
                        tablero[i] = tablero[j];
                        tablero[j] = temp;
                        mezcladas++;
                    }
                }
            }

            turno_jugador = true;
            tiempo_turno_rival = 0;
            intentos_fallidos = 0; // RESETEAR CONTADOR
        }

        dibujar_tablero(200, 200, tablero, 3, 2);
        DrawLine(1400, 0, 1400, 1080, BLACK);
        DrawTextureEx(fract, (Vector2){1450, 680}, 0.0f, 0.4, WHITE);
        pantalla_final(tiempo_restante, vida_rival, 0, &debe_resetear);
        return;
    }

    // TEMPORIZADOR DE ESPERA
    if (tiempo_espera > 0)
    {
        tiempo_espera -= GetFrameTime();
        if (tiempo_espera <= 0)
        {
            tablero[primera_carta].estado = 0;
            tablero[segunda_carta].estado = 0;
            primera_carta = -1;
            segunda_carta = -1;

            // SI LLEGÓ A 2 FALLOS, CAMBIAR TURNO
            if (intentos_fallidos >= 2)
            {
                turno_jugador = false;
            }
        }
    }

    dibujar_tablero(200, 200, tablero, 3, 2);
    DrawLine(1400, 0, 1400, 1080, BLACK);
    DrawTextureEx(fract, (Vector2){1450, 680}, 0.0f, 0.4, WHITE);

    // TURNO DEL JUGADOR
    if (turno_jugador && click && tiempo_espera <= 0 && !click_procesado)
    {
        int index = 0;
        for (int y = 0; y < 2; y++)
        {
            for (int x = 0; x < 3; x++)
            {
                Rectangle cell = {
                    (float)200 + x * 315, // 300 + 15
                    (float)200 + y * 315,
                    300,
                    300};

                if (CheckCollisionPointRec(mouse, cell) && !click_procesado)
                {
                    if (tablero[index].estado == 0 && segunda_carta == -1)
                    {
                        tablero[index].estado = 1;
                        click_procesado = true;

                        if (primera_carta == -1)
                        {
                            primera_carta = index;
                        }
                        else
                        {
                            segunda_carta = index;

                            if (tablero[primera_carta].id_par == tablero[segunda_carta].id_par)
                            {
                                // ACIERTO
                                tablero[primera_carta].estado = 2;
                                tablero[segunda_carta].estado = 2;
                                primera_carta = -1;
                                segunda_carta = -1;
                                pares_encontrados++;
                                vida_rival--;
                                turno_jugador = true;
                                tiempoMensaje = 1.0f; // Iniciar temporizador si estaba apagado
                                // NO INCREMENTAR intentos_fallidos
                                fraseActual = frases[rand() % 14];
                            }
                            else
                            {
                                // FALLO
                                tiempo_espera = 0.8f;
                                intentos_fallidos++; // INCREMENTAR AQUÍ
                            }
                        }
                    }
                }
                index++;
            }
        }
    }

    pantalla_final(tiempo_restante, vida_rival, 0, &debe_resetear);
}

void nivel1()
{
    nivel_actual = NIVEL1;
    static Carta tablero[12];
    static int randomizado = 0;
    Vector2 mouse = GetMousePosition();
    bool click = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    int index = 0;
    static int primera_carta = -1;
    static int segunda_carta = -1;
    static float tiempo_espera = 0;
    bool click_procesado = false;
    static int cargado = 0;
    static Texture2D jefe1;
    static int pares_encontrados = 0;
    static int vida_rival = 6;
    static float tiempo_restante = 150.0f;
    static bool turno_jugador = true;
    static float tiempo_turno_rival = 0;
    static bool debe_resetear = false;
    static int intentos_fallidos = 0; // CAMBIAR NOMBRE
    static float tiempoMensaje = 0;   // Para los mensajes
    static const char frases[14][20] = {"MUY BIEN!", "EXCELENTE!", "GENIAL!", "PERFECTO!", "WOW!", "INCREIBLE!", "FANTASTICO!", "SUPER!", "IMPACTANTE!", "ASOMBROSO!", "BRILLANTE!", "SORPRENDENTE!", "LO LOGRASTE!", "MAGNIFICO!"};
    static const char *fraseActual;

    if (debe_resetear)
    {
        randomizado = 0;
        primera_carta = -1;
        segunda_carta = -1;
        tiempo_espera = 0;
        pares_encontrados = 0;
        vida_rival = 6;
        tiempo_restante = 150.0f;
        turno_jugador = true;
        tiempo_turno_rival = 0;
        debe_resetear = false;
        intentos_fallidos = 0; // RESETEAR
    }

    if (cargado == 0)
    {
        jefe1 = LoadTexture("assets/images/boss1.png");
        cargado = 1;
    }

    if (!randomizado)
    {
        randomizar_nivel1(tablero, 12);
        randomizado = 1;
    }

    // ACTUALIZAR TIEMPO
    if (tiempo_restante > 0)
    {
        tiempo_restante -= GetFrameTime();
    }

    float margenX = 1450.0f;
    float margenY = 50.0f;
    float corazonSize = 40.0f;

    // MOSTRAR INFO
    DrawTextEx(fuenteLilita, TextFormat("Tiempo: %.0f", tiempo_restante), (Vector2){margenX, margenY}, 50, 2, WHITE);

    // TURNO
    if (turno_jugador)
    {
        DrawTextEx(fuenteLilita, "TU TURNO", (Vector2){margenX, margenY + 70}, 40, 2, GREEN);
        DrawTextEx(fuenteLilita, TextFormat("Fallos: %d/2", intentos_fallidos), (Vector2){margenX, margenY + 120}, 35, 2, YELLOW);
    }
    else
    {
        DrawTextEx(fuenteLilita, "TURNO RIVAL", (Vector2){margenX, margenY + 70}, 40, 2, RED);
    }

    // VIDA RIVAL
    DrawTextEx(fuenteLilita, "VIDA RIVAL:", (Vector2){margenX, margenY + 180}, 35, 2, WHITE);
    for (int i = 0; i < vida_rival; i++)
    {
        DrawTextureEx(Corazon, (Vector2){margenX + i * (corazonSize + 5), margenY + 215}, 0.0f, corazonSize / (float)Corazon.width, WHITE);
    }

    // MENSAJES SI ACERTASTE
    if (tiempoMensaje > 0)
    {
        DrawTextEx(fuenteLilita, TextFormat(fraseActual), (Vector2){margenX, margenY + 270}, 35, 2, GREEN);
        tiempoMensaje -= GetFrameTime();
    }

    // TURNO DEL RIVAL
    if (!turno_jugador)
    {
        if (tiempo_turno_rival == 0)
        {
            tiempo_turno_rival = 2.0f;
        }

        tiempo_turno_rival -= GetFrameTime();
        DrawTextEx(fuenteLilita, "EL RIVAL ESTA MEZCLANDO...", (Vector2){margenX, margenY + 300}, 30, 2, YELLOW);

        if (tiempo_turno_rival <= 0)
        {
            int cartas_ocultas = 0;
            for (int i = 0; i < 12; i++)
            {
                if (tablero[i].estado == 0)
                    cartas_ocultas++;
            }

            int cartas_a_mezclar = (cartas_ocultas + 1) / 4;
            int mezcladas = 0;

            for (int i = 11; i > 0 && mezcladas < cartas_a_mezclar; i--)
            {
                if (tablero[i].estado == 0)
                {
                    int j = rand() % (i + 1);
                    if (tablero[j].estado == 0)
                    {
                        Carta temp = tablero[i];
                        tablero[i] = tablero[j];
                        tablero[j] = temp;
                        mezcladas++;
                    }
                }
            }

            turno_jugador = true;
            tiempo_turno_rival = 0;
            intentos_fallidos = 0; // RESETEAR CONTADOR
        }

        dibujar_tablero(200, 150, tablero, 4, 3);
        DrawLine(1400, 0, 1400, 1080, BLACK);
        DrawTextureEx(jefe1, (Vector2){1450, 680}, 0.0f, 0.4, WHITE);
        pantalla_final(tiempo_restante, vida_rival, 0, &debe_resetear);
        return;
    }

    // TEMPORIZADOR DE ESPERA
    if (tiempo_espera > 0)
    {
        tiempo_espera -= GetFrameTime();
        if (tiempo_espera <= 0)
        {
            tablero[primera_carta].estado = 0;
            tablero[segunda_carta].estado = 0;
            primera_carta = -1;
            segunda_carta = -1;

            // SI LLEGÓ A 2 FALLOS, CAMBIAR TURNO
            if (intentos_fallidos >= 2)
            {
                turno_jugador = false;
            }
        }
    }

    dibujar_tablero(200, 150, tablero, 4, 3);
    DrawLine(1400, 0, 1400, 1080, BLACK);
    DrawTextureEx(jefe1, (Vector2){1450, 680}, 0.0f, 0.4, WHITE);

    // TURNO DEL JUGADOR
    if (turno_jugador && click && tiempo_espera <= 0 && !click_procesado)
    {
        int index = 0;
        for (int y = 0; y < 3; y++)
        {
            for (int x = 0; x < 4; x++)
            {
                Rectangle cell = {
                    (float)200 + x * 265, // 250 + 15
                    (float)150 + y * 265,
                    250,
                    250};

                if (CheckCollisionPointRec(mouse, cell) && !click_procesado)
                {
                    if (tablero[index].estado == 0 && segunda_carta == -1)
                    {
                        tablero[index].estado = 1;
                        click_procesado = true;

                        if (primera_carta == -1)
                        {
                            primera_carta = index;
                        }
                        else
                        {
                            segunda_carta = index;

                            if (tablero[primera_carta].id_par == tablero[segunda_carta].id_par)
                            {
                                // ACIERTO
                                tablero[primera_carta].estado = 2;
                                tablero[segunda_carta].estado = 2;
                                primera_carta = -1;
                                segunda_carta = -1;
                                pares_encontrados++;
                                vida_rival--;
                                turno_jugador = true;
                                tiempoMensaje = 1.0f; // Iniciar temporizador si estaba apagado
                                // NO INCREMENTAR intentos_fallidos
                                fraseActual = frases[rand() % 14];
                            }
                            else
                            {
                                // FALLO
                                tiempo_espera = 0.8f;
                                intentos_fallidos++; // INCREMENTAR AQUÍ
                            }
                        }
                    }
                }
                index++;
            }
        }
    }

    pantalla_final(tiempo_restante, vida_rival, 1, &debe_resetear);
}

void nivel2()
{
    nivel_actual = NIVEL2;
    static Carta tablero[16];
    static int randomizado = 0;
    Vector2 mouse = GetMousePosition();
    bool click = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    int index = 0;
    static int primera_carta = -1;
    static int segunda_carta = -1;
    static float tiempo_espera = 0;
    bool click_procesado = false;
    static int cargado = 0;
    static Texture2D jefe2;
    static int pares_encontrados = 0;
    static int vida_rival = 8;
    static float tiempo_restante = 180.0f;
    static bool turno_jugador = true;
    static float tiempo_turno_rival = 0;
    static bool debe_resetear = false;
    static int intentos_fallidos = 0; // CAMBIAR NOMBRE
    static float tiempoMensaje = 0;   // Para los mensajes
    static const char frases[14][20] = {"MUY BIEN!", "EXCELENTE!", "GENIAL!", "PERFECTO!", "WOW!", "INCREIBLE!", "FANTASTICO!", "SUPER!", "IMPACTANTE!", "ASOMBROSO!", "BRILLANTE!", "SORPRENDENTE!", "LO LOGRASTE!", "MAGNIFICO!"};
    static const char *fraseActual;

    if (debe_resetear)
    {
        randomizado = 0;
        primera_carta = -1;
        segunda_carta = -1;
        tiempo_espera = 0;
        pares_encontrados = 0;
        vida_rival = 8;
        tiempo_restante = 180.0f;
        turno_jugador = true;
        tiempo_turno_rival = 0;
        debe_resetear = false;
        intentos_fallidos = 0; // RESETEAR
    }

    if (cargado == 0)
    {
        jefe2 = LoadTexture("assets/images/boss2.png");
        cargado = 1;
    }

    if (!randomizado)
    {
        randomizar_nivel2(tablero, 16);
        randomizado = 1;
    }

    // ACTUALIZAR TIEMPO
    if (tiempo_restante > 0)
    {
        tiempo_restante -= GetFrameTime();
    }

    float margenX = 1450.0f;
    float margenY = 50.0f;
    float corazonSize = 40.0f;

    // MOSTRAR INFO
    DrawTextEx(fuenteLilita, TextFormat("Tiempo: %.0f", tiempo_restante), (Vector2){margenX, margenY}, 50, 2, WHITE);

    // TURNO
    if (turno_jugador)
    {
        DrawTextEx(fuenteLilita, "TU TURNO", (Vector2){margenX, margenY + 70}, 40, 2, GREEN);
        DrawTextEx(fuenteLilita, TextFormat("Fallos: %d/3", intentos_fallidos), (Vector2){margenX, margenY + 120}, 35, 2, YELLOW);
    }
    else
    {
        DrawTextEx(fuenteLilita, "TURNO RIVAL", (Vector2){margenX, margenY + 70}, 40, 2, RED);
    }

    // VIDA RIVAL
    DrawTextEx(fuenteLilita, "VIDA RIVAL:", (Vector2){margenX, margenY + 180}, 35, 2, WHITE);
    for (int i = 0; i < vida_rival; i++)
    {
        DrawTextureEx(Corazon, (Vector2){margenX + i * (corazonSize + 5), margenY + 215}, 0.0f, corazonSize / (float)Corazon.width, WHITE);
    }

    // MENSAJES SI ACERTASTE
    if (tiempoMensaje > 0)
    {
        DrawTextEx(fuenteLilita, TextFormat(fraseActual), (Vector2){margenX, margenY + 270}, 35, 2, GREEN);
        tiempoMensaje -= GetFrameTime();
    }

    // TURNO DEL RIVAL
    if (!turno_jugador)
    {
        if (tiempo_turno_rival == 0)
        {
            tiempo_turno_rival = 2.0f;
        }

        tiempo_turno_rival -= GetFrameTime();
        DrawTextEx(fuenteLilita, "EL RIVAL ESTA MEZCLANDO...", (Vector2){margenX, margenY + 300}, 30, 2, YELLOW);

        if (tiempo_turno_rival <= 0)
        {
            int cartas_ocultas = 0;
            for (int i = 0; i < 16; i++)
            {
                if (tablero[i].estado == 0)
                    cartas_ocultas++;
            }

            int cartas_a_mezclar = (cartas_ocultas + 1) / 4;
            int mezcladas = 0;

            for (int i = 15; i > 0 && mezcladas < cartas_a_mezclar; i--)
            {
                if (tablero[i].estado == 0)
                {
                    int j = rand() % (i + 1);
                    if (tablero[j].estado == 0)
                    {
                        Carta temp = tablero[i];
                        tablero[i] = tablero[j];
                        tablero[j] = temp;
                        mezcladas++;
                    }
                }
            }

            turno_jugador = true;
            tiempo_turno_rival = 0;
            intentos_fallidos = 0; // RESETEAR CONTADOR
        }

        dibujar_tablero(200, 100, tablero, 4, 4);
        DrawLine(1400, 0, 1400, 1080, BLACK);
        DrawTextureEx(jefe2, (Vector2){1450, 680}, 0.0f, 0.4, WHITE);
        pantalla_final(tiempo_restante, vida_rival, 0, &debe_resetear);
        return;
    }

    // TEMPORIZADOR DE ESPERA
    if (tiempo_espera > 0)
    {
        tiempo_espera -= GetFrameTime();
        if (tiempo_espera <= 0)
        {
            tablero[primera_carta].estado = 0;
            tablero[segunda_carta].estado = 0;
            primera_carta = -1;
            segunda_carta = -1;

            // SI LLEGÓ A 3 FALLOS, CAMBIAR TURNO
            if (intentos_fallidos >= 3)
            {
                turno_jugador = false;
            }
        }
    }

    dibujar_tablero(200, 100, tablero, 4, 4);
    DrawLine(1400, 0, 1400, 1080, BLACK);
    DrawTextureEx(jefe2, (Vector2){1450, 680}, 0.0f, 0.4, WHITE);

    // TURNO DEL JUGADOR
    if (turno_jugador && click && tiempo_espera <= 0 && !click_procesado)
    {
        int index = 0;
        for (int y = 0; y < 4; y++)
        {
            for (int x = 0; x < 4; x++)
            {
                Rectangle cell = {
                    (float)200 + x * 235,
                    (float)100 + y * 235,
                    220,
                    220};

                if (CheckCollisionPointRec(mouse, cell) && !click_procesado)
                {
                    if (tablero[index].estado == 0 && segunda_carta == -1)
                    {
                        tablero[index].estado = 1;
                        click_procesado = true;

                        if (primera_carta == -1)
                        {
                            primera_carta = index;
                        }
                        else
                        {
                            segunda_carta = index;

                            if (tablero[primera_carta].id_par == tablero[segunda_carta].id_par)
                            {
                                // ACIERTO
                                tablero[primera_carta].estado = 2;
                                tablero[segunda_carta].estado = 2;
                                primera_carta = -1;
                                segunda_carta = -1;
                                pares_encontrados++;
                                vida_rival--;
                                turno_jugador = true;
                                tiempoMensaje = 1.0f; // Iniciar temporizador si estaba apagado
                                // NO INCREMENTAR intentos_fallidos
                                fraseActual = frases[rand() % 14];
                            }
                            else
                            {
                                // FALLO
                                tiempo_espera = 0.8f;
                                intentos_fallidos++; // INCREMENTAR AQUÍ
                            }
                        }
                    }
                }
                index++;
            }
        }
    }

    pantalla_final(tiempo_restante, vida_rival, 2, &debe_resetear);
}

void nivel3()
{
    nivel_actual = NIVEL3;
    static Carta tablero[20];
    static int randomizado = 0;
    Vector2 mouse = GetMousePosition();
    bool click = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    int index = 0;
    static int primera_carta = -1;
    static int segunda_carta = -1;
    static float tiempo_espera = 0;
    bool click_procesado = false;
    static int cargado = 0;
    static Texture2D jefe3;
    static int pares_encontrados = 0;
    static int vida_rival = 10;
    static float tiempo_restante = 210.0f;
    static bool turno_jugador = true;
    static float tiempo_turno_rival = 0;
    static bool debe_resetear = false;
    static int intentos_fallidos = 0; // CAMBIAR NOMBRE
    static float tiempoMensaje = 0;   // Para los mensajes
    static const char frases[14][20] = {"MUY BIEN!", "EXCELENTE!", "GENIAL!", "PERFECTO!", "WOW!", "INCREIBLE!", "FANTASTICO!", "SUPER!", "IMPACTANTE!", "ASOMBROSO!", "BRILLANTE!", "SORPRENDENTE!", "LO LOGRASTE!", "MAGNIFICO!"};
    static const char *fraseActual;

    if (debe_resetear)
    {
        randomizado = 0;
        primera_carta = -1;
        segunda_carta = -1;
        tiempo_espera = 0;
        pares_encontrados = 0;
        vida_rival = 10;
        tiempo_restante = 210.0f;
        turno_jugador = true;
        tiempo_turno_rival = 0;
        debe_resetear = false;
        intentos_fallidos = 0; // RESETEAR
    }

    if (cargado == 0)
    {
        jefe3 = LoadTexture("assets/images/boss3.png");
        cargado = 1;
    }

    if (!randomizado)
    {
        randomizar_nivel3(tablero, 20);
        randomizado = 1;
    }

    // ACTUALIZAR TIEMPO
    if (tiempo_restante > 0)
    {
        tiempo_restante -= GetFrameTime();
    }

    float margenX = 1450.0f;
    float margenY = 50.0f;
    float corazonSize = 40.0f;

    // MOSTRAR INFO
    DrawTextEx(fuenteLilita, TextFormat("Tiempo: %.0f", tiempo_restante), (Vector2){margenX, margenY}, 50, 2, WHITE);

    // TURNO
    if (turno_jugador)
    {
        DrawTextEx(fuenteLilita, "TU TURNO", (Vector2){margenX, margenY + 70}, 40, 2, GREEN);
        DrawTextEx(fuenteLilita, TextFormat("Fallos: %d/4", intentos_fallidos), (Vector2){margenX, margenY + 120}, 35, 2, YELLOW);
    }
    else
    {
        DrawTextEx(fuenteLilita, "TURNO RIVAL", (Vector2){margenX, margenY + 70}, 40, 2, RED);
    }

    // VIDA RIVAL
    DrawTextEx(fuenteLilita, "VIDA RIVAL:", (Vector2){margenX, margenY + 180}, 35, 2, WHITE);
    for (int i = 0; i < vida_rival; i++)
    {
        DrawTextureEx(Corazon, (Vector2){margenX + i * (corazonSize + 5), margenY + 215}, 0.0f, corazonSize / (float)Corazon.width, WHITE);
    }

    // MENSAJES SI ACERTASTE
    if (tiempoMensaje > 0)
    {
        DrawTextEx(fuenteLilita, TextFormat(fraseActual), (Vector2){margenX, margenY + 270}, 35, 2, GREEN);
        tiempoMensaje -= GetFrameTime();
    }

    // TURNO DEL RIVAL
    if (!turno_jugador)
    {
        if (tiempo_turno_rival == 0)
        {
            tiempo_turno_rival = 2.0f;
        }

        tiempo_turno_rival -= GetFrameTime();
        DrawTextEx(fuenteLilita, "EL RIVAL ESTA MEZCLANDO...", (Vector2){margenX, margenY + 300}, 30, 2, YELLOW);

        if (tiempo_turno_rival <= 0)
        {
            int cartas_ocultas = 0;
            for (int i = 0; i < 20; i++)
            {
                if (tablero[i].estado == 0)
                    cartas_ocultas++;
            }

            int cartas_a_mezclar = (cartas_ocultas + 1) / 5;
            int mezcladas = 0;

            for (int i = 19; i > 0 && mezcladas < cartas_a_mezclar; i--)
            {
                if (tablero[i].estado == 0)
                {
                    int j = rand() % (i + 1);
                    if (tablero[j].estado == 0)
                    {
                        Carta temp = tablero[i];
                        tablero[i] = tablero[j];
                        tablero[j] = temp;
                        mezcladas++;
                    }
                }
            }

            turno_jugador = true;
            tiempo_turno_rival = 0;
            intentos_fallidos = 0; // RESETEAR CONTADOR
        }

        dibujar_tablero(180, 100, tablero, 5, 4);
        DrawLine(1400, 0, 1400, 1080, BLACK);
        DrawTextureEx(jefe3, (Vector2){1450, 680}, 0.0f, 0.4, WHITE);
        pantalla_final(tiempo_restante, vida_rival, 0, &debe_resetear);
        return;
    }

    // TEMPORIZADOR DE ESPERA
    if (tiempo_espera > 0)
    {
        tiempo_espera -= GetFrameTime();
        if (tiempo_espera <= 0)
        {
            tablero[primera_carta].estado = 0;
            tablero[segunda_carta].estado = 0;
            primera_carta = -1;
            segunda_carta = -1;

            // SI LLEGÓ A 4 FALLOS, CAMBIAR TURNO
            if (intentos_fallidos >= 4)
            {
                turno_jugador = false;
            }
        }
    }

    dibujar_tablero(180, 100, tablero, 5, 4);
    DrawLine(1400, 0, 1400, 1080, BLACK);
    DrawTextureEx(jefe3, (Vector2){1450, 680}, 0.0f, 0.4, WHITE);

    // TURNO DEL JUGADOR
    if (turno_jugador && click && tiempo_espera <= 0 && !click_procesado)
    {
        int index = 0;
        for (int y = 0; y < 4; y++)
        {
            for (int x = 0; x < 5; x++)
            {
                Rectangle cell = {
                    (float)180 + x * 215,
                    (float)100 + y * 215,
                    200,
                    200};

                if (CheckCollisionPointRec(mouse, cell) && !click_procesado)
                {
                    if (tablero[index].estado == 0 && segunda_carta == -1)
                    {
                        tablero[index].estado = 1;
                        click_procesado = true;

                        if (primera_carta == -1)
                        {
                            primera_carta = index;
                        }
                        else
                        {
                            segunda_carta = index;

                            if (tablero[primera_carta].id_par == tablero[segunda_carta].id_par)
                            {
                                // ACIERTO
                                tablero[primera_carta].estado = 2;
                                tablero[segunda_carta].estado = 2;
                                primera_carta = -1;
                                segunda_carta = -1;
                                pares_encontrados++;
                                vida_rival--;
                                turno_jugador = true;
                                tiempoMensaje = 1.0f; // Iniciar temporizador si estaba apagado
                                // NO INCREMENTAR intentos_fallidos
                                fraseActual = frases[rand() % 14];
                            }
                            else
                            {
                                // FALLO
                                tiempo_espera = 0.8f;
                                intentos_fallidos++; // INCREMENTAR AQUÍ
                            }
                        }
                    }
                }
                index++;
            }
        }
    }

    pantalla_final(tiempo_restante, vida_rival, 3, &debe_resetear);
}

void pantalla_final(int tiempo_restante, int vida_rival, int nivel, bool *resetear)
{
    Vector2 mouse = GetMousePosition();
    Rectangle btnReintentar = {760, 660, 400, 60};
    Rectangle btnSalir = {760, 760, 400, 60};

    // FIN POR TIEMPO
    if (tiempo_restante <= 0)
    {
        DrawRectangle(0, 0, 1920, 1080, (Color){0, 0, 0, 200});
        DrawText("SE ACABO EL TIEMPO!", 960 - MeasureText("SE ACABO EL TIEMPO!", 50) / 2, 500, 50, RED);
        DrawText("PERDISTE", 960 - MeasureText("PERDISTE", 50) / 2, 570, 50, RED);

        // Botón REINTENTAR
        DrawRectangleRec(btnReintentar, GRAY);
        DrawText("REINTENTAR", btnReintentar.x + btnReintentar.width / 2 - MeasureText("REINTENTAR", 40) / 2, btnReintentar.y + 10, 40, BLACK);

        // Botón SALIR
        DrawRectangleRec(btnSalir, GRAY);
        DrawText("SALIR", btnSalir.x + btnSalir.width / 2 - MeasureText("SALIR", 40) / 2, btnSalir.y + 10, 40, BLACK);

        // Clicks
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            if (CheckCollisionPointRec(mouse, btnReintentar))
            {
                *resetear = true;
                Pantalla_Actual = nivel_actual;
            }
            if (CheckCollisionPointRec(mouse, btnSalir))
            {
                Pantalla_Actual = INICIO;
            }
        }
    }

    // FIN POR DERROTA DEL RIVAL
    if (vida_rival <= 0)
    {
        DrawRectangle(0, 0, 1920, 1080, (Color){0, 0, 0, 200});

        if (Pantalla_Actual != NIVEL3)
        {
            DrawText("DERROTASTE AL RIVAL!", 960 - MeasureText("DERROTASTE AL RIVAL!", 50) / 2, 500, 50, GREEN);
            DrawText("¡VICTORIA!", 960 - MeasureText("¡VICTORIA!", 50) / 2, 570, 50, GREEN);

            // Botón SIGUIENTE NIVEL
            DrawRectangleRec(btnReintentar, GRAY);
            DrawText("SIGUIENTE NIVEL", btnReintentar.x + btnReintentar.width / 2 - MeasureText("SIGUIENTE NIVEL", 40) / 2, btnReintentar.y + 10, 40, BLACK);

            // Botón SALIR
            DrawRectangleRec(btnSalir, GRAY);
            DrawText("SALIR", btnSalir.x + btnSalir.width / 2 - MeasureText("SALIR", 40) / 2, btnSalir.y + 10, 40, BLACK);

            // Clicks
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                if (CheckCollisionPointRec(mouse, btnReintentar))
                {
                    if (nivel == 0)
                    {
                        *resetear = true;
                        Pantalla_Actual = NIVEL1;
                    }
                    else if (nivel == 1)
                    {
                        *resetear = true;
                        Pantalla_Actual = NIVEL2;
                    }
                    else if (nivel == 2)
                    {
                        *resetear = true;
                        Pantalla_Actual = NIVEL3;
                    }
                }
                if (CheckCollisionPointRec(mouse, btnSalir))
                {
                    *resetear = true;
                    Pantalla_Actual = INICIO;
                }
            }
        }
        else
        {
            // GANASTE EL JUEGO
            DrawText("GANASTE EL JUEGO!", 960 - MeasureText("GANASTE EL JUEGO!", 50) / 2, 500, 50, GREEN);
            DrawText("¡SALVASTE EL REINO DE FRACTALIA!", 960 - MeasureText("¡SALVASTE EL REINO DE FRACTALIA!", 50) / 2, 570, 50, GREEN);

            DrawRectangleRec(btnSalir, GRAY);
            DrawText("SALIR", btnSalir.x + btnSalir.width / 2 - MeasureText("SALIR", 40) / 2, btnSalir.y + 10, 40, BLACK);

            if (CheckCollisionPointRec(mouse, btnSalir) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                *resetear = true;
                Pantalla_Actual = INICIO;
            }
        }
    }
}

void estrellas(float dt, int screenWidth, int screenHeight)
{
    if (!estrellas_inicializadas)
    {
        for (int i = 0; i < STAR_COUNT; i++)
        {
            starX[i] = (float)GetRandomValue(0, screenWidth);
            starY[i] = (float)GetRandomValue(0, screenHeight);
            starSpeed[i] = (float)GetRandomValue(40, 200) / 10.0f;
        }
        estrellas_inicializadas = 1;
    }

    for (int i = 0; i < STAR_COUNT; i++)
    {
        starY[i] += starSpeed[i] * dt;

        if (starY[i] > screenHeight)
        {
            starY[i] = 0;
            starX[i] = (float)GetRandomValue(0, screenWidth);
        }

        DrawPixel((int)starX[i], (int)starY[i], WHITE);
    }
}