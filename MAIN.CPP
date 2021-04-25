#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>
#include <math.h>
#include <string.h>

// INPUT MACROS
#define ARROW_UP 0
#define ARROW_DOWN 1
#define PAUSE_KEY 2
#define ENTER_KEY 3
#define BACK_KEY 4

// BOOLEAN MACROS
#define TRUE 1
#define FALSE 0

// COORDINATE MACROS
#define X(p) p*2
#define Y(p) p*2+1

int run = TRUE;
char inputBuffer;

int backgroundColor = CYAN;

// BIRDS
#define RED_BIRD 0
#define YELLOW_BIRD 1
#define BLACK_BIRD 2
int birdHealths[3] = {100, 100, 100};
int birdPositions[6] = {
    145, 230,
    70, 150,
    250, 260
};
int birdShields[3] = {0, 0, 0};
int birdHealMultipliers[3] = {1, 1, 1};
int birdDamageMultipliers[3] = {1, 1, 1};
int electricShield[3] = {0, 0, 0};

// PIGS
#define PIG1 0
#define PIG2 1
#define KINGPIG 2
int pigHealths[3] = {100, 100, 100};
int pigPositions[6] = {
    500, 240,
    420, 160,
    530, 150
};
int pigBurn[3] = {0, 0, 0};
int BURN_DMG = 7;

int pigFocus = -1;
int pigMinDmg = 5;
int pigMaxDmg = 10;
int kingPigBonusDmg = 10;

// MENU
#define CHARACTER_MENU 0
#define ABILITY_TYPE_MENU 1
#define ABILITY_SELECT_MENU 2
#define TARGET_SELECT_MENU 3
int menuState;
int menuSelection = 0;
char* menu[6];

int character;

#define ABILITY_OFENSIVE 0
#define ABILITY_DEFENSIVE 1
#define ABILITY_CONSUMABLE 2
int ability;
int abilityType;

int target;

char* strings[] = {
    "ROJO",
    "AMARILLO",
    "NEGRO",
    "CERDO 1",
    "CERDO 2",
    "REY CERDO",
    "Fuerte y resistente",
    "Rapido y agil",
    "Explosivo",
    "Habilidades ofensivas",
    "PuÃ±etazos y patadas",
    "Habilidades defensivas",
    "Escudos y curaciones",
    "Consumibles",
    "Comida, pociones y elixirs",
    "PuÃ±o de fuego", //index 15
    "Deja al enemigo en llamas",
    "Kamikaze!",
    "Mueres e infliges daÃ±o fatal a un enemigo",
    "Justicia de los pajaros!",
    "DaÃ±o aumentado contra el rey cerdo",
    "Ojo por ojo", //index 21
    "Recibes golpes en vez de tus aliados",
    "Plumas de hierro",
    "Recibes un gran escudo temporal",
    "Hermandad pajarera",
    "Te curas a ti y a un aliado",
    "Pocion de fuerza",
    "Incrementa el poder de tus ataques",
    "Pocion de invencibilidad",
    "Te hace immune a ataques enemigos por un corto tiempo",
    "Pocion de vida",
    "Te curas por una gran cantidad de vida",
    "Ataque rapido",
    "Atacas con un rallo electrocutante",
    "Tres por uno",
    "Te divides golpeando a tres enemigos a la vez",
    "Soy un vampiro",
    "Robas la vida a un enemigo",
    "Rayo sanador",
    "Cura a un aliado con un rallo sanador",
    "Ultima estancia",
    "Mueres y restableces la vida completa de un aliado",
    "Escudo electrico",
    "Inflige daño a los atacantes",
    "Pocion de brujeria",
    "Aumenta el poder de curaciones",
    "Orbe de los pajaros dioses",
    "Revive a un aliado",
    "Chocolate",
    "Ningun uso practico en el combate, pero sabe rico",
    "Minas explosivas",  //black bird
    "Disperar una serie de minas que inflige daÃ±o a todos los enemigos",
    "Carga concentrada",
    "Inflige una gran cantidad de daÃ±o a un solo enemigo",
    "Bomba sorpresa",
    "Sorpresa! Una carga mas poderosa pero incontrolable",
    "Empoderamiento explosivo",
    "Incrementa daño",
    "Soy una bombita",
    "Infliges daño a un aliado... por lo menos intentaste ayudar", //index 60
    "Soy una bombota",
    "Enserio, no les puedo ayudar amigos, soy una bomba",
    "Polvora pirata",
    "Aumenta el daño explosivo dramaticamente por un tiempo",
    "Polvora ninja",
    "Nadie te puede ver, nadie te puede atacar",
    "Bomba nuclear",
    "Extermina a todos los enemigos de un solo golpe",
    "Turno de cerdos:",
    "ataca a", //index 70
    "Victoria!",
    "Derrota!"
};

