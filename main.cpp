#include "FEHLCD.h"
#include <math.h>
#include <FEHUtility.h>
#include <FEHLCD.h>

void main_menu();//main menu scene
void main_menu_decorations();//main menu pixel art
void difficulty();//difficulty selection scene
void scores(int []);//scores scene
void scores_decorations(); //scores decoration scene
void creds();//credits scene
void rules();//rules scene
void game_look(int x, int s);//game scene
void game_over_scene();//game over scene
void score_display(int s);//displaying the current score
void score_display_erase(int s);//erasing previous score to replace with new score
void happycookie();//happy cookies for easy mode (game scene)
void angrycookie();//angry cookies for hard mode (game scene)
int selection();//selection function (detect button presses) for main menu
int selection_dif();//selection function for difficulty scene
int selection_stats();//selection function for stats scene
int selection_rules();//selection function for rules scene
int selection_game_over();//selection function for game over scene
int selection_credits();//selection function for cedits scene
void score_sort(int *, int, int);//add new score into an array that contains all the scores

//class contains all the functions and variables that run the gameplay
//Made by Grace Jiang
class circle
{
    public:
        circle(int radius = 0, float hitbox = 0.); //constructor
        int projectile(); //function that runs the gameplay
        void coordinates(int i); //calculates the physics for projectiles
        void draw(int i); //draws the cookie projectiles
        void erase(int i); //erase the cookie projectile to create animation
        void hit(int i); //changes appearance of cookie if hit
        void draw_bomb(int i); //draw the bomb projectiles
        void hit_bomb(int i); //change appearance of bomb if hit
        void end(int j); //the ending position of the cookie after the animation
        int error(); //calculates the number of cookies not hit 
        int output_score(); //outputs the number of cookies hit
    private:
        float x[5], y[5], radius, time, vx[5], vy, a, hitbox; //vx is velocity in x direction, vy velocity in y direction, a acceleration (gravity)
        int numcookies, state[5], score, type[5]; 
        //state represents the hit or not hit state of the cookie, numcookies - randomly generated 1 - 5, score detects how many cookies were hit
};
//struct that contains x and y variables of mouse position(used to detect hits and button presses)
//Made by Grace Jiang
struct pointer
{   
    float x_mouse;
    float y_mouse;
};

