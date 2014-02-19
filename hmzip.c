#include <stdio.h>
#include <unistd.h>

long dict[256];
#define BUFFER_SIZE 1024
unsigned char buffer[BUFFER_SIZE];

int create_archive(char *filename);
int extract_archive(char *filename);

int main(int argc, char *argv[]){
if (argc == 1){
    printf("Huffman zipping\n");
    printf("usage:\n");
    printf("hmzip -c <filename> : create new archive from file <filename>\n");
    printf("hmzip -x <archive_filename>: extract file from archive\n");
    return 0;
}
char *opts = "c:x:";
int opt;
int result = 0;
while ((opt = getopt(argc, argv, opts)) != -1){
    switch(opt){
        case 'c':
	   result = create_archive(optarg);
           break;
        case 'x':
           result = extract_archive(optarg);
           break;
	default:
           printf("sss");
	   break;
    }
}
return result;
}

void dict_statistic(){
    int i;
    printf("Bytes statistic:\n");
    for(i=0; i<256; i++)
        if ((i < 127) & (i > 32))
	    printf("byte %3i(ANSI '%c') - %li\n",i,i, dict[i]);
        else
            printf("byte %3i           - %li\n",i,dict[i]);
}

int create_archive(char *filename){
    FILE *fp;
    int n;
    short i;
    printf("create\n");
    if ((fp=fopen(filename, "rb")) == NULL){
        perror("Can't open file");
        return 1;
    }
    while( n = fread(&buffer,sizeof(unsigned char),BUFFER_SIZE, fp)){
        for(i=0; i < n;i++){
            dict[buffer[i]]++;
	}

    }
    dict_statistic();
    fclose(fp);
    
}




int extract_archive(char *filename){
    printf("extracting\n");
}
