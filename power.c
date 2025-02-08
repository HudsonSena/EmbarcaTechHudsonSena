#include "stdio.h"

int main()
{
    int a, b, pow = 1;

    printf("Digite o primeiro numero: ");
    scanf("%d", &a);
    printf("Digite o segundo numero: ");
    scanf("%d", &b);

    for (int i = 1; i <= b; i++)
    {
        pow = pow * a;
    }

    printf("%d", pow);
}