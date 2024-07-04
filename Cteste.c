//--------------------------------------------------------------------------------------------
// INTEGRANTES:

// Lucas Goulart de Farias Meres            RA: 10395777
// Pedro Loureiro Morone Branco Volpe       RA: 10395922

//--------------------------------------------------------------------------------------------

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

// DEFINE utilizado para alocar um tamanho máximo do buffer, para assim ler um arquivo e inseri-lo no buffer corretamente
#define MAX_BUFFER_SIZE 10000

//########### INICIO DEFINICOES  mini Analisador Lexico ###################
// DEFINIÇÃO DO ENUM UTILIZADO PELO consome() E ATRIBUIDO PELO obter_atomo()
typedef enum{
    ERRO,
    IDENTIFICADOR,
    NUMERO,
    // PALAVRAS RESERVADAS
    INT,
    BOOL,
    VOID,
    IF,
    ELSE,
    WHILE,
    SCANF,
    PRINTF,
    TRUE,
    FALSE,
    MAIN,
    // 
    ABRE_PAR,
    FECHA_PAR,
    ABRE_CHAVE,
    FECHA_CHAVE,
    VIRGULA,
    PONTO_VIRGULA,
    // OPERAÇÕES
    OP_ATRIBUICAO,
    OP_IGUALDADE,
    OP_MAIOR_IGUAL,
    OP_MAIOR,
    OP_MENOR,
    OP_MENOR_IGUAL,
    OP_DIFERENTE,
    OP_SOMA,
    OP_SUBTRAC,
    OP_DIV,
    OP_MULT,
    AND,
    OR,
    //
    COMENTARIO,
    BLOCO_COMENTARIO,
    EOS,
}TAtomo;
// DEFINIÇÃO DA STRING QUE PRINTARÁ OS ENUNS CORRESPONDENTES
char *strAtomo[]={
    "Erro Lexico",    
    "IDENTIFICADOR",
    "NUMERO",
    "INT",
    "BOOL",
    "VOID",
    "IF",
    "ELSE",
    "WHILE",
    "SCANF",
    "PRINTF",
    "TRUE",
    "FALSE",
    "MAIN",
    "ABRE_PAR",
    "FECHA_PAR",
    "ABRE_CHAVE",
    "FECHA_CHAVE",
    "VIRGULA",
    "PONTO_VIRGULA",
    "OP_ATRIBUICAO",
    "OP_IGUALDADE",
    "OP_MAIOR_IGUAL",
    "OP_MAIOR",
    "OP_MENOR",
    "OP_MENOR_IGUAL",
    "OP_DIFERENTE",
    "OP_SOMA",
    "OP_SUBTRAC",
    "OP_DIV",
    "OP_MULT",
    "AND",
    "OR",
    "COMENTARIO",
    "BLOCO_COMENTARIO",
    "Fim de buffer(EOS)"};

// STRUCT QUE ARMAZENARÁ OS ÁTOMOS
typedef struct{
    TAtomo atomo;
    int linha;
    float atributo_numero;
    char atributo_ID[100];
}TInfoAtomo;
// LINHA, É A VARIAVEL QUE ATRIBUI A LINHA PARA PRINT ADEQUADO DO CÓDIGO 
int linha = 1;
// LINHA_TEMP É UTILIZADA NO CASO DE APARECER UM BLOCO COMENTÁRIO, POIS ELA ARMAZENARÁ A LINHA INICIAL DO BLOCO PARA PRINT ADEQUANDO E CORRETO 
int linhatemp = 1;
//----------------------------------------// BUFFER PARA TESTES //----------------------------------------
char *buffer = NULL;
// TESTE 1 - CORRETO
// *buffer = "/* SALVE TCHURMA\n"
//                 "boa  tarde\n"
//                 "slk*/\n"

//                 "int main(void){\n"
//                 "int _num;\n"
//                 "bool _p, _x;   \t\n" 
//                 "_num = 0xAB7/0x12 ;\n"
//                 "printf(_num);\n"
//                 "if (_num >= _p || _p == _num && _x > _num){\n"
//                 "_p = true; _x = false;}\n"
//                 "}";

// TESTE 2 - INCORRETO 
// *buffer = "int main(void){\n"
//                "int _num\n"
//                "_num = 0x10 ;\n"
//                "printf(_num);\n"
//                "}\n";

// TESTE 3 - CORRETO
// *buffer =  "/*\n"
//                 "programa le dois numeros\n"
//                 "inteiros e encontra o maior\n"
//                 "*/\n"
//                 "int main(void){\n"
//                 "int _num1, _num2   ;\n"
//                 "int _maior;\n"
//                 "scanf(_num1);\n"
//                 "scanf(_num2);\n"

