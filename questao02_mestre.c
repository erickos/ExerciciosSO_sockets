/****************** MESTRE ****************/
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>

#define BUFLEN 512
#define PACOTES 10
#define SRV_IP "127.0.0.1"

int main( int argc, char * argv[] )
{
	int i, j;
	int porta_inicial=atoi(argv[1]); // porta P fornecida via argc, argv
	int N=atoi(argv[2]);
	int sockets[N];
	struct sockaddr_in escravos[N];
	int ports[N];
	socklen_t slen[N];

	//preenche o vetor com o numero das portas
	for(i=0;i<N;i++)
	{
		ports[i] = porta_inicial+i;
	}	

	for(i=0;i<N;i++)
	{
		slen[i] = sizeof(escravos[i]);
	}
	
	srand(time(NULL));

	int matrizA[N][N];
	int matrizB[N][N];

	int matrizResult[N][N];

	for(i=0;i<N;i++)
	{
		if((sockets[i]=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
		{
			printf("ERRO AO CRIAR SOCKET %d\n", i);
			return 1;
		}
	}

	for(i=0;i<N;i++)
	{
		memset((char*) &escravos[i], 0, sizeof(escravos[i]));
		escravos[i].sin_family = AF_INET;
		escravos[i].sin_port = htons(ports[i]);
		if(inet_aton(SRV_IP, &escravos[i].sin_addr) == 0)
		{
			printf("ERRO AO TRANSFORMAR ENDEREÇO %d\n", i);
			return 1;
		}
	}

	//gera os valores para as matrizes
	for(i=0;i<N;i++)
	{
		for(j=0;j<N;j++)
		{
			matrizA[i][j] = rand()%100;
			matrizB[i][j] = rand()%100;
		}
	}

	printf(">>> MatrizA <<<\n");
	for(i=0;i<N;i++)
	{
		for(j=0;j<N;j++)
		{
			printf("\t%d\t|" ,matrizA[i][j]);
		}
		printf("\n");
	}

	printf(">>> MatrizB <<<\n");
	for(i=0;i<N;i++)
	{
		for(j=0;j<N;j++)
		{
			printf("\t%d\t|" ,matrizB[i][j]);
		}
		printf("\n");
	}

	// envia para cada escravo sua linha da matrizA e a matrizB
	for(i=0;i<N;i++)
	{
		//enviando uma linha por vez para cada escravo
		printf("Enviando o TAMANHO da linha da matriz, para o escravo %d\n", i+1);
		    if(sendto(sockets[i], &N, sizeof(int), 0, (struct sockaddr *) &escravos[i], slen[i])==-1){
			printf("ERRO AO ENVIAR PACOTE1\n");
			return 1;
		}
		printf("Enviando a linha %d da matrizA para escravo %d\n", i, i+1);
		    if(sendto(sockets[i], matrizA[i], sizeof(int)*N, 0, (struct sockaddr *) &escravos[i], slen[i])==-1){
			printf("ERRO AO ENVIAR PACOTE1\n");
			return 1;
		}

		printf("Enviando a matrizB para escravo %d\n", i+1);
		for(j=0;j<N;j++)
		{
		    if(sendto(sockets[i], matrizB[j], sizeof(int)*N, 0, (struct sockaddr *) &escravos[i], slen[i])==-1)
		    {
				printf("ERRO AO ENVIAR PACOTE %d\n", j);
				return 1;
			}
		}
	}

	// recebendo os resultados e fechando as conexões
	for(i=0;i<N;i++)
	{
		printf(">>> Recebendo linha %d resultado do escravo %d\n", i, i+1);
		recvfrom(sockets[i], matrizResult[i], sizeof(int)*N, 0, (struct sockaddr *) &escravos[i], &slen[i]);
		close(sockets[i]);
	}

	printf("\n>>> MATRIZ RESULTANTE <<<\n");
	for(i=0;i<N;i++)
	{
		for(j=0;j<N;j++)
			printf("\t %d \t|", matrizResult[i][j]);
		printf("\n");
	}

}