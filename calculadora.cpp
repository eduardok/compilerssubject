#include <iostream>
#include <string>

/*******************************************
*            Compilers Subject
*  Análise Recursiva Preditiva (Top Down)
* Recursive Analysis Prediction (Top Down)
*           === Calculator ===
* ------------------------------------------
* Criado em: 30/MAIO/2006
* Ult. Alt.: 15/JUNHO/2006
* Compilar com: g++ calculadora.cpp -o calc
* ------------------------------------------
* Academico: Eduardo
* ------------------------------------------
* G = ( N, T, P, E )
* N = { E E' T T' F}
* T = { + - numero / * ( ) }
* P = { E  -> T {E'.h := T.val } E' { E.val := E'.s },
*       E' -> + T {E'1.h := E'.h + T.val } E'1 { E'.s := E'1.s }
*       E' -> - T {E'1.h := E'.h - T.val } E'1 { E'.s := E'1.s }
*       E' -> & {E'.s := E'.h}
*       T  -> F {T'.h := F.val } T' { T.val := T'.s }
*       T' -> * F {T'1.h := T'.h * F.val} T'1 {T'.s := T'1.s}
*       T' -> / F {T'1.h := T'.h / F.val} T'1 {T'.s := T'1.s}
*       T' -> & {T'.s := T'.h}
*       F  -> ( E ) {F.val := E.val}
*       F  -> digito {F.val := digito.val}
*     }
*******************************************/

int Tl(double &Tlh, double &Tls);
int El(double &Elh, double &Els);
int E(double &Eval);      //prototypes
int F(double &Fval);
int T(double &Tval);
int ultl = 0; //guarda proximo caractere a ler

std::string token, palavra;
std::string tokens[] = { "(",")","/","*","+","-","0","1",
                         "2","3","4","5","6","7","8","9" };
std::string testes[] = { "( + 84 )","( 554 / 85 ) + 5 - 4","5-85-941+2", 
                         "((5+8)*8)", "55 55 + 8", "5555 + 8",
                         "(5 * * 5)", "55 + ( 9 * (8450) )" };
#define NUM_TESTES 8

/**
* Verifica se o parametro eh um token
*/
bool isToken(std::string txt) {
   for(int a=0;a<16;a++) 
      if(txt==tokens[a]) return true;
   return false;
}

/**
* Verifica se o parametro eh numero (prototype 1)
*/
bool isNumber(int tt) {
   if( !((tt>=48) && (tt<=57)) ) return false;
   return true;
}

/**
* Verifica se o parametro eh numero (prototype 2)
*/
bool isNumber(std::string tt) {
   for(int a=0;a<tt.size();a++) 
      if(!isNumber(tt[a])) return false;
   return true;
}

/**
* Le proximo token
*/
std::string leToken() {
   token = "";
   while(ultl<palavra.size()) {
      if(palavra[ultl]==' ') {
         ultl++;
         continue;
      }
      
      token += palavra[ultl];
      ultl++;
      if(isToken(token)) { //verifica se ja fechou 1 token
         if(isNumber(token)) {  //mas se for um numero
            //se tiver mais digitos, le
            while( (ultl<palavra.size()) && isNumber(palavra[ultl]) ) {
               token += palavra[ultl];
               ultl++;
            }
         }
         break;
      }
   }
   std::cout << "  NOVO TOKEN: [" << token << "]" << std::endl;
   return token;
}

int Tl(double &Tlh, double &Tls) {
   double Fval,Tl1h,Tl1s = 0;

   std::cout << "\tEsta no T linha com palavra " << token << std::endl;
   if(token=="*") {
      token = leToken();
      if(F(Fval)) {
         Tl1h = Tlh * Fval;
         token = leToken();
         if(Tl(Tl1h,Tl1s)) {
             Tls = Tl1s;
             return 1;
         }    
         else return 0;
      }
      else return 0;
   }
   else if(token=="/") {
      token = leToken();
      if(F(Fval)) {
         Tl1h = Tlh / Fval;
         token = leToken();
         if(Tl(Tl1h,Tl1s)) {
             Tls = Tl1s;
             return 1;
         }    
         else return 0;
      }
      else return 0;
   }   
   Tls = Tlh;
   return 1;
}

int El(double &Elh, double &Els) {
   double Tval,El1h,El1s = 0;

   std::cout << "\tEsta no E linha com palavra " << token << std::endl;
   if(token=="+") {
      token = leToken();
      if(T(Tval)) {
         El1h = Elh + Tval;
         if(El(El1h,El1s)) {
             Els = El1s;
             return 1;
         }
         else return 0;
      }
      else return 0;
   }
   else if(token=="-") {
      token = leToken();
      if(T(Tval)) {
         El1h = Elh - Tval;
         if(El(El1h,El1s)) {
            Els = El1s;
            return 1;
         }    
         else return 0;
      }
      else return 0;
   }
   Els = Elh;
   return 1;
}

int F(double &Fval) {
   double Eval,digval = 0;
   std::cout << "\tEsta no F com palavra " << token << std::endl;
   if(token=="(") {
      token = leToken();
      if(E(Eval)) {
         if(token==")") {
             Fval = Eval;
             return 1;
         }    
      }    
   }
   else if(isNumber(token)) {
      const char * t = token.c_str();
      digval = atof(t);
      Fval = digval;
      return 1;
   }   
   return 0;
}

int T(double &Tval) {
   double Tls,Tlh,Fval = 0;

   std::cout << "\tEsta no T com palavra " << token << std::endl;
   if(F(Fval)) {
      Tlh = Fval;
      token = leToken();
      if(Tl(Tlh,Tls)) {
         Tval = Tls;
         return 1;
      }    
   }
   return 0;
}

int E(double &Eval) {
   double Tval,Elh,Els = 0;
   
   std::cout << "\tEsta no E com palavra " << token << std::endl;
   if(T(Tval)) {
      Elh = Tval;
      if(El(Elh,Els)) {
          Eval = Els;
          return 1;
      }    
   }   
   return 0;
}

void analisar(std::string word) {
      double Eval = 0;
      palavra = word;
      std::cout << "Analisando palavra: " << palavra << std::endl;
      token = leToken();
      if (E(Eval) && token.empty()) {
         std::cout << "\t>>> OK! Expressao matematica correta <<<\n";
         std::cout << "\tResultado: " << Eval << std::endl;
      }    
      else 
         std::cout << "\t>>> ERRO! Expressao matematica incorreta <<<\n";
}

int main() {
   for(int i=0;i<NUM_TESTES;i++) {
      ultl = 0;
      std::cout << "\n-----------------------------------------------------\n";
      analisar(testes[i]);
      if(i<(NUM_TESTES-1)) {
         std::cout << "\nPressione ENTER para analisar proxima palavra\n";
         getchar();
      }
   }
   
   while(true) {
      std::cout << "\n\n-----------------------------------------------------\n";
      std::cout << "Finalizados testes padrao, digite nova frase ou\n";
      std::cout << "Pressione ENTER para SAIR\n\t";
      getline(std::cin,palavra);
      if(palavra.empty()) break;
      ultl = 0;
      analisar(palavra);
   }   
   return 0;
}