//                 "/* biribas e maluquices\n"
//                 "isso deve ser ignorado\n"
//                 "biriba*/\n"

//                 "if( _num1 > _num2 )\n"
//                 "_maior = _num1;\n"
//                 "else\n"
//                 "_maior = _num2;\n"
//                 "printf(_maior); // imprime o maior valor\n"
//                 "}";

void grava_buffer(const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r"); // Abre o arquivo para leitura

    if (arquivo == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo.\n");
        exit(1);
    }

    // Aloca memória para o buffer
    buffer = (char *)malloc(MAX_BUFFER_SIZE * sizeof(char));
    if (buffer == NULL) {
        fprintf(stderr, "Erro de alocação de memória.\n");
        exit(1);
    }

    // Lê o conteúdo do arquivo para o buffer
    // A função em seguida possui 4 parâmetros: local de armazenamento de dados, tamanho do item de bytes, numero maximo de itens a serem lidos, arquivo
    size_t numread = fread(buffer, sizeof(char), MAX_BUFFER_SIZE, arquivo);
    // Referencia: https://learn.microsoft.com/pt-br/cpp/c-runtime-library/reference/fread?view=msvc-170
    if (numread == 0) {
        fprintf(stderr, "Erro ao ler o arquivo.\n");
        exit(1);
    }

    // Adiciona um terminador de string nula ao final do buffer
    buffer[numread] = '\0';

    // Fecha o arquivo
    fclose(arquivo);
}

//---------------------------------------------------------------------------------------------------------

// definicao de funcoes DO ANALIZADOR LEXICO 
TInfoAtomo obter_atomo();
TInfoAtomo reconhece_id();
TInfoAtomo reconhece_comentario();
TInfoAtomo reconhece_num();
//########### FIM DEFINICOES  mini Analisador Lexico ###################

//########### INICIO DEFINICOES  Analisador Sintatico ###################

TAtomo lookahead;
TInfoAtomo infoAtomo;
// DEFINIÇÃO DE FUNÇÕES DO ANALIZADOR SINTATICO 
void programa();
void declaracoes();
void declaracao();
void tipo();
void comandos();
void comando();
void bloco_comandos();
void atribuicao();
void comando_if();
void comando_while();
void comando_entrada();
void comando_saida();
void expressao();
void expressao_logica();
void expressao_relacional();
void op_relacional();
void expressao_adicao();
void expressao_multi();
void operando();
void lista_variavel();

// FUNÇÃO CONSOME 
void consome( TAtomo atomo );

//########### FIM DEFINICOES  Analisador Sintatico ###################

int main(void){
    // NOTA IMPORTANTE: passar o PATH completo para o TXT, por exemplo: "C:/Users/win/OneDrive/Documentos/GitHub/Compilador_CLite/entrada.txt";
    // Função para ler de um arquivo .txt e inserir no buffer para verificar
    const char *arquivo = "C:/Users/win/OneDrive/Documentos/GitHub/Compilador_CLite/entrada.txt";
    grava_buffer(arquivo);

    // PRINT DO BUFFER NO INICIO DA ANALISE 
    printf("Analisando:\n%s\n",buffer);
    // ATRIBUIÇÃO INICIAL DO LOOKAHEAD PARA O PRIMEIRO ÁTOMO
    infoAtomo = obter_atomo();
    lookahead = infoAtomo.atomo;

    // INICIO DA ANÁLIZE SINTATICA (CHAMADA DA FUNÇÃO programa())
    programa();

    printf("Analise sintatica terminada com sucesso.\n");
}
//########### INICIO mini Analisador Lexico ###################

