#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>

int PORT=0, N=0;

void *multiplica( void* p )
{
	  struct sockaddr_in mestre;
    int s, i;
  	int index=(int)(size_t) p;
  	int porta=PORT+index;  // usado para definir a porta do socket
  	socklen_t slen=sizeof(mestre);
  	if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
  	{
		    printf("ERRO NA CRICAO DO SOCKET");
		    return (void*)1;
  	}

  	memset((char*) &mestre, 0, sizeof(mestre));
  	mestre.sin_family = AF_INET;
  	mestre.sin_port = htons(porta);
  	mestre.sin_addr.s_addr = inet_addr("127.0.0.1");
  	printf("TENTANDO CONECTAR AO MESTRE\n");
  	if(bind(s, (struct sockaddr *) &mestre, sizeof(mestre))==-1)
  	{
		    printf("ERRO NO BINDING");
   		  return (void*)1;
  	}

  	printf("ESPERANDO O TAMANHO DA LINHA DA MATRIZ\n");
  	if(recvfrom(s, &N, sizeof(int), 0, (struct sockaddr *) &mestre, &slen)==-1)
  	{
	 	   printf("ERRO AO RECEBER DADO");
	 	   return (void*)1;
  	}

  	// aloca s arrays que vai receber do mestre
  	int matrizB[N][N];
  	int linhaA[N];
  	int linhaResult[N];

  	printf("O MESTRE VAI ENVIAR UMA LINHA DE MATRIZ TAMANHO %d\n", N);

  
  	for(i=0;i<N;i++)
  	{
  		linhaA[i]=-1;
  		linhaResult[i] = -1;
  	}


  	printf("LINHA DE MATRIZ ALOCADA!\n");

  	if(recvfrom(s, linhaA, sizeof(int)*N, 0, (struct sockaddr *) &mestre, &slen)==-1)
  	{
		printf("ERRO AO RECEBER DADO");
	 	return (void*)1;
  	}

  	for(i=0;i<N;i++)
  	{
		printf("Pacote recebido de %s:%d\nDado: %d\n", inet_ntoa(mestre.sin_addr), htons(mestre.sin_port), linhaA[i]);
  	}
	printf("\n");
  	
  	printf("MatrizB\n");
  	for(i=0;i<N;i++)
  	{
		if(recvfrom(s, matrizB[i], sizeof(int)*N, 0, (struct sockaddr *) &mestre, &slen)==-1){
		printf("ERRO AO RECEBER DADO");
		return (void*)1;
  		}
  	
    	for(int j=0;j<N;j++)
    	{
			printf("\t%d\t|", matrizB[i][j]);
    	}
		printf("\n");

  	}

  	int coluna, somaprod;
  	// multiplica as matrizes
  	for(coluna=0;coluna<N;coluna++)
  	{
	
		somaprod=0;
		for(i=0;i<N;i++)
		{
			somaprod+=linhaA[i]*matrizB[i][coluna];
		}
		linhaResult[coluna]=somaprod;
  	}

  	printf("\nLinha Resultante\n");
  	for(i=0;i<N;i++)
  	{
  		printf("\t%d\t|", linhaResult[i]);
  	}
  	printf("\n");


  	printf(">>> Enviando resultado para mestre... \n");
  	sendto(s, linhaResult, sizeof(int)*N, 0, (struct sockaddr *)&mestre, slen);

  	pthread_exit(NULL);
}

int main( int argc, char* argv[] )
{
	PORT=atoi(argv[1]);
	N=atoi(argv[2]);

	pthread_t threads[N];

	int i;
	for( i=0; i<N; i++ )
  	{
  		printf( ">>> Criando o %dº escravo...\n", i+1);
  		pthread_create(&threads[i], NULL, multiplica, (void *)(size_t) i);
  	}

  	for( i=0; i<N; i++ )
  	{
  		pthread_join(threads[i], NULL);
  		printf( ">>> %dº Escravo finalizado...\n", i );
  	}

  	printf( ">>> Escravos terminaram a multiplicacao\n" );
}