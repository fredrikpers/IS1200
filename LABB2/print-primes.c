#include <stdio.h>
#include <stdlib.h>

#define COLUMNS 6

int is_prime(int n)
{
    if(n==1)
        return 0;
    if(n==2)
        return 1;

        for(int pos = 2 ; pos < n ;pos++)
        {
            if( (n%pos) == 0)
                return 0;
        }
    return 1;
}
int change_column = 1;
void print_number(int n)
{
    printf("%10d", n);
    if(change_column % COLUMNS == 0)
        printf("\n");
    change_column++;
}

void print_primes(int n)
{
    for (int pos = 1; pos <= n; pos++)
        if (is_prime(pos) == 1)
            print_number(pos);
}

int main(int argc, char *argv[]){
    if(argc == 2)
        print_primes(atoi(argv[1]));
    else
        printf("Please state an interger number.\n");
    return 0;
}
