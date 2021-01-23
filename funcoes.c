#define _WIN32_WINNT 0x0500

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <stdbool.h>
#include <windows.h>
#include <dos.h>
#include "cabecalho.h"

//====== ALOCACAO DE TAD's ========//
COBRA *cria_cobra(){

    int pos_x = (int)(DIMENSAO_X / 3);

    COBRA *cobra = (COBRA*) malloc(sizeof(COBRA));
    cobra->cabec = (LINHA*) malloc(sizeof(LINHA));
    cobra->cabec->elm = CABEC_COB;
    cobra->cabec->prox = NULL;
    cobra->cabec->pos_x = pos_x - pos_x % OFFSET_X;
    cobra->cabec->pos_y = (int)(DIMENSAO_Y / 2);
    cobra->final = cobra->cabec;
    cobra->direcao = cria_direcao();
    cobra->direcao->inc_x = OFFSET_X;
    cobra->direcao->inc_y = 0;

    return cobra;
}

MACA *cria_maca(){

    int pos_x = (int)(DIMENSAO_X / 2);

    MACA *maca = (MACA*) malloc(sizeof(MACA));
    maca->linha = (LINHA*) malloc(sizeof(LINHA));
    maca->linha->elm = MACA_CHAR;
    maca->linha->prox = NULL;
    maca->linha->pos_x = pos_x - pos_x % OFFSET_X;
    maca->linha->pos_y = (int)(DIMENSAO_Y / 2);

    return maca;
}

char **cria_matriz(){

    char **m = (char**) malloc(sizeof(char*) * DIMENSAO_X + OFFSET_X);

    int idx_x, idx_y;
    for(idx_x = 0; idx_x < DIMENSAO_X + OFFSET_X; idx_x++){
        m[idx_x] = (char*) malloc(sizeof(char) * DIMENSAO_Y);
        for(idx_y = 0; idx_y < DIMENSAO_Y; idx_y++){
            m[idx_x][idx_y] = LINHA_VAZIA;
        }
    }

    return m;
}

DIRECAO *cria_direcao(){

    DIRECAO *direcao = (DIRECAO*)malloc(sizeof(DIRECAO));

    direcao->inc_x = 0;
    direcao->inc_y = 0;

    return direcao;
}

LINHA *cria_linha(){
    LINHA *lin = (LINHA*) malloc(sizeof(LINHA));

    lin->prox = NULL;

    return lin;
}

FILA *cria_fila(){
    FILA *fila = (FILA*) malloc(sizeof(FILA));

    fila->ini = NULL;
    fila->fim = NULL;

    return fila;
}

//============= DESENHOS ============//
void imprime_cobra(COBRA *cobra){

    LINHA *r = cobra->final;

    while(r != NULL){
        set_char_by_cursor(r->elm,r->pos_x,r->pos_y);

        r = r->prox;
    }
}

void imprime_maca(MACA *maca){

    LINHA * r = maca->linha;

    set_char_by_cursor(r->elm,r->pos_x,r->pos_y);
}

void imprime_borda(char **matriz){

    //Borda X
    int idx_x, idx_y, off_x, dmi_x = DIMENSAO_X - DIMENSAO_X % OFFSET_X;
    for(idx_x = 0; idx_x < dmi_x; idx_x++){
        matriz[idx_x][0            ] = BORDA_LINHA;
        matriz[idx_x][DIMENSAO_Y -1] = BORDA_LINHA;

        set_char_by_cursor(BORDA_LINHA,idx_x,0);
        set_char_by_cursor(BORDA_LINHA,idx_x, DIMENSAO_Y -1);
    }

    //Borda Y
    for(idx_y = 0; idx_y < DIMENSAO_Y; idx_y++){
        for(off_x = 0; off_x < OFFSET_X; off_x++){
            matriz[off_x          ][idx_y] = BORDA_LINHA;
            matriz[dmi_x -off_x -1][idx_y] = BORDA_LINHA;

            set_char_by_cursor(BORDA_LINHA,off_x,idx_y);
            set_char_by_cursor(BORDA_LINHA,dmi_x - off_x -1, idx_y);
        }
    }

}

void imprime_obstaculo(char **matriz){

    LINHA **obstaculos = sorteia_obstaculos();

    int idx_obs, idx_offset;
    for(idx_obs = 0; idx_obs < OBS_RAIZES; idx_obs++){
        LINHA *obs = obstaculos[idx_obs];

        while(obs != NULL){

            //Iprime nos offsets
            for(idx_offset = 0; idx_offset < OFFSET_X; idx_offset++){
                matriz[obs->pos_x + idx_offset][obs->pos_y] = obs->elm;

                set_char_by_cursor(obs->elm,obs->pos_x + idx_offset, obs->pos_y);
            }

            obs = obs->prox;
        }
    }
}

