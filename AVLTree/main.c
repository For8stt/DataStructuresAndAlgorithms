
#include <stdio.h>
#include <stdlib.h>

typedef struct vrchol{
    int veska;

    int hodnota;
    char str1[16];
    char str2[16];
    char narod[12];

    struct vrchol *lavy, *pravy,*otec;
}Vrchol;
void strcopy(char str[], char source[]) {
    int i;
    for (i=0; source[i] != '\0'; i++) {
        str[i] = source[i];
    }
    str[i] = source[i];
}
int getVyska(Vrchol *v) {
    if (v == NULL) {
        return -1;
    } else {
        return v->veska;
    }
}
void updateVyska(Vrchol *v) {
    while (v != NULL) {
        int leftHeight = getVyska(v->lavy);
        int rightHeight = getVyska(v->pravy);
        v->veska = 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
        v = v->otec;
    }
}
int getBelence(Vrchol *v) {
    if (v== NULL) {
        return 0;
    }
    return  getVyska(v->pravy)-getVyska(v->lavy);

}
void swap(Vrchol*a,Vrchol*b){
    int a_hod=a->hodnota;
    a->hodnota=b->hodnota;
    b->hodnota=a_hod;

    char temp[20];
    strcopy(temp,a->str1);
    strcopy(a->str1,b->str1);
    strcopy(b->str1,temp);


    strcopy(temp,a->str2);
    strcopy(a->str2,b->str2);
    strcopy(b->str2,temp);


    strcopy(temp,a->narod);
    strcopy(a->narod,b->narod);
    strcopy(b->narod,temp);

}
void leftRotate(Vrchol* s) {

    swap(s, s->pravy);
    Vrchol* buffer = s->lavy;
    s->lavy = s->pravy;
    s->pravy = s->lavy->pravy;
    s->lavy->pravy = s->lavy->lavy;
    s->lavy->lavy = buffer;

    if (s->pravy!=NULL)s->pravy->otec=s;
    if (buffer!=NULL) {
        buffer->otec = s->lavy;
    }


    updateVyska(s->lavy);
    updateVyska(s);
}
void rightRotate(Vrchol*s){

    swap(s,s->lavy);
    Vrchol* buffer=s->pravy;
    s->pravy=s->lavy;

    s->lavy=s->pravy->lavy;

    s->pravy->lavy=s->pravy->pravy;

    s->pravy->pravy=buffer;

    if (s->lavy!=NULL) s->lavy->otec=s;
    if (buffer!=NULL) {
        buffer->otec = s->pravy;
    }

    updateVyska(s->pravy);
    updateVyska(s);
}


void balance(Vrchol* s){
    if (s==NULL){
        return;
    }
    int bal= getBelence(s);
    if (bal==-2){
        if (getBelence(s->lavy)==1) leftRotate(s->lavy);
        rightRotate(s);
    } else if(bal==2){
        if (getBelence(s->pravy)==-1) rightRotate(s->pravy);
        leftRotate(s);
    }
    balance(s->otec);
}


void insert(Vrchol **s,char* buffer){
    Vrchol* novy=(Vrchol*)malloc(sizeof(Vrchol));


    novy->lavy=NULL;
    novy->pravy=NULL;
    novy->otec=NULL;
    novy->veska = 0;
//    novy->str1 = (char*)malloc(16 * sizeof(char));
//    novy->str2 = (char*)malloc(16 * sizeof(char));
//    novy->narod = (char*)malloc(12 * sizeof(char));

//    if (novy->str1 == NULL || novy->str2 == NULL || novy->narod == NULL) {
//        free(novy->str1);
//        free(novy->str2);
//        free(novy->narod);
//        free(novy);
//        return;
//    }


    sscanf(buffer, "%d %s %s %s",&novy->hodnota,novy->str1,novy->str2,novy->narod);


    if (*s==NULL){
        *s=novy;
    }else{

        Vrchol *temp=*s;
        Vrchol *otec_temp=temp;

        while (1){
            if (novy->hodnota<=temp->hodnota){
                if (temp->lavy==NULL){
                    temp->lavy=novy;
                    novy->otec = otec_temp;
                    break;
                } else{
                    temp=temp->lavy;
                    otec_temp = temp;
                }
            } else{
                if (temp->pravy==NULL){
                    temp->pravy=novy;
                    novy->otec = otec_temp;
                    break;
                } else{
                    temp=temp->pravy;
                    otec_temp = temp;
                }
            }
        }

    }

    updateVyska(novy);
    balance(novy);

}


