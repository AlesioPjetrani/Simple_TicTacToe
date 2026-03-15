#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

//-------------------------------------------- COLOR SECITON --------------------------------------------

#define LIGHT_YELLOW_OPEN "\033[93m"
#define LIGHT_YELLOW_CLOSE "\033[0m"

#define LIGHT_BLUE_OPEN "\033[1;96m"
#define LIGHT_BLUE_CLOSE "\033[0m"

#define RED_COLOR_OPEN "\033[1;91m"
#define RED_COLOR_CLOSE "\033[0m"

//------------------------------------------------------------------------------------------------------

// #define Texts
#define ENTER_PLAYER_1 "Enter Player 1 Name: "
#define ENTER_PLAYER_2 "Enter Player 2 Name: "
#define COMMAND_INPUT "Type 'start' to begin the game, 'quit' to exit, help for instructions: "

#define CHAR_TO_INT '0'

typedef enum
{
    FAIL_ALLOC = -1,
    SUCCESS = 0,
    START = 1,
    HELP = 2,
    QUIT = 3
} ErrorCode;

typedef struct _Player {
    int playerID;
    char *name;
    int has_turn;
    int moves;
    bool won;
} Player;







int checkWin(char board[][3], char symbol)
{
    // printf("Board state:\n");
    // printf("Pos 1: %c | Pos 2: %c | Pos 3: %c\n", board[0][0], board[0][1], board[0][2]);
    // Check rows
    for (int i = 0; i < 3; i++)
        if (board[i][0] == symbol && board[i][1] == symbol && board[i][2] == symbol)
            return 1;

    // Check columns
    for (int j = 0; j < 3; j++)
        if (board[0][j] == symbol && board[1][j] == symbol && board[2][j] == symbol)
            return 1;

    // Check diagonals
    if (board[0][0] == symbol && board[1][1] == symbol && board[2][2] == symbol)
        return 1;
    if (board[0][2] == symbol && board[1][1] == symbol && board[2][0] == symbol)
        return 1;

    return 0;

}

void print_logo(void)
{
    printf("%s", LIGHT_YELLOW_OPEN);
    printf("d888888P dP  a88888b.    d888888P  .d888888   a88888b.    d888888P  .88888.   88888888b \n");
    printf("   88    88 d8'   `88       88    d8'    88  d8'   `88       88    d8'   `8b  88        \n");
    printf("   88    88 88              88    88aaaaa88a 88              88    88     88 a88aaaa    \n");
    printf("   88    88 88              88    88     88  88              88    88     88  88        \n");
    printf("   88    88 Y8.   .88       88    88     88  Y8.   .88       88    Y8.   .8P  88        \n");
    printf("   dP    dP  Y88888P'       dP    88     88   Y88888P'       dP     `8888P'   88888888P \n");
    printf("oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo\n");
    printf("                                                                                        \n");
    printf("%s\n\n", LIGHT_YELLOW_CLOSE);
}






// Read arbitrary input from user, dynamically resizing the buffer as needed
char *readLine(void)
{
    size_t size = 64;
    size_t len = 0;
    char *buffer = malloc(size);
    int c;

    if (!buffer)
        return (char *)FAIL_ALLOC;
    while ((c = getchar()) != EOF)
    {
        if (c == '\n')
        {
            buffer[len] = '\0';
            break;
        }
        buffer[len++] = c;

        if (len >= size)
        {
            size *= 2;
            char *newbuf = realloc(buffer, size);
            if (!newbuf)
            {
                free(buffer);
                return (char *)FAIL_ALLOC;
            }
            buffer = newbuf;
        }
    }

    char *newbuf = realloc(buffer, len + 1);

    if (!newbuf)
    {
        printf("[ERROR] Memory allocation failed!\n");
        free(buffer);
        return (char *)FAIL_ALLOC;
    }
    buffer = newbuf;
    buffer[len] = '\0';

    return buffer;
}






void printX(void){
    printf("%s X %s", RED_COLOR_OPEN, RED_COLOR_CLOSE);
}