void imprime_mensagem(char *mensagem){

    int idx_x, idx_y, idx_offset, size = 1;

    //Calcula tamanho da string
    while(mensagem[size] != '\0')
        size++;

    int div_y = DIMENSAO_Y / 2, s_x = (DIMENSAO_X / 2) - (size / 2);

    //Borda X
    for(idx_x = -2; idx_x < size +2; idx_x++){

        //Iprime nos offsets
        for(idx_offset = 0; idx_offset < OFFSET_X; idx_offset++){
            int pos_x = s_x + idx_x + idx_offset;

            //Borda externa
            set_char_by_cursor(178,pos_x, div_y -2);
            set_char_by_cursor(178,pos_x, div_y +2);

            //Borda interna
            set_char_by_cursor(LINHA_VAZIA,pos_x, div_y -1);
            set_char_by_cursor(LINHA_VAZIA,pos_x, div_y);
            set_char_by_cursor(LINHA_VAZIA,pos_x, div_y +1);
        }
    }

    //Mensagem
    set_char_by_cursor(LINHA_VAZIA,s_x, div_y);
    puts(mensagem);
    set_char_by_cursor(LINHA_VAZIA,s_x + size, div_y);
}

//=========== ATUALIZAÇÕES ==========//
void atualiza_cobra(COBRA *cobra){

    LINHA *fim = cobra->final;
    LINHA *cab = cobra->cabec;
    int idx_offset;

    for(idx_offset = 0; idx_offset < OFFSET_X; idx_offset++){
        //Define corpo onde está a cabeça
        set_char_by_cursor(BODY_COB,cab->pos_x + idx_offset, cab->pos_y);

        //Limpa ultima posicao
        set_char_by_cursor(LINHA_VAZIA,fim->pos_x + idx_offset, fim->pos_y);
    }

    while(fim != NULL){

        if(fim->prox != NULL){
            fim->pos_x = fim->prox->pos_x;
            fim->pos_y = fim->prox->pos_y;
        }

        fim = fim->prox;
    }

    //Atualiza e imprime cabeça
    cab->pos_x += cobra->direcao->inc_x;
    cab->pos_y += cobra->direcao->inc_y;

    for(idx_offset = 0; idx_offset < OFFSET_X; idx_offset++){
        set_char_by_cursor(cab->elm,cab->pos_x + idx_offset, cab->pos_y);
    }

}

void atualiza_direcao(int tecla, DIRECAO *direcao){

    switch(tecla){
        case KEY_UP:
            if(direcao->inc_y > 0)
                return;
            direcao->inc_x = 0;
            direcao->inc_y =-1;
            break;
        case KEY_DOWN:
            if(direcao->inc_y < 0)
                return;
            direcao->inc_x = 0;
            direcao->inc_y = 1;
            break;
        case KEY_LEFT:
            if(direcao->inc_x > 0)
                return;
            direcao->inc_x =-OFFSET_X;
            direcao->inc_y = 0;
            break;
        case KEY_RIGTH:
            if(direcao->inc_x > 0)
                return;
            direcao->inc_x = OFFSET_X;
            direcao->inc_y = 0;
            break;
    }
}

void atualiza_posicao(LINHA *linha, DIRECAO *direcao){
    linha->pos_x += direcao->inc_x;
    linha->pos_y += direcao->inc_y;
}

void atualiza_maca(COBRA *cobra, char **matriz){

    //Regras de aceitação
    MACA *maca;
    int p_x, p_y;
    char elm;

    do{
        maca = sorteia_maca();
        elm = matriz[maca->linha->pos_x][maca->linha->pos_y];
    }while(elm != LINHA_VALIDA || verifica_barreiras(maca->linha,matriz) >= 3);

    imprime_maca(maca);
}

