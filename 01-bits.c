/*
 * Primeiro laboratório de Computadores e Programação
 *
 * Assinatura:
 *      Aluno: Melissa Pereira Guarilha e Pedro Henrique Gonçalves da Silva Ferreira
 *      DRE: 118051503 e 118168562
 *      versão do GCC utilizada: 9.3.0
 *
 */

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#include <stdio.h>
#include <stdint.h>


/* Número é par ou não
 *      Permitido:
 *          Operações: ~ & ^ | << >>
 *
 *      Número máximo de operações: 3
 *      Monitor: 2
 *
 *      Retorna 1 se x é par, retorna 0 caso contrário
 *
 *      Exemplo:
 *          ehPar(0) -> 1
 *          ehPar(2) -> 1
 *          ehPar(7) -> 0
 */
int32_t ehPar(int32_t x) {
    /*
    * Como números pares representados em forma binária, sempre terminam com 0 e os ímpares terminam em 1,
    * se fizermos uma operação & bit-a-bit com o número binário 0001 (equivalente a 0x1), 
    * para todos os pares obteremos como resultado 0 e para todos os ímpares
    * teremos 1. Portanto como a questão pede o contrário, se fizermos um ^ 
    * para o resultado da operação anterior juntamente ao binário 0001 (equivalente a 0x1), 
    * obteremos o contrário da operação. 
    * Dessa forma, teremos o resultado final esperado, retornando 0 para números ímpares e 1 para números pares.
    */   
    return ((x & 0x1) ^ 0x1);
}

/*
 * Módulo 8
 *      Permitido:
 *          Operações: ~ & ^ | ! << >>
 *
 *      Número máximo de operações: 3
 *      Monitor: 1
 *
 *      Retorna x % 8
 *
 *      Exemplo:
 *          mod8(1) -> 1
 *          mod8(7) -> 7
 *          mod8(10) -> 2
 */
int32_t mod8(int32_t x) {
    /*
    * Primeiramente resolvi testar como funciona o resto da divisão por 8 de alguns números, 
    * na tentativa de achar um padrão. Fazendo isso, percebi que, começando do zero, 
    * os restos da divisão de um numero por 8, vão sempre de 0 até 7.
    * Ao fazer isso, percebi que os restos equivalem aos 3 LSB do número que está sendo dividido pelo 8,
    * Para pegar esses 3 LSB do número, eu faço a operação (x & 0x7) já que 7 em binário equivale a 0111.

    * Exemplo do que eu observei sobre os LSB:
    *    1 % 8 = 1 | 0001 % 1000 = 0001
    *    4 % 8 = 4 | 0100 % 1000 = 0100
    *    7 % 8 = 7 | 0111 % 1000 = 0111
    *    8 % 8 = 0 | 1000 % 1000 = 0000
    */
    return (x & 0x7);
}

/* Negativo sem -
 *      Permitido:
 *          Operações: ~ & ^ | ! << >> +
 *
 *      Número máximo de operações: 5
 *      Monitor: 2
 *
 *      Retorna -x
 *
 *      Exemplo:
 *          negativo(1) -> -1
 *          negativo(42) -> -42
 */
int32_t negativo(int32_t x) {
    /* 
    * Através do conhecimento de que, o complemento de um número é igual ao negativo dele subtraído de 1, 
    * ou seja (~x = (-x) - 1), organizando esta equação de lado temos então que, ~(x) + 1 = (-x), que significa
    * que o complemento de um número adicionado de 1 é igual ao seu negativo. 
    * Isto resolve esta questão portanto, já que ~x + 1 = negativo.
    */
    return (~x + 1);
}

/* Implementação do & usando bitwise
 *      Permitido:
 *          Operações: ~ ^ | ! << >>
 *
 *      Número máximo de operações: 7
 *      Monitor: 4
 *
 *      Retorna x & y
 *
 *      Exemplo:
 *          bitwiseAnd(1, 2) -> 0
 *              01 & 10 -> 00
 *          bitwiseAnd(3, 11) -> 3
 *              11 & 1011 -> 0011
 */
int32_t bitwiseAnd(int32_t x, int32_t y) {
    /*
    * Tomando como exemplo (1 & 2) = 0000.
    * Testando a operação de complemento a dois, percebi que se fizesse (~x | ~y), 
    * eu chegaria exatamente ao inverso do resultado esperado,
    *    (~1 | ~2) => (~0001 | ~0010) => (1110 | 1101) => 1111
    * ou seja, precisaria fazer mais um complemento do resultado anterior ~((~x) | (~y)).
    *    ~(~1 | ~2) => ~(1111) => 0000
    *
    * O uso desse exemplo pode ser comprovado também a partir de uma lei de DeMorgan, que diz que,
    * ~(a & b) = ~a | ~b , reorganizando o ~ temos que, (a & b) = ~(~a | ~b).
    */
    return ~((~x) | (~y));
}