TInfoAtomo obter_atomo(){
    TInfoAtomo infoAtomo;
    infoAtomo.atomo=ERRO;
    // ignora delimitadores
    while(*buffer == ' ' || *buffer == '\n' || *buffer == '\t' || *buffer == '\r'){
        if(*buffer=='\n')
            linha++;
            buffer++;
    }
    // Comentário devera ser verificado antes para evitar problemas verificando divisão"/"
    // BLOCO PARA ANALIZAR COMENTARIOS
    if ((*buffer=='/') && ((*(buffer+1) == '/') || (*(buffer+1) == '*'))){
        buffer++;
        infoAtomo = reconhece_comentario();
        infoAtomo.linha = linha;
        return infoAtomo;
    }
    // BLOCO PARA ANALIZAR OPERADORES DE SOMA E SUBTRAÇÃO
    else if(*buffer=='+' || *buffer=='-'){
        if (*buffer == '+') infoAtomo.atomo = OP_SOMA;
        else infoAtomo.atomo = OP_SUBTRAC;
        buffer++;
    }   
    // BLOCO PARA ANALIZAR OPERADORES DE MULTIPLICAÇÃO E DIVISÃO
    else if (*buffer =='*' || *buffer == '/'){
        if (*buffer == '*') infoAtomo.atomo = OP_MULT;
        else infoAtomo.atomo = OP_DIV;
        buffer++;
    }
    // BLOCO PARA ANALIZAR OPERADORES DE MAIOR, MAIOR IGUAL, MENOR, MENOR IGUAL, IGUALDADE e DIFERENTE
    // “<” | “<=” | “==” | “!=” | “>” | “>=”
    else if ((*buffer == '<' && *(buffer+1) == '=')   
            || (*buffer == '>' && *(buffer+1) == '=') 
            || (*buffer == '!' && *(buffer+1) == '=') 
            || (*buffer == '=' && *(buffer+1) == '=')
            || (*buffer == '<') || (*buffer == '>')){
                if ((*buffer == '<' && *(buffer+1) == '=') ){
                    infoAtomo.atomo = OP_MENOR_IGUAL;
                }
                else if ((*buffer == '>' && *(buffer+1) == '=')){
                    infoAtomo.atomo = OP_MAIOR_IGUAL;
                }
                else if ((*buffer == '!' && *(buffer+1) == '=')){
                    infoAtomo.atomo = OP_DIFERENTE;
                }
                else if ((*buffer == '=' && *(buffer+1) == '=')){
                    infoAtomo.atomo = OP_IGUALDADE;
                }
                else if ((*buffer == '>')){
                    infoAtomo.atomo = OP_MAIOR;
                }
                else if ((*buffer == '<')){
                    infoAtomo.atomo = OP_MENOR;
                }
                if ((*buffer == '<' && *(buffer+1) != '=') || (*buffer == '>' && *(buffer+1) != '=')){
                    buffer++;
                }
                else{
                    buffer++;
                    buffer++;
                }
    }
    // BLOCO PARA ANALIZAR OPERADORES DE AND e OR
    else if ((*buffer == '|' && *(buffer+1) == '|') || (*buffer == '&' && *(buffer+1) == '&')){
        if (*buffer == '|'){
            infoAtomo.atomo = OR;
        }
        else infoAtomo.atomo = AND;
        buffer++;
        buffer++;
            
    }
    // BLOCO PARA ANALIZAR OPERADORES DE ATRIBUIÇÃO
    else if (*buffer == '='){
        buffer++;
        infoAtomo.atomo = OP_ATRIBUICAO;
    }
    // BLOCO PARA ANALIZAR IDENTIFICADORES, TANTO PALAVRAS RESERVADAS QUANTO IDENTIFICADORES (que devem começar com "_")
    else if ((islower(*buffer)) || (*buffer == '_')){            
        infoAtomo = reconhece_id();
    }
    // BLOCO PARA ANALIZAR NUMEROS (que devem começar com caractere "0")
    else if (*buffer == '0'){
        infoAtomo = reconhece_num();  
    }
    // BLOCO PARA ANALIZAR SE UM PARENTESES ABRIU
    else if (*buffer == '('){
        buffer++;
        infoAtomo.atomo = ABRE_PAR;
    }
    // BLOCO PARA ANALIZAR SE UM PARENTESES FECHOU
    else if (*buffer == ')'){
        buffer++;
        infoAtomo.atomo = FECHA_PAR;
    }
    // BLOCO PARA ANALIZAR SE UM CHAVE ABRIU
    else if (*buffer == '{'){
        buffer++;
        infoAtomo.atomo = ABRE_CHAVE;
    }
    // BLOCO PARA ANALIZAR SE UM CHAVE FECHOU
    else if (*buffer == '}'){
        buffer++;
        infoAtomo.atomo = FECHA_CHAVE;
    }
    // BLOCO PARA ANALIZAR SE A UM PONTO e VIRGULA
    else if (*buffer == ';'){
        buffer++;
        infoAtomo.atomo = PONTO_VIRGULA;
    }
    // BLOCO PARA ANALIZAR SE A UMA VIRGULA
    else if (*buffer == ','){
        buffer++;
        infoAtomo.atomo = VIRGULA;     
    }
    // BLOCO PARA ANALIZAR SE ENCONTROU O FINAL DO ARQUIVO 
    else if(*buffer == '\0'){
        infoAtomo.atomo = EOS;
    }

    // SE, NÃO ENTROU EM NENHUM BLOCO, RETORNA UM ATOMO DE ERRO. 
    // SE ENTROU EM UM BLOCO, RETORNA ÁTOMO DO BLOCO.
    infoAtomo.linha = linha;
    return infoAtomo;
}

