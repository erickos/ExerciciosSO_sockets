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

#define PACOTES 10
#define SRV_IP "127.0.0.1"

int main( int argc, char * argv[] )
{
	struct sockaddr_in escravoA, escravoB, escravoC;
	int sock_A, sock_B, sock_C, i, j;
	socklen_t slen_A=sizeof(escravoA);
	socklen_t slen_B=sizeof(escravoB);
	socklen_t slen_C=sizeof(escravoC);
	int PORT_A=atoi(argv[1]); // porta P fornecida via argc, argv
	int PORT_B=PORT_A+1;	  // porta P+1
	int PORT_C=PORT_A+2;	  // porta P+2

	srand(time(NULL));
	int matrizA[3][3];
	int matrizB[3][3];

	int matrizResult[3][3];
	int TAM = 3;

	if((sock_A=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
	{
		printf("ERRO AO CRIAR SOCKET\n");
		return 1;
	}
	if((sock_B=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
	{
		printf("ERRO AO CRIAR SOCKET\n");
		return 1;
	}

	if((sock_C=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
	{
		printf("ERRO AO CRIAR SOCKET\n");
		return 1;
	}

	memset((char*) &escravoA, 0, sizeof(escravoA));
	escravoA.sin_family = AF_INET;
	escravoA.sin_port = htons(PORT_A);
	if(inet_aton(SRV_IP, &escravoA.sin_addr) == 0)
	{
		printf("ERRO AO TRANSFORMAR ENDEREÇO\n");
		return 1;
	}

	memset((char*) &escravoB, 0, sizeof(escravoB));
	escravoB.sin_family = AF_INET;
	escravoB.sin_port = htons(PORT_B);
	if(inet_aton(SRV_IP, &escravoB.sin_addr) == 0)
	{
		printf("ERRO AO TRANSFORMAR ENDEREÇO\n");
		return 1;
	}

	memset((char*) &escravoC, 0, sizeof(escravoC));
	escravoC.sin_family = AF_INET;
	escravoC.sin_port = htons(PORT_C);
	if(inet_aton(SRV_IP, &escravoC.sin_addr) == 0)
	{
		printf("ERRO AO TRANSFORMAR ENDEREÇO\n");
		return 1;
	}

	//gera os valores para as matrizes
	for(i=0;i<TAM;i++)
	{
		for(j=0;j<TAM;j++)
		{
			matrizA[i][j] = rand()%100;
			matrizB[i][j] = rand()%100;
		}
	}

	printf(">>> MatrizA <<<\n");
	for(i=0;i<TAM;i++)
	{
		for(j=0;j<TAM;j++)
		{
			printf("\t%d\t|" ,matrizA[i][j]);
		}
		printf("\n");
	}

	printf(">>> MatrizB <<<\n");
	for(i=0;i<TAM;i++)
	{
		for(j=0;j<TAM;j++)
		{
			printf("\t%d\t|" ,matrizB[i][j]);
		}
		printf("\n");
	}

	//enviando uma linha por vez para cada escravo
	printf("Enviando o TAMANHO da linha 1\n");
	    if(sendto(sock_A, &TAM, sizeof(int), 0, (struct sockaddr *) &escravoA, slen_A)==-1){
		printf("ERRO AO ENVIAR PACOTE1\n");
		return 1;
	}
	printf("Enviando a linha 1 da matrizA para escravo 1\n");
	    if(sendto(sock_A, matrizA[0], sizeof(int)*TAM, 0, (struct sockaddr *) &escravoA, slen_A)==-1){
		printf("ERRO AO ENVIAR PACOTE1\n");
		return 1;
	}

	printf("Enviando a matrizB para escravo 1\n");
	for(i=0;i<TAM;i++)
	{
	    if(sendto(sock_A, matrizB[i], sizeof(int)*TAM, 0, (struct sockaddr *) &escravoA, slen_A)==-1)
	    {
			printf("ERRO AO ENVIAR PACOTE1\n");
			return 1;
		}
	}

	printf("Enviando o TAMANHO da linha 2\n");
	    if(sendto(sock_B, &TAM, sizeof(int), 0, (struct sockaddr *) &escravoB, slen_B)==-1){
		printf("ERRO AO ENVIAR PACOTE2\n");
		return 1;
	}
	printf("Enviando a linha 2 da matrizA para escravo 2\n");
	    if(sendto(sock_B, matrizA[1], sizeof(int)*TAM, 0, (struct sockaddr *) &escravoB, slen_B)==-1){
		printf("ERRO AO ENVIAR PACOTE2\n");
		return 1;
	}

	printf("Enviando a matrizB para escravo 2\n");
	for(i=0;i<TAM;i++)
	{
	    if(sendto(sock_B, matrizB[i], sizeof(int)*TAM, 0, (struct sockaddr *) &escravoB, slen_B)==-1)
	    {
			printf("ERRO AO ENVIAR PACOTE2\n");
			return 1;
		}
	}

	printf("Enviando o TAMANHO do linha 3\n");
	    if(sendto(sock_C, &TAM, sizeof(int), 0, (struct sockaddr *) &escravoC, slen_C)==-1){
		printf("ERRO AO ENVIAR PACOTE3\n");
		return 1;
	}
	printf("Enviando a linha 3 da matrizA para escravo 3\n");
	    if(sendto(sock_C, matrizA[2], sizeof(int)*TAM, 0, (struct sockaddr *) &escravoC, slen_C)==-1){
		printf("ERRO AO ENVIAR PACOTE3\n");
		return 1;
	}

	printf("Enviando a matrizB para escravo 3\n");
	for(i=0;i<TAM;i++)
	{
	    if(sendto(sock_C, matrizB[i], sizeof(int)*TAM, 0, (struct sockaddr *) &escravoC, slen_C)==-1)
	    {
			printf("ERRO AO ENVIAR PACOTE 3\n");
			return 1;
		}
	}
	//fim do envio

	//recebe a matriz linha por linha e imprime 
	recvfrom(sock_A, matrizResult[0], sizeof(int)*TAM, 0, (struct sockaddr *) &escravoA,&slen_A);
	close(sock_A);

	recvfrom(sock_B, matrizResult[1], sizeof(int)*TAM, 0, (struct sockaddr *) &escravoB,&slen_B);
	close(sock_B);

	recvfrom(sock_C, matrizResult[2], sizeof(int)*TAM, 0, (struct sockaddr *) &escravoC, &slen_C);
	close(sock_C);

	printf("\n>>> MATRIZ RESULTANTE <<<\n");
	for(i=0;i<TAM;i++)
	{
		for(j=0;j<TAM;j++)
			printf("\t %d\t|", matrizResult[i][j]);
		printf("\n");
	}

	return 0;
}