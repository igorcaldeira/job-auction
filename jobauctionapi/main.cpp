#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <math.h>
#include "AlocaFacil.h"

using namespace std;

#define INFINITO -99
#define MAX_STRING 101

#define LINGUAGENS 17
/* ->IDS DAS LINGUAGENS:
    0 = C
    1 = C++
    2 = C#
    3 = JAVA
    4 = JAVASCRIPT
    5 = HTML
    6 = PHP
    7 = PYTON
    8 = PERL
    9 = ASSEMBLY
    10 = SQL
    11 = RUBY
    12 = COBOL
    13 = R
    14 = SWIFT
    15 = GO
    16 = BASIC
    17 = OBJECTIVE-C

*/

#define TAREFAS 10
/*  ->IDS DAS TAREFAS:
    0 = DESIGN
    1 = FRONT
    2 = BACK
    3 = INTERFACE
*/

struct Funcionario{
    /*
        DADOS PESSOAIS DO FUNCIONARIO
    */
    int matricula;
    char nome[MAX_STRING];
    char Endereco[MAX_STRING];
    char email[MAX_STRING];
    char telefone1[MAX_STRING];
    char telefone2[MAX_STRING];
    char CPF[MAX_STRING];
    /*
        DADOS RELEVANTES
    */
    bool flagDisponivel;    //FALSE SE O FUNCION�RIO N�O PODE SER ALOCADO POR ALGUM RAZ�O
    int previsaoSplit;      //SPLIT DE PREVIS�O DE REDISPONIBILIDADE, NEGATIVO SE ESTIVER DISPONIVEL
    int splitAlocadoMAX;    //O M�XIMO SPLIT QUE ESTE FUNCION�RIO EST� ALOCADO, NEGATIVO SE N�O EST� ALOCADO
    int *splitsDisponiveis; //VETOR QUE GUARDA OS IDS DOS PROJETOS ALOCADOS, DO SPLIT 1 AT� O SPLIT MAX, NULL SE ESTIVER DESALOCADO


    int linguagens[LINGUAGENS]; //NIVEL DE CONHECIMENTO DA LINGUAGEM i, DE 0 A 3
    int conhecimento[TAREFAS]; // N�VEL DE CONHECIMENTO DA TAREFA i, DE 0 A 3
    int produtividade;   //N�VEL DE PRODUTIVIDADE INDIVIDUAL DO FUNCIONARIO, DE 0 A 10

};


struct Cliente{
    bool juridico;
    char nome[MAX_STRING];
    char email[MAX_STRING];
    char telefone1[MAX_STRING];
    char telefone2[MAX_STRING];
    char documento[MAX_STRING];
};

struct Tarefa{
    int idTarefa; //ID DA TAREFA, IDENTIFICAR QUAL �
    int split; //NUMERO DO SPLIT QUE ESTA TAREFA TEM
    int j; //O PESO QUE A TAREFA TEM, DE 0 A 3
    int idAlocado; //ID DO FUNCIONARIO ALOCADO
    int status; //0 = DESALOCADO
                //1 = ALOCADO- ENTRAGA PENDENTE
                //2 = ENTREGUE NO PRAZO
                //3 = ENTREGUE FORA DO PRAZO
                //4 = ABORTADO
};

struct Projeto{
    Cliente c;      //CLIENTE ASSOCIADO AO PROJETO
    int id;         //DIFERENCIADOR DE PROJETOS
    int splitMAX;   //QUANTIDADE TOTAL DE SPLITS DO PROJETO
    int splitPass;   //QUANTIDADE TOTAL DE SPLITS QUE JA SE PASSARAM DESDE A ALOCA��O
    int urgencia;   //A URGENCIA DO PROJETO, DE 0 a 10
    bool alocado;   //TRUE SE O PROJETO FOI ALOCADO, FALSE SE N�O

    bool ling[LINGUAGENS];      //LISTA DE LINGUAGENS NECESS�RIAS PRO PROJETO

    int quantTarefas;   //QUANTIDADE DE TAREFAS DO PROJETO
    Tarefa *tarefas;    //LISTA DE TAREFAS DO PROJETO, N�O PRECISA ESTAR EM ORDEM

};

//FUN��O QUE APAGA A TELA E MOSTRA DADOS DO AUTOR DO PROGRAMA
void ApagaTela(){
    system("cls");
    printf ("Programa Alocacao de Funcionarios e Projetos \tv 3.1");
    printf ("\nAutor: Vandre Augusto        Matricula: 491322\n\n");
}

//FUN��ES QUE ALTERAM OS DADOS DO VETOR F DE FUNCION�RIOS, N�O TEM NADA A VER COM O M�TODO EM SI
bool Indisponibilisa_Funcionario(Funcionario *F, int f, int matricula, int SplitPrevisao){
    bool achou = false;
    for(int i=0; i<f; i++){
        if(F[i].matricula == matricula){
            if(!F[i].flagDisponivel){
                printf("\nFuncionario %d ja indisponivel.\n", F[i].matricula);
                printf("\n->Aperte qualquer tecla para voltar ao menu. ");
                
                return false;
            }
            else{
                F[i].flagDisponivel = false;
                F[i].previsaoSplit = SplitPrevisao;
                achou = true;
            }
            achou = true;
            i = i+f;
        }

    }
    if(!achou){
        printf("\nFuncionario %d nao encontrado.\n", matricula);
        printf("\n->Aperte qualquer tecla para voltar ao menu. ");
        
        return false;
    }
    else{
        printf("\nFuncionario %d indisponibilizado com sucesso.\n", matricula);
        printf("\n->Aperte qualquer tecla para voltar ao menu. ");
        
        return true;
    }
}
bool Disponibilisa_Funcionario(Funcionario *F, int f, int matricula){
    bool achou = false;
    for(int i=0; i<f; i++){
        if(F[i].matricula == matricula){
            if(F[i].flagDisponivel){
                printf("\nFuncionario %d ja disponivel.", F[i].matricula);
                return false;
            }
            else{
                F[i].flagDisponivel = true;
                F[i].previsaoSplit = -1;
            }
            achou = true;
            i = i+f;
        }
    }
    if(!achou){
        printf("\nFuncionario %d nao encontrado.", matricula);
        printf("\n->Aperte qualquer tecla para voltar ao menu. ");
        
        return false;
    }
    else{
        printf("\nFuncionario %d disponibilizado com sucesso.", matricula);
        printf("\n->Aperte qualquer tecla para voltar ao menu. ");
        
        return true;
    }
}




