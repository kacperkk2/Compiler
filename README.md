# Język skryptowy na potrzeby systemu rekomendacji
## Autor: Kacper Klimczuk

Język posłuży do przeprowadzenia testów algorytmów
zaimplementowanych podczas pracowni inżynierskiej. Pozwoli on tworzyć
obiekty wbudowane: User - użytkownik, Path – rekomendowana ścieżka,
Algorithm – stosowany algorytm.
User będzie tworzony poprzez podanie id, będzie on przechowywał
preferencje użytkownika, metody ustawiające te pola, oraz metodę
określającą w jakim stopniu ścieżka odpowiada danemu użytkownikowi.
Algorithm będzie tworzony przez podanie nazwy algorytmu (string), będzie
przechowywał parametry algorytmu oraz metody z nimi powiązane. Path –
rekomendowany obiekt, zwracany przez algorytm.



### Obiekty wbudowane
##### User:
Pola
int id
float grade
string country
Metody
float
rankRecomendation(Path) –
sprawdza na ile dobra jest
rekomendacja

##### Algorithm:
Pola
string name
int pearsonGroupSize
float regParameter
float lambda
Metody
Path
getRecomendation(User) –
dla danego użytkownika
zwraca rekomendacje w
oparciu o bieżące parametry
i algorytm

##### Path:
Pola
int id
string country
float difficulty
float length
Metody
Brak metod, nie
przewidzana jest żadna
akcja bezpośrednio na tym
obiekcie

### Opis składni w EBNF
PROGRAM = {FUNCTION}

FUNCTION = RET_TYPE ID "(" {TYPE ID} ")" "{" {STATEMENT} "}"

STATEMENT = CONDITION | PRINT | LOOP | ASSIGN | DEFINITION | RETURN
| FUNCTION_CALL

RETURN = "return" EXPRESION ";"

CONDITION = IF [ELSE]

IF = "if" "(" LOGIC ")" "{" {STATEMENT} "}"

ELSEIF = "elseif" "(" LOGIC ")" "{" {STATEMENT} "}"

ELSE = "else" "{" {STATEMENT} "}"

LOOP = "while" "(" LOGIC ")" "{" {STATEMENT} "}"

DEFINITION = TYPE ID ["=" EXPRESION | EMBEDDED_DECL] ";"

ASSIGN = ID "=" (EXPRESION | EMBEDDED_DECL) ";"

EXPRESION = LOGIC | ARITHMETIC | SIMPLY

SIMPLY = VALUE | ID | FUNCTION_CALL | ID “.” (ID | FUNCTION_CALL)

LOGIC = EXPRESION ( "||" | "&&" | "!=" | "==" | "<=" | "<" | ">" | ">=" )
EXPRESION | "~" EXPRESION

ARITHMETIC = EXPRESION ( "+" | "-" | "*" | "/") EXPRESION

FUNCTION_CALL = ID "(" {EXPRESION} ")" ";"

VALUE = STRING | NUMBER | BOOL

BOOL = "true" | "false"

EMBEDDED_DECL = EMBEDDED "(" VALUE ")"

NUMBER = ["-"] DIGIT {DIGIT}

DIGIT = "0" | .. | "9" (każda cyfra)

STRING = ‘ " ’ {CHAR} ‘ " ’

CHAR = "A" | .. | "Z" | "a" | .. | "z" (każda litera alfabetu)

ID = CHAR {CHAR | DIGIT}

RET_TYPE = TYPE | “noret”TYPE = PRIMITIVE | EMBEDDED

EMBEDDED = "User" | "Path" | "Algorithm"

PRIMITIVE = "int" | "float" | "string" | “bool”

PRINT = “print” “(“ ( ID | VALUE) “)” “;”


### Główne moduły
Źródło - przechowuje plik wejściowy, bieżący znak, bieżącą linię w pliku
oraz pozycję w linii.
Lekser - przechowuje obiekt źródła i korzystając z jego interfejsu pobiera
kolejne znaki składając je w tokeny.
Parser - przechowuje obiekt leksera, 2 bufory na tokeny oraz zbiory
tokenów "sety" służące do porównywania z bieżącym tokenem i w ten
sposób określania produkcji.
Generator kodu - korzysta z drzewa wygenerowanego przez Parser.
Przechowuje obiekt parsera, plik wyjściowy, wskaźnik na korzeń drzewa
Parsera oraz tablice symboli.


