#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>

#define CHAT_PIPE_NAME "chat"
#define HISTORY_PIPE_NAME "chat_history"

const int maxChatHistoryLength = 10000;

const char *getMessagesCmd = ".getMessages";

int main(int argc, char *argv[])
{
  printf("server process started with pid [%d]\n", getpid());

  int currentChatLength = 0;
  char *messages = malloc(maxChatHistoryLength);

  if (mkfifo(CHAT_PIPE_NAME, 0777) == -1)
  {

    if (errno != EEXIST)
    {
      printf("error creating [%s] pipe!\n", CHAT_PIPE_NAME);
      return 1;
    }
  }

  if (mkfifo(HISTORY_PIPE_NAME, 0777) == -1)
  {

    if (errno != EEXIST)
    {
      printf("error creating [%s] pipe!\n", HISTORY_PIPE_NAME);
      return 1;
    }
  }

  while (1)
  {

    int chatFd = open(CHAT_PIPE_NAME, O_RDONLY);
    if (chatFd == -1)
    {
      printf("error opening [%s] pipe!\n", CHAT_PIPE_NAME);
      return 1;
    }

    char received[20];

    if (read(chatFd, received, sizeof(received)) == -1)
    {
      printf("error reading from [%s] pipe!\n", CHAT_PIPE_NAME);
      return 1;
    }

    close(chatFd);

    printf("%s\n", received);

    currentChatLength += 20;
    strcat(messages, received);
    strcat(messages, "\n");

    if (strstr(received, getMessagesCmd) != NULL)
    {
      int historyFd = open(HISTORY_PIPE_NAME, O_WRONLY);

      if (historyFd == -1)
      {
        printf("error opening [%s] pipe!\n", HISTORY_PIPE_NAME);
        return 1;
      }
      if (write(historyFd, messages, currentChatLength) == -1)
      {
        printf("error writing to [%s] pipe!\n", HISTORY_PIPE_NAME);
        return 1;
      }

      close(historyFd);
    }
  }

  return 0;
}