//FUN��O QUE DESALOCA O VETOR PROJETO
void LiberaTipoProjeto(Projeto *P, int p){
    for(int i=0; i<p; i++){
        if(P[i].tarefas != NULL){
            free(P[i].tarefas);
        }
    }
    free(P);
}

//FUN��O QUE L� O ARQUIVO COM INFORMA��ES DE PROJETOS E TRAZ PRO VETOR P DE PROJETOS
Projeto *LerArqProjeto(char *NomeArq, int *p){
    int linhas = 12;
    int k, i, pegaI;
    char pegaS[MAX_STRING];
    char buffer[8];
    int a,b,c;
    int *teste;

    FILE *arq = fopen(NomeArq, "r");
    if(arq == NULL){
        return NULL;
    }

    k = 0;
    while(!feof(arq)){
        fgets(pegaS, MAX_STRING, arq);
        k++;
    }

    fclose(arq);

    k = k/linhas;
    Projeto *P = (Projeto*)calloc(k, sizeof(Projeto));
    *p = k;

    arq = fopen(NomeArq, "r");
    if(arq == NULL){
        free(P);
        return NULL;
    }

    k = 0;
    i = 0;
    while(!feof(arq)){
        fgets(pegaS, MAX_STRING, arq);
        pegaS[strlen(pegaS)-1] = '\0'; //APAGA O '\n' QUE VEM COM O FGETS
        //printf("\nk = %d\ti = %d\tpegaS: %s\n", k, i, pegaS);
        switch(k){
        case 0:
            pegaI = atoi(pegaS);
            P[i].id = pegaI;
            break;
        case 1:
            if(strcmp(pegaS, "J") == 0){
                P[i].c.juridico = true;
            }
            else if(strcmp(pegaS, "F") == 0){
                P[i].c.juridico = false;
            }
            else{
                printf("ERRO! sintaxe invalida 0");
                free(P);
                return NULL;
            }
            break;
        case 2:
            strcpy(P[i].c.nome, pegaS);
            break;
        case 3:
            strcpy(P[i].c.email, pegaS);
            break;
        case 4:
            strcpy(P[i].c.telefone1, pegaS);
            break;
        case 5:
            strcpy(P[i].c.telefone2, pegaS);
            break;
        case 6:
            strcpy(P[i].c.documento, pegaS);
            break;
        case 7:
            pegaI = atoi(pegaS);
            P[i].splitMAX = pegaI;
            break;
        case 8:
            pegaI = atoi(pegaS);
            P[i].splitPass = pegaI;
            break;
        case 9:
            pegaI = atoi(pegaS);
            P[i].urgencia = pegaI;
            break;
        case 10:
            if(strcmp(pegaS, "F") == 0)         P[i].alocado = false;
            else if (strcmp(pegaS, "T") == 0)   P[i].alocado = true;
            else{
                printf("ERRO! sintaxe invalida. a = %d", a);
                free(P);
                return NULL;
            }

            break;
        case 11:
            a = 0;
            b = 0;
            memset(buffer, 0, 8);

            for(int m=0; m<strlen(pegaS); m++){
                if(pegaS[m] == ' '){
                    //printf("\nb = %d\tbuffer: '%s'\n", b, buffer);
                    if(strcmp(buffer, "T") == 0){
                        P[i].ling[a] = true;
                    }
                    else if(strcmp(buffer, "F") == 0){
                        P[i].ling[a] = false;
                    }
                    else{
                        printf("ERRO! sintaxe invalida. a = %d", a);
                        free(P);
                        return NULL;
                    }
                    a++;
                    b = 0;
                    memset(buffer, 0, 8);
                }
                else{
                    buffer[b] = pegaS[m];
                    b++;
                }
            }
            break;
        case 12:
            pegaI = atoi(pegaS);
            P[i].quantTarefas = pegaI;
            break;
        case 13:
            if(P[i].quantTarefas < 0){
                printf("ERRO! sintaxe invalida 2");
                free(P);
                return NULL;
            }
            P[i].tarefas = (Tarefa*)calloc(P[i].quantTarefas, sizeof(Tarefa));
            a = 0;
            b = 0;
            c = 0;
            memset(buffer, 0, 8);
            for(int m=0; m<strlen(pegaS); m++){
                if(pegaS[m] == ' '){
                    pegaI = atoi(buffer);
                    //printf("a: %d\tb: %d\tpegaI: %d\tm: %d\tbuffer: %s\n", a, b, pegaI, m, buffer);
                    switch(c){
                    case 0:
                        P[i].tarefas[a].idTarefa = pegaI;
                        break;
                    case 1:
                        P[i].tarefas[a].split = pegaI;
                        break;
                    case 2:
                        P[i].tarefas[a].j = pegaI;
                        break;
                    case 3:
                        P[i].tarefas[a].idAlocado = pegaI;
                        break;
                    case 4:
                        P[i].tarefas[a].status = pegaI;
                        c = -1;
                        a++;
                        break;
                    }
                    c++;
                    b = 0;
                    memset(buffer, 0, 8);
                }
                else{
                    buffer[b] = pegaS[m];
                    b++;
                }
            }
            break;
        case 14:
            i++;
            k = -1;
            break;
        }
        k++;
    }
    fclose(arq);
    return P;
}

