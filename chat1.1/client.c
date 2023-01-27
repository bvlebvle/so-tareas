#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>

#define CHAT_PIPE_NAME "chat"
#define HISTORY_PIPE_NAME "chat_history"

const int maxChatHistoryLength = 10000;

const char *getMessagesCmd = ".getMessages";
const char *shareCmd = ".share";
const char *exitCmd = ".exit";

char *getClientID()
{
  char *clientID = malloc(4);

  for (int i = 0; i < 3; i++)
  {
    clientID[i] = 'A' + rand() % 26;
  }

  clientID[3] = '\0';

  return clientID;
}

int main(int argc, char *argv[])
{
  srand(time(NULL));

  int pid = getpid();
  char *clientID = getClientID();

  printf("Cliente creado con pid [%d] y clientID [%s]\n\nComandos de chat:  \n.getMessages - Obtener mensajes \n.share - Compartirse \n.exit - Abandonar chat \n", pid, clientID);

  while (1)
  {
    char message[20];

    printf("> ");
    // read
    fgets(message, sizeof(message), stdin);
    fflush(stdin);

    for (int i = 0; i < strlen(message); i++)
    {
      if (message[i] == '\n')
      {
        message[i] = '\0';
      }
    }

    // condiociones de comandos
    if (strstr(message, getMessagesCmd) != NULL)
    {
      printf("Obteniendo mensajes de servidor...\n");

      // open the named pipe for read and write
      int chatFd = open("chat", O_WRONLY);

      if (chatFd == -1)
      {
        printf("error al abrir [%s] pipe!\n", CHAT_PIPE_NAME);
        return 1;
      }

      if (write(chatFd, message, sizeof(message)) == -1)
      {
        printf("error escrbir en [%s] pipe!\n", CHAT_PIPE_NAME);
        return 1;
      }

      close(chatFd);

      int historyFd = open(HISTORY_PIPE_NAME, O_RDONLY);
      if (historyFd == -1)
      {
        printf("error abrir [%s] pipe!\n", HISTORY_PIPE_NAME);
        return 1;
      }

      char receivedChat[maxChatHistoryLength];

      if (read(historyFd, receivedChat, maxChatHistoryLength) == -1)
      {
        printf("error leer de [%s] pipe!\n", HISTORY_PIPE_NAME);
        return 1;
      }

      printf("%s", receivedChat);

      close(historyFd);
    }
    else if (strstr(message, shareCmd) != NULL)
    {
      pid = fork();

      if (pid == 0)
      {

        execlp("./client", NULL);
      }
    }
    else if (strstr(message, exitCmd) != NULL)
    {
      printf("Saliendo [%s] cliente...\n> ", clientID);
      return 0;
    }
    else
    {
      printf("Enviando mensaje a servidor desde [%s] proceso...\n", clientID);

      int chatFd = open("chat", O_WRONLY);

      if (chatFd == -1)
      {
        printf("error al abrir [%s] pipe!\n", CHAT_PIPE_NAME);
        return 1;
      }

      if (write(chatFd, message, sizeof(message)) == -1)
      {
        printf("error al escribir en [%s] pipe!\n", CHAT_PIPE_NAME);
        return 1;
      }

      close(chatFd);
    }
  }

  return 0;
}
