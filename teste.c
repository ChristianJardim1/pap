  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>

  #define MEMINST char meminst[256][17] = {{'\0'}}
  #define MEMDADOS int memdados[256] = {0}
  #define REGISTRADOR int registrador[8] = {0}

  //STRUCTS e ENUMS
  typedef enum {
      Tipo_R=0,
      Tipo_I=1,
      Tipo_J=2,
      Tipo_OUTROS=3
  } Tipo_Instrucao;

  struct instrucao {
      char opcode[5];
      char rs[4];
      char rt[4];
      char rd[4];
      char funct[4];
      char imm[7];
      char addr[8];
  };

  typedef struct {
      int opcode;
      int rs;
      int rt;
      int rd;
      int funct;
      int imm;
      int addr;
      Tipo_Instrucao tipo;
  } Deco;

typedef struct Node {
    int ra[8];
    int mda[256];
    int pc;
    struct Node* next;
} Node;

  //NOMES DAS FUNCOES
  void menu();
  void menuOP();
  int contarlinhas(const char *arquivo);
  int carregaMemInst(char mem[256][17]);
  void carregarMemoriaDados(int mem[256]);
  void printMemory(char mem[256][17], struct instrucao *inst, Deco *dec);
  void printmemory(int *memdado);
  void printReg(int *reg);
  void decodificarInstrucao(const char *bin, struct instrucao *inst, Deco *dec);
  void copiarBits(const char *instrucao, char *destino, int inicio, int tamanho);
  int binarioParaDecimal(const char *bin, int sinal);
  void printInstrucao(Deco *dec);
  void controle(Deco *inst, int *reg, int *memdado, int *pc);
  int ULA(int op1, int op2, int opULA, int *overflow);
  void salvarAssembly(char mem[256][17]);
  void executaP(char meminst[256][17], struct instrucao *inst, Deco *dec, int *pc, int *registrador, int *memdados, Node **lista);
  void executaI(char meminst[256][17], struct instrucao *inst, Deco *dec, int *pc, int *registrador, int *memdados, Node **lista);
  void salvarMemDados(int *memdados);
  void inserirFim(Node** head, int *reg, int *md, int *pc);
  Node* criarNo(int *reg, int *md, int *pc );

  //PROGRAMA PRINCIPAL
  int main() {
      menu();
      return 0;
  }

  //MENU
  void menu() {
      Deco dec;
      MEMINST;
      MEMDADOS;
      REGISTRADOR;
      Node* lista = NULL;
      struct instrucao instrucao;
      int op, nlinhas, resul, pc = 0;

      do {
          menuOP();
          printf("Sua escolha: ");
          scanf("%d", &op);

          switch (op) {
          case 1:
              carregaMemInst(meminst);
              break;
          case 2:
              carregarMemoriaDados(memdados);
              break;
          case 3:
              printMemory(meminst, &instrucao, &dec);
              printmemory(memdados);
              break;
          case 4:
              printReg(registrador);
              break;
          case 5:
              printMemory(meminst, &instrucao, &dec);
              printmemory(memdados);
              printReg(registrador);
              printf("\n\nPC: %d", pc);
              break;
          case 6:
              salvarAssembly(meminst);
              break;
          case 7:
              salvarMemDados(memdados);
              break;
          case 8:
              executaP(meminst, &instrucao, &dec, &pc,registrador,memdados,&lista);
              break;
          case 9:
              executaI(meminst, &instrucao, &dec, &pc,registrador, memdados,&lista);
              break;
          case 10:
              break;
          case 11:
              printf("VOCE SAIU!!!");
              break;
          }
      } while(op != 11);
  }

  //FUNCOES IMPLEMENTADAS
  void menuOP() {
      printf("\n\n *** MENU *** \n");
      printf("1 - Carregar memoria de instrucoes\n");
      printf("2 - Carregar memoria de dados\n");
      printf("3 - Imprimir memorias\n");
      printf("4 - Imprimir banco de registradores\n");
      printf("5 - Imprimir todo o simulador\n");
      printf("6 - Salvar .asm\n");
      printf("7 - Salvar .dat\n");
      printf("8 - Executar programa\n");
      printf("9 - Executar instrucao\n");
      printf("10 - Volta uma instrucao\n");
      printf("11 - Sair\n\n");
  }

  int contarlinhas(const char *arquivo)
  {
      char c;
      int contador=1;
      // abre o arquivo em modo leitura
      FILE *arq = fopen (arquivo, "r");
      if (!arq)
      {
          perror ("Erro ao abrir arquivo");
          exit (1) ;
      }
      // Percorre o arquivo e conta as quebras de linha
      while (fread(&c, sizeof(char), 1, arq) == 1)
      {
          if (c == '\n') {
              contador++;
          }
      }
      fclose(arq);
      return contador;
  }

  int carregaMemInst(char mem[256][17])
  {
      char arquivo[20];
      int cont;
      // abre o arquivo em modo leitura
      printf("Nome do arquivo: ");
      scanf("%s", arquivo);
      cont = contarlinhas(arquivo);
      FILE *arq = fopen (arquivo, "r");
      if (!arq)
      {
          perror ("Erro ao abrir arquivo") ;
          exit (1) ;
      }
      int i = 0;
      char linha[20]; // Buffer para leitura
      while (i < 256 && fgets(linha, sizeof(linha), arq)) {
          // Remover quebras de linha e caracteres extras
          linha[strcspn(linha, "\r\n")] = '\0';

          // Ignorar linhas vazias
          if (strlen(linha) == 0) {
              continue;
          }

          strncpy(mem[i], linha, 16); // Copia atC) 16 caracteres
          mem[i][16] = '\0'; // Garante terminaC'C#o de string
          i++; // AvanC'a corretamente para a prC3xima posiC'C#o
      }
      fclose(arq);
      return cont;
  }

  void carregarMemoriaDados(int mem[256]) {
      char arquivo[20];
      printf("Nome do arquivo: ");
      scanf("%s", arquivo);
      FILE *arq = fopen(arquivo, "r");
      if (!arq) {
          perror ("Erro ao abrir arquivo") ;
          exit (1) ;
      }
      int i = 0;
      while (fscanf(arq, "%d", &mem[i]) != EOF) {
          i++;
      }
  }

  void printMemory(char mem[256][17], struct instrucao *inst, Deco *dec)
  {
      printf("\n############## MEMORIA DE INSTRUCOES ##############\n");
      for (int i = 0; i < 256; i++)
      {
          printf("\nInstrucao: %s\n", mem[i]);
          printf("[%d].  ", i);
          decodificarInstrucao(mem[i], inst, dec);
          printInstrucao(dec);
          printf("\n");
      }
  }

  void printmemory(int *memdado) {
      printf("\n############## MEMORIA DE DADOS ##############\n\n");
      for(int i=0; i<256; i++) {
          printf("[%d]. %d   ", i, memdado[i]);
          if (i % 8 == 7)
          {
              printf("\n");
          }
      }
  }

  void printReg(int *reg) {
      for(int i=0; i<8; i++) {
          printf("\nREGISTRADOR [%d] - %d", i, reg[i]);
      }
  }

  // decodifica a instrucao e armazena os valores na struct do tipo Deco ja no formato int
  void decodificarInstrucao(const char *bin, struct instrucao *inst, Deco *dec)
  {
      copiarBits(bin, inst->opcode, 0, 4);    // Copia os 4 bits do opcode (4 bits)
      dec->opcode = binarioParaDecimal(inst->opcode, 0);
      copiarBits(bin, inst->rs, 4, 3);        // Copia os 3 bits do rs
      dec->rs = binarioParaDecimal(inst->rs, 0);
      copiarBits(bin, inst->rt, 7, 3);        // Copia os 3 bits do rt
      dec->rt = binarioParaDecimal(inst->rt, 0);
      copiarBits(bin, inst->rd, 10, 3);       // Copia os 3 bits do rd
      dec->rd = binarioParaDecimal(inst->rd, 0);
      copiarBits(bin, inst->funct, 13, 3);    // Copia os 3 bits do funct
      dec->funct = binarioParaDecimal(inst->funct, 0);
      copiarBits(bin, inst->imm, 10, 6);      // Copia os 6 bits do imm
      dec->imm = binarioParaDecimal(inst->imm, 1);
      copiarBits(bin, inst->addr, 9, 7);     // Copia os 7 bits do addr
      dec->addr = binarioParaDecimal(inst->addr, 0);
  }

  // copia os bits da instruC'C#o para cada campo da struct instrucao
  void copiarBits(const char *instrucao, char *destino, int inicio, int tamanho)
  {
      strncpy(destino, instrucao + inicio, tamanho); // Copia os bits desejados
      destino[tamanho] = '\0';  // Adiciona o terminador de string
  }

  // converte de binario para decimal
  int binarioParaDecimal(const char *bin, int sinal) {
      int valor = (int)strtol(bin, NULL, 2);
      int bits = strlen(bin);

      if (sinal && bits > 0 && (valor & (1 << (bits - 1)))) {
          valor = valor - (1 << bits);
      }
      return valor;
  }

  // FunC'C#o para imprimir a instruC'C#o
  void printInstrucao(Deco *dec)
  {
      printf("opcode: %d  ", dec->opcode);
      printf("rs: %d  ", dec->rs);
      printf("rt: %d  ", dec->rt);
      printf("rd: %d  ", dec->rd);
      printf("funct: %d  ", dec->funct);
      printf("imm: %d  ", dec->imm);
      printf("addr: %d", dec->addr);
  }

  void controle(Deco *dec, int *reg, int *memdado, int *pc)
  {

      int overflow=0;

      if (dec->opcode == 11) {
          reg[dec->rt] = ULA(reg[dec->rs], memdado[dec->imm], 0, &overflow);
      }
      else if (dec->opcode == 15) {
          memdado[ULA(dec->rs, dec->imm, 0, &overflow)] = reg[dec->rt];
      }
      else if (dec->opcode == 4) {
          reg[dec->rt] = ULA(reg[dec->rs], dec->imm, 0, &overflow);
      }
      else if (dec->opcode == 0) {
          if (dec->funct == 0) {
              reg[dec->rd] = ULA(reg[dec->rs], reg[dec->rt], 0, &overflow);
          }
          else if (dec->funct == 2) {
              reg[dec->rd] = ULA(reg[dec->rs], reg[dec->rt], 2, &overflow);
          }
          else if (dec->funct == 4) {
              reg[dec->rd] = ULA(reg[dec->rs], reg[dec->rt], 4, &overflow);
          }
          else if (dec->funct == 5) {
              reg[dec->rd] = ULA(reg[dec->rs], reg[dec->rt], 5, &overflow);
          }
      }
          else if (dec->opcode == 8) {
              if (reg[dec->rs] == reg[dec->rt]) {
                  *pc = *pc + dec->imm + 1;
              }
          }
          else if (dec->opcode == 2) {
              *pc = dec->addr;
          }
      }

  int ULA(int op1, int op2, int opULA, int *overflow) {

      int resultado;
      *overflow = 0;

      if(opULA == 0) {
          resultado = op1 + op2;

          if ((op1 > 0 && op2 > 0 && resultado < 0) || (op1 < 0 && op2 < 0 && resultado > 0)) {
              *overflow = 1;
              printf("OVERFLOW - ADD: %d + %d = %d (fora do intervalo de 8 bits!)\n", op1, op2, resultado);
          }
      }

      else if(opULA == 2) {
          resultado = op1 - op2;

          if ((op1 > 0 && op2 < 0 && resultado < 0) || (op1 < 0 && op2 > 0 && resultado > 0)) {
              *overflow = 1;
              printf("OVERFLOW - SUB: %d - %d = %d (fora do intervalo de 8 bits!)\n", op1, op2, resultado);
      }
      }

      else if(opULA == 4) {
          resultado = op1 & op2;
      }
      else if(opULA == 5) {
          resultado = op1 | op2;
      }

      return resultado & 0xFF; // Máscara para 8 bits
  }

  void salvarAssembly(char mem[256][17]) {
  char arquivo[20];

  printf("Nome do arquivo .asm: ");
  scanf("%s", arquivo);

  FILE *arq = fopen(arquivo, "w");
  if (!arq) {
      perror("Erro ao criar arquivo");
      return;
  }

  for (int i = 0; i < 256; i++) {
      if (mem[i][0] == '\0') continue; // Ignora posições vazias

      struct instrucao inst;
      Deco dec;
      decodificarInstrucao(mem[i], &inst, &dec);

      // Converte para assembly e escreve no arquivo
      switch (dec.opcode) {
          case 0: // Tipo R (add, sub, and, or)
              switch (dec.funct) {
                  case 0: fprintf(arq, "add $%d, $%d, $%d\n", dec.rd, dec.rs, dec.rt);
                  break;

                  case 2: fprintf(arq, "sub $%d, $%d, $%d\n", dec.rd, dec.rs, dec.rt);
                  break;

                  case 4: fprintf(arq, "and $%d, $%d, $%d\n", dec.rd, dec.rs, dec.rt);
                  break;

                  case 5: fprintf(arq, "or $%d, $%d, $%d\n", dec.rd, dec.rs, dec.rt);
                  break;
              }
              break;

          case 4: // addi
              fprintf(arq, "addi $%d, $%d, %d\n", dec.rt, dec.rs, dec.imm);
              break;

          case 11: // lw
              fprintf(arq, "lw $%d, %d($%d)\n", dec.rt, dec.imm, dec.rs);
              break;

          case 15: // sw
              fprintf(arq, "sw $%d, %d($%d)\n", dec.rt, dec.imm, dec.rs);
              break;

          case 8: // beq
              fprintf(arq, "beq $%d, $%d, %d\n", dec.rs, dec.rt, dec.imm);
              break;

          case 2: // j
              fprintf(arq, "j %d\n", dec.addr);
              break;
      }
  }

  fclose(arq);
  printf("Arquivo %s salvo com sucesso!\n", arquivo);
}


  void executaI(char meminst[256][17], struct instrucao *inst, Deco *dec, int *pc, int *registrador, int *memdados, Node **lista) {
      if (strcmp(meminst[*pc], "0000000000000000") == 0) {
          printf("Fim do programa!");
      }
      else
      {
          inserirFim(lista,registrador,memdados,pc);
          decodificarInstrucao(meminst[*pc], inst, dec);
          int pc_antes = *pc;
          printInstrucao(dec);
          printf("\n");
          controle(dec, registrador, memdados, pc);
          if(*pc == pc_antes){
              (*pc)++;
          }
      }
  }

  void executaP(char meminst[256][17], struct instrucao *inst, Deco *dec, int *pc, int *registrador, int *memdados, Node **lista) {
      while ((strcmp(meminst[*pc], "0000000000000000") != 0))
      {       
          executaI(meminst, inst, dec, pc, registrador, memdados, lista);
      }
  }

  void salvarMemDados(int *memdados)
  {
      FILE *arquivo;
      char nomeArquivo[20];

      printf("Salvar como: ");
      scanf("%s", nomeArquivo);

      if ((arquivo = fopen(nomeArquivo, "w")) == NULL)
      {
          printf("Permissão Negada!");
      }
          for (int i = 0; i < 256; i++)
          {
              fprintf(arquivo, "%d\n", memdados[i]);
          }
      fclose(arquivo);
  }
  
  Node* criarNo(int *reg, int *md, int *pc) {
    Node* novoNo = (Node*)malloc(sizeof(Node));
    int i;
    for(i=0; i<8; i++){ 
       novoNo->ra[i] = reg[i];
    }
    for(i=0;i<256;i++){
       novoNo->mda[i]=md[i];
    }
    novoNo->pc=*pc;
    novoNo->next = NULL;
    return novoNo;
}

void inserirFim(Node** head, int *reg, int *md, int *pc) {
    Node* novoNo = criarNo(reg, md, pc);
    if (*head == NULL) {
        *head = novoNo;
        return;
    }
    Node* temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = novoNo;
}
