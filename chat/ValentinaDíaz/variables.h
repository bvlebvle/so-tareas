#include <time.h>

#define SERVER_PIPE "server_pipe"
#define MAX_CLIENTS 10
#define MAX_MSG_SIZE 512 // 512 bytes, 3 for clientID + 509 for message
#define ADD_CLIENT_CMD "add"
#define REMOVE_CLIENT_CMD "rmc"
#define SEND_MSG_CMD "msg"

char* generateClientID() {
  srand(time(NULL));

  char* clientID = malloc(4);

  for (int i = 0; i < 3; i++) {
    clientID[i] = 'A' + rand() % 26;
  }

  clientID[3] = '\0';

  return clientID;
}