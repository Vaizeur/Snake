#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define NORTH 1
#define EAST  2
#define SOUTH 3
#define WEST  4

#define HEIGHT 30
#define WIDTH 50
#define APPLE_DROP 25
#define DELAY 100000

#define KEY_UP_SNAKE    'z'
#define KEY_DOWN_SNAKE  's'
#define KEY_LEFT_SNAKE  'q'
#define KEY_RIGHT_SNAKE 'd'

/**
 * Création des struct
 */
typedef struct SnakeSegment {
   int x,y;
   struct SnakeSegment* next;
} SnakeSegment;

typedef struct Snake {
    SnakeSegment* head;
    int direction,score;
} Snake;

/**
 * Définition de toute les méthodes
 */
void loadMap();
bool update(Snake* snake);
void generateApple();
Snake* createSnake(int x,int y);
void addSegment(Snake* snake);
void updateBodyPositions(Snake* snake, int old_x, int old_y);
void updateText(Snake* snake);
void freeSnake(Snake* snake);

int main() {
    initscr();
    noecho();
    curs_set(FALSE);
    nodelay(stdscr, TRUE); //Ne pas bloquer le script au getch()
    srand(time(NULL));

    loadMap();
    int input;

    Snake* snake = createSnake(5,5);

    while (1) {
        input = getch();
        if (input == 'x') {
            endwin();
            freeSnake(snake);
            return 0;
        }

        switch (input) {
            case KEY_UP_SNAKE    : snake->direction = NORTH; break;
            case KEY_LEFT_SNAKE  : snake->direction = WEST;  break;
            case KEY_DOWN_SNAKE  : snake->direction = SOUTH; break;
            case KEY_RIGHT_SNAKE : snake->direction = EAST;  break;
        }

        if(!update(snake)){
            break;
        }
        int chance = rand() % 100;
        if(chance < APPLE_DROP){
            generateApple();
        }
        usleep(DELAY); //Pause de 100ms
    }
    endwin();
    return 0;
}
void loadMap(){
    for (int y = 0; y < HEIGHT; y++) {
         for (int x = 0; x < WIDTH; x++) {
             if (y == 0 || y == HEIGHT - 1 || x == 0 || x == WIDTH - 1) {
                mvaddch(y, x, '#'); //Bordure de la map
             }  else {
                mvaddch(y, x, ' ');
            }
        }
    }
}
bool update(Snake* snake) {
    int old_x = snake->head->x;
    int old_y = snake->head->y;

    //Changement de la position en fonction de la direction
    switch (snake->direction) {
        case NORTH: snake->head->y--; break;
        case EAST:  snake->head->x++; break;
        case SOUTH: snake->head->y++; break;
        case WEST:  snake->head->x--; break;
    }

    char c = mvinch(snake->head->y,snake->head->x) & A_CHARTEXT; //Récupération du char de la nouvelle position 

    if(c == '#' || c == 'O'){ //Touche la bordure ou le corps
        endwin();
        freeSnake(snake);
        return false;
    }
    if(c == 'A'){ //Mange une pomme
       addSegment(snake);
    }
    updateBodyPositions(snake, old_x,old_y);
    updateText(snake);
    mvaddch(snake->head->y, snake->head->x, 'O'); // afficher la tête du snake
    refresh();
    return true;
}

void generateApple(){
    int y = rand() % HEIGHT;
    int x = rand() % WIDTH;

    char c = mvinch(y,x) & A_CHARTEXT;

    if(c != '#' && c != 'O'){
       mvaddch(y,x,'A');
       refresh();
    }
}

Snake* createSnake(int x, int y) {
    Snake* snake = malloc(sizeof(Snake));
    SnakeSegment* head = malloc(sizeof(SnakeSegment));
    head->x = x;
    head->y = y;
    snake->direction = EAST;
    snake->score = 0;
    head->next = NULL;
    snake->head = head;
    return snake;
}

void addSegment(Snake* snake){
   snake->score++;
   SnakeSegment* current = snake->head;
   while(current->next != NULL){
       current = current->next;
   }
   SnakeSegment* new_segment = malloc(sizeof(SnakeSegment));
   new_segment->x = current->x;
   new_segment->y = current->y;
   new_segment->next = NULL;
   current->next = new_segment;
}

void updateBodyPositions(Snake* snake, int old_x, int old_y) {
    int tmp_x, tmp_y;
    SnakeSegment* current = snake->head->next;

    while(current != NULL) {
        tmp_x = current->x;
        tmp_y = current->y;

        current->x = old_x;
        current->y = old_y;

        old_x = tmp_x;
        old_y = tmp_y;

        current = current->next;
    }

    // Effacer le dernier segment de la map
    mvaddch(old_y, old_x, ' ');
}

void updateText(Snake* snake){
    mvprintw(0,WIDTH+5, "Score : %d", snake->score);
}

void freeSnake(Snake* snake){
    SnakeSegment* current = snake->head;
    while (current != NULL) {
        SnakeSegment* next = current->next;
        free(current);
        current = next;
    }
    free(snake);
}