//FUN��O QUE MOSTRA NA TELA O VETOR P DE PROJETOS
void RelatorioProjetos(Projeto *P, int p){
    ApagaTela();
    printf("\n->RELACAO DOS PROJETOS:\n");
    printf("\n----------------------------------------------------------------------------------------------------------\n\n");
    for(int i=0; i<p; i++){
        printf("ID: %d\n", P[i].id);
        if(P[i].c.juridico){
            printf("Razao social: %s\n", P[i].c.nome);
            printf("CNPJ: %s\n", P[i].c.documento);
        }
        else{
            printf("Nome: %s\n", P[i].c.nome);
            printf("CPF: %s\n", P[i].c.documento);
        }

        printf("E-mail: %s\n", P[i].c.email);
        printf("Telefone1: %s\n", P[i].c.telefone1);
        printf("Telefone2: %s\n", P[i].c.telefone2);
        printf("Quantidade de Splits passados: %d\n", P[i].splitPass);
        printf("Quantidade total de Splits: %d\n", P[i].splitMAX);
        printf("Urgencia: %d\n\n", P[i].urgencia);

        if(P[i].alocado)    printf("Projeto ja alocado.\n\n");
        else                printf("Projeto ainda nao alocado.\n\n");

       printf("Linguagens do projeto:\n");
       for(int j=0; j<LINGUAGENS; j++){
            if(P[i].ling[j])
                printf("\t%d\n", j);
       }


        printf("\nTarefas do projeto:\n");
        for(int j = 0; j < P[i].quantTarefas; j++){
            printf("\tTarefa %d:\n",j+1);
            printf("\t\tId tarefa: %d\n", P[i].tarefas[j].idTarefa);
            printf("\t\tSplit: %d\n", P[i].tarefas[j].split);
            printf("\t\tRelevancia no projeto: %d\n", P[i].tarefas[j].j);
            printf("\t\tStatus: %d\n", P[i].tarefas[j].status);
            if(P[i].tarefas[j].idAlocado > 0) printf("\t\tMatricula do Funcionario alocado: %d\n", P[i].tarefas[j].idAlocado);
            else                              printf("\t\tTarefa desalocada.\n");
        }
        printf("\n----------------------------------------------------------------------------------------------------------\n\n");
    }
    printf("\n->Aperte qualquer tecla para voltar ao menu. ");
}

//FUN��O QUE SALVA EM ARQUIVO AS INFORMA��ES DO VETOR P DE PROJETOS
bool CriaArqProjeto(char *NomeArq, Projeto *P, int p){
    bool primeiro = true;
    FILE *a = fopen(NomeArq, "w");
    if(a == NULL){
        return false;
    }

    for(int i=0; i<p; i++){
        if(!primeiro){
            fprintf(a, "\n");
        }
        primeiro = false;
        fprintf(a, "%d\n", P[i].id);
        if(P[i].c.juridico) fprintf(a, "J\n");
        else                fprintf(a, "F\n");

        fprintf(a, "%s\n%s\n%s\n%s\n%s\n%d\n%d\n%d\n", P[i].c.nome
                                                 , P[i].c.email
                                                 , P[i].c.telefone1
                                                 , P[i].c.telefone2
                                                 , P[i].c.documento
                                                 , P[i].splitMAX
                                                 , P[i].splitPass
                                                 , P[i].urgencia);
        if(!P[i].alocado)    fprintf(a, "F\n");
        else                 fprintf(a, "T\n");

        for(int j=0; j<LINGUAGENS; j++){
            if(!P[i].ling[j])    fprintf(a, "F ");
            else                 fprintf(a, "T ");
        }

        fprintf(a, "\n%d\n", P[i].quantTarefas);

        for(int j=0; j<P[i].quantTarefas; j++){
            fprintf(a, "%d %d %d %d %d ", P[i].tarefas[j].idTarefa
                                        , P[i].tarefas[j].split
                                        , P[i].tarefas[j].j
                                        , P[i].tarefas[j].idAlocado
                                        , P[i].tarefas[j].status);
        }

        fprintf(a, "\n");
    }
    fclose(a);
    return true;
}

//FUN��O QUE DESALOCA O VETOR FUNCION�RIO
void LiberaTipoFuncionario(Funcionario *F, int f){
    for(int i=0; i<f; i++){
        if(F[i].splitsDisponiveis != NULL){
            free(F[i].splitsDisponiveis);
        }
    }
    free(F);
}

