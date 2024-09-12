#include <stdio.h>
#include <stdlib.h>

int peretvor(char ch) {
    return (ch == '(' || ch == '{' || ch == '[' || ch == '<');
}

int charcmp(char stack, char ch) {
    return (stack == '(' && ch == ')') || (stack == '{' && ch == '}') || (stack == '[' && ch == ']') || (stack == '<' && ch == '>');
}

int errors(char* sequence) {
    int Ustack = 0, numerr = 0;

    for (int k = 0; sequence[k] != '\0'; k++) {
        if (peretvor(sequence[k])) {
            sequence[Ustack++] = sequence[k];
        } else if (Ustack > 0 && charcmp(sequence[--Ustack], sequence[k])) {
            continue;
        } else {
            numerr++;
        }
    }
    return (numerr + Ustack);
}

int main() {
    int n;
    scanf("%d", &n);
    char* arr = (char*)malloc(100000 * sizeof(char));

    for (int i = 0; i < n; i++) {
        scanf("%s", arr);
        int d = errors(arr);
        printf("%d", d);
        if (i<n-1){
            printf("\n");
        }
    }

    free(arr);
    return 0;
}