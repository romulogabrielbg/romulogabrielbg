#include <allegro.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

#define LINHAS 11
#define COLUNAS 11
#define PESSOA 'P'
#define ONCA 'O'
#define SAIDA 'S'
#define VAZIO '.'
#define OBSTACULO '#'
#define PIXELS 90
//direcoes de movimento onca





typedef struct{  //construcao de fila para o bfs de busca da onca
	int linha, coluna, distancia;
}lista_fila;


//parametros necessarios para movimento e calculo

int mover_onca(char mapa[LINHAS][COLUNAS], int linha_onca, int coluna_onca,int linha_pessoa, int coluna_pessoa, int*nova_linha, int* nova_coluna ){ 
	
	int visitado[LINHAS][COLUNAS] = {0};
    lista_fila fila[LINHAS*COLUNAS], anterior[LINHAS][COLUNAS];
    int i, inicio = 0, fim = 0;
	
	fila[fim] = (lista_fila){linha_onca, coluna_onca, 0};
	fim++;
    visitado[linha_onca][coluna_onca] = 1;
    
    // Busca em largura
    while (inicio < fim) {
        lista_fila atual = fila[inicio++];

        // Se chegou ao destino
        if (atual.linha == linha_pessoa && atual.coluna == coluna_pessoa) {
            
									// Reconstruir o caminho a partir do destino até o início encontrando melhor trajeto
									
            int l = linha_pessoa;
            int c = coluna_pessoa;

            						// Voltar até encontrar a posição inicial da onça
            while (!(anterior[l][c].linha == linha_onca && anterior[l][c].coluna == coluna_onca)) {
                int temp_l = anterior[l][c].linha;
                int temp_c = anterior[l][c].coluna;
                l = temp_l;
                c = temp_c;
            }

            // Retorna o primeiro passo correto pois ja esta no local desejado
            *nova_linha = l;
            
            *nova_coluna = c;
            return 2;
        }

        // Verifica os 4 lados variaveis
        int vizinhos[4][2] = {
            {atual.linha - 1, atual.coluna}, // cima
            {atual.linha + 1, atual.coluna}, // baixo
            {atual.linha, atual.coluna - 1}, // esquerda
            {atual.linha, atual.coluna + 1}  // direita
        };
		// Verifica os 4 lados loop
        for ( i = 0; i < 4; i++) {
            int nl = vizinhos[i][0];
            int nc = vizinhos[i][1];

			//verifica se a alguma restricao de movimento da onca

            if (nl >= 0 && nl < LINHAS && nc >= 0 && nc < COLUNAS && !visitado[nl][nc] && mapa[nl][nc] != OBSTACULO && mapa[nl][nc]) { 

                visitado[nl][nc] = 1;
                fila[fim++] = (lista_fila){nl, nc, atual.distancia + 1};
                anterior[nl][nc] = atual;
            }
        }
    }

    // Se não encontrar caminho, onça não se move
    *nova_linha = linha_onca;
    *nova_coluna = coluna_onca;
    return 3;
}