//FUN��O QUE L� O ARQUIVO COM INFORMA��ES DE FUNCION�RIOS E TRAZ PRO VETOR F DE FUNCION�RIOS
Funcionario *LerArqFuncionario(char *NomeArq, int *f){
    int linhas = 15;
    int k, i, pegaI;
    char pegaS[MAX_STRING];
    char buffer[8];
    int a,b;

    FILE *arq = fopen(NomeArq, "r");
    if(arq == NULL){
        return NULL;
    }

    k = 0;
    while(!feof(arq)){
        fgets(pegaS, MAX_STRING, arq);
        k++;
    }

    fclose(arq);

    k = k/linhas;
    Funcionario *F = (Funcionario*)calloc(k, sizeof(Funcionario));
    *f = k;

    arq = fopen(NomeArq, "r");
    if(arq == NULL){
        return NULL;
    }

    k = 0;
    i = 0;
    while(!feof(arq)){
        fgets(pegaS, MAX_STRING, arq);
        pegaS[strlen(pegaS)-1] = '\0'; //APAGA O '\n' QUE VEM COM O FGETS
        //printf("\nCHEGOU k = %d\ti = %d\tpegaS: %s\n", k, i, pegaS);
        switch(k){
        case 0:
            pegaI = atoi(pegaS);
            F[i].matricula = pegaI;
            break;
        case 1:
            strcpy(F[i].nome, pegaS);
            break;
        case 2:
            strcpy(F[i].Endereco, pegaS);
            break;
        case 3:
            strcpy(F[i].CPF, pegaS);
            break;
        case 4:
            strcpy(F[i].email, pegaS);
            break;
        case 5:
            strcpy(F[i].telefone1, pegaS);
            break;
        case 6:
            strcpy(F[i].telefone2, pegaS);
            break;
        case 7:
            a = 0;
            b = 0;
            memset(buffer, 0, 8);
            for(int m=0; m<strlen(pegaS); m++){
                if(pegaS[m] == ' '){
                    pegaI = atoi(buffer);
                    //printf("a: %d\tb: %d\tpegaI: %d\tm: %d\tbuffer: %s\n", a, b, pegaI, m, buffer);
                    F[i].linguagens[a] = pegaI;
                    a++;
                    b = 0;
                    memset(buffer, 0, 8);
                }
                else{
                    buffer[b] = pegaS[m];
                    b++;
                }
            }
            break;
        case 8:
            a = 0;
            b = 0;
            memset(buffer, 0, 8);
            for(int m=0; m<strlen(pegaS); m++){
                if(pegaS[m] == ' '){
                    pegaI = atoi(buffer);
                    F[i].conhecimento[a] = pegaI;
                    a++;
                    b = 0;
                    memset(buffer, 0, 8);
                }
                else{
                    buffer[b] = pegaS[m];
                    b++;
                }
            }
            break;
        case 9:
            pegaI = atoi(pegaS);
            F[i].produtividade = pegaI;
            break;
        case 10:
            if(strcmp (pegaS, "T") == 0){
                F[i].flagDisponivel = true;
            }
            else if(strcmp(pegaS, "F") == 0)
                F[i].flagDisponivel = false;
            else{
                printf("AQUI1 ERRO! sintaxe invalida");
                free(F);
                return NULL;
            }
            break;
        case 11:
            pegaI = atoi(pegaS);
            F[i].previsaoSplit = pegaI;
            break;
        case 12:
            pegaI = atoi(pegaS);
            F[i].splitAlocadoMAX = pegaI;
            break;
        case 13:
            if(strcmp(pegaS, "NULL ") == 0){
                F[i].splitsDisponiveis = NULL;
            }
            else{
                if(F[i].splitAlocadoMAX <= 0){
                    printf(" ERRO! sintaxe invalida");
                    free(F);
                    return NULL;
                }
                F[i].splitsDisponiveis = AlocaVetor_Int(F[i].splitAlocadoMAX);
                a = 0;
                b = 0;
                memset(buffer, 0, 8);
                for(int m=0; m<strlen(pegaS); m++){
                    if(pegaS[m] == ' '){
                        pegaI = atoi(buffer);
                        F[i].splitsDisponiveis[a] = pegaI;
                        a++;
                        b = 0;
                        memset(buffer, 0, 8);
                    }
                    else{
                        buffer[b] = pegaS[m];
                        b++;
                    }
                }
            }
            break;
        case 14:
            i++;
            k = -1;
            break;
        }
        k++;
    }
    fclose(arq);
    return F;
}

//FUN��O QUE SALVA EM ARQUIVO AS INFORMA��ES DO VETOR F DE FUNCION�RIOS
bool CriaArqFuncionario(char *NomeArq, Funcionario *F, int f){
    bool primeiro = true;
    FILE *a = fopen(NomeArq, "w");
    if(a == NULL){
        return false;
    }

    for(int i=0; i<f; i++){
        if(!primeiro){
            fprintf(a, "\n");
        }
        primeiro = false;
        fprintf(a, "%d\n%s\n%s\n%s\n%s\n%s\n%s\n", F[i].matricula
                                                 , F[i].nome
                                                 , F[i].Endereco
                                                 , F[i].CPF
                                                 , F[i].email
                                                 , F[i].telefone1
                                                 , F[i].telefone2);

        for(int j=0; j<LINGUAGENS; j++)
            fprintf(a, "%d ", F[i].linguagens[j]);

        fprintf(a, " \n");

        for(int j=0; j<TAREFAS; j++)
            fprintf(a, "%d ", F[i].conhecimento[j]);

        fprintf(a, " \n%d\n", F[i].produtividade);

        if(F[i].flagDisponivel) fprintf(a, "T\n");
        else                     fprintf(a, "F\n");

        fprintf(a, "%d\n%d\n", F[i].previsaoSplit, F[i].splitAlocadoMAX);

        if(F[i].splitAlocadoMAX <= 0)  fprintf(a, "NULL");
        else{
            for(int j=0; j<F[i].splitAlocadoMAX; j++)
                fprintf(a, "%d ", F[i].splitsDisponiveis[j]);
        }
        fprintf(a, " \n");
    }
    fclose(a);
    return true;
}

