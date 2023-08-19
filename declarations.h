#define SOCKET_NAME "/tmp/DemoSocket"
#define BUFFER_SIZE 128
#define MAX_CLIENT_SUPPORTED 32


int monitored_fd_set[MAX_CLIENT_SUPPORTED]; 
static int initialize_monitor_fd_set()
static void add_to_monitored_fd(int );
static void remove_from_monitored_fd(int );
static void refresh_fd_set(fd_set *);
static int get_max_fd();


//    void FD_ZERO(fd_set *set);
//    void FD_SET(int fd, fd_set *set);
