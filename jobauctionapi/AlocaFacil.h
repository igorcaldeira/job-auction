#ifndef ALOCAFACIL_H_INCLUDED
#define ALOCAFACIL_H_INCLUDED

double **AlocaMatriz_Double(int linhas, int colunas){
    double **v;
    if (linhas < 1 || colunas < 1) {
        printf ("** Erro: Parametro invalido **\n");
        return (NULL);
    }
    v = (double **) calloc (linhas, sizeof(double *));
    if (v == NULL) {
        printf ("** Erro: Memoria Insuficiente **");
        return (NULL);
    }

    for (int i = 0; i < linhas; i++ ){
        v[i] = (double*) calloc (colunas, sizeof(double));
        if (v[i] == NULL){
            printf ("** Erro: Memoria Insuficiente **");
            return (NULL);
        }
    }
    return v;
}

double *AlocaVetor_Double(int tamanho){
    double *v;
    if(tamanho < 1){
        printf ("** Erro: Parametro invalido **\n");
        return (NULL);
    }
    v = (double*) calloc (tamanho, sizeof(double));
    if (v == NULL) {
        printf ("** Erro: Memoria Insuficiente **");
        return (NULL);
    }
    return v;
}

double *LiberaVetor_Double(int tamanho, double *variavel){
    if (variavel == NULL) return NULL;
    if(tamanho < 1){
        printf ("** Erro: Parametro invalido **\n");
        return (variavel);
    }
    free(variavel);
    return NULL;
}

double **LiberaMatriz_Double(int linhas, int colunas, double **variavel){
    if (variavel == NULL) return NULL;

    if (linhas < 1 || colunas < 1){
        printf ("** Erro: Parametro invalido **\n");
        return (variavel);
    }
    for(int i=0; i<linhas; i++) free(variavel[i]);
    free(variavel);
    return NULL;
}











int **AlocaMatriz_Int(int linhas, int colunas){
    int **v;
    if (linhas < 1 || colunas < 1) {
        printf ("** Erro: Parametro invalido **\n");
        return (NULL);
    }
    v = (int **) calloc (linhas, sizeof(int *));
    if (v == NULL) {
        printf ("** Erro: Memoria Insuficiente **");
        return (NULL);
    }

    for (int i = 0; i < linhas; i++ ){
        v[i] = (int*) calloc (colunas, sizeof(int));
        if (v[i] == NULL){
            printf ("** Erro: Memoria Insuficiente **");
            return (NULL);
        }
    }
    return v;
}

int *AlocaVetor_Int(int tamanho){
    int *v;
    if(tamanho < 1){
        printf ("** Erro: Parametro invalido **\n");
        return (NULL);
    }
    v = (int*) calloc (tamanho, sizeof(int));
    if (v == NULL) {
        printf ("** Erro: Memoria Insuficiente **");
        return (NULL);
    }
    return v;
}

int *LiberaVetor_Int(int tamanho, int *variavel){
    if (variavel == NULL) return NULL;
    if(tamanho < 1){
        printf ("** Erro: Parametro invalido **\n");
        return (variavel);
    }
    free(variavel);
    return NULL;
}

int **LiberaMatriz_Int(int linhas, int colunas, int **variavel){
    if (variavel == NULL) return NULL;

    if (linhas < 1 || colunas < 1){
        printf ("** Erro: Parametro invalido **\n");
        return (variavel);
    }
    for(int i=0; i<linhas; i++) free(variavel[i]);
    free(variavel);
    return NULL;
}










float **AlocaMatriz_Float(int linhas, int colunas){
    float **v;
    if (linhas < 1 || colunas < 1) {
        printf ("** Erro: Parametro invalido **\n");
        return (NULL);
    }
    v = (float **) calloc (linhas, sizeof(float *));
    if (v == NULL) {
        printf ("** Erro: Memoria Insuficiente **");
        return (NULL);
    }

    for (int i = 0; i < linhas; i++ ){
        v[i] = (float*) calloc (colunas, sizeof(float));
        if (v[i] == NULL){
            printf ("** Erro: Memoria Insuficiente **");
            return (NULL);
        }
    }
    return v;
}