// FUNÇÃO PARA RECONHECER UM IDENTIFICADOR ou UMA PALAVRA RESERVADA
TInfoAtomo reconhece_id(){
    // INICIAÇÃO DO ATOMO E DO LEXEMA (que será utilizado para imprimir o conteudo do identificador, no caso o identificador em si)
    TInfoAtomo infoAtomo;
    char *iniLexema;
    iniLexema = buffer;

// Q0 - ESTADO PARA VERIFICAR SE BUFFER É UM IDENTIFICADOR (com "_") ou SE É UMA PALAVRA RESERVADA (sem "_")
q0:
    // SE FOR IDENTIFICADOR VAI PARA Q1 (verificador de identificador)
    if(*buffer == '_'){
        buffer++;
        goto q1;
    }
    // SE NÃO FOR IDENTIFICADOR, MAS COMEÇAR COM UM CARACTERE MINUSCULO VAI PARA Q3 (verificador de palavra reservada)
    if(islower(*buffer)){
        goto q3;
    }
    // SE NÃO RETORNA ERRO LEXICO
    infoAtomo.atomo = ERRO;
    return infoAtomo;

// Q1 - ESTADO PARA VERIFICAR SE É UM IDENTIFICADOR VÁLIDO
q1:
    // PRIMEIRO CARACTERE APOS "_" DEVE SER UM CARACTERE, SENÃO erro
    if(islower(*buffer) || isupper(*buffer)){
        buffer++;
        goto q2;

    }
    infoAtomo.atomo = ERRO;
    return infoAtomo;
// Q2 - ESTADO PARA VALIDAR OS CARACTERES RESTANTES DO IDENTIFICADOR, SE NÃO FOR UM CARACTERE NUMERICO OU ALFABÉTICO == erro 
q2:
    if(islower(*buffer) || isupper(*buffer) || isdigit(*buffer)){
        buffer++;
        goto q2;
    }
    // DETERMINA FIM DE IDENTIFICADOR E VAI PARA ESTADO FINAL DE ARMAZENAMENTO DE IDENTIFICADOR PPARA PRINT
    if ((*buffer != ' ') || (*buffer != '\n') || (*buffer != '\t') || (*buffer != '\r')){
        goto fim_Identificador;
    }
    // SE NÃO RETORNA ERRO
    infoAtomo.atomo = ERRO;
    return infoAtomo;


// Q3 - ESTADO PARA VERIFICAR SE É PALAVRA RESERVADA VÁLIDA
q3: 
    // CRIA-SE LISTA DE PALAVRAS RESERVADAS VÁLIDAS
    char* reservedWords[] = {"bool", "else", "false", "if", "int", "main", "printf", "scanf", "true", "void", "while"};
    int tamanho = 1;
    // CRIA-SE VETOR DINAMICO PARA RECEBER ATOMO PRESENTE NO BUFFER, A FIM DE COMPARA-LOO COM A LISTA DE PALAVRAS RESERVADAS
    char* word = (char *)malloc(tamanho * sizeof(char));
    word[0] = '\0';
    int i = 0;
    // INSERE NO VETOR A POSSIVEL PALAVRA RESERVADA
    while ((*buffer != ' ') || (*buffer != '\n') || (*buffer != '\t') || (*buffer != '\r')){
        if (islower(*buffer)){
            tamanho = tamanho+1;
            word = (char *)realloc(word, tamanho * sizeof(char));
            word[i] = *buffer;
            i++;
            buffer++;
        }
        else break;
    }
    // COMPARA PALAVRA ARMAZENADA NO VETOR (buffer) COM LISTA DE PALAVRAS RESERVADAS
    // SE FOREM IGUAIS, PALAVRA VALIDA. VAI PÁRA ESTADO FINAL DE PALAVRAS RESERVADAS
    // SE NÃO erro.
    word[i++] = '\0';
    for (int i = 0; i < sizeof(reservedWords) / sizeof(reservedWords[0]); i++) { 
        if (strcmp(word, reservedWords[i]) == 0) { 
            goto fim_Reservado;
        } 
    } 

    free(word);
    infoAtomo.atomo = ERRO;
    return infoAtomo;

    //fim_Identificador ESTADO FINAL DE IDENTIFICADORES QUE REALIZA Return PARA IDENTIFICADOR  
    fim_Identificador:
    
    if( buffer-iniLexema <= 15){
        // referencia:https://cplusplus.com/reference/cstring/strncpy/
        strncpy(infoAtomo.atributo_ID,iniLexema,buffer-iniLexema);
        infoAtomo.atributo_ID[buffer-iniLexema]='\0';// finaliza string
        infoAtomo.atomo = IDENTIFICADOR;
    }
    else
        infoAtomo.atomo = ERRO;

    return infoAtomo;

    //fim_Reservado ESTADO FINAL PARA PALAVRAS RESERVADAS QUE REALIZA Return PARA CADA PALAVRA RESERVADA
    fim_Reservado:  
    if( buffer-iniLexema <= 15){
        // referencia:https://cplusplus.com/reference/cstring/strncpy/
        strncpy(infoAtomo.atributo_ID,iniLexema,buffer-iniLexema);
        infoAtomo.atributo_ID[buffer-iniLexema]='\0';// finaliza string
        if (strcmp(word, "bool") == 0)
            infoAtomo.atomo = BOOL;
        else if (strcmp(word, "else") == 0)
            infoAtomo.atomo = ELSE;
        else if (strcmp(word, "false") == 0)
            infoAtomo.atomo = FALSE;
        else if (strcmp(word, "if") == 0)
            infoAtomo.atomo = IF;
        else if (strcmp(word, "int") == 0)
            infoAtomo.atomo = INT;
        else if (strcmp(word, "main") == 0)
            infoAtomo.atomo = MAIN;
        else if (strcmp(word, "printf") == 0)
            infoAtomo.atomo = PRINTF;
        else if (strcmp(word, "scanf") == 0)
            infoAtomo.atomo = SCANF;
        else if (strcmp(word, "true") == 0)
            infoAtomo.atomo = TRUE;
        else if (strcmp(word, "void") == 0)
            infoAtomo.atomo = VOID;
        else if (strcmp(word, "while") == 0)
            infoAtomo.atomo = WHILE;

        free(word);
    }
    else
        infoAtomo.atomo = ERRO;
        
    return infoAtomo;
}
// FUNÇÃO PARA RECONHECER APROPRIADAMENTE COMENTARIOS, TANTO BLOCO QUANTO INDIVIDUAL
TInfoAtomo reconhece_comentario(){
    TInfoAtomo infoAtomo;
    // INILEXEMA PARA ANALIZAR O TAMANHO DO COMENTARIO
    char *iniLexema;
    // LINHA_TEMP RECEBE LINHA INICIAL DO COMENTARIO, E É UTILIZADA CASO EXISTA UM BLOCO COMENTARIO
    linhatemp = linha;
    iniLexema = buffer;
    // INICIA ATOMO COMO erro
    infoAtomo.atomo = ERRO;
    
    char nextc;
    // VERIFICAÇÃO SE COMENTÁRIO É SIMPLES, E SE FOR, IGNORA CONTEÚDO E ATRIBUI ATOMO COMO comentario
    if (*buffer == '/') {
        while(*buffer != '\n'){ 
            buffer++;
        }
        if( buffer-iniLexema <= 9000){
        infoAtomo.atomo = COMENTARIO;
        }
    }
    // VERIFICA SE COMENTÁRIO É UM BLOCO DE COMENTÁRIO, SE FOR CONTA AS LINHAS E ATRIBUI Á ATOMO bloco_comentario
    else if(*buffer == '*'){
        buffer++;
        // LOOP ENQUANTO NÃO ENCONTRAR "*/" QUE REPRESENTA FIM DE BLOCO DE COMENTARIO, EM QUANTO ISSO IGNORA CONTEUDO E SOMA LINHA SE NECESSARIO
        while(1){
            if (*buffer == '*') {
                buffer++;
                if (*buffer == '/' )
                    break;
                else{
                    buffer--;
                }
            }
            if (*buffer == '\n') linha++;
            buffer++;
        }
        buffer++;
        if( buffer-iniLexema <= 9000){
        infoAtomo.atomo = BLOCO_COMENTARIO;
        }
    }
    // RETORNA CONTEUDO DE ATOMO 
    return infoAtomo;
}