void atualiza_area_trabalho(char **matriz, COBRA *cobra){
    FILA *level_in;
    FILA *level_ex;

    level_in = cria_fila();
    alimenta_fila(level_in, copiar_linha(cobra->cabec));//Raiz

    No *no;
    LINHA *linha;
    do{
        level_ex = cria_fila();

        no = level_in->ini;
        while(no != NULL){
            linha = (LINHA*) no->val;

            linha->prox = (LINHA*) malloc(sizeof(LINHA) * 4);

            int idx_i;
            char elm;
            LINHA *dir;
            for(idx_i = 0; idx_i < 4; idx_i++){
                dir = &linha->prox[idx_i];

                dir->pos_x = linha->pos_x;
                dir->pos_y = linha->pos_y;

                if(idx_i == 0)atualiza_posicao(dir, D_DOW );
                if(idx_i == 1)atualiza_posicao(dir, D_TOP );
                if(idx_i == 2)atualiza_posicao(dir, D_LEF );
                if(idx_i == 3)atualiza_posicao(dir, D_RHT );

                elm = matriz[dir->pos_x][dir->pos_y];

                if(elm == LINHA_VAZIA){
                    //set_char_by_cursor(LINHA_VALIDA,dir->pos_x,dir->pos_y);
                    matriz[dir->pos_x][dir->pos_y] = LINHA_VALIDA;
                    alimenta_fila(level_ex, dir);
                }
            }

            //free(rows);
            no = no->prox;
        }
        //libera_fila(level_in->prox);
        level_in = level_ex;
    }while(!verifica_fila_vazia(level_ex));

    // libera_fila(level_in);
    // libera_fila(level_ex);
}

//========= ELEMENTOS =========//
void alimenta_cobra(COBRA *cobra){

    LINHA *fim = cobra->final;
    LINHA *novo = (LINHA*) malloc(sizeof(LINHA));

    novo->pos_x = fim->pos_x;
    novo->pos_y = fim->pos_y;
    novo->elm = BODY_COB;
    novo->prox = fim;

    cobra->final = novo;
}

void alimenta_fila(FILA *fila, void *val){
    No *no = (No*) malloc(sizeof(No));

    no->val = val;
    no->prox = NULL;

    if(verifica_fila_vazia(fila)){
        fila->ini = no;
        fila->fim = no;
    }else{
        fila->fim->prox = no;
        fila->fim = no;
    }
}

//======== VALIDAÇÕES =========//
char verifica_obstaculo(LINHA *linha, DIRECAO *direcao){

    int new_pos_x = linha->pos_x + direcao->inc_x;
    int new_pos_y = linha->pos_y + direcao->inc_y;

    return get_char_by_cursor(new_pos_x, new_pos_y);
}

int  verifica_barreiras(LINHA *linha, char **matriz){
    int barreiras = 0;
    if(matriz[linha->pos_x + D_LEF->inc_x][linha->pos_y + D_LEF->inc_y] != LINHA_VALIDA)
        barreiras++;
    if(matriz[linha->pos_x + D_RHT->inc_x][linha->pos_y + D_RHT->inc_y] != LINHA_VALIDA)
        barreiras++;
    if(matriz[linha->pos_x + D_TOP->inc_x][linha->pos_y + D_TOP->inc_y] != LINHA_VALIDA)
        barreiras++;
    if(matriz[linha->pos_x + D_DOW->inc_x][linha->pos_y + D_DOW->inc_y] != LINHA_VALIDA)
        barreiras++;

    return barreiras;
}

bool verifica_colisao(char elm){
    return (elm != LINHA_VAZIA && elm != MACA_CHAR);
}

bool verifica_maca(char elm){
    return (elm == MACA_CHAR);
}

bool verifica_cobra(char elm){
    return (elm == (char)CABEC_COB || elm == (char)BODY_COB);
}

bool verifica_fila_vazia(FILA *fila){
    return (fila->ini == NULL && fila->fim == NULL);
}

//============== CÓPIAS =============//
char **copiar_matriz(char **b_matriz){
    char **n_matriz = (char **) malloc(sizeof(char*) * DIMENSAO_X);

    int idx_x, idx_y;
    for(idx_x = 0; idx_x < DIMENSAO_X; idx_x++){
        n_matriz[idx_x] = (char *) malloc(sizeof(char) * DIMENSAO_Y);
        for(idx_y = 0; idx_y < DIMENSAO_Y; idx_y++){
            n_matriz[idx_x][idx_y] = b_matriz[idx_x][idx_y];
        }
    }
    return n_matriz;
}

LINHA *copiar_linha(LINHA *b_linha){
    LINHA *lin = cria_linha();

    lin->pos_x = b_linha->pos_x;
    lin->pos_y = b_linha->pos_y;

    return lin;
}

//========== LIBERANDO =========//
void libera_matriz(char **b_matriz){
    int idx_x;
    for(idx_x = 0; idx_x < DIMENSAO_X + OFFSET_X; idx_x++){
        free(b_matriz[idx_x]);
    }
    free(b_matriz);
}

void libera_cobra(COBRA *cobra){
    LINHA *no = cobra->final;
    LINHA *an;

    while(no != NULL){
        an = no;
        no = no->prox;
        free(an);
    }
}

void libera_maca(MACA *maca){
    free(maca);
}

void libera_fila(FILA *fila){
    No *no = fila->ini;
    No *an;

    while(no != NULL){
        an = no;
        no = no->prox;
        free(an);
    }
}