//FUN��O QUE RECEBE DADOS DE UM FUNCION�RIO PELO CONSOLE, FORNECIDO PELO USU�RIO, E SALVA NO VETOR F DE FUNCION�RIO (IMCOMPLETO)
void NovoFuncionario(char *NomeArq, Funcionario *F, int f){
    int matricula;
    char nome[MAX_STRING];
    char Endereco[MAX_STRING];
    char email[MAX_STRING];
    char telefone1[MAX_STRING];
    char telefone2[MAX_STRING];
    char CPF[MAX_STRING];

    int linguagens[LINGUAGENS];
    int conhecimento[TAREFAS];

    bool controle1, controle2, achou;
    int pega;
    char pegaC;

    controle2 = true;
    achou = false;
    while(controle2){
        printf("\t->Digite a matricula do funcionario: ");
        scanf("%d", &pega);
        for(int i=0; i<f; i++){
            if(F[i].matricula == pega){
                //MENSAGEM DE ERRO, MATRICULA J� EXISTE
                achou = true;
                i = f+1;
            }
        }
        if(!achou)   controle2 = false;
    }
    ApagaTela();
    printf("\t->Digite os dados pessoais do funcionario:\n\nNome: ");
    gets(nome);
    printf("Endereco: ");
    gets(Endereco);
    printf("CPF: ");
    gets(CPF);
    printf("E-mail: ");
    gets(email);
    printf("Telefone 1: ");
    gets(telefone1);
    printf("Telefone 2: ");
    gets(telefone2);

    for(int i=0; i<LINGUAGENS; i++){
        ApagaTela();
        printf("\n\tLINGUAGENS CONHECIDAS (nivel de 0 a 3)");
        printf("Linguagem[%d]: ", i);
        pegaC = getch();
        switch(pegaC){
            case '0': linguagens[i] = 0;
                break;
            case '1': linguagens[i] = 1;
                break;
            case '2': linguagens[i] = 2;
                break;
            case '3': linguagens[i] = 3;
                break;
            default: i--;
        }
    }

    for(int i=0; i<TAREFAS; i++){
        ApagaTela();
        printf("\n\tTAREFAS CONHECIDAS (nivel de 0 a 3)");
        printf("Tarefa[%d]: ", i);
        pegaC = getch();
        switch(pegaC){
            case '0': conhecimento[i] = 0;
                break;
            case '1': conhecimento[i] = 1;
                break;
            case '2': conhecimento[i] = 2;
                break;
            case '3': conhecimento[i] = 3;
                break;
            default: i--;
        }
    }

    ApagaTela();
    printf("\n\t->DADOS DIGITADOS:\n\n");
    printf("Matricula: %d\n", matricula);
    printf("Nome: %s\n", nome);
    printf("Endereco: %s\n", Endereco);
    printf("CPF: %s\n", CPF);
    printf("E-mail: %s\n", email);
    printf("Telefone 1: %s\n", telefone1);
    printf("Telefone 2: %s\n", telefone2);

    printf("\nLinguagens:\n");
    for(int i=0; i<LINGUAGENS; i++)
        printf("%d ", linguagens[i]);

    printf("\n\nTarefas:\n");
    for(int i=0; i<TAREFAS; i++)
        printf("%d ", conhecimento[i]);

    controle2 = true;
    while(controle2){
        printf("\nConfirmar insercao? S para sim, N para n�o: ");
        pegaC = getch();
        if((pegaC == 'S')||(pegaC == 's')){
            controle2 = false;
            controle1 = true;
        }
        else if((pegaC == 'N')||(pegaC == 'n')){
            controle2 = false;
            controle1 = false;
        }
    }
    ApagaTela();


    if(controle1){
        F = (Funcionario *) realloc(F, (f+1)*sizeof(Funcionario));
        if(F == NULL){
            printf("Espa�o em mem�ria insuficiente.\n");
            free(F);
            exit(0);
        }
        F[f].matricula = matricula;
        strcpy(F[f].nome, nome);
        strcpy(F[f].Endereco, Endereco);
        strcpy(F[f].CPF, CPF);
        strcpy(F[f].email, email);
        strcpy(F[f].telefone1, telefone1);
        strcpy(F[f].telefone1, telefone2);

        for(int i=0; i<LINGUAGENS; i++)
            F[f].linguagens[i] = linguagens[i];

        for(int i=0; i<TAREFAS; i++)
            F[f].conhecimento[i] = conhecimento[i];

        F[f].produtividade = 3;
        F[f].flagDisponivel = true;
        F[f].previsaoSplit = -1;
        F[f].splitAlocadoMAX = -1;
        F[f].splitsDisponiveis = NULL;

        f++;
        CriaArqFuncionario(NomeArq, F, f);
    }
    else{
        printf("\nInsercao abortada. ");
        getch();
    }

}

//FUN��O QUE MOSTRA NA TELA O VETOR F DE FUNCION�RIOS
void RelatorioFuncionarios(Funcionario *F, int f){
    ApagaTela();
    printf("\n->RELACAO DOS FUNCIONARIOS: \n");
    printf("\n----------------------------------------------------------------------------------------------------------\n\n");
    for(int i=0; i<f; i++){
        printf("Matricula: %d\n", F[i].matricula);
        printf("Nome: %s\n", F[i].nome);
        printf("Endereco: %s\n", F[i].Endereco);
        printf("CPF: %s\n", F[i].CPF);
        printf("E-mail: %s\n", F[i].email);
        printf("Telefone1: %s\n", F[i].telefone1);
        printf("Telefone2: %s\n", F[i].telefone2);
        printf("Produtividade: %d\n\n", F[i].produtividade);
        if(F[i].flagDisponivel) printf("DISPONIVEL\n");
        else{
            printf("INDISPONIVEL. ");
            if(F[i].previsaoSplit > 0)  printf("Previsao de disponibilidade: Split %d\n\n", F[i].previsaoSplit);
            else                        printf("Sem previsao de disponibilidade.\n\n");
        }

        if(F[i].splitAlocadoMAX < 0)    printf("DESALOCADO\n");
        else{
            printf("ALOCADO para os projetos (id):\n");
            for(int j = 0; j < F[i].splitAlocadoMAX; j++){
                if(F[i].splitsDisponiveis[j] > 0){
                    printf("\tSplit %d -> Projeto %d\n", j+1, F[i].splitsDisponiveis[j]);
                }
            }
        }
        printf("\n----------------------------------------------------------------------------------------------------------\n\n");
    }
    printf("\n->Aperte qualquer tecla para voltar ao menu. ");
}

//FUN��O QUE MOSTRA A TABELA DO ALGORITMO DO LEIL�O NA TELA
void MostraTelaAUX1(double **tabela, int p, int j, int *Aloc, double *Peso){
    printf("\n\t");
    for(int a=0; a<j; a++)
        printf("Out %d\t", a);

    printf("Aloc\n\n");
    for(int a=0; a<p; a++){
        printf("In %d\t", a);
        for(int b=0; b<j; b++){
            printf("%0.1lf\t", tabela[a][b]);
        }
        printf("%d\n", Aloc[a]);
    }

    printf("\nJ:\t");
    for(int a=0; a<j; a++)
        printf("%0.1lf\t", Peso[a]);
}