/* Igual sem ==
 *      Permitido:
 *          Operações: ~ & ^ | << >> !
 *
 *      Número máximo de operações: 3
 *      Monitor: 2
 *
 *      Retorna 1 se x == y, 0 caso contrário
 *
 *      Exemplo:
 *          ehIgual(10, 10) -> 1
 *          ehIgual(16, 8) -> 0
 */
int32_t ehIgual(int32_t x, int32_t y) {
    /*
    * Na operação XOR (ou-exclusivo) temos que se um número for exatamente igual ao outro, o retorno é 0.
    * Uma demonstração é: 
    *   8 ^ 8 => 1000 ^ 1000 => 0000
    *   1 ^ 8 => 0001 ^ 1000 => 1111
    * 
    * Portanto, se nessa operação o resultado é sempre zero quando o número é comparado com ele mesmo, 
    * para resolver a questão precisamos exibir "1" para os números iguais, 
    * e como o XOR retorna apenas zero para números iguais, 
    * precisamos usar a operação ! no resultado anterior, para obter a solução correta.
    */
    return !(x ^ y);
}

/* Limpa bit n
 *      Permitido:
 *          Operações: ~ & ^ | ! << >>
 *
 *      Número máximo de operações: 4
 *      Monitor: 3
 *
 *      Retorna o x com o bit n = 0,
 *      n pode variar entre 0 e 31, do LSB ao MSB
 *
 *      Exemplo:
 *          limpaBitN(3, 0) -> 2
 *          limpaBitN(3, 1) -> 1
 */
int32_t limpaBitN(int32_t x, int8_t n) {
    /*
    * Para explicar, comecemos do AND, para se transformar um número em zero é necessário usar AND com zeros,
    * para isso bastaria utilizar (x & 0), porém o zero precisa estar somente na posição escolhida 
    * isto é, na posição n precisa ter um zero.
    * Para conseguir algo numa posição especifica, dada pelo usuário, precisamos usar o SHIFT,
    * a partir do enésimo bit efetuamos um shift pra esquerda, 
    * já que ele vai modificar os bits a partir do LSB, temos então x & (0 « n)
    * porém isto não funciona, já que o zero é 0000 e o shift neste resultado apenas transformaria tudo em zero,
    * e neste caso não adiantaria. No lugar do zero utilizaremos então o 1, 
    * deslocando ele em n posições para a esquerda, a partir do LSB. 
    * Com isso, teremos um binário com o bit 1 na posição que queremos transformar em 0. 
    * Para finalizar, fazemos o complemento a 2 desse binário, de forma que ele seja todo de 1 
    * e que tenha um bit 0 apenas onde queremos."
    */
    return x & (~(1 << n));
}

/*
 * Bit na posição p do inteiro x
 *      Permitido:
 *          Operações bitwise: << >> | & + -
 *
 *      Número máximo de operações: 6
 *      Monitor: 2
 *
 *      Retorna o valor do bit na posição p no inteiro x
 *      Valor de retorno pode ser apenas 0 ou 1
 *
 *      p será um valor entre 0 e 31
 *
 *      Exemplo:
 *          23 em binário: 0   0 ... 0 1 0 1 1 1
 *               posições: 31 30 ... 5 4 3 2 1 0
 *
 *          bitEmP(23, 31) -> 0
 *          bitEmP(23, 5) -> 0
 *          bitEmP(23, 4) -> 1
 *          bitEmP(23, 3) -> 0
 *          bitEmP(23, 2) -> 1
 *          bitEmP(23, 1) -> 1
 *          bitEmP(23, 0) -> 1
 *
 */
int32_t bitEmP(int32_t x, uint8_t p) {
    /*
    * Primeiramente, para dizer se um bit é 1 ou 0, a melhor forma de fazer isso é fazer um AND com o 1,
    * (entendendo que se o bit for 0 & 1 = o retorno vai ser 0, e se o bit for 1 & 1 = o retorno vai ser 1)
    * A partir disso, precisamos que o bit que queremos descobrir o valor, fique na posição 1, 
    * ou seja, que ele fique no bit LSB, para isso podemos fazer um shift para a direita de p posições" 
    */
    return (x >> p) & 1;
}

/*
 * Byte na posição p do inteiro x
 *      Permitido:
 *          Operações: << >> | ! &
 *
 *      Número máximo de operações: 6
 *      Monitor: 3
 *
 *      Retorna o valor do bit na posição p no inteiro x
 *      Valor de retorno pode ser entre 0 e 0xFF
 *
 *      p será um valor entre 0 e 3
 *      0 retorna LSB
 *      3 retorna MSB
 *
 *      Exemplo:
 *          byteEmP(0x12345678, 0) -> 0x78
 *          byteEmP(0x12345678, 1) -> 0x56
 *          byteEmP(0x12345678, 2) -> 0x34
 *          byteEmP(0x12345678, 3) -> 0x12
 *
 */
