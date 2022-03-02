#include "libft.h"

/**
 * @brief Convert a short integer from host to network byte order
 * 
 * @param n 
 * @return unsigned short 
 */
unsigned short ft_htons(unsigned short n)
{
    return ((unsigned short) ((((n) >> 8) & 0xff) | (((n) & 0xff) << 8)));
}