void printO(void){
    printf("%s O %s", LIGHT_BLUE_OPEN, LIGHT_BLUE_CLOSE);
}

void printfBoard(char board[][3]){
    
    for(int x = 0; x < 3; x++){
        for(int y = 0; y < 3; y++){
            
            if(board[x][y] == 'X') printX();
            else if(board[x][y] == 'O') printO();
            else printf(" %c ", board[x][y]);
            
            if(y < 2) printf("|");
        }
        printf("\n");
        if(x < 2) printf("---+---+---\n");
    }

    printf("\n");
}




void print_help(void)
{
    print_logo();
    printf("\n");
    printf(" 1 | 2 | 3 \n");
    printf("---+---+---\n");
    printf(" 4 | 5 | 6 \n");
    printf("---+---+---\n");
    printf(" 7 | 8 | 9 \n");
    printf("\n");
    printf("Command rules:\n");
    printf("  start - Start a new game\n");
    printf("  quit  - Exit the game\n");
    printf("  help  - Show this help message\n");
    printf("\nDuring the game, players will take turns entering their moves.\n");
    printf("To make a move, enter the numbers 1 - 9 as shown on the board above\n");
    printf("The first player to get three in a row wins!\n");
    printf("\n");
}




int choose_command(char board[][3])
{
    while(true){
        printf("%s", COMMAND_INPUT);
        char *command = readLine();
        if (!command)
            return FAIL_ALLOC;

        if (strcmp(command, "start") == 0)
        {
            system("clear");
            free(command);
            return START;
        }
        else if (strcmp(command, "quit") == 0)
        {
            free(command);
            return QUIT;
        }
        else if (strcmp(command, "help") == 0)
        {
            system("clear");
            print_help();
            free(command);
            continue;
        }
        else
        {
            system("clear");
            print_logo();
            printf("%s[ERROR] Invalid command. Please try again.%s\n", RED_COLOR_OPEN, RED_COLOR_CLOSE);
            free(command);
            continue;
        }

        
    }

    return SUCCESS;
}