//ALGORITMO DO LEIL�O PARA ALOCA��O
int* Leilao_Alocacao(double **tabela, int p, int j, bool EhMIN){
    double e = 1/((double)1.1*p);
    double R, W, Y, maior;
    int q, qR;
    bool controle = true;
    bool passou;

    int *Alocacao = AlocaVetor_Int(p);
    double *Peso = AlocaVetor_Double(j);

    for(int a=0; a<p; a++)
        Alocacao[a] = -1;

    for(int a=0; a<j; a++)
        Peso[a] = 0;

    if(EhMIN){
        for(int a=0; a<p; a++){
            for(int b=0; b<j; b++){
                tabela[a][b] = tabela[a][b] * (-1);
            }
        }
    }

    /*
    printf("\n\nANTES:\n");
    MostraTelaAUX1(tabela, p, j, Alocacao, Peso);
    printf("\n\n");*/


    while(controle){
        passou = true;
        for(int i=0; i<p; i++){
            if(Alocacao[i] < 0){ //cai aqui se existe i desalocado
                passou = false;
                q = 0;
                maior = tabela[i][q] - Peso[q];
                for(int k=q+1; k<j; k++){
                    if((tabela[i][k] - Peso[k]) > maior){
                        maior = tabela[i][k] - Peso[k];
                        q = k;
                    }
                }

                R = maior;
                qR = q;

                if(qR == 0) q = 1;
                else        q = 0;

                maior = tabela[i][q] - Peso[q];
                for(int k=q+1; k<j; k++){
                    if(((tabela[i][k] - Peso[k]) > maior)&&(k != qR)){
                        maior = tabela[i][k] - Peso[k];
                        q = k;
                    }
                }

                W = maior;

                Y = R - W;

                Peso[qR] = Peso[qR] + Y + e;

                for(int k=0; k<p; k++){
                    if(Alocacao[k] == qR){
                        Alocacao[k] = -1;
                        k = p+2;
                    }
                }
                Alocacao[i] = qR;

                /*MostraTelaAUX1(tabela, p, j, Alocacao, Peso);
                printf("\n\nR: %lf\tqR: %d\tW: %lf\n\n", R, qR, W);
                getch();*/
            }
        }
        if(passou)  controle = false;
    }
    /*printf("\n\nDEPOIS:\n");
    MostraTela(tabela, p, j, Alocacao, Peso);
    printf("\n\n");*/

    if(EhMIN){
        for(int a=0; a<p; a++){
            for(int b=0; b<j; b++){
                tabela[a][b] = tabela[a][b] * (-1);
            }
        }
    }

    LiberaVetor_Double(j, Peso);
    return Alocacao;
}

//FUN��O QUE CALCULA E RETORNA O BENEFICIO DE ALOCAR UM FUNCIONARIO x(POSF) PARA A TAREFA y(POST)
int Quantificar(Funcionario *F, Projeto *P, int posf, int posp, int post){

    if(P[posp].alocado) return INFINITO;
    if((!F[posf].flagDisponivel)&&(F[posf].previsaoSplit < P[posp].tarefas[post].split))    return INFINITO;

    bool ZERO, UM, INFI;
    int a, maiorLing, k;

    a = 0;
    ZERO = false;
    UM = false;
    INFI = false;

    maiorLing = 0;
    for(int i=0; i< LINGUAGENS; i++){
        if(P[posp].ling[i]){
            if(F[posf].linguagens[i] > maiorLing){
                maiorLing = F[posf].linguagens[i];
            }
        }
    }

    //printf("\nposf: %d\tposp: %d\tpost: %d\tmaior: %d\n", posf, posp, post, maiorLing);
    if(maiorLing > 3)   maiorLing = 3;
    if(maiorLing < 0)   maiorLing = 0;

    switch(maiorLing){
    case 0:
        switch(P[posp].urgencia){
        case 0:
            UM = true;
            break;
        case 1:
            UM = true;
            break;
        case 2:
            ZERO = true;
            break;
        case 3:
            INFI = true;
            break;
        }
        break;
    case 1:
        switch(P[posp].urgencia){
        case 0:
            a++;
            break;
        case 1:
            a++;
            break;
        case 2:
            a++;
            break;
        case 3:
            a = a+0;
            break;
        }
        break;
    case 2:
        switch(P[posp].urgencia){
        case 0:
            a = a+2;
            break;
        case 1:
            a = a+2;
            break;
        case 2:
            a = a+3;
            break;
        case 3:
            a = a+3;
            break;
        }
        break;
    case 3:
        switch(P[posp].urgencia){
        case 0:
            a = a+3;
            break;
        case 1:
            a = a+3;
            break;
        case 2:
            a = a+5;
            break;
        case 3:
            a = a+5;
            break;
        }
        break;
    }

    k = F[posf].conhecimento[P[posp].tarefas->idTarefa];
    switch(k){
    case 0:
        INFI = true;
        break;
    case 1:
        switch(P[posp].urgencia){
        case 0:
            a++;
            break;
        case 1:
            a = a+2;
            break;
        case 2:
            a = a+2;
            break;
        case 3:
            a--;
            break;
        }

        switch(P[posp].tarefas[posf].j){
        case 0:
            a = a+0;
            break;
        case 1:
            a = a+0;
            break;
        case 2:
            a = a+0;
            break;
        case 3:
            a--;
            break;
        }

        break;
    case 2:
        switch(P[posp].urgencia){
        case 0:
            a = a+2;
            break;
        case 1:
            a = a+4;
            break;
        case 2:
            a = a+5;
            break;
        case 3:
            a = a+5;
            break;
        }

        switch(P[posp].tarefas[posf].j){
        case 0:
            a = a+0;
            break;
        case 1:
            a = a+0;
            break;
        case 2:
            a = a+1;
            break;
        case 3:
            a = a+2;
            break;
        }
        break;
    case 3:
        switch(P[posp].urgencia){
        case 0:
            a = a+3;
            break;
        case 1:
            a = a+5;
            break;
        case 2:
            a = a+7;
            break;
        case 3:
            a = a+9;
            break;
        }

        switch(P[posp].tarefas[posf].j){
        case 0:
            a = a+0;
            break;
        case 1:
            a = a+1;
            break;
        case 2:
            a = a+2;
            break;
        case 3:
            a = a+3;
            break;
        }
        break;
    }

    switch(F[posf].produtividade){
    case 0:
        switch(P[posp].urgencia){
        case 0:
            a = a+0;
            break;
        case 1:
            a = a+0;
            break;
        case 2:
            ZERO = true;
            break;
        case 3:
            INFI = true;
            break;
        }
        break;
    case 1:
        switch(P[posp].urgencia){
        case 0:
            a++;
            break;
        case 1:
            a++;
            break;
        case 2:
            a = a+0;
            break;
        case 3:
            ZERO = true;
            break;
        }
        break;
    case 2:
        switch(P[posp].urgencia){
        case 0:
            a++;
            break;
        case 1:
            a = a+2;
            break;
        case 2:
            a = a+2;
            break;
        case 3:
            a = a+2;
            break;
        }
        break;
    case 3:
        switch(P[posp].urgencia){
        case 0:
            a++;
            break;
        case 1:
            a = a+2;
            break;
        case 2:
            a = a+3;
            break;
        case 3:
            a = a+5;
            break;
        }
        break;
    }

    if(INFI)    return INFINITO;
    if(ZERO)    return 0;
    if(UM)      return 1;
                return a;
}


