static int initialize_monitor_fd_set()
{
    int i=0;
    for(;i<MAX_CLIENT_SUPPORTED;i++)
    {
        monitored_fd_set[i] = -1;
    }

}

static void add_to_monitored_fd(int skt_fd);
{
    int i=0;
    for(;i<MAX_CLIENT_SUPPORTED;i++)
    {
        if(monitored_fd_set[i] != -1)
        continue;
        monitored_fd_set[i]= skt_fd;
        break;
    }
}

static void remove_from_monitored_fd(int skt_fd);
{
    int i=0;
    for(;i<MAX_CLIENT_SUPPORTED;i++)
    {
        if(monitored_fd_set[i] != skt_fd)
        continue;
        monitored_fd_set[i]= -1;
        break;
    }
}

static void refresh_fd_set(fd_set *fd_set_ptr);
{
    FD_ZERO(fd_set_ptr);
//    void FD_ZERO(fd_set *set);

    int i=0;
    for(;i<MAX_CLIENT_SUPPORTED;i++)
    {
        if(monitored_fd_set[i] != -1)
        FD_SET(monitored_fd_set[i],monitored_fd_set[i])
    }
//void FD_SET(int fd, fd_set *set);
}

static int get_max_fd();
{
    int i =0;
    int max = -1;
    for(;i<MAX_CLIENT_SUPPORTED;i++)
    {
        if(monitored_fd_set[i]>max)
        max=monitored_fd_set[i];
    }
    return max;
}