void libera_direcao(DIRECAO *direcao){
    free(direcao);
}

//========= DIREÇÕES =========//
DIRECAO *direcao_cima(){
    DIRECAO *dir = cria_direcao();
    atualiza_direcao(KEY_UP, dir);
    return dir;
}
DIRECAO *direcao_baixo(){
    DIRECAO *dir = cria_direcao();
    atualiza_direcao(KEY_DOWN, dir);
    return dir;
}
DIRECAO *direcao_esquerda(){
    DIRECAO *dir = cria_direcao();
    atualiza_direcao(KEY_LEFT, dir);
    return dir;
}
DIRECAO *direcao_direita(){
    DIRECAO *dir = cria_direcao();
    atualiza_direcao(KEY_RIGTH, dir);
    return dir;
}

//========== SORTEIOS ==========//
MACA *sorteia_maca(){

    MACA *maca = cria_maca();
    DIRECAO *direcao = cria_direcao();
    maca->linha = sorteia_linha(direcao);
    maca->linha->elm = MACA_CHAR;

    return maca;
}

DIRECAO *sorteia_direcao(){

    DIRECAO *direcao = cria_direcao();
    int direcoes[] = { KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGTH };
    int idx_rnd = rand() % 4;
    atualiza_direcao(direcoes[idx_rnd], direcao);

    return direcao;
}

LINHA *sorteia_linha(DIRECAO *direcao){
    LINHA *linha = (LINHA*) malloc(sizeof(LINHA));
    linha->prox = NULL;

    do{
        linha->pos_x = rand() % DIMENSAO_X;
        linha->pos_y = rand() % DIMENSAO_Y;
        linha->pos_x = linha->pos_x - linha->pos_x % OFFSET_X;
    }while(verifica_obstaculo(linha, direcao) != LINHA_VAZIA);

    return linha;
}

LINHA **sorteia_obstaculos(){

    LINHA **obstaculos = (LINHA**) malloc(sizeof(LINHA*) * OBS_RAIZES);
    int idx_raiz = 0, idx_linha = 0;
    DIRECAO *direcao = sorteia_direcao();

    while(idx_raiz < OBS_RAIZES){
        obstaculos[idx_raiz] = sorteia_linha(cria_direcao());
        obstaculos[idx_raiz]->elm = OBST_LINHA;
        LINHA *obstaculo = obstaculos[idx_raiz];

        idx_linha = 1;
        while(idx_linha < OBS_LINHAS){
            //Alterna direção
            if((idx_linha % OBS_BLOK +1) == OBS_BLOK)
                direcao = sorteia_direcao();

            //Se bateu para
            if(verifica_obstaculo(obstaculo, direcao) != LINHA_VAZIA)
                break;

            //Novo obstáculo
            LINHA *novo = (LINHA*) malloc(sizeof(LINHA));

            novo->pos_x = obstaculo->pos_x;
            novo->pos_y = obstaculo->pos_y;
            novo->elm = OBST_LINHA;
            novo->prox = NULL;

            atualiza_posicao(novo, direcao);

            obstaculo->prox = novo;
            obstaculo = novo;

            idx_linha++;
        }

        idx_raiz++;
    }

    return obstaculos;
}

//=========== CONSOLE ==========//
void run_cursor(){
    hide_cursor(true);

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    COORD pos = {DIMENSAO_X,DIMENSAO_Y};
    DWORD esc;
    DWORD tam = csbi.dwSize.X *csbi.dwSize.Y;
    FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE),LINHA_VAZIA,tam,pos,&esc);
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);
}

void set_char_by_cursor(char c,int x,int y){
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);

    putchar(c);
}

char get_char_by_cursor(int x,int y){
    char buf[2];
    COORD coord = {x,y};
    DWORD num_read;
	HANDLE hStd = GetStdHandle(STD_OUTPUT_HANDLE);
	ReadConsoleOutputCharacter(hStd,(LPTSTR)buf,1,coord,(LPDWORD)&num_read);
	return buf[0];
}

void hide_cursor(bool hide)
{
   HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = !hide;
   SetConsoleCursorInfo(consoleHandle, &info);
}

void maximize_window()
{
    HWND consoleWindow = GetConsoleWindow();
    ShowWindow(consoleWindow, SW_MAXIMIZE);
}

void get_size_window(int *col, int *row){
    CONSOLE_SCREEN_BUFFER_INFO cmd;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cmd);
    *col = cmd.srWindow.Right - cmd.srWindow.Left +1;
    *row = cmd.srWindow.Bottom - cmd.srWindow.Top +1;
}
