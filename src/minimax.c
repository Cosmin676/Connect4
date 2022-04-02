#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct trie {
	int playerName;
	char matrix[3][3];
	int depth;
    struct trie *kids[9];
} Trie;

void expand(Trie *root);
Trie *init(int player, char matrix[3][3]);

int checkWin(char matrix[3][3], int player);
int checkHorizontal1(char matrix[3][3], int player);
int checkHorizontal2(char matrix[3][3], int player);
int checkRow(char matrix[3][3], int player);
int checkCol(char matrix[3][3], int player);
void print(Trie *root, FILE *output);
void recursive(Trie *root, FILE *output);
void freeTree(Trie *root);
void do_task1(FILE *input, FILE *output);
void do_task2(FILE *input, FILE *output);
void do_task3(FILE *input, FILE *output);

int main(int argc, char **argv) {
    if(argc < 4) {
        printf("Too few arguments!\n");
        return 0;
    } else {
        FILE *input = fopen(argv[2], "r");
        FILE *output = fopen(argv[3], "w");
        if(output == NULL || input == NULL) {
            printf("Eroare la deschidere fisier input/ouput");
            return 0;
        }

        if(strcmp(argv[1], "-c1") == 0)
            do_task1(input, output);
        if(strcmp(argv[1], "-c2") == 0)
            do_task2(input, output);
        if(strcmp(argv[1], "-c3") == 0)
            do_task3(input, output);
        fclose(input);
        fclose(output);
    }
    return 0;
}

void do_task1(FILE *input, FILE *output) {
    char player;
    char matrix[3][3];

    fscanf(input, "%c", &player);
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            fscanf(input, "%s", &matrix[i][j]);
        }
    }

    Trie *root = init(player, matrix);
    root->depth = 0;
    expand(root);
    recursive(root, output);
    freeTree(root);

}

void do_task2(FILE *input, FILE *output) {

}

void do_task3(FILE *input, FILE *output) {

}

void recursive(Trie *root, FILE *output) {
    if(root) {
        print(root, output);
        for(int i = 0; root->kids[i] != NULL; i++) {
            recursive(root->kids[i], output);
        }
    }
}

Trie *init(int player, char matrix[3][3]) {
    Trie *new = malloc(sizeof(struct trie));

    for(int i = 0; i <3; i++) {
        for(int j = 0; j <3; j++)
            new->matrix[i][j] = matrix[i][j];
    }

    new->playerName = player;
    
    for(int i = 0; i < 9; i++)
        new->kids[i] = NULL;
    return new;
}

void expand(Trie *root) {
    int count = 0;
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {

            if(root->matrix[i][j] == '-') {                
                    root->kids[count] = init(root->playerName, root->matrix);
                    Trie *aux = root->kids[count];

                    aux->depth = root->depth + 1;

                    aux->matrix[i][j] = root->playerName;
                    if(root->playerName == 'X') {
                        aux->playerName = 'O';
                    }
                    if(root->playerName == 'O') {
                        aux->playerName = 'X';
                    }
                    if(checkWin(aux->matrix, root->playerName) != 1)
                        expand(aux);
                    count++;
            }
        }
    }
}

void print(Trie *root, FILE *output) {
    if(root == NULL)
        return;

    for(int i = 0;i < 3; i++) {
        for(int x = 0; x < root->depth; x++)
            fprintf(output, "\t");
        for(int j = 0; j < 2; j++) {
            fprintf(output, "%c ", root->matrix[i][j]);
        }
        fprintf(output, "%c", root->matrix[i][2]);
        fprintf(output, "\n");
    }
    fprintf(output, "\n");
}

int checkWin(char matrix[3][3], int player) {
    if(checkHorizontal1(matrix, player) == 1)
        return 1;
    if(checkHorizontal2(matrix, player) == 1)
        return 1;
    if(checkCol(matrix, player) == 1)
        return 1;
    if(checkRow(matrix, player) == 1)
        return 1;
    return 0;
}

int checkHorizontal1(char matrix[3][3], int player) {
    int win = 1;
    for(int i = 0; i < 3; i++) {
        if(matrix[i][i] != player) {
            win = 0;
        }
    }

    return win == 1 ? 1 : 0;
}

int checkHorizontal2(char matrix[3][3], int player) {
    int win = 1;

    for(int i = 0, j = 2; i < 3 || j >=0; i++, j--) {
        if(matrix[i][j] != player)
            win = 0;
    }

    return win == 1 ? 1 : 0;
}

int checkRow(char matrix[3][3], int player) {
    int win = 0;
    for(int i = 0; i < 3; i++) {
        if(matrix[i][0]==matrix[i][1] &&
            matrix[i][0]==matrix[i][2] &&
            matrix[i][0] == player) {
            win = 1;
        }
    }
    return win == 1 ? 1 : 0;
}

int checkCol(char matrix[3][3], int player) {

    int win = 0;
    for(int i = 0; i < 3; i++) {
        if(matrix[0][i]==matrix[1][i] &&
            matrix[0][i]==matrix[2][i] &&
            matrix[0][i] == player) {
            win = 1;
        }
    }
    return win == 1 ? 1 : 0;
}

void freeTree(Trie *root) {
    if(root == NULL)
        return;
    for(int i = 0; root->kids[i] != NULL; i++) {
        freeTree(root->kids[i]);
    }
    free(root);
    root = NULL;
}