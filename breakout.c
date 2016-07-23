//
// breakout.c
//
// Computer Science 50
// Problem Set 3
//
//

// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Stanford Portable Library
#include <spl/gevents.h>
#include <spl/gobjects.h>
#include <spl/gwindow.h>

// height and width of game's window in pixels
#define HEIGHT 600
#define WIDTH 400

// number of rows of bricks
#define ROWS 5

// number of columns of bricks
#define COLS 10

// radius of ball in pixels
#define RADIUS 10

// lives
#define LIVES 3

// prototypes
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points);
GObject detectCollision(GWindow window, GOval ball);

// my-prototypes
GLabel initLives(GWindow window);
void updateLives(GWindow window, GLabel label, int lives);

int main(void)
{
    // seed pseudorandom number generator
    srand48(time(NULL));

    // instantiate window
    GWindow window = newGWindow(WIDTH, HEIGHT);

    // instantiate bricks
    initBricks(window);

    // instantiate ball, centered in middle of window
    GOval ball = initBall(window);

    // instantiate paddle, centered at bottom of window
    GRect paddle = initPaddle(window);

    // instantiate scoreboard, centered in middle of window, just above ball
    GLabel label = initScoreboard(window);
    GLabel label1 = initLives(window);

    // number of bricks initially
    int bricks = COLS * ROWS;

    // number of lives initially
    int lives = LIVES;

    // number of points initially
    int points = 0;
    
    // speed of ball
    double speed = 2.5;
    double xspeed = drand48() * 3.1;
    
    // onovlenna labels wikna
    updateScoreboard(window, label, bricks);
    updateLives(window, label1, lives);

    // keep playing until game over
    while (lives > 0 || bricks > 0)
    {
        // vyhid iz cyklu
        if (bricks == 0) // perevirka kilkosti blokiv, == 0 - game over (you win)
        {   
            removeGWindow(window, label); // vydalyty zapys kilkosti blokiv
            GLabel label = newGLabel("You win!"); // stvoryty zapys You win
            setFont(label, "SansSerif-36"); // shrift zapysu
            setColor(label, "LIGHT_GRAY"); // colir zapysu
            add(window, label);
            double x = (getWidth(window) - getWidth(label)) / 2;
            double y = (getHeight(window) - getHeight(label)) / 2;
            setLocation(label, x, y); // rozmistyty zapys na misci kilkosti blokiv
            break;
        }
        if (lives == 0) // perevirka lives, == 0 game over (you lose)
        {   
            removeGWindow(window, label);
            GLabel label = newGLabel("You lose!");
            setFont(label, "SansSerif-36");
            setColor(label, "LIGHT_GRAY");
            add(window, label);
            double x = (getWidth(window) - getWidth(label)) / 2;
            double y = (getHeight(window) - getHeight(label)) / 2;
            setLocation(label, x, y);
            break;
        }
        
        GEvent event = getNextEvent(MOUSE_EVENT); // ockuemo na diyu z mouse
        
        if (event != NULL) // yakshcho diya e
        {
            if (getEventType(event) == MOUSE_MOVED) // i ca dia ruch mouse
            {
                double x = getX(event) - getWidth(paddle)/2;
                double y = HEIGHT-100+2*RADIUS;
                setLocation(paddle, x, y); // vyznachaemo roztashuvanna raketky
            }
        }
        
        
        move(ball, xspeed, speed); // zapuskaemo u ruch ball
        
        GObject object = detectCollision(window, ball); // perevirka na zitknenna z drugymy object
        
        if (object != NULL) // yakshcho zitknenna vidbulos
        {
            if ((strcmp(getType(object), "GRect") == 0) && (object != paddle)) // i ce bricks
            {
                removeGWindow(window, object); // vydalaemo brick, z yakym vidbulos zitknenna
                bricks--; // zmenshumo kilkist blokiv
                points += 10; // ochky narachovuemo (or not)
                updateScoreboard(window, label, bricks); // onovlenna zapysu z kilkistu bricks
                speed = -speed; // minaemo napram ruhu
                if (bricks == 40)
                {
                    speed += 0.25;
                }
                if (bricks == 30)
                {
                    speed += 0.25;
                }
                if (bricks == 20)
                {
                    speed += 0.25;
                }
                if (bricks == 10)
                {
                    speed += 0.25;
                }
            }
            else if (object == paddle) // yakshcho ce raketka prosto minaemo napram ruhu i shvudkist
            {
                speed -= 0.25 * drand48();
                speed = -speed - 0.25 * drand48();
            }
            
        }
        
        
        if (getX(ball) + getWidth(ball) >= getWidth(window)) // napram ruhu po osi X minaemo bila kraiv vikna
        {
            xspeed = -xspeed;
        }
        else if (getX(ball) <= 0)
        {
            xspeed = -xspeed;
        }
        if (getY(ball) + getWidth(ball) >= getHeight(window)) // napram ruhu po osi Y (vnyzu vikna)
        {
            lives--; // vidnaty lives
            updateLives(window, label1, lives);
            
            // pereistyty ball u centr vikna
            double x = getWidth(window)/2 - getWidth(ball)/2;
            double y = getHeight(window)/2 - getWidth(ball)/2;
            setLocation(ball, x, y); 
            
            // zvorotnyi vidlick dla prodovshenna gry
            GLabel label = newGLabel(""); 
            setFont(label, "SansSerif-36");
            setColor(label, "LIGHT_GRAY");
            add(window, label);
            for (int i = 3; i > 0; i--)
            {
                if (lives == 0)
                {
                    break;
                }
                char s[3];
                sprintf(s, "%i", i);
                setLabel(label, s);
                double x1 = (getWidth(window) - getWidth(label)) / 2;
                double y2 = (getHeight(window) - getHeight(label)) / 3;
                setLocation(label, x1, y2);
                
                pause(1000);
            }
            removeGWindow(window, label); // vydalaemo zapys iz taimerom
            
            speed = -speed * (-1); // napram ruhu vnyz
        }
        else if (getY(ball) <= 0) // vverhu vikna ball vidbyvaetsya
        {
            speed = -speed;
        }
        pause(10);
          
    }
    
    
    // wait for click before exiting
    waitForClick();

    // game over
    closeGWindow(window);
    return 0;
}