void search(Vrchol *s,int hod,int* first){
    Vrchol *temp =s;
    while (temp!=NULL && temp->hodnota!=hod){
        if(hod<temp->hodnota){
            temp=temp->lavy;
        } else{
            temp = temp->pravy;
        }
    }
    if (temp!=NULL && *first==0){
        printf("%d %s %s %s",temp->hodnota,temp->str1,temp->str2,temp->narod);
        *first=1;
    }
    else if(temp!=NULL && *first==1) {
        printf("\n%d %s %s %s",temp->hodnota,temp->str1,temp->str2,temp->narod);
    }
}
void printInRange(Vrchol *root, int hod1, int hod2,int *first) {
    if (root == NULL) {
        return;
    }

    if (root->hodnota > hod1) {
        printInRange(root->lavy, hod1, hod2,first);
    }

    if (root->hodnota >= hod1 && root->hodnota <= hod2 && *first==0) {
        printf("%d %s %s %s", root->hodnota,root->str1,root->str2,root->narod);
        *first=1;
    } else if (root->hodnota >= hod1 && root->hodnota <= hod2 && *first==1){
        printf("\n%d %s %s %s", root->hodnota,root->str1,root->str2,root->narod);
    }

    if (root->hodnota < hod2) {
        printInRange(root->pravy, hod1, hod2,first);
    }
}

Vrchol *TreeMin(Vrchol*T){
    Vrchol *temp=T;
    while (temp->lavy!=NULL){
        temp=temp->lavy;
    }
    return temp;
}

Vrchol* successor(Vrchol*T){
    Vrchol *temp=T;
    if (temp->pravy!=NULL){
        Vrchol*d =TreeMin(temp->pravy);
        return d;
    } else {
        Vrchol *s = temp->otec;
        while (s != NULL && temp == s->pravy) {
            temp = s;
            s = temp->otec;
        }
        return s;
    }

}
Vrchol *searchh(Vrchol *s,int hod){
    Vrchol *temp =s;
    while (temp!=NULL && temp->hodnota!=hod){
        if(hod<temp->hodnota){
            temp=temp->lavy;
        } else{
            temp = temp->pravy;
        }
    }
    return temp;
}
void delete(Vrchol** s,int hod){
    Vrchol *y=NULL;
    Vrchol *x=NULL;
    Vrchol *v =searchh(*s,hod);

    if (v == NULL) {
        return;
    }

    if (v->lavy==NULL || v->pravy==NULL){
        y=v;
    } else{
        y = successor(v);
    }

    if (y->lavy!=NULL){
        x=y->lavy;
    } else{
        x=y->pravy;
    }

    if (x!=NULL){
        x->otec = y->otec;
    }

    Vrchol *j=y->otec;

    if (y->otec==NULL){
        *s=x;
    } else if(y==j->lavy){
        j->lavy=x;
    } else{
        j->pravy=x;
    }

    if (y!=v){
        swap(v,y);
    }

    updateVyska(y);
    balance(y);
    free(y);

}


void uvolni_strom(Vrchol **koren) {
    if (*koren != NULL) {
        uvolni_strom(&((*koren)->lavy));
        uvolni_strom(&((*koren)->pravy));
        (*koren)->otec = NULL;

        free(*koren);
        *koren = NULL;
    }
}

int main() {
    int first=0;
    Vrchol* s = NULL;

    char typ;
    char *buffer = (char*) malloc(60 * sizeof(char));

    while (scanf("%c ",&typ)!=EOF){
        fgets(buffer, 60, stdin);
        if (typ=='s'){
            int hod1=0,hod2=0;
            sscanf(buffer, "%d %d", &hod1,&hod2);
            if (hod2!=0){
                if (hod1>hod2){
                    int temp=hod1;
                    hod1=hod2;
                    hod2=temp;
                }
                printInRange(s,hod1,hod2,&first);
            } else {
                search(s,hod1,&first);
            }
        } else if (typ=='i'){

            insert(&s,buffer);

        }  else if (typ=='d'){
            int hod5;
            sscanf(buffer, "%d", &hod5);
            delete(&s,hod5);
        }else{
            break;
        }
    }
    free(buffer);
    uvolni_strom(&s);
    free(s);
    return 0;
}