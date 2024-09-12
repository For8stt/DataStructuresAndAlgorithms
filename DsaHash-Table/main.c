#include <stdio.h>
#include <stdlib.h>


typedef struct person{
    char name[16];
    char priezvisko[16];
    char date[11];
    float sum;

    struct person* next;
}person;


typedef struct HashTable{
    person **items;
    int size;
    int count;
} HashTable;

int is_prime(int num) {
    if (num <= 1) {
        return 0;
    }
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) {
            return 0;
        }
    }
    return 1;
}

int next_prime(int num) {
    int next = num + 1;
    while (!is_prime(next)) {
        next++;
    }
    return next;
}

int my_strlen(const char *str) {
    int length = 0;
    while (str[length] != '\0') {
        length++;
    }
    return length;
}
int my_strcmp(char *str1, char *str2) {
    int i = 0;
    while (str1[i] != '\0' || str2[i] != '\0') {
        if (str1[i] != str2[i]) {
            return str1[i] - str2[i];
        }
        i++;
    }
    return 0;
}
void prevod(float cislo,int *first) {

    char str[20];
    sprintf(str, "%.2f", cislo);


    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '.') {
            str[i] = ',';
            break;
        }
    }
    if (*first==0){
        printf("%s", str);
        (*first)=1;
    } else{
        printf("\n%s", str);
    }

}

person *hash_table_lookup(char * name,char * priz,char * date,HashTable *h,int size);
HashTable *resize_table(HashTable *old_table, int new_size);

HashTable *create_table(int size) {
    HashTable *table = (HashTable *) malloc(sizeof(HashTable));
    table->size = size;
    table->count = 0;
    table->items = (person **) calloc(table->size, sizeof(person *));

    for (int i = 0; i < table->size; i++) {
        table->items[i] = NULL;
    }

    return table;
}

void free_table(HashTable *table) {
    if (table == NULL) {
        return;
    }

    free(table->items);
    free(table);
}


