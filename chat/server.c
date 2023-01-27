#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include "variables.h"

char clients[MAX_CLIENTS][4];

void printClients()
{
  for (int i = 0; i < MAX_CLIENTS; i++)
  {
    if (clients[i][0] != '\0')
    {
      printf("[%s] ", clients[i]);
    }
  }

  printf("\n");
}

int findNextEmptyClientSlot()
{
  for (int i = 0; i < MAX_CLIENTS; i++)
  {
    if (clients[i][0] == '\0')
    {
      return i;
    }
  }

  return -1;
}

int addClient(char *id)
{
  int index = findNextEmptyClientSlot();

  if (index == -1)
  {
    printf("error adding client [%s], no more slots available!\n", id);

    return 0;
  }

  // add to clients array
  strcpy(clients[index], id);

  // print active clients
  printClients();

  return 1;
}

int removeClient(char *id)
{
  // remove from clients array
  for (int i = 0; i < MAX_CLIENTS; i++)
  {
    if (strcmp(clients[i], id) == 0)
    {
      clients[i][0] = '\0';
    }
  }

  // print active clients
  printClients();

  return 1;
}

int sendMessageToClients(char *clientID, char *message)
{
  char toSend[MAX_MSG_SIZE];
  strcpy(toSend, clientID);
  strcat(toSend, "> ");
  strcat(toSend, message); 
  
  for (int i = 0; i < MAX_CLIENTS; i++)
  {
    if (clients[i][0] != '\0')
    {
      printf("sending message [%s] to [%s]...\n", toSend, clients[i]);

      int clientFd = open(clients[i], O_WRONLY);

      if (clientFd == -1)
      {
        printf("error opening [%s] pipe!\n", clients[i]);
        return 0;
      }

      if (write(clientFd, toSend, strlen(toSend)) == -1)
      {
        printf("error writing to [%s] pipe!\n", clients[i]);
        return 0;
      }

      close(clientFd);
    }
  }

  return 1;
}

int main(int argc, char *argv[])
{
  printf("server process started with pid [%d]\n", getpid());

  // set clients array to empty
  for (int i = 0; i < MAX_CLIENTS; i++)
  {
    strcpy(clients[i], "");
  }

  // create server pipe
  if (mkfifo(SERVER_PIPE, 0777) == -1)
  {
    if (errno != EEXIST)
    {
      printf("error creating [%s] pipe!\n", SERVER_PIPE);
      return 0;
    }
  }

  // keep listening on server pipe
  while (1)
  {
    int serverFd = open(SERVER_PIPE, O_RDONLY);

    if (serverFd == -1)
    {
      printf("error opening [%s] pipe!\n", SERVER_PIPE);
      return 1;
    }

    // handle pipe incoming data
    char received[MAX_MSG_SIZE];

    // read from named pipe
    if (read(serverFd, received, sizeof(received)) == -1)
    {
      printf("error reading from [%s] pipe!\n", SERVER_PIPE);
      return 1;
    }

    // close server pipe
    close(serverFd);

    // print the received message
    printf("rawMessage >>> %s\n", received);

    // use delimiter to split the message
    int messageLength = 0;
    for (int i = 0; i < MAX_MSG_SIZE; i++)
    {
      if (received[i] == '|')
      {
        // printf("message length >>> %d\n", i);
        messageLength = i;
        break;
      }
    }

    // check commands
    if (strstr(received, ADD_CLIENT_CMD) != NULL)
    {
      char clientID[4];
      memcpy(clientID, &received[3], 4);
      clientID[3] = '\0';

      printf("adding client [%s]...!\n", clientID);
      
      addClient(clientID);
    } else if (strstr(received, REMOVE_CLIENT_CMD) != NULL) {
      char clientID[4];
      memcpy(clientID, &received[3], 4);
      clientID[3] = '\0';

      printf("removing client [%s]...!\n", clientID);

      removeClient(clientID);
    } else {
      char clientID[4];
      memcpy(clientID, &received[0], 4);
      clientID[3] = '\0';

      char message[messageLength - 1];
      memcpy(message, &received[3], messageLength - 1);
      message[messageLength - 2] = '\0';
      
      printf("sending message [%s] to clients...\n", message);

      sendMessageToClients(clientID, message);
    }
  }

  return 0;
}