### Implementacja modułów
##### Źródło:
Gdy lexer wyda polecenie używając funkcji "load_next_ch()" źródło
przesuwa iterator na następny znak w linii, jeśli następny znak to \n, wtedy
źródło ładuje kolejną linię z pliku za pomocą "getline" ustawiając iterator
na pierwszym znaku.
Lexer może podejrzeć bieżący znak za pomocą "see_current_ch" i dopiero
jeśli konsumuje dany znak wywołuje funkcję źródła pobierającą kolejny
znak z pliku.
##### Lexer:
Główną funkcją jest "get_token()", która pozwala parserowi pobrać kolejny
token od lexera. Ta funkcja składa kolejne znaki otrzymywane od źródła
pomijając znaki białe a także komentarze. Gdy dane słowo zostaje złożone,
lexer sprawdza czy jest to keyword za pomocą "check_if_keyword" (gdy
składa operator - nie używa tej funkcji).
Gdy lexer napotka na znak/wyrażenie nie będące częścią języka używa
funkcji "error", która wypisuje bieżącą linię pliku na ekran oraz zaznacza
miejsce wystąpienia błędu.
##### Parser:
Funkcją początkującą budowę drzewa jest "PROGRAM()", następne funkcje
są wywoływane zależnie od tego jaką sekwencję tokenów zwróci Lexer.
Parser korzysta ze zbioru funkcji napisanych według opisu składni w EBNF,
każda funkcja odpowiada jednej produkcji z tego opisu.
Moduł budując drzewo tworzy węzły zdefiniowane w pliku "Nodes.h" i
wypełnia je danymi potrzebnymi do zbudowania kodu.
Funkcja "accept" przyjmuje jako argument oczekiwany token, a następnie
sprawdza czy buforowany token otrzymany od Lexera jest tym
oczekiwanym, jeśli tak - buforuje następny, ewentualnie wywołując funkcję
służącą do wypełnienia bieżącego węzła bieżącym tokenem. Jeśli
buforowany token nie jest tym oczekiwanym, funkcja korzysta z
"print_error_message", która wypisuje błąd i jego lokalizacje, a następnie
"accept" kończy wykonanie programu.
Jest to parser RD - Recursive Descent, który generuje wyprowadzenie
prawostronne.
##### Generator kodu:
Otrzymuje od Parsera wskazanie na pierwszy węzeł (root), zawsze jest to
węzeł Program_node. Ten węzeł przechowuje wskazania na poszczególne
funkcje, a te zaś wskazania na poszczególne wyrażenia językowe w swoim
obrębie.
Każda funkcja ma swoją odrębną tablice symboli, podobnie każdy blok w
obrębie funkcji, z tą różnicą ze bloki w danej funkcji mają dostęp do każdej
wyższej tablicy symboli.
Generator przechodząc po drzewie sprawdza: istnienie zmiennych (w
tablicach danego bloku oraz wyższych), istnienie funkcji, zgodność typów
zwracanych przez funkcje, typu zmiennej i wyrażenia przypisywanego do
zmiennej, liczbę argumentów podczas wywołania funkcji. Dla typów
wbudowanych sprawdzane są: zgodność typu wbudowanego ze zmienna,
do której typ jest przypisywany, istnienie pól typu wbudowanego w
zależności od typu zmiennej oraz typ każdego pola, a w przypadku funkcji
typ parametru.
Język umożliwia stosowanie skomplikowanych wyrażeń z użyciem
nawiasów i negacji dla wyrażeń logicznych, a w skład wyrażeń mogą
wchodzić bezpośrednio wartości jak i wywołania funkcji i zmienne.

### Testowanie
W ramach testów przygotowałem łącznie 20 testów z opisami i
spodziewanymi wynikami wraz z plikami wynikowymi (testy parsera nie
mają plików wynikowych, ponieważ drzewo wypisywane jest na konsolę po
uruchomieniu poszczególnych wariantów). Moduł źródła nie ma testów,
ponieważ jest to najprostrzy moduł który wyłącznie pobiera kolejne znaki z
pliku i błędne wyniki testów lexera zdradzały by błędy źródła.Testy dla poszczególnych modułów:
##### Lexer:
1) Właściwe i niepoprawne identyfikatory
2) Słowa kluczowe
3) Operatory

