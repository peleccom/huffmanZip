#include "archive_format.h"
void file_write_header(FILE *archive, codes_array_t *codes, int64_t data_size){
	unsigned char c;
	int i;
	short codes_count = 0;
	long pos_codes_count, pos_data_begin;
	pos_codes_count = ftell(archive);
	fwrite(&codes_count, sizeof(codes_count), 1, archive);
	for(i =0; i < 256;i++)
	{
		if (codes->code[i] != NULL)
		{
			codes_count ++;
			c = i;
			fwrite(&c, sizeof(c), 1, archive);
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
	short codes_count;
	fread(&codes_count, sizeof(codes_count),1,archive);
	memset(codes->code, 0, sizeof(codes->code)); 
	for (j=0;j< codes_count; j++){
		fread(&i, sizeof(i), 1, archive);
		codes->code[i] = file_read_string(archive);
	}
	fread(data_size, sizeof(*data_size),1,archive);
}


void file_write_encrypted_data(FILE *fp, FILE *archive, codes_array_t *codes){
	unsigned char buffer_read[BUFFER_LENGTH];
	unsigned char buffer_write[BUFFER_LENGTH];
	int readed_bytes;
	int i,j,l;
	unsigned char current_byte, pos;
	char *code_s;
	int index_out_buf = 0;
	pos = 0x80;
	current_byte = 0;
	while(readed_bytes = fread(buffer_read, sizeof(*buffer_read),BUFFER_LENGTH, fp))
	{
		for(i=0; i<readed_bytes; i++){
			code_s = codes->code[buffer_read[i]];
			l = strlen(code_s);
			for (j=0; j<l; j++)
			{
				if (pos == 0) {
					buffer_write[index_out_buf] = current_byte;
					current_byte = 0;
					// next char
					pos = 0x80;
					
					index_out_buf++;
					if ((index_out_buf + 1)>= BUFFER_LENGTH){
						fwrite(buffer_write, sizeof(*buffer_write), index_out_buf, archive);
						index_out_buf = 0;
					}
				}
				if (code_s[j] == '1')
					current_byte |= pos;
				else
					current_byte &= ~pos;
 
				pos >>= 1;
			}
		}	
	}
	if (pos != 0x80)
		buffer_write[index_out_buf] = current_byte;
	fwrite(buffer_write, index_out_buf + 1, 1, archive);
}

void file_read_decrypted_data(FILE *archive, FILE *fp, tree_node_t *root, int64_t data_size){
	unsigned char buffer_read[BUFFER_LENGTH];
	unsigned char buffer_write[BUFFER_LENGTH];
	int readed_bytes;
	int64_t total_decrypted = 0;
	unsigned char pos;
	int index_out_buf = 0;
	int i;
	tree_node_t *current_node;
	pos = 0x80;
	current_node = root;
	while(readed_bytes = fread(buffer_read, sizeof(*buffer_read),BUFFER_LENGTH, archive)){
		for (i=0; i<readed_bytes; i++){
			while(pos){
				if (total_decrypted == data_size){
					fwrite(buffer_write, sizeof(*buffer_write), index_out_buf, fp);
					return;
				}
				if (buffer_read[i] & pos )
					current_node = current_node->right;
				else
					current_node = current_node->left;
				if (current_node->is_leaf){
					buffer_write[index_out_buf] = current_node->data;
					current_node = root;
					index_out_buf++;
					total_decrypted++;
					if ((index_out_buf + 1) >= BUFFER_LENGTH){
						fwrite(buffer_write, sizeof(*buffer_write), index_out_buf, fp);
						index_out_buf = 0;
					}
				}
				
				pos >>= 1;
			}
			pos = 0x80;
		}
	}
	fwrite(buffer_write, sizeof(*buffer_write), index_out_buf, fp);
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
