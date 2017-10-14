#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]){
	
  struct sockaddr_in mestre;
  int s, i;
  socklen_t slen=sizeof(mestre);
  int PORT = atoi(argv[1]);
  int TAM;
  if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1){
	printf("ERRO NA CRICAO DO SOCKET");
	return 1;
  }

  
  memset((char*) &mestre, 0, sizeof(mestre));
  mestre.sin_family = AF_INET;
  mestre.sin_port = htons(PORT);
  mestre.sin_addr.s_addr = inet_addr("127.0.0.1");
  printf("TENTANDO CONECTAR AO MESTRE\n");
  if(bind(s, (struct sockaddr *) &mestre, sizeof(mestre))==-1){
	printf("ERRO NO BINDING");
return 1;
  }

  printf("ESPERANDO O TAMANHO DA LINHA DA MATRIZ\n");
  if(recvfrom(s, &TAM, sizeof(int), 0, (struct sockaddr *) &mestre, &slen)==-1){
	printf("ERRO AO RECEBER DADO");
	return 1;
  }
  
  int matrizB[TAM][TAM];
  int linhaA[TAM];
  int linhaResult[TAM];
  printf("O MESTRE VAI ENVIAR UMA LINHA DE MATRIZ TAMANHO %d\n", TAM);

  
  for(i=0;i<TAM;i++)
  {
  	linhaA[i]=-1;
  	linhaResult[i] = -1;
  }
	
  printf("LINHA DE MATRIZ ALOCADA!\n");

  if(recvfrom(s, linhaA, sizeof(int)*TAM, 0, (struct sockaddr *) &mestre, &slen)==-1){
	printf("ERRO AO RECEBER DADO");
	return 1;
  }

  for(i=0;i<TAM;i++)
	printf("Pacote recebido de %s:%d\nDado: %d\n", inet_ntoa(mestre.sin_addr), htons(mestre.sin_port), linhaA[i]);

  printf("\n");
  printf("MatrizB\n");
  for(i=0;i<TAM;i++)
  {
	if(recvfrom(s, matrizB[i], sizeof(int)*TAM, 0, (struct sockaddr *) &mestre, &slen)==-1){
		printf("ERRO AO RECEBER DADO");
		return 1;
  	}
  	for(int j=0;j<TAM;j++)
		printf("\t%d\t|", matrizB[i][j]);
	printf("\n");

  }

  int coluna, somaprod;
  // multiplica as matrizes
  for(coluna=0;coluna<TAM;coluna++)
  {
	
	somaprod=0;
	for(i=0;i<TAM;i++)
	{
		somaprod+=linhaA[i]*matrizB[i][coluna];
	}
	linhaResult[coluna]=somaprod;
  }

  printf("\nLinha Resultante\n");
  for(i=0;i<TAM;i++)
  {
  	printf("\t%d\t|", linhaResult[i]);
  }
  printf("\n");

  printf(">>> Enviando resultado para mestre... \n");
  sendto(s, linhaResult, sizeof(int)*TAM, 0, (struct sockaddr *)&mestre, slen);
	

  return 0;
}
