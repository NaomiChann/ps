- O código (isto é, nomes de variáveis, funções, arquivos e comentários) deve ser escrito em inglês.

- Uso de indentação por tabs **equivalentes** a 4 espaços.

- Os atributos devem ser protegidos por padrão, a não ser que a classe seja uma folha da herança, onde deve-se, quando possível, usar o tipo privado. Estas delimitações poderão ser alteradas em casos específicos avaliados pelo grupo.

- Sempre definir tipo, nem que seja “default”.

- Setters antes de getters.

- Manter abertura e fechamento de chaves na linha seguinte em todos os usos (if, else, funções, definições, etc.).
```
if ( x )
{
}
```
- Uso de else começa na mesma linha do último fechamento.
```
if ( x )
{
} else {
}
```
- Quaisquer expressões entre parênteses devem ser encapsuladas por espaços.
```
void Class ( arg1, arg2 )
{
} 
```
Ao invés de
```
void Class (arg1,arg2)
{
}
```

- Nomes de variáveis começam por letra minúscula.
```
float x;
```
- Em variáveis com nomes de múltiplas palavras a primeira palavra começa por letra minúscula e as palavras seguintes começam com letra maiúscula.
```
float thisIsAFloat;
```
- Usar especificação de precisão para ponto flutuante, exceto em necessidade explícita para um double.
```
float x = 0.5f;
```
Ao invés de
```
float x = 0.5;
```
E
```
float f = 1.0f;
```

Ao invés de
```
float f = 1.f;
```
- Nomes de função começam por letra maiúscula.
```
void Function ( void );
```
- Em funções com nomes de múltiplas palavras cada palavra começa por letra maiúscula.
```
void FunctionWithNoName ( void );
```
- Funções devem possuir uma descrição abaixo no seguinte padrão.
```
/*
===================
FunctionName
-------------------
Description
===================
*/
```