/*
FUN��O QUE MONTA A TABELA PARA O M�TODO DO LEIL�O PARA ALOCAR FUNCION�RIOS �S TAREFAS DOS PROJETOS
    ->QUANTIDADE DE LINHAS = N� DE FUNCION�RIOS * N� DE SPLITS
    ->QUANTIDADE DE COLUNAS: TOTAL DE TAREFAS
        *SE TEM MAIS TAREFAS DO QUE FUNCION�RIOS DISPON�VEIS EM ALGUM SPLIT, O ALGORITMO N�O RODA
        *SE TEM MAIS FUNCION�RIOS DISPON�VEIS DO QUE TAREFAS, ENT�O HAVER� COLUNAS EXTRAS PARA QUE A TABELA FIQUE EQUILIBRADA.
            TODO FUNCION�RIO QUE FOR ALOCADO PARA UMA COLUNA EXTRA, SIGNIFICA QUE NA VERDADE ELE N�O FOI ALOCADO
        *SE A QUANTIDADE DE FUNCION�RIOS SPLITADOS FOR EXATAMENTE IGUAL � QUANTIDADE DE TAREFAS, O ALGORITMO RODA NORMAL
AP�S A ALOCA��O, A FUN��O ATUALIZA OS STATUS DOS FUNCION�RIOS ALOCADOS E DAS TAREFAS ALOCADAS
*/
bool Splitar(Funcionario *F, Projeto *P, int f, int p){
    int MaiorSplit = 0;
    int qTar = 0;
    int linha, coluna, linhaAUX;
    int a, b, c, d;
    bool achou, aux;

    for(int i=0; i<p; i++){//CALCULA O MAIOR SPLIT MAXIMO ENTRE OS PROJETOS
        if(P[i].splitMAX > MaiorSplit)
            MaiorSplit = P[i].splitMAX;
    }

    for(int i=0; i<p; i++){
        for(int j=0; j<P[i].quantTarefas; j++){
            qTar++;
        }
    }

    linha = f*MaiorSplit;
    if(f*MaiorSplit > qTar) coluna = linha;
    else                    coluna = qTar;

    double **tabela = AlocaMatriz_Double(linha, coluna);
    //printf("linhas: %d\tcolunas:%d\n\n\n", linha, coluna);

    for(int i=0; i<linha; i++){
        for(int j=0; j<coluna; j++){
            tabela[i][j] = INFINITO;
        }
    }

    a = 0;
    for(int i=0; i< p; i++){ //projeto i
        for(int j=0; j< P[i].quantTarefas; j++){//tarefa j do projeto i
            b = P[i].tarefas[j].split - 1;
            for(int k=b*f; k< b*f + f; k++){//funcionario k - b*f no split b
                //printf("a: %d\tj: %d\tb: %d\tk: %d\n", a, j, b, k);
                achou = false;

                if((F[k - b*f].flagDisponivel == false) && (F[k - b*f].previsaoSplit > b)){
                    achou = true;//CAI AQUI SE O FUNCIONARIO EST� INDISPONIVEL NOO SPLIT b
                }
                if(b < F[k - b*f].splitAlocadoMAX){
                    if(F[k - b*f].splitsDisponiveis[b] != -1){
                        achou = true; //CAI AQUI SE O FUNCION�RIO JA EST� ALOCADO PARA ALGUMA OUTRA TAREFA DE OUTRO PROJETO
                                     //SE FOR O CASO, N�O PODE APLICAR A FUN��O DE PESO PARA ESSE FUNCIONARIO NESSE SPLIT
                    }
                }

                if(!achou){
                    //FUN��O DE PESO PARA O FUNCIONARIO k - b*f NO SPLIT b, NA TAREFA j DO PROJETO i
                    tabela[k][a] = Quantificar(F, P, k - b*f, i, j);
                }
            }
            a++;
        }
        //a = a + P[i].quantTarefas;
    }

    FILE *arq = fopen("relatorio.txt", "w");
    if(arq == NULL){
        return NULL;
    }

    a = 0;
    fprintf(arq, "\t");
    for(int i=0; i< p; i++){
        for(int j=0; j< P[i].quantTarefas; j++){
            fprintf(arq, "\t%d", a);
            a++;
        }
    }
    fprintf(arq, "\n\t");

    for(int i=0; i< p; i++){
        for(int j=0; j< P[i].quantTarefas; j++){
            fprintf(arq, "\tP%dT%d", i+1, j+1);
        }
    }

    b = 0;
    for(int s=0; s<MaiorSplit; s++){
        fprintf(arq, "\n");
        for(int k=s*f; k<s*f + f; k++){
            fprintf(arq, "\n%d\tS%dF%d\t", b, s+1, (k+1) - s*f);
            b++;

            a = 0;
            for(int i=0; i< p; i++){
                for(int j=0; j< P[i].quantTarefas; j++){
                    fprintf(arq, "%.1lf\t", tabela[k][a]);
                    a++;
                }
            }
        }
    }
    fprintf(arq, "\n\n");
    fclose(arq);

    for(int s=0; s<MaiorSplit; s++){
        a = 0;
        for(int i=0; i< p; i++){
            for(int j=0; j< P[i].quantTarefas; j++){
                if (P[i].tarefas[j].split == s+1){
                    a++;
                }
            }
        }
        b = 0;
        for(int k=s*f; k< s*f + f; k++){
            for(int m=0; m<qTar; m++){
                if(tabela[k][m] > INFINITO){
                    b++;
                    m = qTar+1;
                }
            }
        }
        //printf("a: %d\tb: %d\n", a, b);
        if(a > b){
            printf("\nERRO! Poucos funcionarios disponiveis no Split %d.\nImpossivel alocar corretamente.", s+1);
            LiberaMatriz_Double(linha, coluna, tabela);
            return false;
        }
    }

    linhaAUX = 0;
    for(int i=0; i<linha; i++){
        for(int j=0; j<coluna; j++){
            if(tabela[i][j] != INFINITO){
                j = coluna +1;
                linhaAUX++;
            }
        }
    }
    a = 0;
    double **tabelaAUX = AlocaMatriz_Double(linhaAUX, coluna);
    for(int i=0; i<linha; i++){
        achou = false;
        for(int j=0; j<coluna; j++){
            if(tabela[i][j] != INFINITO){
                achou = true;
                j = coluna +1;
            }
        }
        if(achou){
            for(int j=0; j<coluna; j++){
                tabelaAUX[a][j] = tabela[i][j];
            }
            a++;
        }

    }

    int *AlocacaoAUX = Leilao_Alocacao(tabelaAUX, linhaAUX, coluna, false);
    int *Alocacao = AlocaVetor_Int(linha);

    b = 0;
    for(int i=0; i<linha; i++){
        a = 0;
        aux = true;
        for(int x=0; x< p; x++){
            for(int y=0; y< P[x].quantTarefas; y++){
                if(tabela[i][a] > INFINITO){
                    aux = false;
                    x = p+1;
                    y = P[x].quantTarefas+1;
                }
                a++;
            }
        }
        if(aux){
            Alocacao[i] = qTar+1;
        }
        else{
            Alocacao[i] = AlocacaoAUX[b];
            b++;
        }

    }

    arq = fopen("relatorio.txt", "a");
    if(arq == NULL){
        return NULL;
    }

    ApagaTela();
    printf("->Quantidade de funcionarios: %d\n", f);
    printf("->Quantidade de tarefas: %d\n", qTar);
    printf("->Quantidade de splits: %d\n\n", MaiorSplit);
    printf("->RESULTADO: \n");
    fprintf(arq, "->RESULTADO: \n");
        for(int i=0; i<linha; i++){
            if((Alocacao[i] > qTar-1)||(tabela[i][Alocacao[i]] <= INFINITO)){
                fprintf(arq, "In %d -> Nao alocado\n", i);
                printf("In %d -> Nao alocado\n", i);
            }
            else{
                fprintf(arq, "In %d -> Out %d\tPeso: %lf\n", i, Alocacao[i], tabela[i][Alocacao[i]]);
                printf("In %d -> Out %d\tPeso: %lf\n", i, Alocacao[i], tabela[i][Alocacao[i]]);
            }
    }
    fclose(arq);

    a = 0;
    for(int i=0; i< p; i++){
        P[i].alocado = true;
        P[i].splitPass = 0;
        for(int j=0; j< P[i].quantTarefas; j++){
            for(int m=0; m < linha; m++){
                if(Alocacao[m] == a){
                    P[i].tarefas[j].idAlocado = F[m % f].matricula;
                    P[i].tarefas[j].status = 1;
                    m = linha+1;
                }
            }
            a++;
        }
    }

    for(int i=0; i<f; i++){
        b = F[i].splitAlocadoMAX;
        c = 0;
        if(F[i].splitsDisponiveis == NULL){
            F[i].splitsDisponiveis = AlocaVetor_Int(qTar);
            for(int j=0; j< qTar; j++){
                F[i].splitsDisponiveis[j] = -1;
            }
        }
        for(int m=i; m < linha; m=m+f){
            c++;
            if((Alocacao[m] > qTar-1)||(tabela[m][Alocacao[m]] <= INFINITO)){
                if(c > b)
                    F[i].splitsDisponiveis[c-1] = -1;
            }
            else{
                a = 0;
                for(int x=0; x<p; x++){
                    for(int y=0; y< P[x].quantTarefas; y++){
                        if(Alocacao[m] == a){
                            a = x;
                            a--;
                            y = P[x].quantTarefas+1;
                            x = p+1;
                        }
                        a++;
                    }
                }
                if(c > b)
                    F[i].splitsDisponiveis[c-1] = P[a].id;
            }
        }
        if(c > b)
            F[i].splitAlocadoMAX = c;
    }

    LiberaMatriz_Double(linha, coluna, tabela);
    LiberaMatriz_Double(linhaAUX, coluna, tabelaAUX);
    free(AlocacaoAUX);
    free(Alocacao);
    return true;

}