// ANIMATIONS
int bombx = 0;
int bomby = 0;
int explosionColors[] = {
    RED,
    YELLOW,
    LIGHTRED
};
int lightingColors[] = {
    YELLOW,
    RED,
    LIGHTRED
};
int healColors[] = {
		GREEN,
		LIGHTGREEN,
		LIGHTBLUE
};

void initGraphics() {
    int gdriver = DETECT, gmode, errorcode;

		initgraph(&gdriver, &gmode, "C:/TURBOC3/BGI/");

    errorcode = graphresult();
    if (errorcode != grOk)
    {
        printf("Graphics error: %s\n", grapherrormsg(errorcode));
        printf("Press any key to halt:");
        getch();
        exit(1);
    }

    setbkcolor(BLACK);
}

void fillRectangle(int x1, int y1, int x2, int y2) {
    int poly[] = {
        x1, y1,
        x2, y1,
        x2, y2,
        x1, y2,
        x1, y1
    };
    fillpoly(5, poly);
    drawpoly(5, poly);
}

void clearScreen() {
    clearviewport();
    setbkcolor(BLACK);
    setcolor(backgroundColor);
    setfillstyle(SOLID_FILL, backgroundColor);
    fillRectangle(0, 0, 639, 479);
}

void drawText(int x, int y, char* text, int color, int background) {
    setcolor(color);
    setbkcolor(background);
    outtextxy(x, y, text);
    setbkcolor(BLACK);
}

void drawBomb(int x, int y) {
    int size = 10;
    setfillstyle(SOLID_FILL, backgroundColor);
    setcolor(backgroundColor);
    fillellipse(bombx, bomby, size, size);
    setfillstyle(SOLID_FILL, BLACK);
    setcolor(DARKGRAY);
    fillellipse(x, y, size, size);
    bombx = x;
    bomby = y;
}

void drawExplosion(int x, int y, int radius, int particleCount, int particleSizeMin, int particleSizeMax, int millis, int cycleCount){
    for(int i = 0; i < cycleCount; i++){
        for(int j = 0; j < particleCount; j++){
            int particleX = x+(rand()%(2*radius))-radius;
            int particleY = y+(rand()%(2*radius))-radius;
            int particleColor = explosionColors[rand()%3];
            int particleSize = particleSizeMin + (rand()%(particleSizeMax-particleSizeMin));
            setfillstyle(SOLID_FILL, particleColor);
            setcolor(particleColor);
            fillellipse(particleX, particleY, particleSize, particleSize);
        }
        delay(millis);
    }
}

void drawLighting(int x1, int y1, int x2, int y2, int colors[], int colorCount){
    const int segments = 4;
    const int pointCount = segments + 1;
    const int deviation = 10;
    const int lines = 5;
    const int millis = 100;

    int i;
    int j;

    int referencePoints[pointCount*2];

    for(i = 0; i < pointCount; i++){
        referencePoints[i*2] = x1 + (x2-x1)/segments*i;
        referencePoints[i*2+1] = y1 + (y2-y1)/segments*i;
    }
    for(i = 0; i < lines; i++){
        int color = colors[rand()%colorCount];
				int points[pointCount*2];
				setcolor(color);
        for(j = 0; j < pointCount*2; j++){
            points[j] = referencePoints[j] + (rand()%(deviation*2)) - deviation;
        }
        for(j = 1; j < pointCount; j++){
            line(points[j*2-2], points[j*2-1], points[j*2], points[j*2+1]);
        }
        delay(millis);
    }
}