// FUNÇÃO PARA VERIFICAR NUMERO
TInfoAtomo reconhece_num(){
    TInfoAtomo infoAtomo;
    infoAtomo.atomo = ERRO;
    char *iniLexema;
    buffer++;
    // VERIFICA SE CARACTERE SEQUIDO DE "0" É UM "x", SE FOR VAI PARA ESTADO Q1 
    if (*buffer == 'x'){
        buffer++;
        goto q1;
    }
    // SE NÃO RETORNA ERRO
    return infoAtomo;
// Q1 - ESTADO PARA VERIFICAR SE O PRIMEIRO NUMERO PRESENTE NO BUFFER É VALIDO PARA UM HEXADECIMAL
q1:
    iniLexema = buffer;
    if (isdigit(*buffer) || *buffer == 'A' || *buffer == 'B' || *buffer == 'C'  || *buffer == 'D' || *buffer == 'E' || *buffer == 'F'){
        buffer++;
        goto q2;
    }
    return infoAtomo;
// Q2 - ESTADO PARA VERFICAR OS DEMAIS NUMEROS ESTÃO NO PADRÃO HEXADECIMAL
q2:
    if (isdigit(*buffer) || *buffer == 'A' || *buffer == 'B' || *buffer == 'C'  || *buffer == 'D' || *buffer == 'E' || *buffer == 'F'){
        buffer++;
        goto q2;
    }
    // VERIFICAÇÃO DE FIM DE NUMERO E VAI PARA ESTADO FINAL fim
    if ((*buffer != ' ') || (*buffer != '\n') || (*buffer != '\t') || (*buffer != '\r')){
        goto fim;
    }
    // SE NÃO RETORNA ERRO
    return infoAtomo;

// fim - ESTADO FINAL VALIDANDO QUE NUMERO PRESENTE EM BUFFER É VÁLIDO 
fim:
    // referencia:https://cplusplus.com/reference/cstring/strncpy/
    strncpy(infoAtomo.atributo_ID,iniLexema,buffer-iniLexema);
    infoAtomo.atributo_ID[buffer-iniLexema]='\0';// finaliza string
    infoAtomo.atomo = NUMERO;
    return infoAtomo;

}
//########### FIM mini Analisador Lexico ###################