//FUN��O QUE FAZ UM BACKUP DOS ARQUIVOS DE ENTRADA
void Faz_BKP(char *NomeF, Funcionario *F, int f, char *NomeP, Projeto *P, int p){
    char nome[MAX_STRING];

    strcpy(nome, "_BKP_");
    strcat(nome, NomeF);
    CriaArqFuncionario(nome, F, f);


    strcpy(nome, "_BKP_");
    strcat(nome, NomeP);
    CriaArqProjeto(nome, P, p);
}



int main(int argc , char *argv[]){
    char ArqFun[MAX_STRING];
    char ArqProj[MAX_STRING];
    int f, p;
    bool AchaErro;

    strcpy(ArqFun, "funcionarios.txt");
    Funcionario *F = LerArqFuncionario(ArqFun, &f);

    strcpy(ArqProj, "projetos.txt");
    Projeto *P = LerArqProjeto(ArqProj, &p);

    Faz_BKP(ArqFun, F, f, ArqProj, P, p);

    RelatorioFuncionarios(F, f);

    RelatorioProjetos(P, p);

    AchaErro = Splitar(F, P, f, p);
    if(AchaErro)    printf("\n\n->Alocacao realizada com sucesso. ");
    else            printf("\n\n->Alocacao nao foi realizada. ");

    RelatorioFuncionarios(F, f);

    RelatorioProjetos(P, p);

    strcpy(ArqFun, "funcionarios_out.txt");
    CriaArqFuncionario(ArqFun, F, f);

    strcpy(ArqProj, "projetos_out.txt");
    CriaArqProjeto(ArqProj, P, p);

    LiberaTipoFuncionario(F, f);
    LiberaTipoProjeto(P, p);

    printf("\n->Aperte uma tecla para finalizar. ");
}
