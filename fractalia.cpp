#include "raylib.h"

#define STAR_COUNT 300

typedef enum
{
    PANT_MENU,
    PANT_TUTORIAL,
    PANT_NIVEL1,
    PANT_NIVEL2,
    PANT_NIVEL3
} Pantalla;

typedef struct
{
    Vector2 pos;
    float speed;
} Star;

static Star stars[STAR_COUNT];

void iniciar_estrellas(int screenWidth, int screenHeight);
void dibujar_estrellas(float dt, int screenWidth, int screenHeight);
void PantallaMenu(Pantalla *pantallaActual, Texture2D logo, int screenWidth, int screenHeight);

int main(void)
{
    const int screenWidth = 1920;
    const int screenHeight = 1080;

    InitWindow(screenWidth, screenHeight, "FRACTALIA");
    InitAudioDevice();
    SetTargetFPS(60);

    Texture2D logo = LoadTexture("assets/images/LOGO FRACTALIA.png");
    Music musicaMenu = LoadMusicStream("assets/music/musica_menu.mp3");
    PlayMusicStream(musicaMenu);

    iniciar_estrellas(screenWidth, screenHeight);

    Pantalla Pantalla_Actual = PANT_MENU;

    while (!WindowShouldClose())
    {
        UpdateMusicStream(musicaMenu);

        float dt = GetFrameTime();

        BeginDrawing();
        ClearBackground(DARKBLUE);

        dibujar_estrellas(dt, screenWidth, screenHeight);

        switch (Pantalla_Actual)
        {
        case PANT_MENU:
            PantallaMenu(&Pantalla_Actual, logo, screenWidth, screenHeight);
            break;
        }

        EndDrawing();
    }

    UnloadTexture(logo);
    UnloadMusicStream(musicaMenu);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}

void iniciar_estrellas(int screenWidth, int screenHeight)
{
    for (int i = 0; i < STAR_COUNT; i++)
    {
        stars[i].pos.x = (float)GetRandomValue(0, screenWidth);
        stars[i].pos.y = (float)GetRandomValue(0, screenHeight);
        stars[i].speed = (float)GetRandomValue(40, 200) / 10.0f;
    }
}

void dibujar_estrellas(float dt, int screenWidth, int screenHeight)
{
    for (int i = 0; i < STAR_COUNT; i++)
    {
        stars[i].pos.y += stars[i].speed * dt;

        if (stars[i].pos.y > screenHeight)
        {
            stars[i].pos.y = 0;
            stars[i].pos.x = (float)GetRandomValue(0, screenWidth);
        }

        DrawPixelV(stars[i].pos, WHITE);
    }
}

void PantallaMenu(Pantalla *pantallaActual, Texture2D logo, int screenWidth, int screenHeight)
{
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
        *pantallaActual = PANT_TUTORIAL;
    }
}