int main(void) 
{
    int choice, played_before_easy = 0, played_before_hard = 0, high_score[100];/*choice - detect button press, played_before_easy/hard used for play again
    high_score - all the scores from previously played rounds (sorted later and used in score scene)*/
    //booleans used to go from scene to scene
    bool menu = true, game = false, instructions = false, stats = false, diff = false, game_over = false, hard = false, easy = false, credits = false;
    pointer point;//struct call for button hits

    //fill in score array with 0
    for(int s = 0; s < 100; s++)
    {
        high_score[s] = 0;
    }

    //infinte loop
    while (true) 
    {
        //menu scene
        while (menu == true)
        {
            main_menu(); //menu setup
            main_menu_decorations();

            if (LCD.Touch(&point.x_mouse, &point.y_mouse))
            {
                choice = selection(); //detect button press
                switch(choice)
                {
                    //stay on main menu if no button is pushed
                    case 0:
                        break;
                    //start button is pushed --> go to difficulty selection
                    case 1:
                        menu = false;
                        diff = true;
                        LCD.Update();
                        break;
                    //statistics button is pushed
                    case 2:
                        menu = false;
                        stats = true;
                        LCD.Update();
                        break;
                    //instructions button is pushed
                    case 3:
                        menu = false;
                        instructions = true;
                        LCD.Update();
                        break;
                    //credits button is pushed
                     case 4:
                       menu = false;
                       credits = true;
                       LCD.Update();
                    break;
                }//end switch
            }//end if touched
            LCD.Update();
        }//end while menu

        //switch to difficulty scene
        while(diff == true && menu == false)
        {
            difficulty(); // difficulty setup

            if (LCD.Touch(&point.x_mouse, &point.y_mouse))
            {
                choice = selection_dif(); //detect button press
                switch(choice)
                {
                    //back button
                    case 0:
                        diff = false;
                        menu = true;
                        LCD.Update();
                        break;
                    //easy button
                    case 1:
                        diff = false;
                        game = true;
                        easy = true;
                        LCD.Update();
                        break;
                    //hard button
                    case 2:
                        diff = false;
                        game = true;
                        hard = true;
                        LCD.Update();
                        break;
                    //click not on a button
                    case 3:
                        break;
                }//end switch case
            }//end if touched           
            LCD.Update();
        }//end difficulty scene

        //switch to statistics scene
        while(stats == true && menu == false)
        {   
            scores_decorations(); //score decorations
            scores(high_score); //score setup

            if (LCD.Touch(&point.x_mouse, &point.y_mouse))
            {
                choice = selection_stats(); //detect button press
                switch(choice)
                {
                    //back button
                    case 0:
                        stats = false;
                        menu = true;
                        LCD.Update();
                        break;
                    //nothing pressed
                    case 1:
                        LCD.Update();
                        break;
                }//end switch
            } //end if touched
            LCD.Update();
        }//end score scene

        //switch to credits scene
        while (credits == true && menu == false)
        {
            creds(); //credits setup
 
            if (LCD.Touch(&point.x_mouse, &point.y_mouse))
            {
                choice = selection_credits(); //detect button press
                switch(choice)
                {
                    //back button
                    case 0:
                        credits = false;
                        menu = true;
                        LCD.Update();
                        break;
                    //nothing pressed
                    case 1:
                        LCD.Update();
                        break;
                
                } //end switch
            } //end if touched
            LCD.Update();
        } //end credit scene


        //switch to instructions scene
        while(instructions == true && menu == false)
        {
            rules(); //rules setup

            if (LCD.Touch(&point.x_mouse, &point.y_mouse))
            {
                choice = selection_rules(); //detect button press
                switch(choice)
                {
                    //back button
                    case 0:
                        instructions = false;
                        menu = true;
                        LCD.Update();
                        break;
                    //nothing pressed
                    case 1:
                        LCD.Update();
                        break;
                } //end switch case
            } //end if touch
            LCD.Update();
        } //end rules scene

        int miss = 0; //count how many cookies were missed
        int total_score = 0; //counter for score
        //game scene
        while(game == true && menu == false)
        {
            played_before_easy = 0; //reset so player can infinitely play again
            played_before_hard = 0;
            //easy mode
            while(easy == true)
            {
                game_look(miss, total_score); //easy game setup
                happycookie();
                played_before_easy++; //increment so player can play again with the same difficulty
                //ball performs most game functions
                circle ball(5, 2); //radius 4, hitbox 3
                miss += ball.projectile(); //returns how many cookies were missed
                score_display_erase(total_score); //erase previous score
                total_score += ball.output_score(); //increment total score by new amount
                score_display(total_score); //print out new score
                //gameover condition
                if(miss >= 3) 
                {
                    game = false;
                    game_over = true;
                    easy = false;
                }
                LCD.Update();
            }//end easy while loop
            while(hard == true)
            {
                game_look(miss, total_score); //hard game setup
                angrycookie();
                played_before_hard++; //increment so player can play again with the same difficulty
                //ball performs most game functions
                circle ball(4, 2); //radius 3, hitbox 2
                miss += ball.projectile(); //returns how many cookies were missed
                score_display_erase(total_score); //erase previous score
                total_score += ball.output_score() * 5; //increment total score by new amount (cookie in hard mode is worth 5 points)
                score_display(total_score); //print out new score
                //gameover condition
                if(miss >= 3)
                {
                    game = false;
                    game_over = true;
                    hard = false;
                }
                LCD.Update();
            } //end hard while loop

            score_sort(high_score, 100, total_score); //input new score into high_score array
            //sorting array in descending order
            for(int i = 0; i < 100; i++)
            {
                for(int j = 0; j < 100; j++)
                {
                    if(high_score[j] < high_score[i])
                    {
                        int tmp = high_score[i];
                        high_score[i] = high_score[j];
                        high_score[j] = tmp;
                    }
                }
            }
            LCD.Update();
        } //end game scene

        //game over scene
        while(game_over == true)
        {
            game_over_scene(); //game over setup

            if (LCD.Touch(&point.x_mouse, &point.y_mouse))
            {
                choice = selection_game_over(); //detect button press
                switch(choice)
                {
                    //Play Again button
                    case 0:
                        game_over = false;
                        game = true;
                        //detects which difficulty level was played before to play again
                        if(played_before_easy > 0)
                        {
                            easy = true;
                            LCD.Update();
                        }
                        else if(played_before_hard > 0)
                        {
                            hard = true;
                            LCD.Update();
                        }
                        LCD.Update();
                        break;
                    //Main Menu button
                    case 1:
                        game_over = false;
                        menu = true;
                        LCD.Update();
                        break;
                    //nothing pressed
                    case 2:
                        LCD.Update();
                        break;
                } //end switch case
            } //end if touched
            LCD.Update();
        } //end game over scene
        LCD.Update();
    } //end infinite while loop
    return 0;
}//end main
//display what main menu looks like
//Made By Grace Jiang
void main_menu()
{

    LCD.SetBackgroundColor(CORNFLOWERBLUE); //background color
    LCD.Clear();

    LCD.SetFontColor(WHITE); //print out buttons and text
    LCD.WriteAt("Cookie Ninja",85,70);
    LCD.WriteAt("Start",130,120);
    LCD.WriteAt("Statistics",101,140);
    LCD.WriteAt("Instructions",90,160);
    LCD.WriteAt("Credits",120,180);
}
//main menu pixel art
//Made by Grace Jiang and Caitlin Chien 
void main_menu_decorations()
{
    int oreo[10][24] = {
    {CORNFLOWERBLUE, SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN, SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN, CORNFLOWERBLUE},
    {SADDLEBROWN, PERU, SADDLEBROWN, PERU, SADDLEBROWN, PERU, SADDLEBROWN, PERU, SADDLEBROWN, PERU, SADDLEBROWN, PERU, SIENNA, SADDLEBROWN,SIENNA, SADDLEBROWN,SIENNA, SADDLEBROWN,SIENNA, SADDLEBROWN,SIENNA, SADDLEBROWN,SIENNA, SADDLEBROWN},
    {SADDLEBROWN, SIENNA,SIENNA,SIENNA,SIENNA,SIENNA,SIENNA,SIENNA,SIENNA,SIENNA,SIENNA,SIENNA,SIENNA,SIENNA,SIENNA,SIENNA,SIENNA,SIENNA,SIENNA,SIENNA,SIENNA,SIENNA,SIENNA,SADDLEBROWN},
    {SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN},
    {CORNFLOWERBLUE, PEACHPUFF, LINEN,LINEN,LINEN,LINEN,LINEN,LINEN,LINEN,LINEN,LINEN,LINEN,LINEN,LINEN,LINEN,LINEN,LINEN,LINEN,LINEN,LINEN,LINEN,LINEN,PEACHPUFF,CORNFLOWERBLUE},
    {CORNFLOWERBLUE, PEACHPUFF, LINEN,LINEN,LINEN,LINEN,LINEN,LINEN,LINEN,LINEN,LINEN,LINEN,LINEN,LINEN,LINEN,LINEN,LINEN,LINEN,LINEN,LINEN,LINEN,LINEN,PEACHPUFF,CORNFLOWERBLUE},
    {SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN},
    {SADDLEBROWN, PERU,PERU,PERU,PERU,PERU,PERU,PERU,PERU,PERU,PERU,SIENNA,SIENNA,SIENNA,SIENNA,SIENNA,SIENNA,SIENNA,SIENNA,SIENNA,SIENNA,SIENNA,SIENNA,SADDLEBROWN},
    {SADDLEBROWN, SIENNA,SADDLEBROWN, SIENNA,SADDLEBROWN, SIENNA,SADDLEBROWN, SIENNA,SADDLEBROWN, SIENNA,SADDLEBROWN, SIENNA,SIENNA,SADDLEBROWN,SIENNA,SADDLEBROWN,SIENNA,SADDLEBROWN,SIENNA,SADDLEBROWN,SIENNA,SADDLEBROWN,SIENNA,SADDLEBROWN},
    {CORNFLOWERBLUE, SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN, SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN,SADDLEBROWN, CORNFLOWERBLUE}
    };

    //bottom of screen cookies
    for(int i = 0; i < 13; i++)
    {
        for(int y = 0; y < 10; y++)
        {
            for(int x = 0; x < 24; x++)
            {
                LCD.SetFontColor(oreo[y][x]);
                LCD.DrawPixel(x + (i+1) * 24 -20, y + 230);
            }
        }
    }
    for(int j = 0; j < 4; j++) //cookie stacks
    {
        for(int y = 0; y < 10; y++)
        {
            for(int x = 0; x < 24; x++)
            {
                LCD.SetFontColor(oreo[y][x]);
                LCD.DrawPixel(x, y + 240 - (j + 1)*20);
            }
        }
    }
    for(int j = 0; j < 4; j++)
    {
        for(int y = 0; y < 10; y++)
        {
            for(int x = 0; x < 24; x++)
            {
                LCD.SetFontColor(oreo[y][x]);
                LCD.DrawPixel(x + 5, y + 230 - (j + 1)*20);
            }
        }
    }
    for(int j = 0; j < 2; j++)
    {
        for(int y = 0; y < 10; y++)
        {
            for(int x = 0; x < 24; x++)
            {
                LCD.SetFontColor(oreo[y][x]);
                LCD.DrawPixel(x + 29, y + 230 - (j + 1)*10);
            }
        }
    }
    for(int j = 0; j < 3; j++)
    {
        for(int y = 0; y < 10; y++)
        {
            for(int x = 0; x < 24; x++)
            {
                LCD.SetFontColor(oreo[y][x]);
                LCD.DrawPixel(x + 58, y + 230 - (j + 1)*20);
            }
        }
    }
    for(int j = 0; j < 3; j++)
    {
        for(int y = 0; y < 10; y++)
        {
            for(int x = 0; x < 24; x++)
            {
                LCD.SetFontColor(oreo[y][x]);
                LCD.DrawPixel(x + 53, y + 240 - (j + 1)*20);
            }
        }
    }
    for(int y = 0; y < 10; y++)
    {
        for(int x = 0; x < 24; x++)
        {
            LCD.SetFontColor(oreo[y][x]);
            LCD.DrawPixel(x + 77, y + 220);
        }
    }
    for(int j = 0; j < 2; j++)
    {
        for(int y = 0; y < 10; y++)
        {
            for(int x = 0; x < 24; x++)
            {
                LCD.SetFontColor(oreo[y][x]);
                LCD.DrawPixel(x + 101, y + 240 - (j + 1)*20);
            }
        }
    }
    for(int y = 0; y < 10; y++)
    {
        for(int x = 0; x < 24; x++)
        {
            LCD.SetFontColor(oreo[y][x]);
            LCD.DrawPixel(x + 96, y + 210);
        }
    }
    for(int j = 0; j < 3; j++)
    {
        for(int y = 0; y < 10; y++)
        {
            for(int x = 0; x < 24; x++)
            {
                LCD.SetFontColor(oreo[y][x]);
                LCD.DrawPixel(x + 125, y + 230 - (j + 1)*10);
            }
        }
    }
    for(int y = 0; y < 10; y++)
    {
        for(int x = 0; x < 24; x++)
        {
            LCD.SetFontColor(oreo[y][x]);
            LCD.DrawPixel(x + 154, y + 220);
        }
    }
    for(int j = 0; j < 2; j++)
    {
        for(int y = 0; y < 10; y++)
        {
            for(int x = 0; x < 24; x++)
            {
                LCD.SetFontColor(oreo[y][x]);
                LCD.DrawPixel(x + 192, y + 240 - (j + 1)*20);
            }
        }
    }
    for(int y = 0; y < 10; y++)
    {
        for(int x = 0; x < 24; x++)
        {
            LCD.SetFontColor(oreo[y][x]);
            LCD.DrawPixel(x + 197, y + 210);
        }
    }
    for(int j = 0; j < 2; j++)
    {
        for(int y = 0; y < 10; y++)
        {
            for(int x = 0; x < 24; x++)
            {
                LCD.SetFontColor(oreo[y][x]);
                LCD.DrawPixel(x + 221, y + 230 - (j + 1)*10);
            }
        }
    }
    for(int j = 0; j < 3; j++)
    {
        for(int y = 0; y < 10; y++)
        {
            for(int x = 0; x < 24; x++)
            {
                LCD.SetFontColor(oreo[y][x]);
                LCD.DrawPixel(x + 269, y + 240 - (j + 1)*20);
            }
        }
    }
    for(int j = 0; j < 3; j++)
    {
        for(int y = 0; y < 10; y++)
        {
            for(int x = 0; x < 24; x++)
            {
                LCD.SetFontColor(oreo[y][x]);
                LCD.DrawPixel(x + 264, y + 230 - (j + 1)*20);
            }
        }
    }
    for(int j = 0; j < 4; j++)
    {
        for(int y = 0; y < 10; y++)
        {
            for(int x = 0; x < 24; x++)
            {
                LCD.SetFontColor(oreo[y][x]);
                LCD.DrawPixel(x + 297, y + 240 - (j + 1)*20);
            }
        }
    }
    for(int j = 0; j < 4; j++)
    {
        for(int y = 0; y < 10; y++)
        {
            for(int x = 0; x < 24; x++)
            {
                LCD.SetFontColor(oreo[y][x]);
                LCD.DrawPixel(x + 292, y + 230 - (j + 1)*20);
            }
        }
    }

    //top of screen cookies
    for(int i = 0; i < 13; i++)
    {
        for(int y = 0; y < 10; y++)
        {
            for(int x = 0; x < 24; x++)
            {
                LCD.SetFontColor(oreo[y][x]);
                LCD.DrawPixel(x + (i+1) * 24 -20, y);
            }
        }
    }
    //star array
    int stararray[15][9] = {
    {CORNFLOWERBLUE,CORNFLOWERBLUE,CORNFLOWERBLUE,CORNFLOWERBLUE,GRAY,CORNFLOWERBLUE,CORNFLOWERBLUE,CORNFLOWERBLUE,CORNFLOWERBLUE},
    {CORNFLOWERBLUE ,CORNFLOWERBLUE,CORNFLOWERBLUE,CORNFLOWERBLUE,GRAY,CORNFLOWERBLUE,CORNFLOWERBLUE,CORNFLOWERBLUE,CORNFLOWERBLUE},
    {CORNFLOWERBLUE,CORNFLOWERBLUE,CORNFLOWERBLUE,CORNFLOWERBLUE,GRAY,CORNFLOWERBLUE,CORNFLOWERBLUE,CORNFLOWERBLUE,CORNFLOWERBLUE},
    {CORNFLOWERBLUE,CORNFLOWERBLUE,CORNFLOWERBLUE,CORNFLOWERBLUE,GRAY,CORNFLOWERBLUE,CORNFLOWERBLUE,CORNFLOWERBLUE,CORNFLOWERBLUE},
    {CORNFLOWERBLUE,CORNFLOWERBLUE,CORNFLOWERBLUE,GRAY,SEASHELL,GRAY,CORNFLOWERBLUE,CORNFLOWERBLUE,CORNFLOWERBLUE},
    {CORNFLOWERBLUE,CORNFLOWERBLUE,CORNFLOWERBLUE,GRAY,SEASHELL,GRAY,CORNFLOWERBLUE,CORNFLOWERBLUE,CORNFLOWERBLUE},
    {CORNFLOWERBLUE,CORNFLOWERBLUE,GRAY,SEASHELL,SEASHELL,SEASHELL,GRAY,CORNFLOWERBLUE,CORNFLOWERBLUE},
    {GRAY,GRAY,SEASHELL,SEASHELL,SEASHELL,SEASHELL,SEASHELL,GRAY,GRAY},
    {CORNFLOWERBLUE,CORNFLOWERBLUE,GRAY,SEASHELL,SEASHELL,SEASHELL,GRAY,CORNFLOWERBLUE,CORNFLOWERBLUE},
    {CORNFLOWERBLUE,CORNFLOWERBLUE,CORNFLOWERBLUE,GRAY,SEASHELL,GRAY,CORNFLOWERBLUE,CORNFLOWERBLUE,CORNFLOWERBLUE},
    {CORNFLOWERBLUE,CORNFLOWERBLUE,CORNFLOWERBLUE,GRAY,SEASHELL,GRAY,CORNFLOWERBLUE,CORNFLOWERBLUE,CORNFLOWERBLUE},
    {CORNFLOWERBLUE,CORNFLOWERBLUE,CORNFLOWERBLUE,CORNFLOWERBLUE,GRAY,CORNFLOWERBLUE,CORNFLOWERBLUE,CORNFLOWERBLUE,CORNFLOWERBLUE},
    {CORNFLOWERBLUE,CORNFLOWERBLUE,CORNFLOWERBLUE,CORNFLOWERBLUE,GRAY,CORNFLOWERBLUE,CORNFLOWERBLUE,CORNFLOWERBLUE,CORNFLOWERBLUE},
    {CORNFLOWERBLUE,CORNFLOWERBLUE,CORNFLOWERBLUE,CORNFLOWERBLUE,GRAY,CORNFLOWERBLUE,CORNFLOWERBLUE,CORNFLOWERBLUE,CORNFLOWERBLUE},
    {CORNFLOWERBLUE,CORNFLOWERBLUE,CORNFLOWERBLUE,CORNFLOWERBLUE,GRAY,CORNFLOWERBLUE,CORNFLOWERBLUE,CORNFLOWERBLUE,CORNFLOWERBLUE}
    };
    //draw out stars on screen
   for(int y = 0; y < 15; y++)
   {
       for(int x = 0; x < 9; x++)
       {
           LCD.SetFontColor(stararray[y][x]);
           LCD.DrawPixel(x + 30, y + 100);
           LCD.DrawPixel(x + 50, y + 70);
           LCD.DrawPixel(x + 20, y + 20);
           LCD.DrawPixel(x + 35, y + 120);
           LCD.DrawPixel(x + 100, y +100);
           LCD.DrawPixel(x + 300, y + 70);
           LCD.DrawPixel(x + 275, y + 85);
           LCD.DrawPixel(x + 250, y + 150);
       }
   }


}
//displays difficulty screen
//Made By Grace Jiang
void difficulty()
{
    LCD.SetBackgroundColor(LIGHTSTEELBLUE);
    LCD.Clear();
    
    LCD.SetFontColor(WHITE);
    LCD.WriteAt("Back", 1, 224);
    LCD.WriteAt("Easy", 92, 112);
    LCD.DrawRectangle(90, 110, 54, 19); //border
    LCD.WriteAt("Hard", 180, 112);
    LCD.DrawRectangle(178, 110, 54, 19);
    LCD.WriteAt("Select Difficulty", 64, 75);
}
//array input is the sorted array
//scores scene
//Made by Caitlin Chien 
void scores(int high_score[])
{
    LCD.SetFontColor(WHITE);
    LCD.WriteAt("Back", 1, 224);
    LCD.WriteAt("High Scores", 95, 4);
    LCD.DrawLine(95,20,223,20);

    //print out the top 5 high scores using the array
    LCD.WriteAt("1.",123,40);
    LCD.WriteAt(high_score[0], 147,40);
    LCD.WriteAt("2.",123,70);
    LCD.WriteAt(high_score[1],147,70);
    LCD.WriteAt("3.",123,100);
    LCD.WriteAt(high_score[2],147,100);
    LCD.WriteAt("4.",123,130);
    LCD.WriteAt(high_score[3],147,130);
    LCD.WriteAt("5.",123,160);
    LCD.WriteAt(high_score[4],147,160);
}
//scores decoration scene
//Made by Caitlin Chien 
void scores_decorations()
{
    LCD.SetBackgroundColor(LIGHTSTEELBLUE);
    LCD.Clear();

    int hot_chocolate[11][12] = {
    {LIGHTSTEELBLUE,LIGHTSTEELBLUE,LIGHTSTEELBLUE,LIGHTSTEELBLUE,GHOSTWHITE,GHOSTWHITE,LIGHTSTEELBLUE,LIGHTSTEELBLUE,LIGHTSTEELBLUE,LIGHTSTEELBLUE,LIGHTSTEELBLUE,LIGHTSTEELBLUE},
    {LIGHTSTEELBLUE,LIGHTSTEELBLUE,LIGHTSTEELBLUE,GHOSTWHITE,GHOSTWHITE,GHOSTWHITE,LIGHTSTEELBLUE,LIGHTSTEELBLUE,LIGHTSTEELBLUE,LIGHTSTEELBLUE,LIGHTSTEELBLUE,LIGHTSTEELBLUE},
    {LIGHTSTEELBLUE,DARKRED,DARKRED,GHOSTWHITE,GHOSTWHITE,GHOSTWHITE,DARKRED,DARKRED,DARKRED,LIGHTSTEELBLUE, LIGHTSTEELBLUE, LIGHTSTEELBLUE},
    {LIGHTSTEELBLUE, DARKRED, GHOSTWHITE, GHOSTWHITE, GHOSTWHITE, GHOSTWHITE, GHOSTWHITE, BROWN, DARKRED, LIGHTSTEELBLUE,LIGHTSTEELBLUE,LIGHTSTEELBLUE},
    {DARKRED, DARKRED, BROWN, BROWN, BROWN, BROWN, BROWN, BROWN, DARKRED, DARKRED, DARKRED, DARKRED},
    {DARKRED, DARKRED, DARKRED, DARKRED, DARKRED, DARKRED, DARKRED, DARKRED, DARKRED, DARKRED, DARKRED, DARKRED},
    {DARKRED, DARKRED, DARKRED, GHOSTWHITE, DARKRED, GHOSTWHITE, DARKRED, GHOSTWHITE, DARKRED, DARKRED, LIGHTSTEELBLUE, DARKRED},
    {LIGHTSTEELBLUE, DARKRED, DARKRED, DARKRED, GHOSTWHITE, GHOSTWHITE, GHOSTWHITE, DARKRED, DARKRED, DARKRED, DARKRED, DARKRED},
    {LIGHTSTEELBLUE, DARKRED, DARKRED, DARKRED, GHOSTWHITE, GHOSTWHITE, GHOSTWHITE, DARKRED, DARKRED, DARKRED, LIGHTSTEELBLUE, LIGHTSTEELBLUE},
    {LIGHTSTEELBLUE, LIGHTSTEELBLUE, DARKRED, GHOSTWHITE, DARKRED, GHOSTWHITE, DARKRED, GHOSTWHITE, DARKRED, LIGHTSTEELBLUE, LIGHTSTEELBLUE, LIGHTSTEELBLUE},
    {GHOSTWHITE, GHOSTWHITE, DARKRED, DARKRED, DARKRED, DARKRED, DARKRED, DARKRED, GHOSTWHITE, GHOSTWHITE, GHOSTWHITE, GHOSTWHITE}
    };
 
    for(int j = 0; j < 25; j++)
    {
        for(int y = 0; y < 11; y++)
        {
            for(int x = 0; x < 12; x++)
            {
                LCD.SetFontColor(hot_chocolate[y][x]);
                LCD.DrawPixel(x + (j+1) * 12, y + 21);
            }
        }
    }

}
//credits scene
//Made by Caitlin Chien 
void creds()
{
    LCD.SetBackgroundColor(MEDIUMPURPLE);
    LCD.Clear();
 
    LCD.SetFontColor(WHITE);
    LCD.WriteAt("Back",1,224);
    LCD.WriteAt("Made by:",105,70);
    LCD.WriteAt("Grace Jiang",90,90);
    LCD.WriteAt("Caitlin Chien",75,110);
}
//rules scene
//Made by Caitlin Chien 
void rules()
{
    LCD.SetBackgroundColor(DARKSALMON);
    LCD.Clear();

    LCD.SetFontColor(WHITE);
    LCD.WriteAt("Back", 1, 224);
    LCD.WriteAt("To start the game, choose",1,1);
    LCD.WriteAt("a difficulty level. Tap", 1,18);
    LCD.WriteAt("where the cookies appear",1,36);
    LCD.WriteAt("to earn points.",1,54);
    LCD.WriteAt("The game ends when 3", 1,72);
    LCD.WriteAt("cookies are missed or", 1,90);
    LCD.WriteAt("a bomb is hit.", 1,108);
    LCD.WriteAt("Then, decide to play again", 1,144);
    LCD.WriteAt("or go back to the menu.", 1,162);
    LCD.WriteAt("to see high scores.",1,180);
}
//happy cookie decorations for game scene
//Made by Grace Jiang
void happycookie()
{
    //cookie array!
    //cookie array for the cookies at the bottom of the screen
    int cookiearrayfloor[8][8] = {
        {POWDERBLUE, POWDERBLUE, SADDLEBROWN, CHOCOLATE, CHOCOLATE, CHOCOLATE, POWDERBLUE, POWDERBLUE},
        {POWDERBLUE, CHOCOLATE, CHOCOLATE, CHOCOLATE, CHOCOLATE, CHOCOLATE, SADDLEBROWN, POWDERBLUE},
        {CHOCOLATE, CHOCOLATE, CHOCOLATE, CHOCOLATE, SADDLEBROWN, CHOCOLATE, CHOCOLATE, CHOCOLATE},
        {SADDLEBROWN, CHOCOLATE, BLACK, CHOCOLATE, CHOCOLATE, BLACK, CHOCOLATE, CHOCOLATE},
        {CHOCOLATE, LIGHTPINK, CHOCOLATE, CHOCOLATE, CHOCOLATE, SADDLEBROWN, LIGHTPINK, CHOCOLATE,},
        {CHOCOLATE, SADDLEBROWN, CHOCOLATE, SADDLEBROWN, CHOCOLATE, CHOCOLATE, CHOCOLATE, SADDLEBROWN},
        {DIMGRAY, CHOCOLATE, CHOCOLATE, CHOCOLATE, CHOCOLATE, SADDLEBROWN, CHOCOLATE, DIMGRAY},
        {DIMGRAY, DIMGRAY, SADDLEBROWN, CHOCOLATE, CHOCOLATE, CHOCOLATE, DIMGRAY, DIMGRAY},
    };
    //cookie array for the cookies on the top of the screen
    int cookiearrayceiling[8][8] = {
        {DIMGRAY, DIMGRAY, SADDLEBROWN, CHOCOLATE, CHOCOLATE, CHOCOLATE, DIMGRAY, DIMGRAY},
        {DIMGRAY, CHOCOLATE, CHOCOLATE, CHOCOLATE, CHOCOLATE, CHOCOLATE, SADDLEBROWN, DIMGRAY},
        {CHOCOLATE, CHOCOLATE, CHOCOLATE, CHOCOLATE, SADDLEBROWN, CHOCOLATE, CHOCOLATE, CHOCOLATE},
        {SADDLEBROWN, CHOCOLATE, BLACK, CHOCOLATE, CHOCOLATE, BLACK, CHOCOLATE, CHOCOLATE},
        {CHOCOLATE, LIGHTPINK, CHOCOLATE, CHOCOLATE, CHOCOLATE, SADDLEBROWN, LIGHTPINK, CHOCOLATE,},
        {CHOCOLATE, SADDLEBROWN, CHOCOLATE, SADDLEBROWN, CHOCOLATE, CHOCOLATE, CHOCOLATE, SADDLEBROWN},
        {POWDERBLUE, CHOCOLATE, CHOCOLATE, CHOCOLATE, CHOCOLATE, SADDLEBROWN, CHOCOLATE, POWDERBLUE},
        {POWDERBLUE, POWDERBLUE, SADDLEBROWN, CHOCOLATE, CHOCOLATE, CHOCOLATE, POWDERBLUE, POWDERBLUE},
    };

    //printing out the cookie array multiple times across the screen 
    for(int j = 0; j < 40; j++)
    {
        for(int y = 0; y < 8; y++)
        {
            for(int x = 0; x < 8; x++)
            {
                LCD.SetFontColor(cookiearrayfloor[y][x]);
                LCD.DrawPixel(x + (j+1) * 8, y + 232);
            }
        }
    }

    for(int j = 0; j < 40; j++)
    {
        for(int y = 0; y < 8; y++)
        {
            for(int x = 0; x < 8; x++)
            {
                LCD.SetFontColor(cookiearrayceiling[y][x]);
                LCD.DrawPixel(x + (j+1) * 8, y);
            }
        }
    }
}
//angry cookie decorations for game scene
//Made by Grace Jiang
void angrycookie()
{
    //cookie array!
    //cookie array for the cookies at the bottom of the screen
    int cookiearrayfloor[8][8] = {
        {POWDERBLUE, POWDERBLUE, SADDLEBROWN, CHOCOLATE, CHOCOLATE, CHOCOLATE, POWDERBLUE, POWDERBLUE},
        {POWDERBLUE, CHOCOLATE, CHOCOLATE, CHOCOLATE, CHOCOLATE, CHOCOLATE, SADDLEBROWN, POWDERBLUE},
        {CHOCOLATE, CHOCOLATE, CHOCOLATE, CHOCOLATE, SADDLEBROWN, CHOCOLATE, CHOCOLATE, CHOCOLATE},
        {SADDLEBROWN, CHOCOLATE, RED, CHOCOLATE, CHOCOLATE, RED, CHOCOLATE, CHOCOLATE},
        {CHOCOLATE, CHOCOLATE, CHOCOLATE, CHOCOLATE, CHOCOLATE, SADDLEBROWN, CHOCOLATE, CHOCOLATE,},
        {CHOCOLATE, SADDLEBROWN, CHOCOLATE, SADDLEBROWN, CHOCOLATE, CHOCOLATE, CHOCOLATE, SADDLEBROWN},
        {DIMGRAY, CHOCOLATE, CHOCOLATE, CHOCOLATE, CHOCOLATE, SADDLEBROWN, CHOCOLATE, DIMGRAY},
        {DIMGRAY, DIMGRAY, SADDLEBROWN, CHOCOLATE, CHOCOLATE, CHOCOLATE, DIMGRAY, DIMGRAY},
    };
    //cookie array for the cookies on the top of the screen
    int cookiearrayceiling[8][8] = {
        {DIMGRAY, DIMGRAY, SADDLEBROWN, CHOCOLATE, CHOCOLATE, CHOCOLATE, DIMGRAY, DIMGRAY},
        {DIMGRAY, CHOCOLATE, CHOCOLATE, CHOCOLATE, CHOCOLATE, CHOCOLATE, SADDLEBROWN, DIMGRAY},
        {CHOCOLATE, CHOCOLATE, CHOCOLATE, CHOCOLATE, SADDLEBROWN, CHOCOLATE, CHOCOLATE, CHOCOLATE},
        {SADDLEBROWN, CHOCOLATE, RED, CHOCOLATE, CHOCOLATE, RED, CHOCOLATE, CHOCOLATE},
        {CHOCOLATE, CHOCOLATE, CHOCOLATE, CHOCOLATE, CHOCOLATE, SADDLEBROWN, CHOCOLATE, CHOCOLATE,},
        {CHOCOLATE, SADDLEBROWN, CHOCOLATE, SADDLEBROWN, CHOCOLATE, CHOCOLATE, CHOCOLATE, SADDLEBROWN},
        {POWDERBLUE, CHOCOLATE, CHOCOLATE, CHOCOLATE, CHOCOLATE, SADDLEBROWN, CHOCOLATE, POWDERBLUE},
        {POWDERBLUE, POWDERBLUE, SADDLEBROWN, CHOCOLATE, CHOCOLATE, CHOCOLATE, POWDERBLUE, POWDERBLUE},
    };
    //printing out the cookie array multiple times across the screen 
    for(int j = 0; j < 40; j++)
    {
        for(int y = 0; y < 8; y++)
        {
            for(int x = 0; x < 8; x++)
            {
                LCD.SetFontColor(cookiearrayfloor[y][x]);
                LCD.DrawPixel(x + (j+1) * 8, y + 232);
            }
        }
    }

    for(int j = 0; j < 40; j++)
    {
        for(int y = 0; y < 8; y++)
        {
            for(int x = 0; x < 8; x++)
            {
                LCD.SetFontColor(cookiearrayceiling[y][x]);
                LCD.DrawPixel(x + (j+1) * 8, y);
            }
        }
    }
}
//game scene
//Made by Grace Jiang
void game_look(int miss, int score)
{
    LCD.SetBackgroundColor(POWDERBLUE);
    LCD.Clear();
    //Dispays the Xs that shows the errors based on the number missed by the player
    if(miss == 0) //no errors
    {
        LCD.SetFontColor(DARKGRAY);
        LCD.WriteAt("X",282,14);
        LCD.WriteAt("X",294,14);
        LCD.WriteAt("X",306,14);
    }
    else if(miss == 1) //one error
    {
        LCD.SetFontColor(CRIMSON);
        LCD.WriteAt("X",282,14);
        LCD.SetFontColor(DARKGRAY);
        LCD.WriteAt("X",294,14);
        LCD.WriteAt("X",306,14);
    }
    else if(miss == 2) //two errors
    {
        LCD.SetFontColor(CRIMSON);
        LCD.WriteAt("X",282,14);
        LCD.WriteAt("X",294,14);
        LCD.SetFontColor(DARKGRAY);
        LCD.WriteAt("X",306,14);
    }
    else if(miss >= 3) //three errors
    {
        LCD.SetFontColor(CRIMSON);
        LCD.WriteAt("X",282,14);
        LCD.WriteAt("X",294,14);
        LCD.WriteAt("X",306,14);
    }

    //display the score format
    LCD.SetFontColor(DIMGRAY);
    LCD.WriteAt("Score:", 2, 14);
    LCD.WriteAt(score, 74, 14);
}
//game over scene
//Made by Grace Jiang
void game_over_scene()
{
    LCD.SetBackgroundColor(CRIMSON);
    LCD.Clear();

    LCD.SetFontColor(WHITE);
    LCD.WriteAt("GAME OVER",106,90);
    LCD.WriteAt("Play Again", 24, 122);
    LCD.DrawRectangle(23, 120, 123, 19);
    LCD.WriteAt("Main Menu", 182, 122);
    LCD.DrawRectangle(175, 120, 122, 19);
}

