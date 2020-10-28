#include <stdio.h>

int is_prime(int n)
{
    if(n==2)
        return 1;
    else
    {
        for(int pos = 2 ; pos < n ;pos++)
        {

            if( (n%pos) == 0)
                return 0;
        }
    }
    return 1;
}

int main(void){
    printf("%d\n", is_prime(11));  // 11 is a prime.      Should print 1.
    printf("%d\n", is_prime(383)); // 383 is a prime.     Should print 1.
    printf("%d\n", is_prime(987)); // 987 is not a prime. Should print 0.
}