void drawRedBird(int x, int y, int size) {
    int bird[52] = {
        x - 4 * size, y + 8 * size,
        x + 6 * size, y + 8 * size,
         x + 10 * size, y + 4 * size,
        x + 10 * size, y - 3 * size,
        x + 9 * size, y - 3 * size,
        x + 9 * size, y - 5 * size,
        x, y - 14 * size,
        x - 2 * size, y - 14 * size,
        x - 3 * size, y - 13 * size,
        x - 3 * size, y - 12 * size,
        x - 4 * size, y - 11 * size,
        x - 6 * size, y - 11 * size,
        x - 7 * size, y - 10 * size,
        x - 7 * size, y - 9 * size,
        x - 6 * size, y - 8 * size,
        x - 4 * size, y - 8 * size,
        x - 6 * size, y - 8 * size,
        x - 8 * size, y - 6 * size,
        x - 8 * size, y - 5 * size,
        x - 9 * size, y - 4 * size,
        x - 9 * size, y - 3 * size,
        x - 10 * size, y - 2 * size,
        x - 10 * size, y + 3 * size,
        x - 6 * size, y + 7 * size,
        x - 5 * size, y + 7 * size,
        x - 4 * size, y + 8 * size
    };
    int mouth[22] = {
        x + 1 * size, y + 1 * size,
        x + 1 * size, y + 3 * size,
        x + 3 * size, y + 5 * size,
        x + 5 * size, y + 5 * size,
        x + 5 * size, y + 4 * size,
        x + 4 * size, y + 3 * size,
        x + 5 * size, y + 4 * size,
        x + 8 * size, y + 4 * size,
        x + 9 * size, y + 3 * size,
        x + 7 * size, y + 1 * size,
        x + 1 * size, y + 1 * size
    };
    int eyes[24] = {
        x + 1 * size, y + 1 * size,
        x, y,
        x - 1 * size, y - 1 * size,
        x - 1 * size, y - 4 * size,
        x, y - 3 * size,
        x + 2 * size, y - 3 * size,
        x + 3 * size, y - 2 * size,
        x + 6 * size, y - 2 * size,
        x + 7 * size, y - 3 * size,
        x + 8 * size, y - 3 * size,
        x + 8 * size, y,
        x + 7 * size, y + 1 * size
    };
    int belly[22] = {
        x + 8 * size, y + 6 * size,
        x + 6 * size, y + 8 * size,
        x - 4 * size, y + 8 * size,
        x - 5 * size, y + 7 * size,
        x - 6 * size, y + 7 * size,
        x - 6 * size, y + 6 * size,
        x - 3 * size, y + 3 * size,
        x + 1 * size, y + 3 * size,
        x + 3 * size, y + 5 * size,
        x + 7 * size, y + 5 * size,
        x + 8 * size, y + 6 * size
    };

    setcolor(BLACK);

    setfillstyle(SOLID_FILL, LIGHTRED);
    fillpoly(26, bird);
    drawpoly(26, bird);

    setfillstyle(SOLID_FILL, YELLOW);
    fillpoly(11, mouth);
    drawpoly(11, mouth);

    setfillstyle(SOLID_FILL, WHITE);
    fillpoly(12, eyes);
    drawpoly(12, eyes);

    setfillstyle(SOLID_FILL, BLACK);
    fillellipse(x + 3 * size, y - 1 * size, size / 2, size / 2);
    fillellipse(x + 6 * size, y - 1 * size, size / 2, size / 2);

    setfillstyle(SOLID_FILL, WHITE);
    fillpoly(11, belly);
    drawpoly(11, belly);
}

void drawYellowBird(int x, int y, int size) {
    int bird[48] = {
        x - 4 * size, y + 7 * size,
        x + 6 * size, y + 7 * size,
        x + 7 * size, y + 6 * size,
        x + 10 * size, y + 6 * size,
        x + 11 * size, y + 5 * size,
        x + 11 * size, y + 3 * size,
        x + 9 * size, y + size,
        x + 9 * size, y,
        x + 8 * size, y - size,
        x + 5 * size, y - 4 * size,
        x + 5 * size, y - 5 * size,
        x + 2 * size, y - 9 * size,
        x - 3 * size, y - 9 * size,
        x - 3 * size, y - 8 * size,
        x - 5 * size, y - 6 * size,
        x - 5 * size, y - 5 * size,
        x - 6 * size, y - 4 * size,
        x - 6 * size, y - 3 * size,
        x - 7 * size, y - 2 * size,
        x - 7 * size, y + size,
        x - 8 * size, y + 2 * size,
        x - 8 * size, y + 5 * size,
        x - 6 * size, y + 6 * size,
        x - 4 * size, y + 6 * size
    };
    int leftBrow[6] = {
        x - 2 * size, y,
        x - 2 * size, y - size,
        x + 3 * size, y
    };
    int rightBrow[6] = {
        x + 6 * size, y,
        x + 10 * size, y,
        x + 11 * size, y - size
    };
    int belly[14] = {
        x - 5 * size, y + 6 * size,
        x - 4 * size, y + 7 * size,
        x + 6 * size, y + 7 * size,
        x + 7 * size, y + 6 * size,
        x + 8 * size, y + 6 * size,
        x + 8 * size, y + 5 * size,
        x + size, y + 5 * size
    };
    int mouth[12] = {
        x + size, y + 4 * size,
        x + 3 * size, y + 2 * size,
        x + 6 * size, y + 2 * size,
        x + 8 * size, y + 3 * size,
        x + 4 * size, y + 5 * size,
        x + 3 * size, y + 5 * size
    };
    int hair[14] = {
        x - 3 * size, y - 9 * size,
        x - 4 * size, y - 12 * size,
        x - 2 * size, y - 11 * size,
        x - size, y - 13 * size,
        x + size, y - 11 * size,
        x + 4 * size, y - 12 * size,
        x + 2 * size, y - 9 * size
    };

    setcolor(BLACK);
    setfillstyle(SOLID_FILL, YELLOW);
    fillpoly(24, bird);
    drawpoly(24, bird);

    setfillstyle(SOLID_FILL, WHITE);
    fillellipse(x + size, y + size, size, size);
    fillellipse(x + 7 * size, y + size, size, size);
    setfillstyle(SOLID_FILL, BLACK);
    fillellipse(x + size, y + size, 0.5 * size, 0.5 * size);
    fillellipse(x + 7 * size, y + size, 0.5 * size, 0.5 * size);

    setfillstyle(SOLID_FILL, RED);
    fillpoly(3, leftBrow);
    fillpoly(3, rightBrow);

    setfillstyle(SOLID_FILL, WHITE);
    fillpoly(7, belly);
    drawpoly(7, belly);

    setfillstyle(SOLID_FILL, BROWN);
    fillpoly(6, mouth);
    drawpoly(6, mouth);
    line(x + 3 * size, y + 4 * size, x + 8 * size, y + 3 * size);

    setfillstyle(SOLID_FILL, BLACK);
    fillpoly(7, hair);
}

