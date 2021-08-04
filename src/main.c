#include "../inc/ft_ping.h"

static void parse_options(int argc, char *argv[])
{
    for (int i = 0; i < argc; ++i)
    {
        printf("%zu\n",ft_strlen(""));
    }

}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(STDERR_FILENO, "usage: ft_ping [-hv]\n");
        exit(64);
    }
    for (int i = 1; i < argc; ++i)
    {
        printf("%s\n", argv[i]);
    }
}