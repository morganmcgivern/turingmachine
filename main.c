#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
        Morgan McGivern 2019
        this was done as a project for a class, I will not mention it for fear of copycat
        please email morg1147@gmail.com in case of need for how to use

        input adds two numbers!
        111B1111 -> 1111111

*/

//Doubly Linked List methods
struct node{
    char data;
    struct node* next;
    struct node* prev;
};

struct node* newNode(char ndata, struct node* nnext, struct node* nprev){
    struct node* n = (struct node*)malloc(sizeof(struct node));
    n->data = ndata;
    n->next = nnext;
    n->prev = nprev;
    return n;
}
//struct node* newerNode = newNode(data, next, prev);

void append(char data, struct node *head){
    if(head->next == NULL){
        head->next = newNode(data, NULL, head);
    }else{
        append(data, head->next);
    }
};

struct node* prepend(char data, struct node* head){
    struct node* nn = newNode(data, head, NULL);
    head->prev = nn;
    return nn;
};
//head = prepend('new char', 'old head');

struct node* getNode(int index, struct node* head){
    struct node* curr;
    curr = head;
    for(int i = 0; i<index; i++){
        curr = curr->next;
    }
    return curr;
}

//instructions
struct instruction{
    char write;
    char moveDir;
    int newState;
};

struct instruction* newInstruction(char nwrite, char nmove, int nnewState){
    struct instruction* n = (struct instruction*)malloc(sizeof(struct instruction));
    n->write = nwrite;
    n->moveDir = nmove;
    n->newState = nnewState;
    return n;
}

struct instruction** buildTable(int states){
    struct instruction** tab = malloc(sizeof(struct instruction*) * 255);
    for (int i = 0; i < 255; i++){
        tab[i] = malloc(sizeof(struct instruction) * states);
    }
    return tab;
};

int main() {
    printf("C Turing Machine\n");
    //B is 66
    struct node* head = newNode('B',NULL, NULL);

    char path[100];
    int stateNum;
    int startState;
    int haltingState;
    int state;
    char read;
    char write;
    char move;
    int newState;

    printf("Enter path or filename:");
    fgets(path, sizeof(path), stdin);
    sscanf(path, "%s", &path);

    //istrcpy(path, "input.txt");

    //file loading

    FILE *input;

    char tape[255];
    input = fopen(path, "r");
    fscanf(input, "%s", tape);
    fscanf(input, "%d", &stateNum);
    fscanf(input, "%d", &startState);
    fscanf(input, "%d", &haltingState);
    //printf("halting state: %d \n", haltingState);

    struct instruction** instructionTable = buildTable(stateNum);

    //State, ReadVal, WriteVal, MoveDirection, NewState
    while(fscanf(input, "%d %c %c %c %d \n" , &state, &read, &write, &move, &newState) != EOF)
    {
        instructionTable[(int)read][state] = *newInstruction(write, move, newState);
    }
    fclose(input);
    printf("Regular Tape: %s \n", tape);

    int inputLen = strlen(tape);

    for(int i = 0; i<inputLen; i++){
        append(tape[i], head);
    }

    printf("DLL Input Tape: ");
    for(int i = 0; i<=inputLen; i++){
        printf("%c", getNode(i, head)->data);
    }

    printf("\n");

    //Turing Machine

    state = startState;
    struct node *pointer = head->next;

    while(state != haltingState){
        //See it in progress
//        printf("State: %d \n", state);
//        printf("DLL Progress Tape: ");
//        struct node* curr = head;
//        while(curr != NULL){
//            printf("%c", curr->data);
//            curr = curr->next;
//        }
//        printf("\n");

        if(instructionTable[(int)pointer->data][state].moveDir == 'R'){
            if(pointer->next == NULL)
                append('B', head);

            newState = instructionTable[(int)pointer->data][state].newState;
            pointer->data = instructionTable[(int)pointer->data][state].write;
            pointer = pointer->next;
        }
        else if(instructionTable[(int)pointer->data][state].moveDir == 'L'){
            if(pointer->prev == NULL)
                head = prepend('B', head);

            newState = instructionTable[(int)pointer->data][state].newState;
            pointer->data = instructionTable[(int)pointer->data][state].write;
            pointer = pointer->prev;
        }

        state = newState;
    }

    printf("DLL Output Tape: ");
    struct node* curr = head;
    while(curr != NULL){
        printf("%c", curr->data);
        curr = curr->next;
    }

    return 0;
}