/**
 * Initializes window with a grid of bricks.
 */
void initBricks(GWindow window)
{
    float w = 35.6,
        h = 10.0;
    for (int j = 1; j <= ROWS; j++)
    {
        for (int i = 1; i <= COLS; i++)
        {
            GRect rect = newGRect((i-1)*w + 4*i, j*h + (j-1)*4, w, h);
            setFilled(rect, true);
            
            if (j == 1)
            {
                setColor(rect, "RED");  
            }
            else if (j == 2)
            {
                setColor(rect, "ORANGE");  
            }
            else if (j == 3)
            {
                setColor(rect, "YELLOW");  
            }
            else if (j == 4)
            {
                setColor(rect, "GREEN");  
            }
            else
            {
                setColor(rect, "BLUE");  
            }
            
            add(window, rect);
        }
    }
           
}

/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
    GOval circle = newGOval(WIDTH/2 - RADIUS, HEIGHT/2 - RADIUS, 2*RADIUS, 2*RADIUS);
    setColor(circle, "BLACK");
    setFilled(circle, true);
    add(window, circle);
    return circle;
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window)
{
    GRect paddle = newGRect(WIDTH/2 - 2.5*RADIUS, HEIGHT-100+2*RADIUS, 5*RADIUS, RADIUS);
    setColor(paddle, "BLACK");
    setFilled(paddle, true);
    add(window, paddle);
    return paddle;
}

/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{
    GLabel label = newGLabel("");
    add(window, label);
    
    return label;
}

GLabel initLives(GWindow window)
{
    GLabel textlabel = newGLabel("Lives:");
    addToRegion(window, textlabel, "SOUTH");
    GLabel labelscore = newGLabel("");
    add(window, labelscore);
    
    return labelscore;
}
void updateLives(GWindow window, GLabel label, int lives)
{
    // update label
    char s[3];
    sprintf(s, "%i", lives);
    setLabel(label, s);
    addToRegion(window, label, "SOUTH");
}

/**
 * Updates scoreboard's label, keeping it centered in window.
 */
void updateScoreboard(GWindow window, GLabel label, int points)
{
    // update label
    char s[12];
    sprintf(s, "%i", points);
    setLabel(label, s);
    setFont(label, "SansSerif-36");
    setColor(label, "LIGHT_GRAY");

    // center label in window
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
}

/**
 * Detects whether ball has collided with some object in window
 * by checking the four corners of its bounding box (which are
 * outside the ball's GOval, and so the ball can't collide with
 * itself).  Returns object if so, else NULL.
 */
GObject detectCollision(GWindow window, GOval ball)
{
    // ball's location
    double x = getX(ball);
    double y = getY(ball);

    // for checking for collisions
    GObject object;

    // check for collision at ball's top-left corner
    object = getGObjectAt(window, x, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's top-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-left corner
    object = getGObjectAt(window, x, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // no collision
    return NULL;
}
