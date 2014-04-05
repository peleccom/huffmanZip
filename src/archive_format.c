#include "archive_format.h"
void file_write_header(FILE *archive, codes_array_t *codes, int64_t data_size){
	unsigned char i;
	unsigned char codes_count = 0;
	long pos_codes_count, pos_data_begin;
	
	pos_codes_count = ftell(archive);
	fwrite(&codes_count, sizeof(codes_count), 1, archive);
	for(i =0; i < 255;i++)
	{
		if (codes->code[i] != NULL)
		{
			codes_count ++;
			fwrite(&i, sizeof(i), 1, archive);
			file_write_string(codes->code[i], archive);
		}
	}
	fwrite(&data_size, sizeof(data_size), 1, archive);
	pos_data_begin = ftell(archive);
	fseek(archive, pos_codes_count ,SEEK_SET);
	fwrite(&codes_count, sizeof(codes_count), 1, archive);
	fseek(archive, pos_data_begin ,SEEK_SET);
}

void file_read_header(FILE *archive, codes_array_t *codes, int64_t *data_size){
	unsigned char i;
	int j;
	unsigned char codes_count;
	fread(&codes_count, sizeof(codes_count),1,archive);
	memset(codes->code, 0, sizeof(codes->code)); 
	for (j=0;j< codes_count; j++){
		fread(&i, sizeof(i), 1, archive);
		codes->code[i] = file_read_string(archive);
	}
	fread(data_size, sizeof(*data_size),1,archive);
}


void file_write_string(char *s, FILE *fp){
	uint16_t l = strlen(s);
	int i;
	fwrite(&l, sizeof(l), 1, fp);
	
	for(i=0; i<l ; i++)
	{
		char c = s[i];
		fwrite(&c,sizeof(c), 1, fp);
	}
}


char* file_read_string(FILE *fp){
	uint16_t l;
	int i;
	char *s;
	char c;
	fread(&l, sizeof(l),1, fp);
	s  = malloc(sizeof(char) * (l + 1));
	if (!s) return NULL;
	for(i=0; i<l;i++){
		fread(&c, sizeof(c),1,fp);
		s[i] = c;
	}
	s[l] = 0;
	return s;
}