//function that adds new scores to the array
//Made by Grace Jiang
void score_sort(int *a, int elements, int newscore)
{
    for(int k = 0; k < elements; k++)
    {
        if(*(a += k) == 0) //if score = 0 (no score in that index yet), add newscore
        {
            *(a += k) = newscore;
            k = elements;
        }
    }
}
//choosing start = 1, stats = 2, instructions = 3, or credits = 4 (main menu = 0)
//Made by Grace jiang
int selection()
{
    pointer menucursor;
    if(LCD.Touch(&menucursor.x_mouse, &menucursor.y_mouse))
    {
        //detect start button
        if(menucursor.x_mouse >= 130 && menucursor.x_mouse <= 190 && menucursor.y_mouse <= 135 && menucursor.y_mouse >= 120)
        {
            return 1;
        }
        //detect statistics button
        else if(menucursor.x_mouse >= 101 && menucursor.x_mouse <= 221 && menucursor.y_mouse <= 155 && menucursor.y_mouse >= 140)
        {
            return 2;
        }
        //detect instructions button
        else if(menucursor.x_mouse >= 90 && menucursor.x_mouse <= 234 && menucursor.y_mouse <= 175 && menucursor.y_mouse >= 160)
        {
            return 3;
        }
        //detect credits button
        else if(menucursor.x_mouse >= 120 && menucursor.x_mouse <= 225 && menucursor.y_mouse <= 195 && menucursor.y_mouse >= 180)
        {
            return 4;
        }
    }
    return 0;
}

