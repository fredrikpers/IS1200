#include <stdio.h>
#include <math.h>

#define COLUMNS 6

int change_column = 1;
void print_number(int n)
{
    printf("%10d", n);
    if(change_column % COLUMNS == 0)
        printf("\n");
    change_column++;
}

void print_sieves(int n) {
    int sequence_of_numbers[n - 1];

    for (int pos = 0; pos < n - 1; pos++) {
        sequence_of_numbers[pos] = pos + 2;
    }

    for (int i = 2; i <= sqrt(n); i++)
    {
        if (sequence_of_numbers[i - 2] != 0)
        {
            for (int j = i * i; j <= n; j += i)
                sequence_of_numbers[j - 2] = 0;
        }

    }
    for (int k = 0; k < n - 1; k++)
    {
        if (sequence_of_numbers[k] != 0)
            print_number(sequence_of_numbers[k]);
    }

}

int main(int argc, char *argv[]){
    if(argc == 2)
        print_sieves(atoi(argv[1]));
    else
        printf("Please state an interger number.\n");
    return 0;
}
