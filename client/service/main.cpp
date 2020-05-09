#include "client.h"

int main()
{
  client cli;
  cli.initSocket();
  cli.connectServer();
  cli.Excecute();
  return 0;
}