//########### INICIO  Analisador Sintatico ###################

// FUNÇÃO consome() DO ANALIZADOR SINTATICO
// ELA VERIFICA SE ATOMO PASSADO É O DESEJADO, SE NÃO FOR RETORNA erro
// SE FOR, lookahead IGUALA AO PROXIMO ATOMO, PRINTA A INFORMAÇÃO 
// OBS: EM CASO DE COMENTARIO, CONSOME IGNORA E PASSA PARA O PROXIMO ATOMO
void consome( TAtomo atomo ){
// Q1 - ESTADO UTILIZADO PARA REALIZAR UM LOOP, CASO SEJA IDENTIFICADO UM COMENTARIO PELO CONSOME
//        DESSA FORMA O COMENTARIO É IGNORADO E VERIFICA SE PROXIMO ATOMO É O DESEJADO

// OBS: ESSE LOOP SERVE PARA O CASO DO consome() AINDA NÃO TER CONSIMIDO O ATOMO CORRETO 
q1:
    // SE ATOMO DESEJADO FOR O ATOMO PRESENTE EM lookahead
    if( lookahead == atomo ){
        // IMPRIME CORRETAMENTE E lookahead SE IGUALA AO PROXIMO ÁTOMO
        printf("%d# %s ",infoAtomo.linha, strAtomo[infoAtomo.atomo]);
        if(infoAtomo.atomo == IDENTIFICADOR)
            printf("| %s",infoAtomo.atributo_ID);
        if (infoAtomo.atomo == NUMERO){
            int dec_num; char *ptr;
            dec_num = strtol(infoAtomo.atributo_ID, &ptr, 16);

            printf("| %d",dec_num);
        }
        printf("\n");

        infoAtomo = obter_atomo();
        lookahead = infoAtomo.atomo;
        if ((lookahead == COMENTARIO) || (lookahead == BLOCO_COMENTARIO)) goto q2;
    }
    // SE FOR ENCONTRADO UM COMENTARIO, PRINTA COMENTARIO E BUSCA PROXIMO ATOMO PARA VERIFICAR SE É O DESEJADO 
    else if (lookahead == COMENTARIO || lookahead == BLOCO_COMENTARIO){
        if(infoAtomo.atomo == BLOCO_COMENTARIO){
            printf("%d# %s ",linhatemp, strAtomo[infoAtomo.atomo]);
            printf("\n");
        }
        else{
            printf("%d# %s ",infoAtomo.linha, strAtomo[infoAtomo.atomo]);
            printf("\n");
        }
        infoAtomo = obter_atomo();
        lookahead = infoAtomo.atomo;
        goto q1;
    }
    // SE FOR PASSADO UM ERRO (CASO ESPECIFICO EXPLICADO NA FUNÇÃO operando()) DEVIDO A UM ERRO ENCONTRADO, RETORNA ERRO
    else if (lookahead == ERRO){
        printf("erro sintatico: esperado [%s] encontrado [%s]\n",strAtomo[atomo],strAtomo[lookahead]);
        exit(1);
    }
    // ELSE PARA CASO ATOMO DESEJADO NÃO FOR O CORRESPONDENTE, GERANDO erro SINTATICO (ou lexico)
    else{
        printf("erro sintatico: esperado [%s] encontrado [%s]\n",strAtomo[atomo],strAtomo[lookahead]);
        exit(1);
    }

// Q2 - ESSE ESTADO SERVE PARA QUANDO UM COMENTARIO FOR ENCONTRADO DEPOIS DO consome() CONSUMIR O ATOMO CORRETO, ISTO É, NÃO A ERROS
//      ASSIM ESSE ESTADO IRA APENAS IGNORAR COMENTÁRIOS E ATRIBUIR A lookahead O PROXIMO ATOMO  
q2: 
    // SE FOR ENCONTRADO UM COMENTARIO, PRINTA COMENTARIO E BUSCA PROXIMO ATOMO PARA VERIFICAR SE É O DESEJADO 
    if (lookahead == COMENTARIO || lookahead == BLOCO_COMENTARIO){
        if(infoAtomo.atomo == BLOCO_COMENTARIO){
            printf("%d# %s ",linhatemp, strAtomo[infoAtomo.atomo]);
            printf("\n");
        }
        else{
            printf("%d# %s ",infoAtomo.linha, strAtomo[infoAtomo.atomo]);
            printf("\n");
        }
        infoAtomo = obter_atomo();
        lookahead = infoAtomo.atomo;
        goto q2;
    }
}

