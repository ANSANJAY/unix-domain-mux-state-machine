#include "declarations.h"
#include "headers.h"

int main(int argc, char *argv[]) {
  struct sockaddr_un name;
  // Initialization
  int connection_socket, i;
  int data_socket;
  int ret;
  int data;
  int result;
  fd_set readfds;
  char buffer[BUFFER_SIZE];

  initialize_monitor_fd_set();

  //
  // create the socket
  unlink(SOCKET_NAME);

  connection_socket = socket(AF_UNIX, SOCK_STREAM, 0);
  if (connection_socket < 0) {
    error("socket");
    exit(EXIT_FAILURE);
  }
  printf("Socket created successfully\n");
  memset(&name, 0, sizeof(struct sockaddr_un));
  name.sun_family = AF_UNIX;
  strcpy(name.sun_path, SOCKET_NAME, sizeof(name.sun_path) - 1);
  //
  ret = bind(connection_socket, (struct sockaddr *)&name,
             sizeof(struct sockaddr_un));
  if (ret == -1) {
    perror("bind");
    exit(EXIT_FAILURE);
  }
  printf("Socket binding successful "\n);

  ret = listen(connection_socket, 20);
  if (ret == -1) {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  add_to_monitored_fd(connection_socket);

  for (;;) {
    refresh_fd_set(&readfds);
    select(get_max_fd() + 1, &readfds, NULL, NULL, NULL);
    /***
           int select(int nfds, fd_set *_Nullable restrict readfds,
                      fd_set *_Nullable restrict writefds,
                      fd_set *_Nullable restrict exceptfds,
                      struct timeval *_Nullable restrict timeout);
    ****/
    // select is a blocking call, server waits for request from the client
    //        int  FD_ISSET(int fd, fd_set *set);

    if(FD_ISSET(connection_socket,&readfds)
    { // if the fd is same as master fd of the socket ==> implies this is a request from new client
      printf("Connection request from a new client , accept the request\n");
      // aacept the request

      data_socket = accept(connection_socket, NULL, NULL);
      if (data_socket < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
      }
      printf("Conenction established for new client\n");
      add_to_monitored_fd(data_socket);
    }
    else // fd already exist in the fd _set ==> it's an request from a client laready connected
    {
      // find the client fd in the fd_set
      i = 0;
      comm_fd = -1;
      for (; i < MAX_CLIENT_SUPPORTED; i++)
        if (FD_ISSET(monitored_fd_set[i], &readfds)) {
          comm_fd = monitored_fd_set[i];

          // read the data from the comm_fd
          memset(buffer, 0, BUFFER_SIZE);

          // waiting to read the data , read is a block call

          printf("Waiitng for the data"\n);
          ret = read(comm_fd, buffer, BUFFER_SIZE);
          if (ret == -1) {
            perror("read");
            exit(EXIT_FAILURE);
          }
          // copy the read data from buffer ==> data , design is expecting only
          // int data
          memcpy(&data, buffer, sizeof(int));
          if (data == 0) // design ==> if the received data is zero send the
                         // result back to the client
          {
            memset(buffer, 0, BUFFER_SIZE);
            sprintf(buffer, "Result =%d", client_result[i]);
            printf("Sending result back to the client\n");
            ret = write(comm_fd, buffer, BUFFER_SIZE);
            if (ret < 0) {
              perror("write");
              exit(EXIT_FAILURE);
            }
            close(comm_fd);
            client_result[i] = 0;
            remove_from_monitored_fd(comm_fd);
            continue; // go to select bloc syscall
          }
          client_result[i] += data;
        }
        
  } // go to select bloc syscall
  }
  close(connection_socket);
  remove_from_monitored_fd(connection_socket);
  printf("Connection closed\n");
  unlink(SOCKET_NAME);
  exit(EXIT_SUCCESS);
}

