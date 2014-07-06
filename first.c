#include <stdio.h>
/*****************************************
*            Compilers Subject 
*       Recursive Analysis Prediction
*        Análise Recursiva Preditiva
* ----------------------------------------
* Eduardo
* ----------------------------------------
* G = ( N, T, P, S )
* N = { S E T }
* T = { if then else id := v ~ ( ) }
* P = { S -> if E then S else S | id := E
*       E -> ~ id F | id F | ( E ) F
*       F -> v E | &
*     }
******************************************/

#define NUM_TESTES 4
char * palavra = NULL;
char * testes[] = { "id := id" , "id := ~ id" ,
                    "if id v id then id := id else if ~ id then id := ~ id else id := ( id )",
                    "if id then id := id else id := ( id )" };

void leToken(){
   while(strncmp(palavra," ",1)!=0) palavra++;
   palavra++;
   printf("PALAVRA: %s\n",palavra);
   return;
}

int F(){
   printf("Esta no F\n" );
   if(strncmp(palavra,"v",1)==0){
      leToken();
      if(E()) return 1;
      else return 0;
   }
   palavra--;
   return 1;
}

int E(){
   printf("Esta no E\n");
   if(strncmp(palavra,"~",1)==0){
      leToken();
      if(strncmp(palavra,"id",2)==0){
         leToken();
         if(F()) return 1;
      }
   }
   else if(strncmp(palavra,"(",1)==0){
      leToken();
      if(E()) {
         leToken();
         if(strncmp(palavra,")",1)==0) {
            leToken();
            if(F()) return 1;
         }
      }
   }
   else if(strncmp(palavra,"id",2)==0){
      leToken();
      if(F()) return 1;
   }
   return 0;
}

int S(){
   printf("Esta no S\n");
   if (strncmp(palavra,"if",2)==0){
      leToken();
      if(E()) {
         leToken();
         if(strncmp(palavra,"then",4)==0){
            leToken();
            if(S()){
               leToken();
               if(strncmp(palavra,"else",4)==0){
                  leToken();
                  if(S()) return 1;
               }
            }
         }
      }
   }
   else if(strncmp(palavra,"id",2)==0){   //id := v
      leToken();
      if(strncmp(palavra,":=",2)==0){
         leToken();
         if(E()) return 1;
      }
   }
   return 0;
}

main() {
   int i;
   for(i=0;i<NUM_TESTES;i++) {
      printf("Analisando palavra %d\n",i+1);
      palavra = testes[i];
      if (S()) printf("\t>> Reconhece <<\n");
      else printf("\t>> Nao reconhece <<\n");
      printf("Pressione ENTER para %s",( (i<(NUM_TESTES-1)) ?"analisar proxima palavra\n":"finalizar\n"));
      #ifdef WIN32
         getch();
      #else
         getchar();
      #endif
   }
   exit(0);
}