// Representa como um programa deve ser iniciado em C (estrutura da main())
void programa(){

    consome(INT);
    consome(MAIN);
    consome(ABRE_PAR);
    consome(VOID);
    consome(FECHA_PAR);
    consome(ABRE_CHAVE);
    declaracoes();
    comandos();
    consome(FECHA_CHAVE);

}

// Checa se está havendo múltiplas declarações de variáveis INT ou BOOL
void declaracoes(){
    while(lookahead == INT || lookahead == BOOL){
    declaracao();
    }
}

// Estrutura para a declaração de variáveis
void declaracao(){
    tipo();
    
    lista_variavel();
    
    consome(PONTO_VIRGULA);
}

// Lista de variáveis no programa
void lista_variavel(){
    consome(IDENTIFICADOR);
    while(lookahead == VIRGULA){
        consome(VIRGULA);
        consome(IDENTIFICADOR);
    }
}

// Checa se é um tipo válido à linguaguem 
void tipo(){
    if (lookahead == INT) consome(INT);
    else consome(BOOL);
}

// Checa se um novo comando foi lido no buffer
void comandos(){
    while(lookahead == PONTO_VIRGULA || lookahead == IDENTIFICADOR || lookahead == IF || lookahead == WHILE
    || lookahead == SCANF || lookahead == PRINTF){
        comando();
    }
}

// Lista de comandos válidos na linguagem, se não ERRO
void comando(){
    if (lookahead == PONTO_VIRGULA)
        consome(PONTO_VIRGULA);
    else if (lookahead == ABRE_CHAVE){
        bloco_comandos();
    }
    else if (lookahead == IDENTIFICADOR){
        atribuicao();
    }
    else if (lookahead == IF){
        comando_if();
    }
    else if(lookahead == WHILE){
        comando_while();
    }
    else if(lookahead == SCANF){
        comando_entrada();
    }
    else {
        comando_saida();
    }
}

// Representa um bloco de comandos{}
void bloco_comandos(){
    consome(ABRE_CHAVE);
    comandos();
    consome(FECHA_CHAVE);
}

// Operação de atribuição de valor a uma variavel / identificador
void atribuicao(){
    consome(IDENTIFICADOR);
    consome(OP_ATRIBUICAO);
    expressao();
    consome(PONTO_VIRGULA);
}
// Operação que representa a estrutura do comando IF da linguagem, com a condição de caso apareça um ELSE, ele execute-o
void comando_if(){
    consome(IF);
    consome(ABRE_PAR);
    expressao();
    consome(FECHA_PAR);
    comando();
    if (lookahead == ELSE){
        consome(ELSE);
        comando();
    }
}
// Operação que representa a estrutura do comando WHILE
void comando_while(){
    consome(WHILE);
    consome(ABRE_PAR);
    expressao();
    consome(FECHA_PAR);
    comando();
}
// Operação que representa a estrutura do comando SCANF
void comando_entrada(){
    consome(SCANF);
    consome(ABRE_PAR);
    lista_variavel();
    consome(FECHA_PAR);
    consome(PONTO_VIRGULA);
}
// Operação que representa a estrutura do comando PRINTF, com adição da condição caso exista mais de um identificador a sem impresso
void comando_saida(){
    consome(PRINTF);
    consome(ABRE_PAR);
    expressao();
    while(lookahead == VIRGULA){
        consome(VIRGULA);
        expressao();
    }
    consome(FECHA_PAR);
    consome(PONTO_VIRGULA);
}

