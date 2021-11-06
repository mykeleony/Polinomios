/**************************************************************/
/* Aluno: Myke Leony dos Santos Amorim
/* Número USP: 12543721
/* Disciplina/Ano/EP: ACH2023/2021/EP1
/**************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#define VALOR_INVALIDO -1

// Como a informação única de um polinômio são os números de seus expoentes, a chave da lista linear duplamente encadeada será inteira:
typedef int TIPOCHAVE;

FILE* saida;

typedef struct estrutura {
  TIPOCHAVE expoente;
  float coeficiente;
  struct estrutura *prox;
  struct estrutura *ant;
} TERMO; // Cada nó equivale a um termo (monômio) do polinômio.

typedef struct {
  TERMO* cabeca;    // Estrutura anterior ao primeiro termo do polinômio, que também é utilizada como sentinela na busca sequencial.
} POLINOMIO;

typedef struct {
  float x1;
  float x2;
} SOLUCOES; // Estrutura que armazena as soluções de polinômios de segundo grau.

// Cria um polinômio nulo:
void inicializaPolinomio (POLINOMIO* polinomio) {
  polinomio->cabeca = (TERMO*) malloc(sizeof(TERMO));
  polinomio->cabeca->prox = polinomio->cabeca;
  polinomio->cabeca->ant = polinomio->cabeca;   // Marcas de uma lista duplamente encadeada vazia.
}

// Retorna o número de termos presentes em um polinômio:
int tamanhoPolinomio (POLINOMIO polinomio) {
  int n = 0;
  TERMO* i = polinomio.cabeca->prox;

  while (i != polinomio.cabeca) {
    n++;

    i = i->prox;
  }

  return n;
}

// Busca um expoente no polinômio e retorna sua posição ou NULL (além de armazenar seu elemento anterior):
TERMO* buscaSequencialOrdenada (TIPOCHAVE expoente, POLINOMIO polinomio, TERMO* *anterior) {
  TERMO* i = polinomio.cabeca->prox;

  *anterior = polinomio.cabeca;

  polinomio.cabeca->expoente = expoente;

  while (i->expoente < expoente) {
    *anterior = i;
    i = i->prox;
  }

  if (i != polinomio.cabeca && i->expoente == expoente)
    return i;

  else
    return NULL;
}

// Insere um termo (monômio) no polinômio:
bool insereTermo (float coeficiente, TIPOCHAVE expoente, POLINOMIO* polinomio) {
  if (expoente < 0 || coeficiente == 0) // Caso o monômio a inserir possua expoente inválido (negativo) ou coeficiente nulo, a inserção não é realizada.
    return false;

  TERMO* monomio;
  TERMO* anterior;

  monomio = buscaSequencialOrdenada(expoente, *polinomio, &anterior); // Testando se o termo a inserir já estava no polinômio.

  if (monomio) {  // O monômio a inserir já existe no polinômio: basta somar os coeficientes do monômio já existente e do novo termo.
    monomio->coeficiente += coeficiente;
    return true;
  }

  monomio = (TERMO*) malloc(sizeof(TERMO));
  monomio->coeficiente = coeficiente;
  monomio->expoente = expoente;

  // Como a lista é circular e possui nó cabeça, não é necessária nenhuma testagem para inserir o monômio - basta ajustar os ponteiros:
  monomio->prox = anterior->prox;
  monomio->ant = anterior;
  monomio->prox->ant = monomio;
  anterior->prox = monomio;

  return true;
}

// Exclui um termo específico do polinômio:
bool excluiTermo (TIPOCHAVE expoente, POLINOMIO *polinomio) {
  TERMO* monomio;
  TERMO* anterior;

  monomio = buscaSequencialOrdenada(expoente, *polinomio, &anterior);

  if (monomio == NULL)
    return false; // Se o termo a excluir não existir no polinômio, nada há a excluir.

  anterior->prox = monomio->prox; // Ajustando os ponteiros.
  monomio->prox->ant = anterior;

  free(monomio);

  return true;
}

// Exclui todos os elementos nulos (de coeficientes iguais a zero) de um polinômio:
void excluiNulos (POLINOMIO polinomio) {
  TERMO* i = polinomio.cabeca->ant;

  while (i != polinomio.cabeca) {
    if (i->coeficiente == 0)
      excluiTermo(i->expoente, &polinomio);

    i = i->ant;
  }
}

// Retorna o grau do polinômio (maior expoente):
TIPOCHAVE grauPolinomio (POLINOMIO polinomio) {
  if (polinomio.cabeca->prox == polinomio.cabeca)
    return VALOR_INVALIDO;  // O polinômio está vazio.

  return polinomio.cabeca->ant->expoente; // Como o polinômio está ordenado crescentemente pelos expoentes, o maior expoente pertence ao último elemento da lista.
}

// Imprime o conteúdo de um polinômio:
void imprimePolinomio (POLINOMIO polinomio) {
  excluiNulos(polinomio); // Retirando os termos de coeficientes nulos do polinômio.

  TERMO* i = polinomio.cabeca->ant; // Como polinômios são escritos em ordem decrescente de expoente, é necessário começar a impressão pelo fim.

  if (grauPolinomio(polinomio) >= 1) { // Testando se a lista está vazia.

    while (i != polinomio.cabeca) {
      if (i->ant != polinomio.cabeca) { // O polinômio possui mais de um termo.
        if (i->coeficiente == 1 && i->ant->coeficiente <= 0 && i->expoente > 1)  // Caso o coeficiente seja 1, esse deve ser omitido. Além disso, se o próximo coeficiente for negativo ou nulo, o sinal negativo ficará explícito ou nada será imprimido.
          fprintf(saida, "x^%d", i->expoente);

        else if (i->coeficiente == 1 && i->ant->coeficiente > 0 && i->expoente == 1)  // Caso o coeficiente seguinte seja positivo, o sinal positivo deve ser inserido antes.
          fprintf(saida, "x+");

        else if (i->coeficiente == 1 && i->ant->coeficiente <= 0 && i->expoente == 1)  // Caso o expoente seja 1, esse deve ser omitido.
          fprintf(saida, "x");

        else if (i->coeficiente == 1 && i->ant->coeficiente > 0)  // Coeficiente omitido e expoente explícito.
          fprintf(saida, "x^%d+", i->expoente);

        else if (i->coeficiente == -1 && i->ant->coeficiente <= 0)  // Caso o coeficiente seja -1, esse também deve ser omitido.
          fprintf(saida, "-x^%d", i->expoente);

        else if (i->coeficiente == -1 && i->ant->coeficiente > 0) // Coeficiente omitido e sinal positivo explícito.
          fprintf(saida, "-x^%d+", i->expoente);

        else if (i->expoente == 0)
          fprintf(saida, "%.2f", i->coeficiente); // Caso o expoente seja nulo, apenas o coeficiente é imprimido.

        else if (i->expoente == 1 && i->ant->coeficiente > 0)
          printf("%.2fx+", i->coeficiente);

        else if (i->expoente == 1 && i->ant->expoente == 0)
          fprintf(saida, "%.2fx", i->coeficiente); // Caso o coeficiente difira de 1 e expoente seja 1, o sinal de exponenciação é omitido.

        else if (i->expoente == 1 && i->ant->coeficiente < 0)
          fprintf(saida, "%.2fx-", i->coeficiente);

        else if (i->ant->coeficiente <= 0)
          fprintf(saida, "%.2fx^%d", i->coeficiente, i->expoente);

        else
          fprintf(saida, "%.2fx^%d+", i->coeficiente, i->expoente);
      }

      else { // Último ou único termo do polinômio.
        if (i->coeficiente == 0)
          putchar('\0');

        else if (i->coeficiente == 1 && i->expoente > 1)
          fprintf(saida, "x^%d", i->expoente);

        else if (i->coeficiente == 1 && i->expoente == 1)
          fprintf(saida, "x");

        else if (i->coeficiente == -1 && i->expoente > 1)
          fprintf(saida, "-x^%d", i->expoente);

        else if (i->coeficiente == -1 && i->expoente == 1)
          fprintf(saida, "-x");

        else if (i->expoente == 0)
          fprintf(saida, "%.2f", i->coeficiente);

        else if (i->expoente == 1)
          fprintf(saida, "%.2fx", i->coeficiente);

        else
          fprintf(saida, "%.2fx^%d", i->coeficiente, i->expoente);
      }

      i = i->ant; // Passa para o próximo termo do polinômio.
    }
  }

  else
    fprintf(saida, "0");  // Polinômio nulo.
}

// Retorna o polinômio resultante da soma de dois polinômios quaisquer:
POLINOMIO somaPolinomios (POLINOMIO polinomio1, POLINOMIO polinomio2) {
  if (grauPolinomio(polinomio2) < 0)
    return polinomio1;  // Se ambos os polinômios forem nulos, nada há a somar. Além disso, a soma do polinômio 1 com um polinômio nulo é o próprio polinômio 1.

  if (grauPolinomio(polinomio1) < 0)
    return polinomio2;  // A soma do polinômio 2 com um polinômio nulo é o próprio polinômio 2.

  TERMO* i = polinomio1.cabeca->ant;
  TERMO* j = polinomio2.cabeca->ant;

  // O polinômio com menor (ou igual) número de termos é totalmente percorrido:
  if (tamanhoPolinomio(polinomio1) >= tamanhoPolinomio(polinomio2)) {
    while (j != polinomio2.cabeca) {
      insereTermo (j->coeficiente, j->expoente, &polinomio1); // Inserindo expoentes não existentes no polinômio com maior (ou igual) número de termos e somando os coeficientes de expoentes preexistentes.

      j = j->ant;
    }

    return polinomio1;
  }

  // No caso do polinômio 2 ser maior que o 1, basta trocar o polinômio percorrido:
  else {
    while (i != polinomio1.cabeca) {
      insereTermo (i->coeficiente, i->expoente, &polinomio2);

      i = i->ant;
    }

    return polinomio2;
  }
}

// Retorna o polinômio resultante da multiplicação de dois polinômios:
POLINOMIO multiplicaPolinomios (POLINOMIO polinomio1, POLINOMIO polinomio2) {
  TERMO* i = polinomio1.cabeca->ant;
  TERMO* j = polinomio2.cabeca->ant;

  POLINOMIO resultado;
  inicializaPolinomio(&resultado);

  while (j != polinomio2.cabeca) {
    while (i != polinomio1.cabeca) {
      insereTermo (i->coeficiente * j->coeficiente, i->expoente + j->expoente, &resultado);

      i = i->ant;
    }

    j = j->ant;
    i = polinomio1.cabeca->ant;
  }

  return resultado;
}

// Retorna o polinômio resultante da subtração de dois polinômios:
POLINOMIO subtraiPolinomios (POLINOMIO polinomio1, POLINOMIO polinomio2) {
  POLINOMIO negativo;  // Polinômio auxiliar para inverter o sinal de um dos polinômios.

  inicializaPolinomio(&negativo);
  insereTermo(-1, 0, &negativo);  // P(x) = -1

  POLINOMIO polinomio1_negativo = multiplicaPolinomios(polinomio1, negativo); // Multiplicando o polinômio 1 por -1.

  POLINOMIO soma;

  inicializaPolinomio(&soma);
  soma = somaPolinomios(polinomio2, polinomio1_negativo); // P(x)-P(y) = P(x)+(-(P(y))).

  return soma;
}

// Retorna a derivada de um polinômio:
POLINOMIO derivaPolinomio (POLINOMIO polinomio) {
  TERMO* i = polinomio.cabeca->ant;

  while (i != polinomio.cabeca) { // Todos os termos do polinômio são percorridos.
    i->coeficiente = i->expoente * i->coeficiente;  // O coeficiente da derivada do monômio é a multiplicação do próprio pelo expoente do termo.
    i->expoente -= 1; // O expoente do monômio é decrescido em 1.

    i = i->ant;
  }

  return polinomio;
}

// Verifica se um número é uma raiz de um polinômio:
bool ehRaiz (float numero, POLINOMIO polinomio) {
  if (grauPolinomio(polinomio) < 1)  // Se o polinômio for nulo, inválido ou possuir apenas o termo independente, o polinômio não apresenta raiz.
    return false;

  TERMO* i = polinomio.cabeca->ant;
  float solucao = 0;

  while (i != polinomio.cabeca) {
    solucao += i->coeficiente * pow(numero, i->expoente); // Substituindo as incógnitas do polinômio pelo número a verificar.
    i = i->ant; // Passando para o termo seguinte.
  }

  if (solucao == 0) // Se a substituição da variável resultar em 0, o número inserido é uma raiz do polinômio.
    return true;

  return false;
}

// Retorna as soluções de um polinômio de grau 2:
SOLUCOES raizesPolinomioQuadrado (POLINOMIO polinomio) {
  SOLUCOES solucoes;

  if (grauPolinomio(polinomio) != 2) {  // Caso o grau do polinômio difira de 2, -99999 e 99999 são retornados como soluções.
    solucoes.x1 = -99999;
    solucoes.x2 = 99999;

    return solucoes;
  }

  // A solução de polinômios de segundo grau é dada pela Fórmula de Bhaskara:
  float a, b, c, x1, x2, delta;

  a = polinomio.cabeca->ant->coeficiente; // O termo de maior expoente (2) é o último elemento da lista duplamente encadeada ordenada.
  b = polinomio.cabeca->ant->ant->coeficiente;
  c = polinomio.cabeca->prox->coeficiente;  //O termo independente é o primeiro elemento da lista.

  if (tamanhoPolinomio(polinomio) == 2) // Caso o polinômio apresente dois termos, apenas o termo independente está ausente (vale 0).
    c = 0;

  else if (tamanhoPolinomio(polinomio) == 1) {  // Caso o polinômio tenha um único termo, apenas o "a" é não nulo.
    b = 0;
    c = 0;
  }

  delta = (b*b)-(4*a*c);

  if (delta < 0) {  // Caso a raiz de delta não exista como um número real, -99999 e 99999 são retornados como soluções.
    solucoes.x1 = -99999;
    solucoes.x2 = 99999;

    return solucoes;
  }

  x1 = (-b + sqrt(delta))/(2*a);
  x2 = (-b - sqrt(delta))/(2*a);

  solucoes.x1 = x1;
  solucoes.x2 = x2;

  return solucoes;
}

// Destrói (mais precisamente, reinicia) um polinômio:
void destruirPolinomio (POLINOMIO* polinomio) {
  TERMO* atual = polinomio->cabeca->prox;
  TERMO* prox;

  while (atual != polinomio->cabeca) {
    prox = atual->prox; // Preserva a posição seguinte.

    free(atual);  // Libera a memória alocada pelo monômio atual.

    atual = prox; // Ajusta o início do polinômio.
  }

  polinomio->cabeca->ant = polinomio->cabeca;
  polinomio->cabeca->prox = polinomio->cabeca; // Permite a reutilização da polinômio.
}

int main(int agrc, char* argv[]) {
  int operacao, qtd_operacoes;

  // Manipulação de arquivos:
  FILE* entrada = fopen(argv[1], "r"); // Abrindo o arquivo de entrada para leitura.
  saida = fopen(argv[2], "w");  // Abrindo o arquivo de saída para escrita.

  fscanf(entrada, "%d", &qtd_operacoes);  // A primeira linha do arquivo de entrada contém a quantidade de operações contidas no arquivo.

  if (qtd_operacoes <= 0 || entrada == NULL) {
    fprintf(saida, "-1"); // Caso o número de operações seja inválido ou o arquivo esteja vazio, o número -1 é imprimido na primeira linha do arquivo de saída.
    return 0;
  }

  fprintf(saida, "%d", qtd_operacoes);  // A primeira linha do arquivo de saída contém a quantidade de operações contidas no arquivo de entrada.

  for (int i = 0; i < qtd_operacoes; i++) {
    int qtd_argumentos;

    fscanf(entrada, "%d", &operacao);
    fprintf(saida, "\n%d\n", operacao);  // Linha do arquivo de saída que especifica o código da operação realizada.
    fscanf(entrada, "%d", &qtd_argumentos); // O identificador do número de argumentos não é considerado no processamento dos argumentos:

    // Cada argumento pode ter até 10 caracteres:
    char* auxiliar1 = malloc(sizeof(int) * (qtd_argumentos*10));
    auxiliar1 = fgets(auxiliar1, qtd_argumentos*10, entrada);  // Armazenando a string da linha de argumentos.

    char argumentos1[100];  // Vetor de caracteres que armazena os argumentos.

    strcpy(argumentos1, auxiliar1); // Copiando o conteúdo da string auxiliar para o vetor de caracteres para possibilitar a tokenização dos argumentos.

    free(auxiliar1);

    char* tokens1;
    tokens1 = strtok(argumentos1, " "); // Separando a string de argumentos em partes, com cada parte sendo uma informação (coeficiente ou expoente) de um termo do polinômio.

    int contador = 1; // Identificador de elemento: informa se o argumento atual é referente ao coeficiente ou ao expoente do monômio. Inicia-se em ímpar (coeficiente).

    POLINOMIO a;
    inicializaPolinomio(&a);

    POLINOMIO resultado;  // Polinômio resultante das operações que retornam polinômios.
    inicializaPolinomio(&resultado);

    float coeficiente = 0;
    int expoente = 0;

    while(tokens1) {
      if (contador % 2 == 0) { // Se o contador for par, o termo é um expoente; caso contrário, um coeficiente.
        expoente = atoi(tokens1); // Como expoentes são inteiros, é necessário realizar a conversão string -> int.
        insereTermo(coeficiente, expoente, &a);
      }

      else
        coeficiente = atof(tokens1);  // Coeficientes são representados por números reais.

      tokens1 = strtok(NULL, " ");

      contador++; // O identificador se altera a cada iteração, até o último argumento.
    }

    if (operacao == 4)  // Derivar um polinômio.
      resultado = derivaPolinomio(a);

    if (operacao == 5) {  // Verifica se um dado número é raiz de um polinômio.
      float raiz;

      fscanf(entrada, "%f", &raiz);

      fprintf(saida, "%d", ehRaiz(raiz, a));
      continue;
    }

    if (operacao == 6) {  // Resolver um polinômio de grau 2:
      SOLUCOES x = raizesPolinomioQuadrado(a);

      fprintf(saida, "%.2f %.2f", x.x1, x.x2);
      continue;
    }

    if (operacao == 7) {  // Imprimir um polinômio em tela:
      imprimePolinomio(a);
      continue;
    }

    if (operacao == 8) {  // Verificar o grau de um polinômio:
      fprintf(saida, "%d", grauPolinomio(a));
      continue;
    }

    if (operacao > 0 && operacao < 4) {  // As operações de 1 a 3 utilizam dois polinômios.
      fscanf(entrada, "%d", &qtd_argumentos);

      char* auxiliar2 = malloc(sizeof(int) * (qtd_argumentos*10 + 1));
      auxiliar2 = fgets(auxiliar2, qtd_argumentos*10+1, entrada);

      char argumentos2[100];

      strcpy(argumentos2, auxiliar2);
      free(auxiliar2);

      char* tokens2;
      tokens2 = strtok(argumentos2, " ");

      contador = 1;

      POLINOMIO b;
      inicializaPolinomio(&b);

      coeficiente = 0;
      expoente = 0;

      while(tokens2) {
        if (contador % 2 == 0) { // Se o contador for par, o termo é um expoente; caso contrário, um coeficiente.
          expoente = atoi(tokens2);
          insereTermo(coeficiente, expoente, &b);
        }

        else
          coeficiente = atof(tokens2);

        tokens2 = strtok(NULL, " ");

        contador++; // O identificador se altera a cada iteração, até o último argumento.
      }

      if (operacao == 1)  // Soma de dois polinômios.
         resultado = somaPolinomios(a, b);

      if (operacao == 2) {  // Subtração de dois polinômios.
        if (tamanhoPolinomio(a) >= tamanhoPolinomio(b))
          resultado = subtraiPolinomios(b, a);  // O polinômio com menor número de termos é multiplicado por -1 e então somado ao outro.

        else
          resultado = subtraiPolinomios(a, b);
      }

      if (operacao == 3)  // Multiplicação de dois polinômios.
        resultado = multiplicaPolinomios(a, b);
    }

    excluiNulos(resultado); // Retirando termos nulos do polinômio resultante.

    TERMO* i = resultado.cabeca->ant; // Iterador do polinômio.

    fprintf(saida, "%d", tamanhoPolinomio(resultado));  // O primeiro elementos dos argumentos é o número de argumentos do polinômio, que equivale ao número de termos do próprio.

    while (i != resultado.cabeca) { // Todos os coeficientes e expoentes do polinômio são separados por espaços.
      fprintf(saida, " %.2f ", i->coeficiente);
      fprintf(saida, "%d", i->expoente);

      i = i->ant;
    }

    continue; // Passa para a operação seguinte.
  }
}
