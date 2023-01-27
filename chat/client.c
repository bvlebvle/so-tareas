#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include "variables.h"

#define SHARE_CMD ".share"
#define EXIT_CMD ".exit"

int registerClient(char *id)
{
  int registerClientFd = open(SERVER_PIPE, O_WRONLY);

  if (registerClientFd == -1)
  {
    printf("error opening [%s] pipe!\n", SERVER_PIPE);
    return 0;
  }

  // concat add + id
  char *toSend = malloc(strlen(id) + 4);
  strcpy(toSend, "add");
  strcat(toSend, id);

  if (write(registerClientFd, toSend, 7) == -1)
  {
    printf("error writing to [%s] pipe!\n", SERVER_PIPE);
    return 0;
  }

  close(registerClientFd);

  return 1;
}

int removeClient(char *id)
{
  printf("exiting [%s] client...\n", id);

  int removeClientFd = open(SERVER_PIPE, O_WRONLY);

  if (removeClientFd == -1)
  {
    printf("error opening [%s] pipe!\n", SERVER_PIPE);
    return 0;
  }

  // concat rmc + id
  char *toSend = malloc(strlen(id) + 4);
  strcpy(toSend, "rmc");
  strcat(toSend, id);

  if (write(removeClientFd, toSend, 7) == -1)
  {
    printf("error writing to [%s] pipe!\n", SERVER_PIPE);
    return 0;
  }

  close(removeClientFd);

  return 1;
}

int sendMessage(char *id, char *message)
{
  // concat clientID + message
  char toSend[MAX_MSG_SIZE];
  strcpy(toSend, id);
  strcat(toSend, message);

  // printf("sending message [%s] to server...\n", toSend);

  int sendMessageFd = open(SERVER_PIPE, O_WRONLY);

  if (sendMessageFd == -1)
  {
    printf("error opening [%s] pipe!\n", SERVER_PIPE);
    return 0;
  }

  if (write(sendMessageFd, toSend, strlen(toSend)) == -1)
  {
    printf("error writing to [%s] pipe!\n", id);
    return 0;
  }

  close(sendMessageFd);

  return 1;
}

int main(int argc, char *argv[])
{
  char *id = generateClientID();

  printf("client sender process started with clientID [%s]\n", id);

  if (registerClient(id) == 0)
  {
    return 1;
  }

  int pid = fork();

  if (pid > 0)
  {
    // parent will listen for messages from server
    printf("client listener process started with clientID [%s]\n", id);

    // create clientID pipe
    if (mkfifo(id, 0777) == -1)
    {
      if (errno != EEXIST)
      {
        printf("error creating [%s] pipe!\n", id);
        return 0;
      }
    }

    while (1)
    {
      int clientFd = open(id, O_RDONLY);

      if (clientFd == -1)
      {
        printf("error opening [%s] pipe!\n", id);
        return 0;
      }

      // handle pipe incoming data
      char received[MAX_MSG_SIZE];

      // read from named pipe
      if (read(clientFd, received, sizeof(received)) == -1)
      {
        printf("error reading from [%s] pipe!\n", id);
        return 1;
      }

      int messageLength = 0;
      for (int i = 0; i < MAX_MSG_SIZE; i++)
      {
        if (received[i] == '|')
        {
          messageLength = i - 1;
          break;
        }
      }

      char message[messageLength + 3 - 1];
      memcpy(message, &received[0], messageLength + 3 - 1);
      message[messageLength + 3 - 2] = '\0';

      close(clientFd);

      printf("\n[SERVER] %s\n", message);
    }

    printf("client listener process ended with clientID [%s]\n", id);
  }
  else if (pid == 0)
  {
    // child process will send messages to server
    while (1)
    {
      // user input buffer
      char message[MAX_MSG_SIZE];
      // // request user input
      // printf("> ");
      // read from stdin
      fgets(message, sizeof(message), stdin);
      // flush if the message is more than 20 characters
      fflush(stdin);

      // clean message from newline character
      for (int i = 0; i < strlen(message); i++)
      {
        if (message[i] == '\n')
        {
          message[i] = '|';
          message[i+1] = '\0';
        }
      }

      // check commands
      if (strstr(message, SHARE_CMD) != NULL)
      {
        int pid = fork();

        if (pid == 0)
        {
          // child process
          // execute the client program
          execlp("./client", NULL);
        }
      }
      else if (strstr(message, EXIT_CMD) != NULL)
      {
        if (removeClient(id) == 0)
        {
          return 1;
        }

        printf("client sender process ended with clientID [%s]\n", id);

        return 0;
      }
      else
      {
        sendMessage(id, message);
      }
    }
  }
  else
  {
    printf("error creating both processes!\n");
  }

  return 0;
}