// Engloba a estrutura de uma expressão geral, então sempre pede por uma expressão lógica, por conta de ser a primeira a ser analisada
void expressao(){
    expressao_logica();
    while(lookahead == OR){
        consome(OR);
        expressao_logica();
    }
}

// Representa a estrutura de uma expressão lógica, sempre pedindo por uma expressão relacional para preservar a estrutura da expressão inserida on buffer
void expressao_logica(){
    expressao_relacional();
    while(lookahead == AND){
        consome(AND);
        expressao_relacional();
    }
}
// Representa a estrutura de uma expresão relacional, sempre pedindo por uma expressão adição
void expressao_relacional(){
    expressao_adicao();
    if (lookahead == OP_MENOR || lookahead == OP_MENOR_IGUAL || lookahead == OP_IGUALDADE || lookahead == OP_DIFERENTE ||
    lookahead == OP_MAIOR || lookahead == OP_MAIOR_IGUAL){
        op_relacional();
        expressao_adicao();
    }
}
// Operação que verifica qual foi o operando relacional inserido, entre "<,<=,==,!=,==,>=,>"
void op_relacional(){
    if(lookahead == OP_MENOR) consome(OP_MENOR);
    else if(lookahead == OP_MENOR_IGUAL) consome(OP_MENOR_IGUAL);
    else if(lookahead == OP_IGUALDADE) consome(OP_IGUALDADE);
    else if(lookahead == OP_DIFERENTE) consome(OP_DIFERENTE);
    else if(lookahead == OP_MAIOR) consome(OP_MAIOR);
    else consome(OP_MAIOR_IGUAL);
}

// Representa a estrutura de uma expresão de adição/subtração, sempre pedindo por uma expressão de multiplicação/divisão

void expressao_adicao(){
    expressao_multi();
    while(lookahead == OP_SOMA || lookahead == OP_SUBTRAC){
        if (lookahead == OP_SOMA) consome(OP_SOMA);
        else consome(OP_SUBTRAC);
        expressao_multi();
    }
}

// Representa a estrutura de uma expresão e multiplicação/divisão, sempre pedindo por um operando, concluindo a estrutura de uma expressão
void expressao_multi(){
    operando();
    while(lookahead == OP_MULT || lookahead == OP_DIV){
        if (lookahead == OP_MULT) consome(OP_MULT);
        else consome(OP_DIV);
        operando();
    }
}

/* Representa os possíveis operandos de acordo com a linguagem.
OBS: Nota-se que existe uma condição extra nessa função para consumir átomos de ERRO. Essa exceção ocorre por conta de um bug
que acontece entre as funções "expressao" => "expressao_logica" => "expressao_relacional" => "expressao_adicao" => 
"expressao_multi" => "operando" => "expressao". Podemos observar que é possível ocorrer um loop infinito entre essas funções
caso a entrada passe por todas elas e não encontre nenhum átomo consumível, como por exemplo:

else
    _num = num;

Podemos ver que neste exemplo, o programa entra no "comando_if", consome um IDENTIFICADOR e logo em seguida pede um átomo
OP_ATRIBUICAO. Porém quando entramos numa operação de atribuição, ela pede por um IDENTIFICADOR | NUMERO | TRUE | FALSE, então
em caso do átomo ser um ERRO (não ser nenhum destes citados), a OP_ATRIBUICAO fica loopando entre as funções infinitamente
até achar um átomo válido, que no exemplo, não acha nunca, pois "num" não é um átomo que existe dentro à linguagem.
*/
void operando(){
    if (lookahead == IDENTIFICADOR) consome(IDENTIFICADOR);
    else if(lookahead == NUMERO) consome(NUMERO);
    else if(lookahead == TRUE) consome(TRUE); 
    else if(lookahead == FALSE) consome(FALSE);
    else if(lookahead == ERRO) consome(ERRO);
    else expressao();
}


//########### FIM  Analisador Sintatico ###################
