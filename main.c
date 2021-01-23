
#define _WIN32_WINNT 0x0500

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <stdbool.h>
#include <windows.h>
#include <dos.h>

#include "cabecalho.h"

//============== MAIN ============//
int main(){

    //Iniciando tela
    maximize_window();
    get_size_window(&DIMENSAO_X, &DIMENSAO_Y);
    run_cursor();

    //Iniciando jogo
    bool jogando = true;
    do{
        //Iniciando variáveis
        srand(time(NULL));
        char **quadro = cria_matriz();
        COBRA *snake  = cria_cobra();
        MACA *maca    = cria_maca();
        bool vivo = true;

        //Desenhando componentes
        system("cls");
        imprime_borda(quadro);
        //TODO: sortear no meio
        imprime_maca(maca);
        imprime_cobra(snake);
        //TODO: colocar pra cima
        imprime_obstaculo(quadro);
        atualiza_area_trabalho(quadro, snake);

        //Atualização de frames
        int tecla = getch();
        do{
            if(tecla == SETAS){  //Código das setas
                tecla = getch(); //Direção da seta

                atualiza_direcao(tecla, snake->direcao);
            }

            char elm_coli = verifica_obstaculo(
                snake->cabec,
                snake->direcao);

            if(verifica_colisao(elm_coli)){
                hide_cursor(false);
                imprime_mensagem("VOCE COLIDIU!, deseja reiniciar? [s/n] ");
                scanf("%c",&tecla);
                while(tecla != 's' && tecla != 'n'){
                    imprime_mensagem("TECLA INCORRETA!, deseja reiniciar? [s/n] ");
                    scanf("%c",&tecla);
                }
                hide_cursor(true);
                jogando = tecla == 's';
                vivo = false;
                break;
            }

            if(verifica_maca(elm_coli)){
                alimenta_cobra(snake);
                atualiza_maca(snake, quadro);
            }

            atualiza_cobra(snake);
            Sleep(200);

            if(kbhit())
                tecla = getch();
        }while(vivo);

        //libera_matriz(quadro);
        libera_cobra(snake);
        libera_maca(maca);
    }while(jogando);


    hide_cursor(false);
    imprime_mensagem("FIM DE JOGO!, tecle qualquer tecla para fechar...");
    getch();

    return 0;
}