int main() {
	
	 setlocale(LC_ALL, "Portuguese");
    char mapa[LINHAS][COLUNAS];
    int linha_onca, coluna_onca, linha_pessoa, coluna_pessoa, linha_saida, coluna_saida; // variaveis de posicao inicalmente fixas

	int i, j, linha_nova, coluna_nova; //variaveis de posicao nova da onca e loop
	
	
	int x = 0, y = 0, z = 0, t = 0, f =0; //variaveis do allegro e manipulacao do jogo
	
	

    // Inicializa a matriz
    for ( i = 0; i < LINHAS; i++) {
        for (j = 0; j < COLUNAS; j++) {
            mapa[i][j] = VAZIO;
        }
    }

    // Inicializa o gerador de números aleatórios
    srand(time(NULL));
    

	
    // Preenche aleatoriamente a matriz com o símbolo '#'
    for ( i = 0; i < LINHAS; i++) {
        for (j = 0; j < COLUNAS; j++) {
            if (rand() % 8 == 0) {
                mapa[i][j] = OBSTACULO;
                
            }
        }
    }
    
      // COLOCA 'P' DE PESSOA EM UM LOCAL DO MAPA QUE NÃO SEJA OBSTACULO
    do {
    	linha_pessoa = rand() % LINHAS;
    	coluna_pessoa = rand() % COLUNAS;
    }while (mapa[linha_pessoa][coluna_pessoa] == OBSTACULO);

    mapa[linha_pessoa][coluna_pessoa] = PESSOA;

    // COLOCA 'O' DE ONÇA EM UM LOCAL DO MAPA QUE NÃO É OBSTACULO E NEM PESSOA

    do{
	    linha_onca = rand() % LINHAS;
	    coluna_onca = rand() % COLUNAS;
    }while (mapa[linha_onca][coluna_onca] == OBSTACULO || (linha_onca == linha_pessoa && coluna_onca == coluna_pessoa));

    mapa[linha_onca][coluna_onca] = ONCA;

	// COLOCA 'S' DE SAIDA EM UMA POSIÇÃO ALEATORIA ONDE NÃO SEJA OBSTACULO, PESSOA E NEM ONÇA

    do{
        linha_saida = rand() % LINHAS;
        coluna_saida = rand() % COLUNAS;
    }while (mapa[linha_saida][coluna_saida] == OBSTACULO || (linha_saida == linha_pessoa && coluna_saida == coluna_pessoa) || (linha_saida == linha_onca && coluna_saida == coluna_onca));
    
	mapa[linha_saida][coluna_saida] = SAIDA;
    

	allegro_init();
	install_timer();
	install_keyboard();
	set_color_depth(32);
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, 1000, 990, 0, 0 );
	set_window_title("Teste allegro");
	
	//VARIAVEIS

	BITMAP* buffer = create_bitmap(91*LINHAS,90*COLUNAS); //Cria a tela do .exe e define os pixels


	//lendo as imagems
	BITMAP* mapas = load_bitmap("mapa.bmp",NULL); 
	BITMAP* saida = load_bitmap("saida_1.bmp",NULL);
	BITMAP* pessoa =load_bitmap("pessoa_1.bmp",NULL);
	BITMAP* onca =load_bitmap("Onca_1.bmp",NULL);
	BITMAP* obstaculo = load_bitmap("Obstaculo.bmp",NULL);
	BITMAP* venceu = load_bitmap("voce_ganhou.bmp",NULL);
	BITMAP* perdeu = load_bitmap("voce_perdeu.bmp",NULL); 
	

	linha_nova = linha_onca;
	coluna_nova = coluna_onca;
	
	do{//loop do programa,movimento,atualizacao imagens
	
		// teste console matriz / imprime a matriz
		if(z == 0){
		
			for ( i = 0; i < LINHAS; i++) {
	        		for (j = 0; j < COLUNAS; j++) {
	        				
						printf("%c\t",mapa[j][i]);
			
						}
				printf("\n");
			}
			printf("\n");
			z++;
		}
		
		if(key[KEY_RIGHT]) {
			if( ((LINHAS -1)> linha_pessoa)&& (mapa[linha_pessoa+1][coluna_pessoa] != OBSTACULO)){//limitacao de movimento player
				
				
				//atribuicoes de vencer ou perder
				if(mapa[linha_pessoa+1] [coluna_pessoa]== SAIDA){
						t = 1;
					}
				if (mapa[linha_pessoa+1] [coluna_pessoa]==ONCA) { 
  						 f = 5;
  						 x = 1;
					}
				//mudando posicao pessoa
				mapa[linha_pessoa][coluna_pessoa] = VAZIO;
				
				mapa[linha_pessoa+1] [coluna_pessoa]= PESSOA;
				
				
			
				linha_pessoa++;
			
				z = 0;// teste console matriz
				if(x != 1) mover_onca(mapa,linha_onca,coluna_onca,linha_pessoa,coluna_pessoa,&linha_nova,&coluna_nova);
		}
		
		rest(300);//delay para tentar impedir o jogador de fazer mais de um movimento por vez
		}
		if(key[KEY_LEFT]){
			if( linha_pessoa != 0  && (mapa[linha_pessoa-1][coluna_pessoa] != OBSTACULO)){//limitacao de movimento player
			
			
				//atribuicoes de vencer ou perder
				if(mapa[linha_pessoa-1] [coluna_pessoa]== SAIDA){
					t = 1;
				}
					
				if (mapa[linha_pessoa-1] [coluna_pessoa]==ONCA) {
	  				f = 5;
	  				x = 1;
				}
				
				
				//mudando posicao pessoa
				mapa[linha_pessoa][coluna_pessoa] = VAZIO;
					
				mapa[linha_pessoa-1] [coluna_pessoa]= PESSOA;
					
					
					
				linha_pessoa--;
				z = 0;// teste console matriz
				if( x != 1) mover_onca(mapa,linha_onca,coluna_onca,linha_pessoa,coluna_pessoa,&linha_nova,&coluna_nova);
		
			}
			 
		rest(300);//delay para tentar impedir o jogador de fazer mais de um movimento por vez
		}
		
		if(key[KEY_UP])  {
			if( coluna_pessoa != 0  && (mapa[linha_pessoa][coluna_pessoa-1] != OBSTACULO)){//limitacao de movimento player
			
				
				//atribuicoes de vencer ou perder
				if(mapa[linha_pessoa] [coluna_pessoa-1]== SAIDA){
					t = 1;
				}
				
				if(mapa[linha_pessoa] [coluna_pessoa-1] == ONCA){
					t = 5;
					x = 1;
				}
				
				//mudando posicao pessoa
	
				mapa[linha_pessoa][coluna_pessoa] = VAZIO;
					
				mapa[linha_pessoa] [coluna_pessoa-1]= PESSOA;
					
				
					
				coluna_pessoa--;
				z = 0;// teste console matriz
				if(x != 1)mover_onca(mapa,linha_onca,coluna_onca,linha_pessoa,coluna_pessoa,&linha_nova,&coluna_nova);
				
			}
			
			rest(300);//delay para tentar impedir o jogador de fazer mais de um movimento por vez
			
		}
		
		if(key[KEY_DOWN]) {
			
			if( coluna_pessoa < (COLUNAS-1)  && (mapa[linha_pessoa][coluna_pessoa+1] != OBSTACULO)){ //limitacao de movimento player
					
				//atribuicoes de vencer ou perder
				if(mapa[linha_pessoa] [coluna_pessoa+1]== SAIDA){
					t = 1;
				}
				if (mapa[linha_pessoa] [coluna_pessoa+1]==ONCA) {
	  				f = 5;
	  				x = 1;
				}
							
				mapa[linha_pessoa][coluna_pessoa] = VAZIO;
						
				mapa[linha_pessoa] [coluna_pessoa+1]= PESSOA;
						
						
				coluna_pessoa++;
				z = 0;// teste console matriz
						
				if(x != 1 )mover_onca(mapa,linha_onca,coluna_onca,linha_pessoa,coluna_pessoa,&linha_nova,&coluna_nova);
			
			}
			rest(300); //delay para tentar impedir o jogador de fazer mais de um movimento por vez
		}
	
		
		
		draw_sprite(buffer, mapas,0,0); //inicia a imagem mapa no exe e continua atualizando
		
		
		
			for(i = 0; i < LINHAS; i++){	//atualizacao dos obstaculos
				for(j = 0; j < COLUNAS; j++){
					if(mapa[i][j] == OBSTACULO)
						draw_sprite(buffer, obstaculo,i*PIXELS,j*PIXELS);
					}	
			}	
		
		
		if (linha_nova == linha_pessoa && coluna_nova == coluna_pessoa) { //verificacao para nao apagar a onca ou o personagem
		    f = 5; 
			// Onça pegou a pessoa!
		} else {
			//se o movimento da onca nao for apagar a posicao do jogador entao troca a posicao
		    mapa[linha_onca][coluna_onca] = VAZIO;
		    mapa[linha_nova][coluna_nova] = ONCA;
		    linha_onca = linha_nova;
		    coluna_onca = coluna_nova;
		}
		
		draw_sprite(buffer, pessoa,linha_pessoa*PIXELS,coluna_pessoa*PIXELS); //inicia a imagem no exe e atualiza pessoa
		draw_sprite(buffer, saida,linha_saida * PIXELS,coluna_saida * PIXELS); //inicia a imagem no exe e atualiza saida
		draw_sprite(buffer, onca,linha_onca * PIXELS,coluna_onca * PIXELS); //inicia a imagem no exe e atualiza onca
		
		
		
		draw_sprite(screen, buffer, 0, 0); //cria o buffer para impedir piscar as imagens e nao utilizar muito delay
		clear(buffer);
		
		

	}while(  (  !key[KEY_ESC])  &&   (f != 5 )  &&   (t!= 1&&t !=2)); //limitacoes de fim de jogo
	
	rest(300);
	//verificacao de vencedor
	if(t == 1){
		do{
			if(key[KEY_ESC]){
			y = 1;
			}
			
			
	
			draw_sprite(buffer,venceu,0,0);//escrevendo no buffer
			
	
			draw_sprite(screen, buffer, 0, 0); //criando buffer
			
			
		
			
		}while (y!= 1);//loop para que o usuario encerre o programa

	
	}
	
	
	//verificacao de perdedor
	if(f == 5){
		do{
		if(key[KEY_ESC]){
			y = 1;
		}
		
				

		draw_sprite(buffer, perdeu, 0, 0 ); //escrevendo no buffer
		
		

		draw_sprite(screen, buffer, 0, 0); //criando buffer
		
	
		}while(y != 1);//loop para que o usuario encerre o programa
		

	}
	
	
							//destruindo os bitmaps
	destroy_bitmap(buffer);
	destroy_bitmap(mapas);
	destroy_bitmap(saida);
	
	destroy_bitmap(pessoa);
	destroy_bitmap(onca);
	destroy_bitmap(obstaculo);
	destroy_bitmap(venceu);
	destroy_bitmap(perdeu);
	
	return 0;	
}

END_OF_MAIN();	