int32_t byteEmP(int32_t x, uint8_t p) {
    /*
    * Analoga às funções anteriores, só que desta vez trabalhando com byte, 
    * faremos um AND com 0xFF para pegar o valor do byte correspondente, 
    * mas antes de fazer essa operação, precisamos deslocar o byte que queremos para a posição do LSB, 
    * isto é, 0xFF.
    * Como queremos o valor de um byte, que equivale a 8 bits, e o shift é feito de bit a bit, 
    * precisamos multiplicar o valor da posição do byte por 8, 
    * para isso faremos um shift de 3 posições para esquerda no valor de p, (p << 3), 
    * que equivale a p * 2^3, de forma que o shift a direita que faremos no final
    * seja multiplicado por 8, já que estamos trabalhando com bytes.
    */
    return (x >> (p << 3)) & 0xFF;
}

/*
 * Seta byte na posição p do inteiro x como y
 *      Permitido:
 *          Operações: << >> | ~ ! &
 *
 *      Número máximo de operações: 7
 *      Monitor: 5
 *
 *      Retorna x com o valor y no byte da posição p
 *
 *      p será um valor entre 0 e 3
 *      0 retorna LSB
 *      3 retorna MSB
 *
 *      Exemplo:
 *          setaByteEmP(0x12345678, 0xFF, 0) -> 0x123456FF
 *          setaByteEmP(0x12345678, 0xFF, 1) -> 0x1234FF78
 *          setaByteEmP(0x12345678, 0xFF, 2) -> 0x12FF5678
 *          setaByteEmP(0x12345678, 0xFF, 3) -> 0xFF345678
 *
 */
int32_t setaByteEmP(int32_t x, int32_t y, uint8_t p) {
    return -1;
}

/*
 * Minimo
 *      Permitido:
 *          Operações: << >> | ^ < > ~ ! & -
 *
 *      Número máximo de operações: 15
 *      Monitor: 5
 *
 *      Retorna o menor numero entre x e y
 *
 *      Exemplo:
 *          minimo(10, 15) -> 10
 *          minimo(-2, -1) -> -2
 *          minimo(-1, 2) -> -1
 *
 */
int32_t minimo(int32_t x, int32_t y) {
    return -1;
}

/*
 * Negação lógica sem !
 *      Permitido:
 *          Operações: << >> | & + ~
 *
 *      Número máximo de operações: 15
 *      Monitor: 5
 *
 *      Retorna 1 se x == 0, retorna 0 caso contrário
 *
 *      Exemplo:
 *          negacaoLogica(0) -> 1
 *          negacaoLogica(37) -> 0
 *
 */
int32_t negacaoLogica(int32_t x) {
  return -1;
}

void teste(int32_t saida, int32_t esperado) {
    static uint8_t test_number = 0;
    test_number++;
    if(saida == esperado)
        printf(ANSI_COLOR_GREEN "PASSOU! Saída: %-10d\t Esperado: %-10d\n" ANSI_COLOR_RESET,
            saida, esperado);

    else
        printf(ANSI_COLOR_RED "%d: FALHOU! Saída: %-10d\t Esperado: %-10d\n" ANSI_COLOR_RESET,
            test_number, saida, esperado);
}

