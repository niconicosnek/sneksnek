#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <mmsystem.h> //biblioteca para áudio (não muda o código em nada)

int i, j, randX, randY, movement, dir, score, hscore[11]; //contadores
int snekX, snekY, gY, head, tail, food; //snek
int game; //estado do jogo

//campo de jogo
void game_box(int rows, int cols, int camp[rows][cols])
{
    for (i = 0; i <= (cols + 1); i++) //print da primeira linha
    {
        if (i == 0)
        {
            printf("*");
        }
        else if (i == (cols + 1))
        {
            printf("*");
        }
        else
        {
            printf("-");
        }
    }
    printf(" SCORE: %d / HIGHSCORE: %d", score, hscore[0]); //print dos scores
    printf("\n");

    for (i = 0; i < rows; i++) //print das colunas
    {
        printf("|");
        for (j = 0; j < cols; j++)
        {
            if (camp[i][j] == 0)
            {
                printf(" ");
            }
            if (camp[i][j] > 0 && camp[i][j] != head)
            {
                printf("#");
            }
            if (camp[i][j] == head)
            {
                printf("S");
            }
            if (camp[i][j] == -1)
            {
                printf("*");
            }
            if (j == cols - 1)
            {
                printf("|\n");
            }
        }
    }

    for (i = 0; i <= (cols + 1); i++) //print da última linha
    {
        if (i == 0)
        {
            printf("*");
        }
        else if (i == (cols + 1))
        {
            printf("*");
        }
        else
        {
            printf("-");
        }
    }
    printf("\n");

}

//posicionador de tela (para não parecer que o jogo está sendo escrito e deixar visível)
void reset_screen()
{
    HANDLE output;
    COORD pos;
    output = GetStdHandle(STD_OUTPUT_HANDLE);
    pos.X = 0;
    pos.Y = 0;
    SetConsoleCursorPosition(output, pos);
}

//randomizador da comida (food) e aumento de velocidade
void Random(int rows, int cols, int camp[rows][cols], int speed)
{
    srand(time(0));
    randX = 1 + rand() % (rows - 2);
    randY = 1 + rand() % (cols - 2);

    if (food == 0 && camp[randX][randY] == 0)
    {
        camp[randX][randY] = -1;
        food = 1;
        if (speed > 10 && score != 0)
        {
            speed -= 5;
        }
    }
}

FILE *pont_arc;

//inicializador da cobra (SNEK)
void snek_init(int rows, int cols, int camp[rows][cols])
{
    pont_arc = fopen("hscore.txt", "r"); //leitura do arquivo
    fscanf(pont_arc, "%d", &hscore);
    fclose(pont_arc);

    for (i = 0; i < rows; i++) //inicialização do campo
    {
        for (j = 0; j < cols; j++)
        {
            camp[i][j] = 0;
        }
    }

    snekX = rows/2; //declaração das variáveis
    snekY = cols/2; 
    head = 5;
    tail = 1;
    gY = snekY;
    food = 0;
    dir = 'd';
    game = 0;
    score = 0;


    for (i = 0; i < head; i++) //inicialização da cabeça
    {
        gY++;
        camp[snekX][gY-head] = (i + 1);
    }
}

//keyboard function (leitura do teclado "wasd")
int get_ch()
{
    if (_kbhit())
    {
        return _getch();
    }
    else
    {
        return -1;
    }
}

//movemento da snek
void move_snek(int rows, int cols, int camp[rows][cols])
{
    movement = get_ch(); //função de leitura do teclado
    movement = tolower(movement); //lê maiúsculo (WASD)

    if (((movement == 'd' || movement == 'a') || (movement == 'w' || movement == 's')) && (abs (dir - movement) > 5)) //direções possíveis
    {
        dir = movement;
    }

    if (dir == 'd')
    {
        snekY++; //aumenta a coluna (direita)
        if ((camp[snekX][snekY] != 0) && (camp[snekX][snekY] != -1)) //gameover
        {
            game_over(rows, cols, camp);
        }
        if (snekY == cols - 1) //ela não vai para a posição oposta à qual está indo
        {
            snekY = 0;
        }
        if (camp[snekX][snekY] == -1) //reset de comida e aumento do snek/score
        {
            food = 0;
            score += 1;
            tail -= 3;
        }
        head++;
        camp[snekX][snekY] = head;
    }
    
    if (dir == 'a')
    {
        snekY--; //diminui a coluna (esquerda)
        if ((camp[snekX][snekY] != 0) && (camp[snekX][snekY] != -1))
        {
            game_over(rows, cols, camp);
        }
        if (snekY == 0)
        {
            snekY = cols - 1;
        }
        if (camp[snekX][snekY] == -1) //reset de comida e aumento do snek
        {
            food = 0;
            score += 1;
            tail -= 3;
        }
        head++;
        camp[snekX][snekY] = head;
    }
    
    if (dir == 'w')
    {
        snekX--; //diminui a linha (sobe)
        if ((camp[snekX][snekY] != 0) && (camp[snekX][snekY] != -1))
        {
            game_over(rows, cols, camp);
        }
        if (snekX == 0)
        {
            snekX = rows - 1;
        }
        if (camp[snekX][snekY] == -1) //reset de comida e aumento do snek
        {
            food = 0;
            score += 1;
            tail -= 3;
        }
        head++;
        camp[snekX][snekY] = head;
    }
    
    if (dir == 's')
    {
        snekX++; //aumenta a linha (desce)
        if ((camp[snekX][snekY] != 0) && (camp[snekX][snekY] != -1))
        {
            game_over(rows, cols, camp);
        }
        if (snekX == rows - 1)
        {
            snekX = 0;
        }
        if (camp[snekX][snekY] == -1) //reset de comida e aumento do snek
        {
            food = 0;
            score += 1;
            tail -= 3;
        }
        head++;
        camp[snekX][snekY] = head;
    }
}

