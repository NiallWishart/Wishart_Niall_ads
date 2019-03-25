#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <stdbool.h>

bool gameWin(); //Sets up functions and variables for the squares positions
void display();
void clear();
char squares[9] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};

struct state //Initalise linked lists, one for current game data and one for previouse game storage
{
    char data[9];
    struct state *next;
    struct state *prev;
};
typedef struct state *node;

struct games
{
    int gameID;
    node gameStart;
    struct games *next;
};
typedef struct games *GameNode;

GameNode createGameNode(int ID, node head) //Node creation for overall game for replay
{
    GameNode start;
    start = (GameNode)malloc(sizeof(struct games));
    start->gameID = ID;
    start->gameStart = head;
    start->next = NULL;
    return start;
}

GameNode addGameNode(int ID, GameNode head, node start) //Adds the current game to the list when it is complete
{
    GameNode temp, p;
    temp = createGameNode(ID, start);
    if(head == NULL)
    {
        head = temp;
    }
    else
    {
        p = head;
        while(p->next != NULL)
        {
            p = p->next;
        }
        p->next = temp;
    }
    printf("Game saved succesfully, Game ID - %d\n", temp->gameID);
    getch();
    return temp;
}

node createNode() //Setup node for the current game data
{
    node start;
    start = (node)malloc(sizeof(struct state));
    for (int i = 0; i < 9; i++)
    {
        start->data[i] = squares[i];
    }
    start->next = NULL;
    start->prev = NULL;
    return start;
}

node addNode(node head) //Adds current node to current game data
{
    node temp, p;
    temp = createNode();
    if (head == NULL)
    {
        head = temp;
    }
    else
    {
        p = head;
        while (p->next != NULL)
        {
            p = p->next;
        }
        p->next = temp;
        temp->prev = p;
    }
    return temp;
}

void printList(node p) //Creates the board based for replay
{
    if(p == NULL)
    {
        printf("There is not a game stored\n");
        getch();
    }
    else
    {
        while (p->prev !=NULL)
        {
            p = p->prev;
        }
    }
    while(p != NULL)
    {
        for(int i = 0; i < 9; i++)
        {
            squares[i] = p->data[i];
        }
        display();
        p = p->next;
        Sleep(500);
    }
    getch();
}

void searchList(GameNode p, int searchID) //Searches through the pased games linked list to find ID given
{
    if(p == NULL)
    {
        printf("No stored games could be found\n");
        getch();
    }
    else
    {
        while(p->gameID != searchID && p->next != NULL)
        {
            p = p->next;
        }
        if(p->gameID == searchID)
        {
            node foundNode = p->gameStart;
            printList(foundNode); //Puts the data from the found game and puts it to display function
        }
        else
        {
            printf("The game selected was not found, please try another\n");
            getch();
        }
    }
}

node revertMove(node p)  //Goes to the previous node then puts the previous square data to current
{
    if(p==NULL || p->prev == NULL)
    {
        printf("No more actions can be reverted");
    }
    else
    {
        p = p->prev;
        for (int i = 0; i < 9; i++)
        {
            squares[i] = p->data[i];
        }
        display();
    }
    return p;
}

node redoMove(node p) //Goes to the next node then puts the next square data into current
{
    if (p == NULL || p->next == NULL)
    {
        printf("No more actions can be re-done");
    }
    else
    {
        p = p->next;
        for (int i = 0; i < 9; i++)
        {
            squares[i] = p->data[i];
        }
        display();
    }
    return p;
}

