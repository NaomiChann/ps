# Repositório para a disciplina de Programação de Sistemas - UFPel - 2023/1

> **Normas de declaração e uso geral de código:**
<sub>A seguir encontra-se um trecho de código de multiplicação de variáveis a fim de demonstrar a prática de escrita que será implementada.</sub>


'''
#include<stdio.h>
#include<conio.h>

int multiplicaNum ( int N1, int N2 ) 
{
  int resultado;
  resultado = N1 * N2;
  return( resultado ) ;
}
'''
=========================
Funcão: multiplicaNum
-------------------------
Descrição: Uma função que recebe dois inteiros e retorna a multiplicação deles.
=========================
'''
int main( void )
{
  int V1, V2, resultado;
  printf( "Digite o primeiro valor:" );
  scanf( "%d", &V1 );
  printf( "Digite o segundo valor:" );
  scanf( "%d", &V2 );
  
  resultado = multiplica(V1,V2);
  
  printf( "Resultado = %d\n", resultado );
  getch();
  
  return 0;
}
'''
**Referências:**
- Nome de função com primeira letra da primeira palavra minúscula e primeiras letras das demais palavras maiúsculas.
-  recebimento de parâmetros separados por espaços. multiplicaNum ( int N1, int N2 ) ao invés de multiplicaNum(int N1,int N2).
-  Abre chaves em linha separada.
-  printf com argumentos separados por espaços. 