##### Parser:
1) Pusty plik
2) Plik bez zdefiniowanych funkcji
3) Plik składający się z pojedynczej funkcji z instrukcjami
4) Poprawny kod wykorzystujący znaczną część wszystkich możliwości
języka
5) Wyrażenie zawierające wiele zagnieżdżonych nawiasowań
6) Wyrażenie logiczne z negacją
7) Kod łamiący semantyke STATEMENT
8) Niepoprawne wykorzystanie operatora "."

##### Generator kodu:
1) Kompletny, poprawny kod
2) Kompletny, poprawny kod
3) Definicja zmiennej o tej samej nazwie w innych blokach funkcji
4) Odwołanie do nieistniejącego pola typu wbudowanego
5) Odwołanie do nieistniejącej zmiennej
6) Przypisanie do zmiennej wyniku funkcji o innym typie zwracanym niż
zmienna
7) Przypisanie stringa do zmiennej typu float
8) Funkcja o zadeklarowanym zwracanym typie string, zwrócenie typu int
9) Wywołanie funkcji z mniejszą liczbą parametrów

### Obiekty wejściowe
Program przyjmuje na wejściu plik tekstowy (.txt) podając nazwe po
uruchomieniu programu. Plik musi zawierać jedynie kod w tworzonym
języku (bez bibliotek) - funkcja main poprzedzona zbiorem funkcji.
Błędy napotkane na poziomie leksera i parsera zostaną wyświetlone na
konsoli wraz z komunikatem, numerem linii i numerem znaku w linii. Błędy
napotkane na poziomie generatora kodu zostaną dopisane w pliku
wejściowym - w miejscu wystąpienia błędu zostanie dopisany stosowny
komunikat.

### Obiekty wyjściowe
Program generuje na wyjściu plik źródłowy (.cpp) w języku C++ z
dołączonymi potrzebnymi bibliotekami: iostream - wypisywanie
komunikatów na standardowe wyjście, Embedded_types - zawiera
zdefiniowane obiekty wbudowane. Do projektu dołączony jest najprostrzy
makefile, który kompiluje plik wyjściowy z flagami: -g -Wall -std=c++11.

### Przykładowy kod pliku wejściowego
```
int ffunction(int value, int fvalue) {
  return 3;
}

float function2(int fvalue) {
  return 2.99;
}

noret main() {
  User u = User(1); @ komentarz @
  int value = 1;
  float fvalue = 2;
  Algorithm a = Algorithm("fast");
  a.lambda = 1.0;
  a.pearsonGroupSize = ffunction(value, fvalue) +
  function2(fvalue) + 4.8;
  float param = 0.5;
  while(param < 10.5) {
    param = param + 0.5;
    print("Parameter value: ");
    print(param);
    print("\n");
  }
  Path p = a.getRecomendation(u);
  float arank = u.rankRecomendation(p);
  if(arank > a.regParameter) {
    print("Algorithm typed path correctly");
  }
  else { 
    print("stringg string1"); 
  }
}
```

### Kod pliku wyjściowego w C++
```
#include <iostream>
#include "Embedded_types.h"

int ffunction(int value, int fvalue)
{
  return 3;
}

float function2(int fvalue)
{
  return 2.990000;
}

int main()
{
  User u = User(1);
  int value = 1;
  float fvalue = 2;
  Algorithm a = Algorithm("fast");
  a.lambda = 1.000000;
  a.pearsonGroupSize = ffunction(value, fvalue) + (function2(fvalue) + 4.800000);
  float param = 0.500000;
  while(param < 10.500000)
  {
    param = param + 0.500000;
    std::cout << "Parameter value: ";
    std::cout << param;
    std::cout << "\n";
  }
  Path p = a.getRecomendation(u);
  float arank = u.rankRecomendation(p);
  if(arank > a.regParameter)
  {
    std::cout << "Algorithm typed path correctly";
  }
  else
  {
    std::cout << "stringg string1";
  }
  return 1;
  }
```