//removendo o rabo pois se não ele cresceria infinitamente conforme a cobra anda
void tail_fix(int rows, int cols, int camp[rows][cols])
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (camp[i][j] == tail)
            {
                camp[i][j] = 0;
            }
        }
    }
    tail++;
}

//acabando o jogo
int game_over(int rows, int cols, int camp[rows][cols])
{
    PlaySound (TEXT("C:\\Users\\nicol\\OneDrive\\Documentos\\Development\\cod_c\\sneksnek\\gameover2.wav"), NULL, SND_ASYNC);
    //printf("\a"); //ruído para o gameover
    Sleep(2000);
    system("cls");

    //snekinho (NÃO MEXER)
    printf("           /^\\/^\\\n");
    printf("         _|__| O |\n");
    printf("\\/     /~     \\_/ \\\n");
    printf(" \\____|__________/  \\\n");
    printf("        \\_______      \\\n");
    printf("                `\\     \\                 \\\n");
    printf("                  |     |                  \\\n");
    printf("                 /      /                    \\\n");
    printf("                /     /                       \\\n");
    printf("              /      /                         \\ \\\n");
    printf("             /     /                            \\  \\\n");
    printf("           /     /             _----_            \\   \\\n");
    printf("          /     /           _-~      ~-_         |   |\n");
    printf("         (      (        _-~    _--_    ~-_     _/   |\n");
    printf("          \\      ~-____-~    _-~    ~-_    ~-_-~    /\n");
    printf("            ~-_           _-~          ~-_       _-~\n");
    printf("               ~--______-~                ~-___-~\n\n\n");

    printf("\t\tSnek? SNEK?! SNEEEEEEEEEEEEEEEEEEK!\n\n \t\t\t     GAME OVER!\n\n");

    if (score > hscore[0])
    {
        printf("\t\t\t   NEW RECORD: %d\n\n", score);
        pont_arc = fopen("hscore.txt", "w");
        fprintf(pont_arc, "%d", score);
        fclose(pont_arc);
    }

    printf("\tPRESS: ~ENTER~ TO TRY AGAIN / ~ESC~ TO EXIT THE GAME\n");

    while (1)
    {
        movement = get_ch();
        if (movement == 13)
        {
            game = 0;
            snek_init(rows, cols, camp);
            break;
        }
        else if (movement == 27)
        {
            game = 1;
            break;
        }
    }
    system("cls");
    return 0;
}

//desenho snekinho2
void print_drawing()
{
    printf("             ________________________________________________\n");
    printf("            /                                                \\\n");
    printf("           |    _________________________________________     |\n");
    printf("           |   |                                         |    |\n");
    printf("           |   |  C:\\> -----------SNEK GAME--------------|    |\n");
    printf("           |   |                                         |    |\n");
    printf("           |   |                                         |    |\n");
    printf("           |   |                      ____               |    |\n");
    printf("           |   |                     / . .\\              |    |\n");
    printf("           |   |                     \\  ---<             |    |\n");
    printf("           |   |                      \\  /               |    |\n");
    printf("           |   |            __________/ /                |    |\n");
    printf("           |   |         -=:___________/                 |    |\n");
    printf("           |   |                                         |    |\n");
    printf("           |   |                                         |    |\n");
    printf("           |   |                GET READY!               |    |\n");
    printf("           |   |_________________________________________|    |\n");
    printf("           |                                                  |\n");
    printf("            \\_________________________________________________/\n");
    printf("                   \\___________________________________/\n");
    printf("                ___________________________________________\n");
    printf("             _-'    .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.  --- `-_\n");
    printf("          _-'.-.-. .---.-.-.-.-.-.-.-.-.-.-.-.-.-.-.--.  .-.-.`-_\n");
    printf("       _-'.-.-.-. .---.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-`__`. .-.-.-.`-_\n");
    printf("    _-'.-.-.-.-. .-----.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-----. .-.-.-.-.`-_\n");
    printf(" _-'.-.-.-.-.-. .---.-. .-------------------------. .-.---. .---.-.-.-.`-_\n");
    printf(":-------------------------------------------------------------------------:\n");
    printf("`---._.-------------------------------------------------------------._.---'\n");

    Sleep(4000);

    system("cls");
}

void main()
{
    int rows, cols;
    int dif, speed;

    system("cls");
    printf("YOU'RE ABOUT TO BEGIN THE SNEK GAME, PLEASE CHOOSE A DIFFICULTY (1, 2 or 3): ");
    scanf("%d", &dif);

    system("cls");

    print_drawing();

    if (dif == 1)
    {
        rows = 25;
        cols = 25;
        speed = 99;
    }
    else if (dif == 2)
    {
        rows = 25;
        cols = 50;
        speed = 75;
    }
    else if (dif == 3)
    {
        rows = 25;
        cols = 75;
        speed = 60;
    }

    int camp[rows][cols];

    snek_init(rows, cols, camp);

    while (game == 0)
    {
        game_box(rows, cols, camp);
        reset_screen();
        Random(rows, cols, camp, speed);
        move_snek(rows, cols, camp);
        tail_fix(rows, cols, camp);
        Sleep(speed);
    }    
}