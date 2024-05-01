#include <ft_ping.h>


double pow(double x, double y){
    while (y > 1){
        x *= x;
        y--;
    }
    return x;
}