//difficulty selection (back - 0, easy - 1, hard - 2, no button pushed - 3)
//Made by Grace Jiang
int selection_dif()
{
    pointer menucursor;
    if(LCD.Touch(&menucursor.x_mouse, &menucursor.y_mouse))
    {
        //detect back button
        if(menucursor.x_mouse >= 1 && menucursor.x_mouse <= 49 && menucursor.y_mouse <= 239 && menucursor.y_mouse >= 224)
        {
            return 0;
        }
        //detect easy button
        else if(menucursor.x_mouse >= 90 && menucursor.x_mouse <= 144 && menucursor.y_mouse <= 129 && menucursor.y_mouse >= 110)
        {
            return 1;
        }
        //detect hard button
        else if(menucursor.x_mouse >= 178 && menucursor.x_mouse <= 232 && menucursor.y_mouse <= 129 && menucursor.y_mouse >= 110)
        {
            return 2;
        }
        //no button pushed
        else
        {
            return 3;
        }
    }
    else
    {
        return 3;
    }
    return 3;
}
//stats selection back button - 0, no button pressed - 1
//Made by Caitlin Chien 
int selection_stats()
{
    pointer menucursor;
    if(LCD.Touch(&menucursor.x_mouse, &menucursor.y_mouse))
    {
        //back button
        if(menucursor.x_mouse >= 1 && menucursor.x_mouse <= 49 && menucursor.y_mouse <= 239 && menucursor.y_mouse >= 224)
        {
            return 0;
        }
        //no button pressed
        else
        {
            return 1;
        }
    }
    else
    {
        return 1;
    }
    return 1;
}
//credits selection back button - 0, no button pressed - 1
//Made by Caitlin Chien 
int selection_credits()
{
    pointer menucursor;
    if(LCD.Touch(&menucursor.x_mouse, &menucursor.y_mouse))
    {
        //back button 
        if (menucursor.x_mouse >=1 && menucursor.x_mouse <= 49 && menucursor.y_mouse <= 239 && menucursor.y_mouse >= 224)
        {
            return 0;
        }
        //no button pressed
        else
        {
            return 1;
        }
    }
    else
    {
        return 1;
    }
    return 1;
 
}
//rules selection back button - 0, no button pressed - 1
//Made by Caitlin Chien 
int selection_rules()
{
    pointer menucursor;
    if(LCD.Touch(&menucursor.x_mouse, &menucursor.y_mouse))
    {
        //back button
        if(menucursor.x_mouse >= 1 && menucursor.x_mouse <= 49 && menucursor.y_mouse <= 239 && menucursor.y_mouse >= 224)
        {
            return 0;
        }
        //no button pressed
        else
        {
            return 1;
        }
    }
    else
    {
        return 1;
    }
    return 1;
}
//play again - 0, main menu - 1, not button - 2
//Made by Grace Jiang
int selection_game_over()
{
    pointer menucursor;
    if(LCD.Touch(&menucursor.x_mouse, &menucursor.y_mouse))
    {
        //detect Play Again button
        if(menucursor.x_mouse >= 23 && menucursor.x_mouse <= 139 && menucursor.y_mouse <= 139 && menucursor.y_mouse >= 120)
        {
            return 0;
        }
        //detect Main Menu button
        else if(menucursor.x_mouse >= 175 && menucursor.x_mouse <= 297 && menucursor.y_mouse <= 139 && menucursor.y_mouse >= 120)
        {
            return 1;
        }
        //no button pushed
        else
        {
            return 2;
        }
    }
    else
    {
        return 2;
    }
    return 2;
}