//----------------------------------------------------------------------------------------------------------------------
/// @brief Calculates the power of a base raised to an exponent (integer only)
///
/// @param base The base integer
/// @param exp The exponent integer
///
/// @return The result of base raised to the power of exp
///
int power(int base, int exp)
{
    int result = 1;
    for (int i = 0; i < exp; i++)
        result *= base;

    return result;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Converts a string representing a positive integer to its int equivalent
/// It multiplies the digit value in the position it is found by 10 raised to the power of its position from the right
/// Example: "123" -> 1*10^2 + 2*10^1 + 3*10^0 = 100 + 20 + 3 = 123
///
/// @param str The string to convert
///
/// @return The int equivalent of the string, or NOT_INT if the string is not a valid positive integer
///
int toInt(char *str)
{
    int return_value = 0;
    int length = strlen(str);
    for (int number_index = 0; str[number_index] != '\0'; number_index++)
        return_value += (power(10, (length - number_index - 1))) * (str[number_index] - CHAR_TO_INT);

    return return_value;
}





int player_input(Player *player, char board[][3], char symbol)
{
    while(true){
        printf("Player move count is: %d\n", player->moves);
        printf("%s%s, enter your move (1-9): %s", symbol == 'X' ? RED_COLOR_OPEN : LIGHT_BLUE_OPEN, player->name, symbol == 'X' ? RED_COLOR_CLOSE : LIGHT_BLUE_CLOSE);
        char *input = readLine();
        int number;
        if (!input)
            return FAIL_ALLOC;

        if (strlen(input) != 1)
        {
            printf("\n %s[ERROR]%s Invalid move. Please enter a number between 1 and 9.\n", RED_COLOR_OPEN, RED_COLOR_CLOSE);
            continue;
        }

        
        if(input[0] < '1' || input[0] > '9'){
            printf("\n %s[ERROR]%s Invalid move. Please enter a number between 1 and 9.\n", RED_COLOR_OPEN, RED_COLOR_CLOSE);
            free(input);
            continue;
        }

        number = toInt(input);
        
        if(number >= 1 && number <= 3)
        {
            if(board[0][number - 1] != ' '){
                printf("\n %s[ERROR]%s That spot is already taken. Please choose another one.\n", RED_COLOR_OPEN, RED_COLOR_CLOSE);
                free(input);
                continue;
            }
            board[0][number - 1] = symbol;
            free(input);
        }
        else if(number >= 4 && number <= 6)
        {
            if(board[1][(number - 1) % 3] != ' '){
                printf("\n %s[ERROR]%s That spot is already taken. Please choose another one.\n", RED_COLOR_OPEN, RED_COLOR_CLOSE);
                free(input);
                continue;
            }
            board[1][(number -1 ) % 3] = symbol;
            free(input);
        }
        else
        {
            if(board[2][(number - 1) % 3] != ' '){
                printf("\n %s[ERROR]%s That spot is already taken. Please choose another one.\n", RED_COLOR_OPEN, RED_COLOR_CLOSE);
                free(input);
                continue;
            }
            board[2][(number - 1) % 3] = symbol;
            free(input);
        }

        player->moves++;

        if (checkWin(board, 'X') != 0)
        {
            system("clear");
            print_logo();
            printf("\n");
            printfBoard(board);
            printf("%sCongratulations!\nPlayer 1 wins!%s\n\n", RED_COLOR_OPEN, RED_COLOR_CLOSE);
            return 1;
        }
        else if (checkWin(board, 'O') != 0)
        {
            system("clear");
            print_logo();
            printf("\n");
            printfBoard(board);
            printf("%sCongratulations!\nPlayer 2 wins!%s\n\n", LIGHT_BLUE_OPEN, LIGHT_BLUE_CLOSE);
            return 2;
        }


        return 0;
    }
}


int game_loop(Player *p1, Player *p2, char board[][3])
{
    print_logo();
    printf("\n");
    int winner;
    int return_value = choose_command(board);
    if (return_value == QUIT || return_value == FAIL_ALLOC)
        return return_value;

    system("clear");
    print_logo();
    printf("\n");
    printfBoard(board);
    while(true){
            winner = player_input(p1, board, 'X');
            if (winner != 0) {
                return SUCCESS;
            }
            system("clear");
            print_logo();
            printf("\n");
            printfBoard(board);
            // Check for win condition or draw here

            // printf("Sum of moves is: %d\n", p1->moves + p2->moves);

            if (p1->moves + p2->moves == 9)
            {
                printf("%sCongratulations LOL!\n It's a draw!%s\n\n", LIGHT_YELLOW_OPEN, LIGHT_YELLOW_CLOSE);
                return SUCCESS;
            }

            winner = player_input(p2, board, 'O');
            if (winner != 0) {
                return SUCCESS;
            }
            system("clear");
            print_logo();
            printf("\n");
            printfBoard(board);
    }
    // Placeholder for the main game loop
    // This is where the game logic will be implemented
    return SUCCESS;
}





int main(void) {
    print_logo();

    char board[] = {
        ' ', ' ', ' ',
        ' ', ' ', ' ',
        ' ', ' ', ' '
    };

    // printf("\033[2J\033[H"); // Clears screen and moves cursor to home

    printf("%s  X: %s%s", RED_COLOR_OPEN, RED_COLOR_CLOSE, ENTER_PLAYER_1);
    Player p1 = {1, readLine(), 0, 0, false};
    if (p1.name == (char *)FAIL_ALLOC)
        return FAIL_ALLOC;

    printf("%s  O: %s%s",LIGHT_BLUE_OPEN, LIGHT_BLUE_CLOSE, ENTER_PLAYER_2);
    Player p2 = {2, readLine(), 0, 0, false};
    if (p2.name == (char *)FAIL_ALLOC)
        return FAIL_ALLOC;


    system("clear");
    int result = game_loop(&p1, &p2, board);
    if(result != SUCCESS){
        free(p1.name);
        free(p2.name);
        return result;
    }


    free(p1.name);
    free(p2.name);
    return EXIT_SUCCESS;
}
