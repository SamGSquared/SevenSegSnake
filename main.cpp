

#define ITERATIONS_PER_TICK 50 //how many times the game redraws the screen per tick

#define DIR_N 0
#define DIR_E 1
#define DIR_S 2
#define DIR_W 3

#define EMPTY 0
#define SNAKE 1
#define FOOD 2


#include <Arduino.h>
#include <deque>
#include <seven.h>

struct segment
{
    byte bitmask;
    byte digit;
    bool real;
    int contains;
};

struct snake_seg
{
    int posX;
    int posY;
};

//these connect to the max6921
int latchPin = 4;
int clockPin = 2;
int dataPin = 3;

int leftPin = 26; //connect to left button
int rightPin = 27; //connect to right button
int ADCpin = 29; //any pin with an ADC, leave floating

segment real_board[8][5][3];
int dir_buffer = DIR_E;
std::deque<snake_seg> snake;

void construct_board()
{
    byte currentDigit = 0b00000001; //start at first digit
    for (int i = 0; i < 8; i++) //iterate over the 8 digits of the vfd
    {
        //create each segment and map to board location
        real_board[i][0][0] = {0, 0, false, 0};
        real_board[i][0][1] = {SEG_A, currentDigit, true, 0};
        real_board[i][0][2] = {0, 0, false, 0};
        real_board[i][1][0] = {SEG_F, currentDigit, true, 0};
        real_board[i][1][1] = {0, 0, false, 0};
        real_board[i][1][2] = {SEG_B, currentDigit, true, 0};
        real_board[i][2][0] = {0, 0, false, 0};
        real_board[i][2][1] = {SEG_G, currentDigit, true, 0};
        real_board[i][2][2] = {0, 0, false, 0};
        real_board[i][3][0] = {SEG_E, currentDigit, true, 0};
        real_board[i][3][1] = {0, 0, false, 0};
        real_board[i][3][2] = {SEG_C, currentDigit, true, 0};
        real_board[i][4][0] = {0, 0, false, 0};
        real_board[i][4][1] = {SEG_D, currentDigit, true, 0};
        real_board[i][4][2] = {0, 0, false, 0};
        currentDigit = currentDigit << 1; //bit shift to next digit
    }
}
void draw_board() //this is definitely not optimal lol 
{
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            for (int k = 0; k < 8; k++)
            {
                if (real_board[k][i][j].real && (real_board[k][i][j].contains == SNAKE || real_board[k][i][j].contains == FOOD))
                {
                    digitalWrite(latchPin, LOW);
                    shiftOut(dataPin, clockPin, LSBFIRST, 0x00); //upper byte of max6921 internal shift register is not used
                    shiftOut(dataPin, clockPin, LSBFIRST, real_board[k][i][j].bitmask); //shift out bitmask for segment
                    shiftOut(dataPin, clockPin, LSBFIRST, real_board[k][i][j].digit); //shfit out digit selection bitmask
                    digitalWrite(latchPin, HIGH);
                    delay(5);
                }
            }
        }
    }
}
void placeFood() //spawns a new food item in a random section
{
    snake_seg foodSegment;
    do
    {
        foodSegment.posX = rand() % 23;
        foodSegment.posY = rand() % 5;
    } while (!real_board[foodSegment.posX / 3][foodSegment.posY][foodSegment.posX % 3].real || !(real_board[foodSegment.posX / 3][foodSegment.posY][foodSegment.posX % 3].contains == EMPTY)); //makes sure food isn't spawned in an invalid spot

    real_board[foodSegment.posX / 3][foodSegment.posY][foodSegment.posX % 3].contains = FOOD; //put food on the board
}
void setup()
{
    //set up our input
    pinMode(latchPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(dataPin, OUTPUT);
    pinMode(leftPin, INPUT_PULLDOWN);
    pinMode(rightPin, INPUT_PULLDOWN);
    pinMode(ADCpin, INPUT);
    construct_board(); //setup our board
    srand(analogRead(ADCpin)); //seed PRNG using floating ADC pin 

    placeFood(); //put a food item in a random starting spot
    snake_seg head = {1, 2}; //create starter snake segment
    real_board[head.posX / 3][head.posY][head.posX % 3].contains = SNAKE; //add snake segment to board
    snake.push_front(head); //add head to snake queue
}

void game_over()
{
    while (true) //Just loops game over screen forever, I might add a game reset button later
    {
        digitalWrite(latchPin, LOW);
        shiftOut(dataPin, clockPin, LSBFIRST, 0x00);
        shiftOut(dataPin, clockPin, LSBFIRST, CHAR_D);
        shiftOut(dataPin, clockPin, LSBFIRST, digit8);
        digitalWrite(latchPin, HIGH);
        delay(5);
        digitalWrite(latchPin, LOW);
        shiftOut(dataPin, clockPin, LSBFIRST, 0x00);
        shiftOut(dataPin, clockPin, LSBFIRST, CHAR_E);
        shiftOut(dataPin, clockPin, LSBFIRST, digit7);
        digitalWrite(latchPin, HIGH);
        delay(5);
        digitalWrite(latchPin, LOW);
        shiftOut(dataPin, clockPin, LSBFIRST, 0x00);
        shiftOut(dataPin, clockPin, LSBFIRST, CHAR_A);
        shiftOut(dataPin, clockPin, LSBFIRST, digit6);
        digitalWrite(latchPin, HIGH);
        delay(5);
        digitalWrite(latchPin, LOW);
        shiftOut(dataPin, clockPin, LSBFIRST, 0x00);
        shiftOut(dataPin, clockPin, LSBFIRST, CHAR_D);
        shiftOut(dataPin, clockPin, LSBFIRST, digit5);
        digitalWrite(latchPin, HIGH);
        delay(5);
    }
}
void update_dir() //rotates the direction the snake is moving left or right
{
    if (digitalRead(leftPin))
    {
        dir_buffer--;
    }
    if (digitalRead(rightPin))
    {
        dir_buffer++;
    }
    if (dir_buffer > 3)
    {
        dir_buffer = 0;
    }
    if (dir_buffer < 0)
    {
        dir_buffer = 3;
    }
}
void find_next_segment()
{   
    //grab current head position
    int currentX = snake.front().posX;
    int currentY = snake.front().posY;

    switch (dir_buffer) //seek forward in the current direction, loop to the other side if we go out of bounds
    {
    case DIR_N:
        currentY++;
        break;
    case DIR_E:
        currentX++;
        break;
    case DIR_S:
        currentY--;
        break;
    case DIR_W:
        currentX--;
    default:
        break;
    }
    if (currentX < 0)
    {
        currentX = 23;
    }
    if (currentX > 23)
    {
        currentX = 0;
    }
    if (currentY < 0)
    {
        currentY = 4;
    }
    if (currentY > 4)
    {
        currentY = 0;
    }
    update_dir(); //rotate direction if necessary
    while (!real_board[currentX / 3][currentY][currentX % 3].real) //repeat the process if we're still in a non existent segment
    {

        switch (dir_buffer)
        {
        case DIR_N:
            currentY++;
            break;
        case DIR_E:
            currentX++;
            break;
        case DIR_S:
            currentY--;
            break;
        case DIR_W:
            currentX--;
        default:
            break;
        }
        if (currentX < 0)
        {
            currentX = 23;
        }
        if (currentX > 23)
        {
            currentX = 0;
        }
        if (currentY < 0)
        {
            currentY = 4;
        }
        if (currentY > 4)
        {
            currentY = 0;
        }
    }
    snake_seg newSeg = {currentX, currentY}; //create our new head
    switch (real_board[currentX / 3][currentY][currentX % 3].contains) //determine contents of next segment
    {
    case EMPTY: //if there's nothing in the next segment
        real_board[snake.back().posX / 3][snake.back().posY][snake.back().posX % 3].contains = EMPTY; //mark tail segment as empty
        snake.pop_back();//remove tail segment from queue
        real_board[currentX / 3][currentY][currentX % 3].contains = SNAKE; //add head segment to board
        snake.push_front(newSeg); //add head segment to queue
        break;
    case FOOD: //if there's food in the next segment
        real_board[currentX / 3][currentY][currentX % 3].contains = SNAKE; //turn the food segment into a snake segment
        snake.push_front(newSeg); //food segment is new head segment
        placeFood(); //add food to board
        break;
    case SNAKE: //if snake runs into itself
        game_over(); //go to game over loop
        break;
    default:
        break;
    }
}

void loop()
{

    for (int i = 0; i < ITERATIONS_PER_TICK; i++)  //refresh board a set number of times (need to change how I time this, game slows when you have a lot of segments)
    {
        draw_board();
        delay(5);
    }
    find_next_segment(); //move the snake
}