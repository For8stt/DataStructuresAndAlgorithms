#include <stdio.h>
#include <stdlib.h>

long binar(long *firstCilo,long *lastCislo,long highh,long nfirstCislo,long nlastCislo){

    long low=0;
    long high=highh-1;


    while (low<=high){
        long midle=(high+low)/2;

        long value1= firstCilo[midle];
        long value2= lastCislo[midle];


        if (nfirstCislo>=0){
            if (nfirstCislo>value1 || (nfirstCislo==value1 && nlastCislo>value2) )
                low=midle+1;
            else if (nfirstCislo<value1 || (nfirstCislo==value1 && nlastCislo<value2) )
                high=midle-1;
            else
                return midle;
        } else{
            if (nfirstCislo>value1 || (nfirstCislo==value1 && nlastCislo<value2) )
                low=midle+1;
            else if (nfirstCislo<value1 || (nfirstCislo==value1 && nlastCislo>value2) )
                high=midle-1;
            else
                return midle;
        }

    }
    return -1;
}



int main() {

    int n,m;

    scanf("%d",&n);
    if (n<1||n>50000){
        return 0;
    }
//    long firstCislo[n];
//    long lastCislo[n];
    long *firstCislo = (long*)malloc(n * sizeof(long));
    long *lastCislo = (long *)malloc(n * sizeof(long));


    if (firstCislo == NULL || lastCislo == NULL) {
        return 0;
    }

    for (int i = 0; i < n; ++i) {
        char buffer[256];
        scanf("%s",buffer);

        int length=0;
        int k=0;

        while (buffer[k]!='\0'){
            k++;
        }
        while (buffer[length]!=',' && buffer[length]!='\0'){
            length++;
        }

        if (length<k){

            sscanf(buffer, "%ld", &firstCislo[i]);

            sscanf(buffer + length+1, "%ld", &lastCislo[i]);
            for (int j = 0; j <10-(k-length) ; ++j) {
                lastCislo[i]=lastCislo[i]*10;
            }
        } else{
            sscanf(buffer, "%ld", &firstCislo[i]);
            lastCislo[i] = 0;
        }

    }


    scanf("%d",&m);
    if (m<1 || m>100000){
        free(firstCislo);
        free(lastCislo);
        return 0;
    }



    for (int i = 0; i < m; ++i) {
        long nfirstCislo=0;
        long nlastCislo=0;

        char buffer[256];
        scanf("%s",buffer);

        int length=0;
        int k=0;
        while (buffer[k]!='\0'){
            k++;
        }
        while (buffer[length]!=',' && buffer[length]!='\0'){
            length++;
        }


        if (length<k){

            sscanf(buffer, "%ld", &nfirstCislo);

            sscanf(buffer + length+1, "%ld", &nlastCislo);
            for (int j = 0; j <10-(k-length) ; ++j) {
                nlastCislo=nlastCislo*10;
            }
        } else{
            sscanf(buffer, "%ld", &nfirstCislo);
        }


        long poz= binar(firstCislo,lastCislo,n,nfirstCislo,nlastCislo);
        printf("%ld",poz+1);

        if (i<m-1){
            printf("\n");
        }
    }

    free(firstCislo);
    free(lastCislo);

    return 0;
}