//circle constructor (input radius of cookie and hitbox multiplier)
//Made by Grace Jiang
circle::circle(int r, float h)
{
    for(int i = 0; i < 5; i++)
    {
        x[i] = rand() % 185 + 15; //randomly generate a cookie across the screen 
        vx[i] = rand() % 3 - 0.5; //randomly generate the x velocty
        y[i] = 230.; //cookies start at the same y coordinate
        state[i] = 0; //cookie states(hit or no hit) start in a no hit state
        type[i] = rand() % 2;
    }
    radius = r;
    time = 0.;
    vy = 17.; //y velocity
    a = -20.; //acceleration
    numcookies = rand() % 5 + 1; //randomly generate the number of cookies per round 1-5
    hitbox = h;
    score = 0;
}
//projectile function of the circle
//Made by Grace Jiang
int circle::projectile()
{
    //generate random number of cookies

    //call struct pointer
    pointer cursorposition;
    //projectile physics
    for(time = 0.0; time <= 2.5; time += 0.05) //time the cookie animation takes
    {
        if(!LCD.Touch(&cursorposition.x_mouse, &cursorposition.y_mouse)) //no click 
        {   
            if(y[0] <= 230.0 && y[0] >= 5.0) //if y is within certain projectile range
            {
                for(int i = 0; i < numcookies; i++) //loop through number of cookies on screen
                {
                    if(state[i] == 0 && type[i] == 0) //cookie not hit, display the plain cookie
                    {
                        erase(i); //erase previous cookie 
                        //calculate coordinates
                        coordinates(i);
                        //draw circle and erase circle
                        draw(i);
                    }
                    else if(state[i] == 1 && type[i] == 0) //cookie hit, display frosted cookie
                    {
                        erase(i);
                        coordinates(i);
                        //changes cookie appearnace if hit
                        hit(i); //frosted cookie look
                    }//end state check/change appearance
                    else if(state[i] == 0 && type[i] == 1) //bomb not hit, display black bomb
                    {
                        erase(i);
                        coordinates(i);
                        draw_bomb(i); //black bomb appearance
                    }
                    else if(state[i] == 1 && type[i] == 1)//bomb hit, display red bomb
                    {
                        erase(i);
                        coordinates(i);
                        hit_bomb(i); //red bomb appearance
                    }
                }//end number of cookie loops
            }//end time loop
            else //if y value of cookie is not within range
            {
                for(int j = 0; j < numcookies; j++)
                {
                    end(j); //display ending position of cookie
                    LCD.Update();
                }//end for loop
            }//end y position if else
        }//end touch if
        //checks if pointer is touching screen
        else if(LCD.Touch(&cursorposition.x_mouse, &cursorposition.y_mouse))
        {
            for(int i = 0; i < numcookies; i++)
            {
                //hitbox of cookie
                if(cursorposition.x_mouse >= x[i] - radius * hitbox && cursorposition.x_mouse <= x[i] + radius * hitbox && cursorposition.y_mouse >= y[i] - radius * hitbox && cursorposition.y_mouse <= y[i] + radius * hitbox)
                {

                    state[i] = 1; //change hiut state to true
                    
                }//end hitbox loop
            }//end cookie for loop
        }//end touch else if

        Sleep(0.02); //animation sleep
        
    }//end of time loop

    //increments number of cookies hit
    for(int j = 0; j < numcookies; j++)
        {
            if(state[j] == 1 && type[j] == 0)
            {
                score++; //increment how many cookies were hit
            }
        }

    return error(); //return number of cookies missed
    
}
//calculate the x and y coordinates of cookie projectile
//Made by Grace Jiang
void circle::coordinates(int i)
{
    x[i] += vx[i] * time;
    y[i] -= vy * time + 0.5 * a * pow(time,2);
}
//draw the standard look of the cookie before hit
//Made by Grace Jiang
void circle::draw(int i)
{
    LCD.SetFontColor(BISQUE);
    LCD.DrawCircle(x[i], y[i], radius);
    LCD.FillCircle(x[i], y[i], radius);
}
//draw frosted cookie look of hit cookie
//Made by Grace Jiang
void circle::hit(int i)
{   
    LCD.SetFontColor(BISQUE);
    LCD.DrawCircle(x[i], y[i], radius);
    LCD.FillCircle(x[i], y[i], radius);
    LCD.SetFontColor(HOTPINK);
    LCD.DrawCircle(x[i], y[i], radius - 1);
    LCD.FillCircle(x[i], y[i], radius - 1);
}
//erase cookie in previous position before drawing cookie in new position
//Made by Grace Jiang
void circle::erase(int i)
{
    LCD.SetFontColor(POWDERBLUE);
    LCD.DrawCircle(x[i] ,y[i] ,radius);
    LCD.FillCircle(x[i],y[i],radius);
    LCD.Update();
}
//draw bomb cookie
//Made by Grace Jiang
void circle::draw_bomb(int i)
{
    LCD.SetFontColor(BLACK);
    LCD.DrawCircle(x[i], y[i], radius);
    LCD.FillCircle(x[i], y[i], radius);
}
//draw bomb cookie if hit
//Made by Grace Jiang
void circle::hit_bomb(int i)
{
    LCD.SetFontColor(RED);
    LCD.DrawCircle(x[i], y[i], radius);
    LCD.FillCircle(x[i], y[i], radius);
}
//ending position of cookie projectile
//Made by Grace Jiang
void circle::end(int j)
{
    if(state[j] == 0) //if cookie was not hit by the end appearance
    {
        LCD.SetFontColor(BISQUE);
        LCD.DrawCircle(x[j], 230, radius);
        LCD.FillCircle(x[j], 230, radius);
        LCD.Update();
    }
    else if(state[j] == 1) //cookie hit appearance
    {
        LCD.SetFontColor(BISQUE);
        LCD.DrawCircle(x[j], 230, radius);
        LCD.FillCircle(x[j], 230, radius);
        LCD.SetFontColor(HOTPINK);
        LCD.DrawCircle(x[j], 230, radius - 1);
        LCD.FillCircle(x[j], 230, radius - 1);
        LCD.Update();
    }
    //border around game
    LCD.SetFontColor(DIMGRAY);
    LCD.DrawRectangle(0,0,320,8);
    LCD.FillRectangle(0,0,320,8);
    LCD.Update();
}
//return the number of cookies missed
//Made by Grace Jiang
int circle::error()
{
    int c = 0; //number of cookies missed

    for(int k = 0; k < numcookies; k++)
    {
        //if player missed a cookie
        if(state[k] == 0 && type[k] == 0)
        {
            c++;
        }
        //if player hit a bomb
        else if(state[k] == 1 && type[k] == 1)
        {
            c = 3;
        }
    }
    return c;
}
//return the number of cookies hit
//Made by Caitlin Chien 
int circle::output_score()
{
    return score;
}
//display the current score onto screen
//Made by Grace Jiang
void score_display(int score)
{
    LCD.SetFontColor(DIMGRAY);
    LCD.WriteAt(score, 74, 14);
}
//erase previous score so new score can be printed
//Made by Grace Jiang
void score_display_erase(int score)
{
    LCD.SetFontColor(POWDERBLUE);
    LCD.WriteAt(score, 74, 14);
}