float *AlocaVetor_Float(int tamanho){
    float *v;
    if(tamanho < 1){
        printf ("** Erro: Parametro invalido **\n");
        return (NULL);
    }
    v = (float*) calloc (tamanho, sizeof(float));
    if (v == NULL) {
        printf ("** Erro: Memoria Insuficiente **");
        return (NULL);
    }
    return v;
}

float *LiberaVetor_Float(int tamanho, float *variavel){
    if (variavel == NULL) return NULL;
    if(tamanho < 1){
        printf ("** Erro: Parametro invalido **\n");
        return (variavel);
    }
    free(variavel);
    return NULL;
}

float **LiberaMatriz_Float(int linhas, int colunas, float **variavel){
    if (variavel == NULL) return NULL;

    if (linhas < 1 || colunas < 1){
        printf ("** Erro: Parametro invalido **\n");
        return (variavel);
    }
    for(int i=0; i<linhas; i++) free(variavel[i]);
    free(variavel);
    return NULL;
}







char **AlocaMatriz_Char(int linhas, int colunas){
    char **v;
    if (linhas < 1 || colunas < 1) {
        printf ("** Erro: Parametro invalido **\n");
        return (NULL);
    }
    v = (char **) calloc (linhas, sizeof(char *));
    if (v == NULL) {
        printf ("** Erro: Memoria Insuficiente **");
        return (NULL);
    }

    for (int i = 0; i < linhas; i++ ){
        v[i] = (char*) calloc (colunas, sizeof(char));
        if (v[i] == NULL){
            printf ("** Erro: Memoria Insuficiente **");
            return (NULL);
        }
    }
    return v;
}

char *AlocaVetor_Char(int tamanho){
    char *v;
    if(tamanho < 1){
        printf ("** Erro: Parametro invalido **\n");
        return (NULL);
    }
    v = (char*) calloc (tamanho, sizeof(char));
    if (v == NULL) {
        printf ("** Erro: Memoria Insuficiente **");
        return (NULL);
    }
    return v;
}

char *LiberaVetor_Char(int tamanho, char *variavel){
    if (variavel == NULL) return NULL;
    if(tamanho < 1){
        printf ("** Erro: Parametro invalido **\n");
        return (variavel);
    }
    free(variavel);
    return NULL;
}

char **LiberaMatriz_Char(int linhas, int colunas, char **variavel){
    if (variavel == NULL) return NULL;

    if (linhas < 1 || colunas < 1){
        printf ("** Erro: Parametro invalido **\n");
        return (variavel);
    }
    for(int i=0; i<linhas; i++) free(variavel[i]);
    free(variavel);
    return NULL;
}












long int **AlocaMatriz_LongInt(int linhas, int colunas){
    long int **v;
    if (linhas < 1 || colunas < 1) {
        printf ("** Erro: Parametro invalido **\n");
        return (NULL);
    }
    v = (long int **) calloc (linhas, sizeof(long int *));
    if (v == NULL) {
        printf ("** Erro: Memoria Insuficiente **");
        return (NULL);
    }

    for (int i = 0; i < linhas; i++ ){
        v[i] = (long int*) calloc (colunas, sizeof(long int));
        if (v[i] == NULL){
            printf ("** Erro: Memoria Insuficiente **");
            return (NULL);
        }
    }
    return v;
}

long int *AlocaVetor_LongInt(int tamanho){
    long int *v;
    if(tamanho < 1){
        printf ("** Erro: Parametro invalido **\n");
        return (NULL);
    }
    v = (long int*) calloc (tamanho, sizeof(long int));
    if (v == NULL) {
        printf ("** Erro: Memoria Insuficiente **");
        return (NULL);
    }
    return v;
}

long int *LiberaVetor_LongInt(int tamanho, long int *variavel){
    if (variavel == NULL) return NULL;
    if(tamanho < 1){
        printf ("** Erro: Parametro invalido **\n");
        return (variavel);
    }
    free(variavel);
    return NULL;
}

long int **LiberaMatriz_LongInt(int linhas, int colunas, long int **variavel){
    if (variavel == NULL) return NULL;

    if (linhas < 1 || colunas < 1){
        printf ("** Erro: Parametro invalido **\n");
        return (variavel);
    }
    for(int i=0; i<linhas; i++) free(variavel[i]);
    free(variavel);
    return NULL;
}

#endif // ALOCAFACIL_H_INCLUDED