void drawBlackBird(int x, int y, int size) {
    int mouth[12] = {
        x - 2 * size, y + size,
        x, y - size,
        x + 4 * size, y + size,
        x + 2 * size, y + 2 * size,
        x - 1 * size, y + 2 * size,
        x - 2 * size, y + size
    };
    int rightBrow[6] = {
        x - 2 * size, y - 4 * size,
        x - size, y - 5 * size,
        x - 6 * size, y - 6 * size
    };
    int leftBrow[6] = {
        x + 4 * size, y - 4 * size,
        x + 3 * size, y - 5 * size,
        x + 8 * size, y - 6 * size
    };
    int hair[12] = {
        x + size, y - 11 * size,
        x + 2 * size, y - 12 * size,
        x + 2 * size, y - 13 * size,
        x - size, y - 12 * size,
        x - size, y - 8 * size,
        x + size, y - 8 * size
    };

    setcolor(BLACK);
    setfillstyle(SOLID_FILL, BLACK);
    fillellipse(x, y, 9 * size, 9 * size);
    setfillstyle(SOLID_FILL, WHITE);
    setcolor(WHITE);
    fillellipse(x - 4 * size, y - 2 * size, 2 * size, 2 * size);
    fillellipse(x + 6 * size, y - 2 * size, 2 * size, 2 * size);
    setfillstyle(SOLID_FILL, BLACK);
    setcolor(BLACK);
    fillellipse(x - 4 * size, y - 2 * size, size, size);
    fillellipse(x + 6 * size, y - 2 * size, size, size);
    setcolor(backgroundColor);
    arc(x, y + 10 * size, 30, 150, 6 * size);
    setfillstyle(SOLID_FILL, DARKGRAY);
    //floodfill(x, y + 8 * size, backgroundColor);
    setcolor(LIGHTGRAY);
    arc(x, y + 10 * size, 30, 150, 6 * size);

    setcolor(BLACK);
    setfillstyle(SOLID_FILL, YELLOW);

    fillpoly(6, mouth);
    line(x + 4 * size, y + size, x - size, y + size);

    setfillstyle(SOLID_FILL, LIGHTRED);
    fillpoly(3, rightBrow);
    fillpoly(3, leftBrow);

    setfillstyle(SOLID_FILL, BLACK);
    fillpoly(6, hair);
    setfillstyle(SOLID_FILL, LIGHTRED);
    fillpoly(4, hair);
}

void drawBird(int bird, int x, int y, int size) {
    switch (bird) {
    case RED_BIRD:
        drawRedBird(x, y, size);
        break;
    case YELLOW_BIRD:
        drawYellowBird(x, y, size);
        break;
    case BLACK_BIRD:
        drawBlackBird(x, y, size);
        break;
    }
}

