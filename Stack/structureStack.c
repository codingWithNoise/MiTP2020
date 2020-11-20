#include <stdio.h>
#include <stdlib.h>

void** tab;
int lastindex;

void Push(void* ele);

void* Pop();

void Init();

void Clean();

struct car{
    int year;
    char* model;
};

int main() {
    Init();

    struct car eleCar = {.year = 2000, .model = "Roomster"};
    Push(&eleCar);
    printf("First push on stack: %d, %s\n", eleCar.year, eleCar.model);

    char eleStr = 'd';
    Push(&eleStr);
    printf("Second push on stack: %c\n", eleStr);

    int eleInt = 5;
    Push(&eleInt);
    printf("Third push on stack: %d\n", eleInt);

    void* pop1st = Pop();
    int* checkInt = (int*) pop1st;
    printf("First pop from stack: %d\n", *checkInt);

    void* pop2nd = Pop();
    char* checkStr = (char*) pop2nd;
    printf("Second pop from stack: %c\n", *checkStr);

    void* pop3rd = Pop();
    struct car* checkCar = (struct car*) pop3rd;
    printf("Third pop from stack: %d, %s\n", (*checkCar).year, (*checkCar).model);

    Clean();
    return 0;
}

void Push(void* ele) {
    if (lastindex < 31) {
        lastindex ++;
        tab[lastindex] = ele;
    }
}

void* Pop() {
    if (lastindex > -1) {
        return tab[lastindex --];
    }
    return '0';
}

void Init() {
    lastindex = -1;
    tab = malloc(sizeof(void*) * 32);
}

void Clean() {
    lastindex = -1;
    free(tab);
}