//unsigned int hash(char *name,int size,char*priz,char *date){
//    int length = my_strlen(name);
//    unsigned int hash_value=0;
//    for (int i = 0; i < length; ++i) {
//        hash_value+=name[i];
//        hash_value=(hash_value*name[i])%size;
//    }
//
//    int length1 = my_strlen(priz);
//    for (int i = 0; i < length1; ++i) {
//        hash_value+=priz[i];
//        hash_value=(hash_value*priz[i])%size;
//    }
//
//    int length2 = my_strlen(date);
//    for (int i = 0; i < length2; ++i) {
//        hash_value+=date[i];
//        hash_value=(hash_value*date[i])%size;
//    }
//
//    return hash_value;
//
//}
unsigned long hash(const char* name, int size, const char* priz, const char* date) {
    unsigned long hash = 5381;
    int c;


    while ((c = *name++)) {
        hash = ((hash << 5) + hash) + c;
    }

    while ((c = *priz++)) {
        hash = ((hash << 5) + hash) + c;
    }

    while ((c = *date++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash % size;
}

void print_table(HashTable *h,int size){
    printf("Start\n");
    for (int i = 0; i < size; ++i) {
        if (h->items[i]==NULL) {
            printf("\t%i\t----\n", i);
        } else{
            printf("\t%i\t",i);
            person *tmp=h->items[i];
            while (tmp!=NULL){
                printf("%s %s %s %.2f - ",tmp->name,tmp->priezvisko,tmp->date,tmp->sum);
                tmp=tmp->next;
            }
            printf("\n");
        }
    }
    printf("End\n");
}

void hash_table_insert(person* p,HashTable **h,int *size,int *first){

    if (p==NULL)return ;

    int index= (int)hash(p->name,*size,p->priezvisko,p->date);

    person *existing = hash_table_lookup(p->name,p->priezvisko,p->date, *h,*size);

    if (existing != NULL) {
        if(*first==0){
            printf("insert failed");
            *first=1;
        }else{
            printf("\ninsert failed");
        }
        free(p);
        return;
    }

    p->next=(*h)->items[index];
    (*h)->items[index]=p;
    (*h)->count++;

    if ((*h)->count>=*size){
        *(size)=next_prime((int)((*size)*1.4));
//        *(size)*=2;
        *h=resize_table(*h,*size);
    }
}

person *hash_table_lookup(char * name,char * priz,char * date,HashTable *h,int size){
    int index= (int)hash(name,size,priz,date);
    person *tmp=h->items[index];
    while (tmp!=NULL && (my_strcmp(tmp->name, name) != 0 || my_strcmp(tmp->priezvisko, priz) != 0 || my_strcmp(tmp->date, date) != 0)){

        tmp=tmp->next;
    }
    return tmp;
}

void hash_table_delete(char * name,char * priz,char * date,HashTable *h,int size,int *first){
    int index= (int)hash(name,size,priz,date);
    person *tmp=h->items[index];
    person *prev=NULL;

    while (tmp!=NULL && (my_strcmp(tmp->name, name) != 0 || my_strcmp(tmp->priezvisko, priz) != 0 || my_strcmp(tmp->date, date) != 0)){
        prev= tmp;
        tmp=tmp->next;
    }

    if (tmp==NULL){
        if(*first==0){
            printf("delete failed");
            *first=1;
        }else{
            printf("\ndelete failed");
        }

        return;
    }
    if (prev==NULL){
        h->items[index]=tmp->next;
    } else{
        prev->next=tmp->next;
    }
    free(tmp);
    h->count--;

}


HashTable *resize_table(HashTable *old_table, int new_size) {

    HashTable *new_table = create_table(new_size);


    for (int i = 0; i < old_table->size; i++) {
        person *current_person = old_table->items[i];
        while (current_person != NULL) {
            person *next_person = current_person->next;

            int new_index = (int)hash(current_person->name, new_size, current_person->priezvisko, current_person->date);

            current_person->next = new_table->items[new_index];
            new_table->items[new_index] = current_person;

            current_person = next_person;
        }
    }
    new_table->count = old_table->count;


    free_table(old_table);

    return new_table;
}

void update_person_sum(person* p, float new_sum) {
    p->sum += new_sum;
}
void update_person_sum_by_key(char* name, char* priz, char* date, HashTable* h, int size, float new_sum,int *first) {
    person* p = hash_table_lookup(name, priz, date, h, size);
    if (p != NULL) {
        if ((p->sum+new_sum)>=0) {
            update_person_sum(p, new_sum);
        } else{
            if(*first==0){
                printf("update failed");
                *first=1;
            }else{
                printf("\nupdate failed");
            }

        }
    } else {
        if(*first==0){
            printf("update failed");
            *first=1;
        }else{
            printf("\nupdate failed");
        }
    }
}
person* create_person(char* buffer,int *first) {
    person* new_person = (person*)malloc(sizeof(person));

    for (int i = 0; buffer[i] != '\0'; i++) {
        if (buffer[i] == ',') {
            buffer[i] = '.';
        }
    }

    sscanf(buffer, "%s %s %s %f", new_person->name, new_person->priezvisko, new_person->date, &new_person->sum);
    if (new_person->sum <0) {
        if(*first==0){
            printf("insert failed");
            *first=1;
        }else{
            printf("\ninsert failed");
        }
        free(new_person);
        return NULL;
    }
    new_person->next = NULL;

    return new_person;
}

int main() {
    int size=31;
    HashTable *h= create_table(size);

    char typ;
    float hod;
    int first=0;
    char *meno=(char*) malloc(12 * sizeof(char));
    char*priz=(char*) malloc(12 * sizeof(char));
    char*date=(char*) malloc(12 * sizeof(char));
    char *buffer = (char*) malloc(60 * sizeof(char));

    while (scanf("%c ",&typ)!=EOF){
        fgets(buffer, 60, stdin);
        if (typ=='s'){

            sscanf(buffer, "%s %s %s", meno,priz,date);
            person *d=hash_table_lookup(meno,priz,date,h,size);
            if (d!=NULL){
                prevod(d->sum,&first);
            } else{
                if (first==0){
                    printf("search failed");
                    first=1;
                } else{
                    printf("\nsearch failed");

                }
            }

        } else if (typ=='i'){

            person* p1=create_person(buffer,&first);
            if (p1!=NULL){
                hash_table_insert (p1,&h,&size,&first);
            }


        }  else if (typ=='d'){

            sscanf(buffer, "%s %s %s", meno,priz,date);
            hash_table_delete(meno,priz,date,h,size,&first);
        } else if (typ=='u'){

            for (int i = 0; buffer[i] != '\0'; i++) {
                if (buffer[i] == ',') {
                    buffer[i] = '.';
                }
            }
            sscanf(buffer, "%s %s %s %f", meno,priz,date,&hod);
            update_person_sum_by_key(meno,priz,date,h,size,hod,&first);
        }else if (typ=='p'){
            print_table(h,size);
        }else{
            break;
        }
    }


    free(buffer);
    free(meno);
    free(priz);
    free(date);
    free_table(h);
    return 0;
}