void drawPig(int x, int y, int size, int drawCrown) {
    int pig[38] = {
        x - 3 * size, y + 7 * size,
        x + 3 * size, y + 7 * size,
        x + 6 * size, y + 4 * size,
        x + 6 * size, y - 3 * size,
        x + 4 * size, y - 6 * size,
        x - 2 * size, y - 6 * size,
        x - 2 * size, y - 8 * size,
        x - 4 * size, y - 8 * size,
        x - 4 * size, y - 6 * size,
        x - 2 * size, y - 6 * size,
        x - 4 * size, y - 6 * size,
        x - 5 * size, y - 5 * size,
        x - 6 * size, y - 5 * size,
        x - 6 * size, y - 6 * size,
        x - 8 * size, y - 6 * size,
        x - 8 * size, y - 4 * size,
        x - 7 * size, y - 4 * size,
        x - 7 * size, y + 3 * size,
        x - 3 * size, y + 7 * size
    };

    setcolor(BLACK);

    setfillstyle(SOLID_FILL, GREEN);
    fillpoly(19, pig);
    drawpoly(19, pig);

    setfillstyle(SOLID_FILL, LIGHTGREEN);
    fillellipse(x, y, 2 * size, 2 * size);

    setfillstyle(SOLID_FILL, WHITE);
    fillellipse(x - 5 * size, y, (3 * size) / 2, (3 * size) / 2);
    fillellipse(x + 5 * size, y, (3 * size) / 2, (3 * size) / 2);

    setfillstyle(SOLID_FILL, BLACK);
    fillellipse(x - 5 * size, y, size / 2, size / 2);
    fillellipse(x + 5 * size, y, size / 2, size / 2);

    fillellipse(x - size, y, size / 2, size / 2);
    fillellipse(x + size, y, size / 2, size / 2);

    if (drawCrown) {
        int crown[14] = {
            x, y - 6 * size,
            x, y - 10 * size,
            x + 1 * size, y - 11 * size,
            x + 2 * size, y - 10 * size,
            x + 3 * size, y - 11 * size,
            x + 4 * size, y - 10 * size,
            x + 4 * size, y - 6 * size
        };

        setfillstyle(SOLID_FILL, YELLOW);
        fillpoly(7, crown);
        drawpoly(7, crown);
        setfillstyle(SOLID_FILL, LIGHTBLUE);
        fillellipse(x + 2 * size, y - 8 * size, size, size);
    }
}

void drawBirds(){
    int i;
    for (i = 0; i < 3; i++) {
        if (birdHealths[i] > 0) drawBird(i, birdPositions[i * 2], birdPositions[i * 2 + 1], 4);
    }
}

void drawPigs() {
	int i;
	for (i = 0; i < 3; i++) {
        if (pigHealths[i] > 0) drawPig(pigPositions[i * 2], pigPositions[i * 2 + 1], 4 + i / 2, i / 2);
    }
}

void drawMenu() {
    int i;
    int bkcolor = LIGHTGRAY;
    int itmcolor = BLACK;
    int selcolor = YELLOW;
    int descolor = DARKGRAY;

    setcolor(BLACK);
    setfillstyle(SOLID_FILL, bkcolor);
    fillRectangle(10, 300, 630, 470);

    for (i = 0; i < 6; i += 2) {
                int clr = itmcolor;
                if(menuSelection == i/2) clr = selcolor;
                drawText(20, 310 + i * 25, menu[i], clr, bkcolor);
                drawText(20, 310 + (i + 1) * 25, menu[i + 1], descolor, bkcolor);
    }
}

void setMenuText() {
		int i;

		switch (menuState) {
            case CHARACTER_MENU:
				if(birdHealths[RED_BIRD] > 0){
                    menu[0] = strings[0];
				    menu[1] = strings[6];
                }
                else{
                    menu[0] = "";
                    menu[1] = "";
                }
				if(birdHealths[YELLOW_BIRD] > 0){
                    menu[2] = strings[1];
				    menu[3] = strings[7];
				}
                else{
                    menu[2] = "";
                    menu[3] = "";
                }
                if(birdHealths[BLACK_BIRD] > 0){
                    menu[4] = strings[2];
				    menu[5] = strings[8];
				}
                else{
                    menu[4] = "";
                    menu[5] = "";
                }
                break;
            case ABILITY_TYPE_MENU:
				for (i = 0; i < 6; i++) menu[i] = strings[i + 9];
				break;
            case ABILITY_SELECT_MENU:
				for (i = 0; i < 6; i++) menu[i] = strings[i+18*character+6*abilityType+15];
				break;
            case TARGET_SELECT_MENU:
				if (abilityType == ABILITY_OFENSIVE) for (i = 0; i < 3; i++) menu[i * 2] = strings[3 + i];
				if (abilityType == ABILITY_DEFENSIVE) for (i = 0; i < 3; i++) menu[i * 2] = strings[i];
				for(int i = 0; i < 3; i++) menu[i * 2 + 1] = "";
				break;
		}
}

