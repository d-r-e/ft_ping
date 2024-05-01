#include <ft_ping.h>

int is_digit(const char *str){
    while (*str)
    {
        if (!isdigit(*str))
            return 0;
        str++;
    }
    return 1;
}