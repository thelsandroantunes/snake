//============= TAD's ============//
typedef struct ROW{
    char elm;

    int pos_x;
    int pos_y;

    struct ROW *prox;
} ROW;

typedef struct DIRECAO{
    int inc_x;
    int inc_y;
} DIRECAO;

typedef struct COBRA{
    DIRECAO *direcao;
    ROW *final;
    ROW *cabec;
} COBRA;

typedef struct MACA{
    ROW *row;
} MACA;

typedef struct No{
    void *val;

    struct No *prox;
} No;

typedef struct FILA{
    No *ini;
    No *fim;
} FILA;

//=========== VARIÁVEIS ==========//
int DIMENSAO_X, DIMENSAO_Y;

//========== ASSINATURAS =========//
COBRA   *cria_cobra();
MACA    *cria_maca();
ROW     *cria_row();
FILA    *cria_fila();
char    **cria_matriz();
DIRECAO *cria_direcao();

void imprime_cobra(COBRA *cobra);
void imprime_maca(MACA *maca);
void imprime_borda(char **matriz);
void imprime_obstaculo(char **matriz);
void imprime_mensagem(char *mensagem);

void atualiza_direcao(int tecla, DIRECAO *direcao);
void atualiza_cobra(COBRA *cobra);
void atualiza_posicao(ROW *row, DIRECAO *direcao);
void atualiza_maca(COBRA *cobra, char **matriz);
void atualiza_area_trabalho(char **matriz, COBRA *cobra);

char verifica_obstaculo(ROW *row, DIRECAO *direcao);
int  verifica_barreiras(ROW *row, char **matriz);
bool verifica_colisao(char elm);
bool verifica_maca(char elm);
bool verifica_cobra(char elm);
bool verifica_fila_vazia(FILA *fila);

char **copiar_matriz(char **b_matriz);
ROW *copiar_row(ROW *b_row);

void libera_matriz(char **b_matriz);
void libera_cobra(COBRA *cobra);
void libera_maca(MACA *maca);
void libera_fila(FILA *fila);
void libera_direcao(DIRECAO *direcao);

void alimenta_cobra(COBRA *cobra);
void alimenta_fila(FILA *fila, void *val);

DIRECAO *direcao_cima();
DIRECAO *direcao_baixo();
DIRECAO *direcao_esquerda();
DIRECAO *direcao_direita();

ROW     *sorteia_row(DIRECAO *direcao);
ROW     **sorteia_obstaculos();
MACA    *sorteia_maca();
DIRECAO *sorteia_direcao();

void set_char_by_cursor(char c,int x,int y);
char get_char_by_cursor(int x,int y);
void get_size_window(int *col, int *row);
void hide_cursor(bool hide);
void run_cursor();
void maximize_window();