void drawHealthBar(int x, int y, int w, int h, int c) {
    setfillstyle(SOLID_FILL, DARKGRAY);
    fillRectangle(x, y, x + w, h);
    setfillstyle(SOLID_FILL, LIGHTRED);
    fillRectangle(x, y, x + c, h);
}

void drawHp() {
	int i;
	int bkcolor = LIGHTGRAY;
	setcolor(BLACK);
	setfillstyle(SOLID_FILL, bkcolor);
	fillRectangle(10, 10, 630, 80);
	for (i = 0; i < 3; i++) {
		if (birdHealths[i] > 0) {
				drawText(35, 11 + 23 * i, strings[i], BLACK, bkcolor);
				drawHealthBar(110, 13 + 23 * i, 100, 32 + 23 * i, birdHealths[i]);
		}
	}
	for (i = 0; i < 3; i++) {
        if (pigHealths[i] > 0) {
            drawText(400, 11 + 23 * i, strings[i + 3], BLACK, bkcolor);
            drawHealthBar(485, 13 + 23 * i, 100, 32 + 23 * i, pigHealths[i]);
        }
    }
}

void drawAll(){
    clearScreen();
    drawBirds();
    drawPigs();
    setMenuText();
    drawHp();
}

void attackPig(int pig, int damage){
    pigHealths[pig] -= damage * birdDamageMultipliers[character];
}

void healBird(int bird, int health){
    birdHealths[bird] += health * birdHealMultipliers[bird];
    if(birdHealths[bird] > 100) birdHealths[bird] = 100;
}

void attackBird(int bird, int damage){
    int shield = birdShields[bird];
    birdShields[bird] -= damage;
    damage -= shield;
    if(birdShields[bird] < 0) birdShields[bird] = 0;
    birdHealths[bird] -= damage;
}

void redAbility() {
    if (abilityType == ABILITY_OFENSIVE && ability == 0) {
        attackPig(target, 10);
        pigBurn[target] = 2;
    }
    if (abilityType == ABILITY_OFENSIVE && ability == 1) {
        birdHealths[RED_BIRD] = 0;
        pigHealths[target] = 0;
    }
    if (abilityType == ABILITY_OFENSIVE && ability == 2) {
        if(target == KINGPIG) attackPig(KINGPIG, 30);
        else attackPig(target, 10);
    }
    if (abilityType == ABILITY_DEFENSIVE && ability == 0) {
        pigFocus = RED_BIRD;
    }
    if (abilityType == ABILITY_DEFENSIVE && ability == 1) {
        birdShields[RED_BIRD] = 35;
    }
    if (abilityType == ABILITY_DEFENSIVE && ability == 2) {
        healBird(target, 20);
        healBird(RED_BIRD, 20);
    }
    if (abilityType == ABILITY_CONSUMABLE && ability == 0) {
        birdDamageMultipliers[RED_BIRD] = 2;
    }
    if (abilityType == ABILITY_CONSUMABLE && ability == 1) {
        birdShields[RED_BIRD] = 100;
    }
    if (abilityType == ABILITY_CONSUMABLE && ability == 2) {
        healBird(RED_BIRD, 70);
    }
}