int main()
{
    GameNode Game = NULL; //Initalise the current game for the pased games linked list
    int gameID = 1;

    while(1)
    {
        system("CLS");
        printf("Welcome!\n 1 - Play game   2 - Print previous game    3 - Exit\n"); //Main menu
        int choice;
        scanf("%d", &choice);

        switch(choice) //Case statements for the menu input
        {
            case 1: //Play game
                int turncount, player; //Set variables for current game
                bool winner = false;
                turncount = 0;
                player = 1;

                node currentGame = createNode(); //Sets current game node
                node currentHead = NULL;
                clear();


                display(); //Initalise the blank display

                while (!winner)
                {

                    printf("\nSelect your square, or enter 0 for the option menue - Player %d \n", player); //Gets user input
                    bool validInput = false;
                    int select;
                    scanf("%d", &select);

                    while(!validInput) //Check if number entered is valid
                    {
                      int x = (int)squares[select-1] - 48; //Converts the squares array to int

                        if ((select >= 1 && select <= 9 && x >= 1 && x <=9) || select == 0) //Is a number between 9&1, not already filled on board
                        {
                            validInput = true;
                            break;
                        }
                        else
                        {
                            printf("Invalid selection, you chose: %d\n", select);
                            scanf("%d", &select);
                        }
                    }
                    if (validInput = true)
                    {

                        if (select != 0)
                        {
                            squares[select-1] = player==1 ? 'x' : 'o'; //Sets selected square to X or O depending on player, increase turncount
                            display();
                            turncount++;
                            currentGame = addNode(currentGame); //Updates linked list with latest move
                            if (currentHead == NULL)
                            {
                                currentHead = currentGame;
                            }
                            winner = gameWin(); //Checks if someone has won
                            if( winner == true) //Ends the game, adds game to passed game linked list, gives user ID
                            {
                                printf("\nPlayer %d wins the game!\n", player);
                                getch();
                                Game == NULL ? Game = addGameNode(gameID, Game, currentHead) : addGameNode(gameID, Game, currentHead);
                                gameID++;
                            }
                            if(turncount == 9 && winner == false) //If game exceeds 9 turns without a winner, game ends in draw
                            {
                                printf("\nThe game is a draw!\n");
                                getch();
                                winner = true;
                                Game == NULL ? Game = addGameNode(gameID, Game, currentHead) : addGameNode(gameID, Game, currentHead);
                                gameID++;
                            }
                        }
                        if (select != 0)
                        {
                            printf("Press 1 to undo your last move, 2 to redo, or 3 to exit\n"); //Redo/undo menu
                            scanf("%d", &select);
                            if(select == 1 || select == 2)
                            {
                                currentGame = select==1 ? revertMove(currentGame) : redoMove(currentGame); //Revert or redo depending on input
                                select == 1 ? turncount--: turncount++;
                            }
                            else
                            {
                                printf("Goodbye!\n"); //Ends game, saves into past games linked list, gives user ID
                                getch();
                                winner = true;
                                Game == NULL ? Game = addGameNode(gameID, Game, currentHead) : addGameNode(gameID, Game, currentHead);
                                gameID++;
                                break;
                            }

                        }
                        player==1 ? player++ : player--; //Changes player
                    }
                }
                break;

            case 2: //Replay
                int searchID;
                printf("Please enter a valid game ID\n"); //Gets search ID, then runs the search function
                scanf("%d", &searchID);
                searchList(Game, searchID);
                break;

            case 3: //Exit
                printf("Shutting down...\n");
                exit(0);
                break;

            default: //Validation
                printf("Invalid option, please try again\n");
                break;
        }
    }

    return 0;
}

void display() //Displays the current game board
{
    system("CLS");

        printf("     |     |     \n");
        printf("  %c  |  %c  |  %c \n", squares[0], squares[1], squares[2]);

        printf("_____|_____|_____\n");
        printf("     |     |     \n");

        printf("  %c  |  %c  |  %c \n", squares[3], squares[4], squares[5]);

        printf("_____|_____|_____\n");
        printf("     |     |     \n");

        printf("  %c  |  %c  |  %c \n", squares[6], squares[7], squares[8]);

        printf("     |     |     \n\n");
    }

bool gameWin() //Checks if a user wins
{
    for (int i = 0; i < 3; i++)
    {
        if ((squares[(i * 3)] == squares[(i * 3) + 1] && squares[(i*3)] == squares[(i * 3) + 2] && squares[(i * 3)] != '-')
        ||(squares[i] == squares[(i + 3)] && squares[i] == squares[(i + 6)] && squares[i] != '-'))
            return true;
    }
        if ((squares[0] == squares[4] && squares[4] == squares[8] && squares[0] != '-')
        || (squares[2] == squares[4] && squares[4] == squares[6] && squares[2] != '-'))
            return true;
    return false;
}

void clear() //Resets the board to numbers when a new
{
    for(int i = 0; i < 9; i++)
    {
      char squaresreset[9] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
      squares[i]=squaresreset[i];
    }

}