int main() {
    puts(ANSI_COLOR_BLUE "Primeiro lab - bits" ANSI_COLOR_RESET);
    puts("");

    puts("Teste: ehPar");
    teste(ehPar(2), 1);
    teste(ehPar(1), 0);
    teste(ehPar(3), 0);
    teste(ehPar(13), 0);
    teste(ehPar(100), 1);
    teste(ehPar(125), 0);
    teste(ehPar(1024), 1);
    teste(ehPar(2019), 0);
    teste(ehPar(2020), 1);
    teste(ehPar(-1), 0);
    teste(ehPar(-27), 0);
    teste(ehPar(-1073741825), 0);
    teste(ehPar(1073741824), 1);
    teste(ehPar(2147483647), 0);
    teste(ehPar(-2147483648), 1);
    teste(ehPar(0), 1);
    puts("");

    puts("Teste: mod8");
    teste(mod8(0), 0);
    teste(mod8(4), 4);
    teste(mod8(7), 7);
    teste(mod8(8), 0);
    teste(mod8(-1), 7);
    teste(mod8(-8), 0);
    teste(mod8(2147483647), 7);
    teste(mod8(-2147483648), 0);
    puts("");

    puts("Teste: negativo");
    teste(negativo(0), 0);
    teste(negativo(1), -1);
    teste(negativo(-1), 1);
    teste(negativo(2147483647), -2147483647);
    teste(negativo(-2147483647), 2147483647);
    teste(negativo(-2147483648), 2147483648);
    puts("");

    puts("Teste: bitwiseAnd");
    teste(bitwiseAnd(1, 3), 1);
    teste(bitwiseAnd(-1, 0), 0);
    teste(bitwiseAnd(-1, 0x7FFFFFFF), 0x7FFFFFFF);
    teste(bitwiseAnd(0b0100, 0b1100), 0b0100);
    puts("");

    puts("Teste: ehIgual");
    teste(ehIgual(1,1), 1);
    teste(ehIgual(1,0), 0);
    teste(ehIgual(0,1), 0);
    teste(ehIgual(-1,1), 0);
    teste(ehIgual(-1,-1), 1);
    teste(ehIgual(2147483647,-1), 0);
    teste(ehIgual(2147483647,-2147483647), 0);
    teste(ehIgual(2147483647,-2147483648), 0);
    teste(ehIgual(2147483647,-2147483648), 0);
    puts("");

    puts("Teste: limpaBitN");
    teste(limpaBitN(1,0), 0);
    teste(limpaBitN(0b1111,1), 0b1101);
    teste(limpaBitN(15,3), 7);
    teste(limpaBitN(-1,31), 2147483647);
    teste(limpaBitN(-1,0), -2);
    teste(limpaBitN(2147483647, 30), 1073741823);
    puts("");

    puts("Teste: bitEmP");
    teste(bitEmP(1, 0), 1);   //    b01 => retorna 1
    teste(bitEmP(1, 1), 0);   //    b01 => retorna 0
    teste(bitEmP(2, 0), 0);   //    b10 => retorna 0
    teste(bitEmP(2, 1), 1);   //    b10 => retorna 1
    teste(bitEmP(9, 2), 0);   //  b1001 => retorna 0
    teste(bitEmP(-4194305, 22), 0);
    teste(bitEmP(9, 3), 1);
    teste(bitEmP(16, 3), 0);
    teste(bitEmP(0x1 << 5, 4), 0);
    teste(bitEmP(0x1 << 31, 31), 1);
    teste(bitEmP(-1073741825, 30), 0);
    teste(bitEmP(-1073741825, 31), 1);
    puts("");

    puts("Teste: byteEmP");
    teste(byteEmP(0x766B, 1), 0x76);
    teste(byteEmP(0x766B, 0), 0x6B);
    teste(byteEmP(0x8420, 0), 0x20);
    teste(byteEmP(0x12345678, 3), 0x12);   // retorna 0x12
    teste(byteEmP(0x12345678, 2), 0x34);   // retorna 0x34
    teste(byteEmP(0x12345678, 1), 0x56);   // retorna 0x56
    teste(byteEmP(0x12345678, 0), 0x78);   // retorna 0x78
    teste(byteEmP(0x321, 1), 0x03);        // retorna 0x03
    teste(byteEmP(0x321, 0), 0x21);        // retorna 0x21
    puts("");

    puts("Teste: setaByteEmP");
    teste(setaByteEmP(0x00, 0xFF, 0), 0x000000FF);
    teste(setaByteEmP(0x00, 0xFF, 1), 0x0000FF00);
    teste(setaByteEmP(0x00, 0xFF, 2), 0x00FF0000);
    teste(setaByteEmP(0x00, 0xFF, 3), 0xFF000000);
    teste(setaByteEmP(0x01234567, 0x33, 2), 0x01334567);
    teste(setaByteEmP(0xdeadbeef, 0x00, 0), 0xdeadbe00);
    teste(setaByteEmP(0xdeadbeef, 0x00, 1), 0xdead00ef);
    puts("");

    puts("Teste: minimo");
    teste(minimo(0,1), 0);
    teste(minimo(0,10), 0);
    teste(minimo(1, 128), 1);
    teste(minimo(-1, 0), -1);
    teste(minimo(-1, -2), -2);
    teste(minimo(2147483647, 2147483646), 2147483646);
    teste(minimo(-2147483648, -2147483647), -2147483648);
    teste(minimo(-2147483648, -1), -2147483648);
    puts("");

    puts("Teste: negacaoLogica");
    teste(negacaoLogica(0), 1);
    teste(negacaoLogica(1), 0);
    teste(negacaoLogica(-1), 0);
    teste(negacaoLogica(64), 0);
    teste(negacaoLogica(-64), 0);
    teste(negacaoLogica(2147483647), 0);
    teste(negacaoLogica(-2147483648), 0);
    puts("");

}