void yellowAbility() {
    if (abilityType == ABILITY_OFENSIVE && ability == 0) {
        drawLighting(
            birdPositions[X(YELLOW_BIRD)], birdPositions[Y(YELLOW_BIRD)],
            pigPositions[X(target)], pigPositions[Y(target)],
            lightingColors, 3);
        delay(1000);
        pigHealths[target] -= 15;
    }
    if (abilityType == ABILITY_OFENSIVE && ability == 1) {
        clearScreen();
        drawPigs();
        setMenuText();
        drawHp();
        int clone1x = birdPositions[X(RED_BIRD)];
        int clone1y = birdPositions[Y(RED_BIRD)];
        int clone2x = birdPositions[X(BLACK_BIRD)];
        int clone2y = birdPositions[Y(BLACK_BIRD)];
        drawYellowBird(birdPositions[X(YELLOW_BIRD)], birdPositions[Y(YELLOW_BIRD)], 4);
        drawYellowBird(clone1x, clone1y, 4);
        drawYellowBird(clone2x, clone2y, 4);
        drawLighting(
            clone1x, clone1y,
            pigPositions[X(KINGPIG)], pigPositions[Y(KINGPIG)],
            lightingColors, 3);
        drawLighting(
            clone2x, clone2y,
						pigPositions[X(PIG2)], pigPositions[Y(PIG2)],
            lightingColors, 3);
        drawLighting(
            birdPositions[X(YELLOW_BIRD)], birdPositions[Y(YELLOW_BIRD)],
            pigPositions[PIG1*2], pigPositions[PIG1*2+1],
            lightingColors, 3);
        attackPig(PIG1, 10);
        attackPig(PIG2, 10);
        attackPig(KINGPIG, 10);
        delay(500);
    }
    if (abilityType == ABILITY_OFENSIVE && ability == 2) {
        drawLighting(birdPositions[X(YELLOW_BIRD)], birdPositions[Y(YELLOW_BIRD)], pigPositions[X(target)], pigPositions[Y(target)], healColors, 3);
        attackPig(target, 15);
        healBird(YELLOW_BIRD, 15);
        delay(500);
    }
    if (abilityType == ABILITY_DEFENSIVE && ability == 0) {
        drawLighting(
            birdPositions[X(YELLOW_BIRD)], birdPositions[Y(YELLOW_BIRD)],
            birdPositions[X(target)], birdPositions[Y(target)],
            healColors, 3);
        healBird(target, 30);
        delay(500);
    }
    if (abilityType == ABILITY_DEFENSIVE && ability == 1) {
        birdHealths[YELLOW_BIRD] = 0;
        birdHealths[target] = 100;
    }
    if (abilityType == ABILITY_DEFENSIVE && ability == 2) {
        electricShield[target] = 2;
    }
    if (abilityType == ABILITY_CONSUMABLE && ability == 0) {
        birdHealMultipliers[YELLOW_BIRD] = 4;
    }
    if (abilityType == ABILITY_CONSUMABLE && ability == 1) {
        if(birdHealths[RED_BIRD] < 0) birdHealths[RED_BIRD] = 50;
        else if(birdHealths[BLACK_BIRD] < 0) birdHealths[BLACK_BIRD] = 50;
    }
    if (abilityType == ABILITY_CONSUMABLE && ability == 2) {
        // Ã±om!
    }
}

void blackAbility() {
    if (abilityType == ABILITY_OFENSIVE && ability == 0) {
        drawExplosion(pigPositions[X(PIG1)], pigPositions[Y(PIG1)], 30, 10, 5, 10, 100, 10);
        pigHealths[PIG1] -= 10;
        drawExplosion(pigPositions[X(PIG2)], pigPositions[Y(PIG2)], 30, 10, 5, 10, 100, 10);
        pigHealths[PIG2] -= 10;
        drawExplosion(pigPositions[X(KINGPIG)], pigPositions[Y(KINGPIG)], 30, 10, 5, 10, 100, 10);
        pigHealths[KINGPIG] -= 10;
    }
    if (abilityType == ABILITY_OFENSIVE && ability == 1) {
        drawExplosion(pigPositions[X(target)], pigPositions[Y(target)], 30, 10, 5, 10, 100, 10);
        pigHealths[target] -= 20;
    }
    if (abilityType == ABILITY_OFENSIVE && ability == 2) {
        target = rand()%3;
        drawExplosion(pigPositions[X(target)], pigPositions[Y(target)], 30, 10, 5, 10, 100, 10);
        pigHealths[target] -= 30;

    }
    if (abilityType == ABILITY_DEFENSIVE && ability == 0) {
        birdDamageMultipliers[target] = 2;
    }
    if (abilityType == ABILITY_DEFENSIVE && ability == 1) {
        attackBird(target, 10);
        drawExplosion(birdPositions[X(target)], birdPositions[Y(target)], 30, 10, 5, 10, 100, 10);
    }
    if (abilityType == ABILITY_DEFENSIVE && ability == 2) {
        drawExplosion(birdPositions[X(RED_BIRD)], birdPositions[Y(RED_BIRD)], 100, 20, 5, 10, 50, 40);
        birdHealths[RED_BIRD] = 0;
        birdHealths[YELLOW_BIRD] = 0;
        birdHealths[BLACK_BIRD] = 0;
    }
    if (abilityType == ABILITY_CONSUMABLE && ability == 0) {
        birdDamageMultipliers[BLACK_BIRD] = 3;
    }
    if (abilityType == ABILITY_CONSUMABLE && ability == 1) {
        birdShields[BLACK_BIRD] = 100;
    }
    if (abilityType == ABILITY_CONSUMABLE && ability == 2) {
        drawExplosion(pigPositions[X(RED_BIRD)], pigPositions[Y(RED_BIRD)], 100, 20, 5, 10, 50, 40);
        pigHealths[PIG1] = 0;
        pigHealths[PIG2] = 0;
        pigHealths[KINGPIG] = 0;
    }
}

