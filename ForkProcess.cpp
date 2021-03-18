#include <vector>
#include <unistd.h>
#include <iostream>

int main()
{
    int pid = fork();
    printf("pid: %d\n", pid);
    if (pid > 0)
    {
        while (1)
        {
        }
    }
    else if (pid < 0)
    {
        printf("fork failed\n");
    }

    std::vector<char *> params{"/bin/ls", "/"};
    execv(params[0], &params[0]);
    printf("error: sub process exits with failure.\n");
}