#include <sys/types.h>
#include <sys/stat.h> //responsaveis pelas system calls
#include <fcntl.h>
#include <stdio.h> 
#include <errno.h> //pega o codigo de erro

char *strerror(int errnum); //referenciando

int main (int * argc, char * argv[]){ //qtde de argumentos e valores dos argumentos

    if( argc != 3 ){
      printf("filecopy: Quantidade de argumentos inválida.\n Use: filecopy <arquivoorigem> <arquivodestino>");
      exit(1);
    }
    
    int filecopied = open(argv[1], O_RDONLY, 0755); //
    //Verifica se o arquivo a copiar existe
    if(filecopied < 0) { //se retorna -1 n consegue ler (num do arquivo na pilha q eh enviado pro disco)
      printf("filecopy: Não foi possível abrir o arquivo %s: %s  \n", argv[1], strerror(errno));

      exit(1);
    }
    
    //abrir arquivo se o arquivo não existir cria o arquivo
    int filetarget = open(argv[2], O_RDWR | O_CREAT, 0755); 
    //Verifica se a ação de abrir ou criar ocorreu com sucesso
    if(filetarget < 0){
      printf("filecopy: Não foi possível abrir ou criar o arquivo: %s \n", strerror(errno) );

      exit(1);
    }
    char buffer[4096]; //qtde de bytes que podem ser enviados do disco para a memoria em um unico pacote
    int totalbytes = 0; //qtde inicial de bytes escritos

    /* read write */
      while(1){
        
        //read retorna sempre um valor; o valor do buffer,a quantidade lida, e 0 endoffile
        int rbytes = read(filecopied, buffer, 4096); //retorna a qtde de bytes lidos
        if(rbytes == 0){ //ja passou do endfile
          break;
        }else if(rbytes < 0){ 
          printf("Não foi possivel ler o arquivo: %s \n",strerror(errno));

          exit(1);
        }
        
        totalbytes += rbytes; 

        int wbytes = write(filetarget, buffer, rbytes); //vai escrever a qtde de bytes q ele le
        if(wbytes == 0){ //funcao write calcula o offset automaticamente
          break;
        }else if(wbytes < 0){
          printf("Não foi possivel escrever o arquivo: %s \n", strerror(errno));

          exit(1);
        }
      }
  int copiedClosed = close(filecopied);
    if(copiedClosed < 0){
      printf("Não foi possível fechar o arquivo copiado: %s \n", strerror(errno));

      exit(1);
    }
  int targetClosed = close(filetarget);
    if(targetClosed < 0){
        printf("Não foi possível fechar a cópia do arquivo original: %s \n", strerror(errno));

        exit(1);
    }

  printf("filecopy: foram copiados %d bytes do arquivo %s para o arquivo %s \n",totalbytes, argv[1], argv[2]);

  exit(0);
}