void pigTurn() {
    int i;
    int j;
    int birdTarget;
    int damage;
    for(i = 0; i < 3; i++){
        if(pigHealths[i] > 0){
            if(pigFocus < 0) birdTarget = rand()%3;
            else birdTarget = pigFocus;
            if(birdHealths[birdTarget] <= 0) birdTarget = RED_BIRD;
            if(birdHealths[birdTarget] <= 0) birdTarget = BLACK_BIRD;
            if(birdHealths[birdTarget] <= 0) birdTarget = YELLOW_BIRD;
            if(birdHealths[birdTarget] > 0){
                damage = pigMinDmg + rand()%(pigMaxDmg-pigMinDmg);
                if(i == KINGPIG) damage += kingPigBonusDmg;
                for(j = 0; j < 6; j++) menu[i] = "";
                menu[0] = strings[69];
                menu[2] = strings[3+i];
                menu[3] = strings[70];
                menu[4] = strings[birdTarget];

                attackBird(birdTarget, damage);

                drawMenu();
                drawHp();
                delay(1000);
            }
        }
    }
    pigFocus = -1;
    menuState = CHARACTER_MENU;
}

void executeAbility() {
    switch (character) {
    case RED_BIRD:
        redAbility();
        break;
    case YELLOW_BIRD:
        yellowAbility();
        break;
    case BLACK_BIRD:
        blackAbility();
        break;
    }
    pigTurn();
    drawAll();
}

int getInput() {
    while (TRUE) {
        inputBuffer = getch();
        if (inputBuffer == 0) {
            inputBuffer = getch();
            switch (inputBuffer) {
            case 72:
                return ARROW_UP;
            case 80:
                return ARROW_DOWN;
            case 75:
                return BACK_KEY;
            case 77:
                return ENTER_KEY;
            }
        }
        else if(inputBuffer == 'q' || inputBuffer == 'Q') return PAUSE_KEY;
        else if(inputBuffer == ' ' || inputBuffer == 13) return ENTER_KEY;
        else if(inputBuffer == 27) return BACK_KEY;
    }
}

void changeSelection(int step){
    menuSelection += step;
    if(menuSelection > 2) menuSelection = 0;
    if(menuSelection < 0) menuSelection = 2;
    if(menuState == CHARACTER_MENU){
        if(birdHealths[menuSelection] < 0) changeSelection(step);
    }
}

void menuSelect() {
    switch (menuState) {
    case CHARACTER_MENU:
        character = menuSelection;
        menuState = ABILITY_TYPE_MENU;
        break;
    case ABILITY_TYPE_MENU:
        abilityType = menuSelection;
        menuState = ABILITY_SELECT_MENU;
        break;
    case ABILITY_SELECT_MENU:
        ability = menuSelection;
        menuState = TARGET_SELECT_MENU;
        if(abilityType == ABILITY_CONSUMABLE) executeAbility();
        break;
    case TARGET_SELECT_MENU:
        target = menuSelection;
        executeAbility();
        break;
    }
    menuSelection = 0;
    changeSelection(1);
    changeSelection(-1);
}

void returnMenu(){
    menuState--;
    if(menuState < 0) run = FALSE;
}

void winCondition(){
    if(pigHealths[PIG1] <= 0 && pigHealths[PIG2] <= 0 && pigHealths[KINGPIG] <= 0){
        run = FALSE;
        int midx = getmaxx() / 2;
        int midy = getmaxy() / 2;
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
        settextjustify(CENTER_TEXT, CENTER_TEXT);
        drawText(midx, midy, strings[71], YELLOW, RED);
        getch();
    }
    if(birdHealths[RED_BIRD] <= 0 && birdHealths[YELLOW_BIRD] <= 0 && birdHealths[BLACK_BIRD] <= 0){
        run = FALSE;
        int midx = getmaxx() / 2;
        int midy = getmaxy() / 2;
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
        settextjustify(CENTER_TEXT, CENTER_TEXT);
        drawText(midx, midy, strings[72], RED, BLACK);
    }
}

int main() {
    initGraphics();
    drawAll();
    menuState = CHARACTER_MENU;
    while (run) {
        setMenuText();
        drawMenu();
        int input = getInput();
        if(input == ARROW_UP) changeSelection(-1);
        if(input == ARROW_DOWN) changeSelection(1);
        if(input == ENTER_KEY) menuSelect();
        if(input == BACK_KEY) returnMenu();
        if(input == PAUSE_KEY) run = FALSE;
        winCondition();
    }
    getch();
    closegraph();
    return 0;
}
