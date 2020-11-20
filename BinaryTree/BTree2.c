#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

struct tnode {
  void * element; //wskaznik void, zeby wezel mogl przechowywac dane dowolnego typu
  int count;
  struct tnode * left;
  struct tnode * right;
};

//wskaznik na funkcje porownujaca elementy drzewa
struct tnode * tree(struct tnode *, void *, int (*compar)(const void *, const void *), int);
//wskaznik na funkcje wypisujaca wezel
void treeprint(struct tnode *, void (*printer)(const void *, FILE *), FILE *);
void treefree(struct tnode *);

//struktura, funkcje i zmienne potrzebne do przykladu z szeregiem czasowym
struct data {
  int on;
  double value;
};

double first = 0.0;  //zakladamy, ze wartosci szeregu czasowego sa rozne od 0.0
double second = 0.0;

struct data * read(FILE *);
int compare(const void *, const void *);
void printer(const void *, FILE *);

int main(){
  FILE *fin;
  FILE *fout;
  fin = fopen("BTree2test.txt", "r"); //plik testowy zawiera wymieszany fragment danych "dzien pazdiernika;kurs euro"
  fout = fopen("BTree2result.txt", "w");
  fprintf(fout,"Numer rekordu | wartosc oryginalna | wartosc wygladzona\n\n");

  struct tnode * root;
  int size = sizeof(struct data);
  struct data * ele;
  ele = read(fin);
  root = NULL;

  while(ele != NULL){
    root = tree(root, ele, compare, size);
    ele = read(fin);
  }

  treeprint(root, printer, fout);

  //zamkniecie plikow i zwolnienie alokowanej pamieci
  fclose (fin);
  fclose (fout);
  free(ele);
  treefree(root);
  free(root);

  return 0;
}

//funkcja wczytujaca elementy szeregu czasowego z pliku, dane sa zadane w formacie "numer rekordu;wartosc szeregu"
struct data * read(FILE * fp){
    struct data * ele;
    ele = malloc(sizeof(struct data));
    int result = fscanf(fp, "%d;%lf\n", &(ele->on), &(ele->value));

    if (result != -1) {
        return ele;
    } else {
        return NULL;
    }
};

//funkcja porownujaca elementy szeregu czasowego na podstawie numeru rekordu
int compare(const void * ele1, const void * ele2){
    struct data * ele1s = (struct data *) ele1;
    struct data * ele2s = (struct data *) ele2;
    int result = ele1s->on - ele2s->on;

    return ele1s->on - ele2s->on;
}

//funkcja wypisujaca elementy szeregu czasowego w postaci "Numer rekordu | wartosc oryginalna | wartosc wygladzona",
//gdzie wartosc wygladzona to srednia obecnej i dwoch poprzednich wartosci, dla drugiego elementu jest to srednia jego
//i pierwszej wartosci, dla pierwszego jest to po prostu jego wartosc
void printer(const void * param, FILE * fout) {
    double sum = 0.0;
    int counter = 0;
    double current = ((struct data *)((struct tnode *)param)->element)->value;
    sum += current;
    counter ++;
    if (second == 0.0 && first ==0.0) {
        first = current;
    } else {
        sum += first;
        counter ++;
        if (second != 0.0) {
            sum += second;
            first = second;
            counter ++;
        }
        second = current;
    }
    double result;
    result = sum/counter;

    fprintf(fout,"%2d | %lf | %lf\n", ((struct data *)((struct tnode *)param)->element)->on, current, result);
}

//tree dziala dla dowolnej struktury, porownuje wezly za pomoca zadanej wskaznikiem funkcji compar
struct tnode * tree(struct tnode * p, void * ele, int (*compar)(const void *, const void *), int size){
  int cond;
  if(p==NULL){
    p = (struct tnode *)malloc(sizeof(struct tnode));
    p->element = (void *)malloc(size);
    memcpy(p->element, ele, size);
    p->count = 1;
    p->left=p->right=NULL;
  } else{
    int comp = compar(ele,p->element);
    if(comp== 0)
      p->count++;
    else
    if (comp<0)
      p->left = tree(p->left, ele, compar, size);
    else
      p->right = tree(p->right, ele, compar, size);
  }

  return(p);
}

//treeprint dziala dla dowolnej struktury, zapisujac wyniki w podanym pliku za pomoca zadanej wskaznikiem funkcji printer
void treeprint(struct tnode * p, void (*printer)(const void *, FILE *), FILE * fout){
   if (p !=NULL){
     treeprint(p->left, printer, fout);
     printer(p, fout);
     treeprint(p->right, printer, fout);
   };
}

//treefree dziala dla dowolnej struktury, zwalnia alokowana pamiec w kazdym wezle
void treefree(struct tnode * p){
   if (p !=NULL){
     treefree(p->left);
     free(p->element);
     free(p->left);
     free(p->right);
     treefree(p->right);
   };
}
