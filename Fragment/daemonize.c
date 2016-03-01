/*
 * This code is an example of a server process to run background
 */
bool daemonize()
{
    /* create subprocess, close parent process to make PROCESS run
     * background
     */
    pid_t pid = fork();
    if (pid < 0) {
        return false;
    } else if (pid > 0) {
        exit(0);
    }

    /*
     * set umask. When create a new file, its permission will be mode & 0777
     */
    umask(0);

    /*
     * create a new session, set PROCESS pgroup leader
     */
    pid_t sid = setsid();
    if (sid < 0) {
        return false;
    }

    /*
     * change current working directory
     */
    if (chdir("/") < 0) {
        return false;
    }

    /*
     * close stdin, stdout and stderr
     */
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    /* close other file descriptors (code skipped) */
    // nothing...

    /* redirect stdin, stdout, stderr to /dev/null */
    open("/dev/null", O_RDONLY);
    open("/dev/null", O_RDWR);
    open("/dev/null", O_RDWR);

    return true;
}
