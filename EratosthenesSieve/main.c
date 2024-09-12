#include <stdio.h>

int proste(int cislo){
    for (int i = 2; i*i <= cislo; ++i) {
        if (cislo%i==0){
            return 0;
        }
    }
    return 1;
}
void sito(int a, int b){
    int k=1;
    for (int i = a; i <= b; ++i) {
        if (i>1&&proste(i)){
            if (k==1){
                printf("%d",i);
                k=0;
            } else{
                printf(", %d",i);
            }
        }
    }
}
int main(){
    int a,b;
    scanf("%d %d",&a,&b);
    sito(a,b);
    return 0;
}
