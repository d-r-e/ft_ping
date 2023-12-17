#include "libft.h"

/// @brief  The ft_getopt() function parses the command-line arguments.
/// @param argc  The argc argument is the number of command-line arguments.
/// @param argv   The argv argument is an array of pointers to the arguments.
/// @param optstring  The optstring argument is a string containing the legitimate option characters.
/// @param optind  The optind argument is the index of the next element of the argv[] vector to be processed.
/// @return  The ft_getopt() function returns the next option character (if one is found) otherwise returns -1.
int ft_getopt(int argc, char *const argv[], const char *optstring, int *optind)
{
    static int optpos = 1;

    if (*optind >= argc || argv[*optind][0] != '-' || argv[*optind][1] == '\0')
        return (-1);

    if (ft_strchr(optstring, argv[*optind][optpos]) == NULL)
    {
        ft_putstr_fd("Illegal option -- ", 2);
        ft_putchar_fd(argv[*optind][optpos], 2);
        ft_putchar_fd('\n', 2);
        return ('?');
    }
    return (argv[*optind][optpos++]);
}
