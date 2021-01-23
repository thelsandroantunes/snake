//========== CONSTANTES =========//

#define OBS_RAIZES 30
#define OBS_LINHAS 30
#define OBS_BLOK 5
#define OFFSET_X 2

#define LINHA_VAZIA ' '
#define LINHA_VALIDA '+'
#define BORDA_LINHA 177
#define OBST_LINHA  176

#define CABEC_COB 178
#define BODY_COB  178
#define MACA_CHAR '@'

#define SETAS     0xE0
#define KEY_UP    0x48
#define KEY_DOWN  0x50
#define KEY_LEFT  0x4B
#define KEY_RIGTH 0x4D

#define D_TOP direcao_cima()
#define D_DOW direcao_baixo()
#define D_LEF direcao_esquerda()
#define D_RHT direcao_direita()

//============= TAD's ============//
struct linha{
    char elm;

    int pos_x;
    int pos_y;

    struct linha *prox;
};

typedef struct linha LINHA;

struct direcao{
    int inc_x;
    int inc_y;
};

typedef struct direcao DIRECAO;

struct cobra{
    DIRECAO *direcao;
    LINHA *final;
    LINHA *cabec;
};

typedef struct cobra COBRA;

struct maca{
    LINHA *linha;
};

typedef struct maca MACA;

struct no{
    void *val;

    struct No *prox;
};

typedef struct no No;

struct fila{
    No *ini;
    No *fim;
};

typedef struct fila FILA;

//=========== VARIÁVEIS ==========//
int DIMENSAO_X, DIMENSAO_Y;

//========== ASSINATURAS =========//
COBRA   *cria_cobra();
MACA    *cria_maca();
LINHA   *cria_linha();
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
void atualiza_posicao(LINHA *linha, DIRECAO *direcao);
void atualiza_maca(COBRA *cobra, char **matriz);
void atualiza_area_trabalho(char **matriz, COBRA *cobra);

char verifica_obstaculo(LINHA *linha, DIRECAO *direcao);
int  verifica_barreiras(LINHA *linha, char **matriz);
bool verifica_colisao(char elm);
bool verifica_maca(char elm);
bool verifica_cobra(char elm);
bool verifica_fila_vazia(FILA *fila);

char **copiar_matriz(char **b_matriz);
LINHA *copiar_linha(LINHA *b_linha);

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

LINHA     *sorteia_linha(DIRECAO *direcao);
LINHA     **sorteia_obstaculos();
MACA    *sorteia_maca();
DIRECAO *sorteia_direcao();

void set_char_by_cursor(char c,int x,int y);
char get_char_by_cursor(int x,int y);
void get_size_window(int *col, int *linha);
void hide_cursor(bool hide);
void run_cursor();
void maximize_window();
