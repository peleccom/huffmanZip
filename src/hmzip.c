#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "huffman_tree.h"
#include "archive_format.h"
#include "my_utils.h"
#include <inttypes.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>

frequency_array_t frequency_array;
#define BUFFER_SIZE 65536
unsigned char buffer[BUFFER_SIZE];
int verbose;

int create_archive(char *filename, char *archive_name);
int extract_archive(char *archive, char *file);
char* replace_ext(char *filename, char *new_ext);


void usage(){
	printf("****Huffman zipping****\n");
    printf("usage:\n");
    printf("hmzip -c <filename> : create new archive from file <filename>\n");
    printf("hmzip -x <archive_filename>: extract file from archive\n");
    printf("use -v for verbose mode\n");
}
int main(int argc, char *argv[]){
	if (argc == 1){
		usage();
		return 0;
	}
	char *out_filename = NULL;
	char *default_out_filename = NULL;
	char *input_filename = NULL;
	char *opts = "vc:x:o:";
	int opt;
	int result = 1;
	int create_mode ,extract_mode;
	create_mode  = extract_mode = 0;

	verbose = 0;
	while ((opt = getopt(argc, argv, opts)) != -1){
		switch(opt){
			case 'v':
				verbose++;
				break;
			case 'c':
				if (extract_mode)
				{
					printf("Invalid options combination\n");
					usage();
					return 2;
				}
				default_out_filename = malloc((strlen(optarg) +4 )* sizeof(char));
				strcpy(default_out_filename, optarg);
				strcat(default_out_filename, ".hz");
				out_filename = default_out_filename;
				input_filename = optarg;
				create_mode = 1;
				break;
			case 'x':
				if (create_mode){
					printf("Invalid options combination\n");
					usage();
					return 2;
				}
				default_out_filename = replace_ext(optarg, "");
				out_filename = default_out_filename;
				input_filename = optarg;
				extract_mode = 1;
				break;
			case 'o':
				out_filename = optarg;
				break;
			default:
				printf("Invalid options\n");
				break;
		}
    
	}
	if (create_mode)
	{
		result = create_archive(input_filename, out_filename);
		FREE(default_out_filename);
		return result;
	}
	if (extract_mode){
		if (!out_filename){
			out_filename = replace_ext(optarg, "");
		}
		result = extract_archive(input_filename, out_filename);
		FREE(default_out_filename);
		return result;
	}
	
	printf("invalid input\n");
	usage();
	return 1;
}



void dict_statistic(){
    int i;
    printf("Bytes statistic:\n");
    for(i=0; i<256; i++)
        if ((i < 127) & (i > 32))
	    printf("byte %3i(ANSI '%c') - %"PRIu64"\n",i,i, frequency_array.freq[i]);
        else
            printf("byte %3i           - %"PRIu64"\n",i,frequency_array.freq[i]);
}

int create_archive(char *filename, char *archive_name){
    FILE *fp;
    FILE *archive_fp;
    int n;
    int i;
    struct stat stat_s;
    int64_t data_size;
    if ((fp=fopen(filename, "rb")) == NULL){
        perror("Can't open file");
        return 1;
    }
    if ((archive_fp=fopen(archive_name, "wr")) == NULL){
        perror("Can't create archive file");
        return 1;
    }
    while( (n = fread(buffer,sizeof(*buffer),BUFFER_SIZE, fp))){
        for(i=0; i < n;i++){
            frequency_array.freq[buffer[i]]++;
	}

    }
    stat(filename, &stat_s);
    data_size = stat_s.st_size;
    if (verbose)
		dict_statistic();
    codes_array_t *codes = generate_codes(&frequency_array);
	fseek(fp, 0, SEEK_SET);
	file_write_header(archive_fp, codes, data_size);
    file_write_encrypted_data(fp, archive_fp, codes);
	fclose(fp);
    fclose(archive_fp);
    free_codes(codes);
    FREE(codes);
    return 0;
}

char* replace_ext(char *filename, char *new_ext){
	int i;
	int l  = strlen(filename);
	int e_l = strlen(new_ext);
	char *s = malloc(sizeof(char) * (l + 1 + e_l));
	int pos;
	for(i=l-1; i>=0;i--)
	{
		if ((filename[i] == '.') || (filename[i] == '/') || (filename[i] =='\\'))
			break;
	}
	if (i > 0) 
		pos = i;
	else
		pos = l;
	strncpy(s, filename, pos);
	strcat(s, new_ext);
	return s;
}


int extract_archive(char *archive_filename, char *filename){
	FILE *fp;
	FILE *archive;
	codes_array_t codes;
	tree_node_t *root;
	int64_t data_size;
	if ((archive=fopen(archive_filename, "rb")) == NULL){
		perror("Can't open file");
        return 1;
	} 
	if ((fp=fopen(filename, "w+")) == NULL){
		perror("Can't create file");
        return 1;
	} 
	file_read_header(archive, &codes, &data_size);
	root = generate_tree(&codes);
	free_codes(&codes);
	file_read_decrypted_data(archive, fp,root, data_size);
	free_tree(root);
	fclose(fp);
	fclose(archive);
	return 0;
}
