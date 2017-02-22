#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 128
#define MAX_HASH 256
#define FILENAME "tom.txt"
#define TIMES 1000

struct item{
	int count;
	char *word;
	struct item *next;
};
struct item *hashtable[MAX_HASH]={0};

char *create_string(char *str){
	char *cp;
	cp = malloc(strlen(str)+1);
	strcpy(cp,str);
	return cp;
}

struct item *create_item(char *key, struct item *p){
	struct item *ret = malloc(sizeof(struct item));
	ret->count = 1;
	ret->word = create_string(key);
	ret->next = p;
	return ret;
}

struct item *search_item(char *key, struct item *p){
	while (p) {
		if(!strcmp(key,p->word))
			return p;
		p = p->next;
	}
	return 0;
}

int hash(char *str){
	int ret=0;
	while(*str){
		if(*str>='A'&&*str<='Z')
			*str=*str-('A'-'a');
		ret += *str;
		str++;
	}
	return ret % MAX_HASH;
}

void chainhash(char *key){
	int h;
	struct item *list,*p;
	h = hash(key);
	list = hashtable[h];
	p = search_item(key, list);

	if (p)
		p->count++;
	else
		hashtable[h] = create_item(key,list);
}

struct item *delete_item(char *key, struct item *p){
	struct item *ori_p=p,*old_p;

	if(p && !strcmp(key,ori_p->word)){
		free(ori_p);
		return p->next;
	}

	while (p) {
		old_p = p;
		p = p->next;
		if (!strcmp(key,p->word)) {
			old_p->next = p->next;
			free(p);
			break;
		}
	}
	return ori_p;
}

void print_list(struct item *p);

int delete_key(char *key){
	int h;
	struct item *p,*list;

	h = hash(key);
	list = hashtable[h];
	p = search_item(key, list);

	if (p){
		printf("befor: [%d],",h);
		print_list(hashtable[h]);
		printf("\ndelete %s.\n",key);
		hashtable[h] = delete_item(key,hashtable[h]);
		printf("after: [%d],",h);
		print_list(hashtable[h]);
		return 1;
	} else {
		printf("%s is not in the hash table.\n",key);
		return 0;
	}

}

void print_list(struct item *p) {
	if(p==0)
		printf("this list is NULL.\n");


	while (p) {
		printf("=> <%s,%d> ", p->word, p->count);
		p = p->next;
	}
	printf("\n");
}

void dump_ht(){
	int i;
	for(i=0;i<256;i++){
		if(hashtable[i]){
			printf("[%d],",i);
			print_list(hashtable[i]);
		}
	}
}

int main(int argc, char *argv[])
{
	FILE *istream;
	char getstr[SIZE],delstr[SIZE];
	int i,h;

	if((istream = fopen(FILENAME, "r")) == 0 ) {
		fprintf(stderr, "%s\n", "error: can't read file.");
		exit(EXIT_FAILURE);
	}
	
	for(i=0;i<TIMES;i++){
		if(fscanf(istream,"%s",getstr)== EOF )
			break;
		chainhash(getstr);
	}

	fclose(istream);
	dump_ht();

	while(1){
		printf("\n");
		printf("inp:");
		scanf("%s",delstr);

		if(!strcmp(delstr,"DUMP_HT"))
			dump_ht();
		else if(!strcmp(delstr,"EXIT_DEL"))
			break;
		else
			delete_key(delstr);
	}


	return EXIT_SUCCESS;
}
