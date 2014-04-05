#ifndef ARCHIVE_FORMAT_H
#define ARCHIVE_FORMAT_H
#include "huffman_tree.h"
#include <stdio.h>
#include <string.h>

#define BUFFER_LENGTH 2048
/* void pack_file(FILE *orig_fp, 
			FILE *archive_fp, codes_array_t *codes);
void unpack_file(FILE *archive_fp, 
			codes_array_t *codes, FILE *unpacked_fp);
*/
/**
 * write archive file header
 */
void file_write_header(FILE *archive, codes_array_t *codes, int64_t data_size);
/**
 * read archive file header
 */
void file_read_header(FILE *archive, codes_array_t *codes, int64_t *data_size);
/**
 * write string to file
 */
void file_write_string(char *s, FILE *fp);
/**
 * alloc memory and
 * read string from file 
 */
char* file_read_string(FILE *fp);


void file_write_encrypted_data(FILE *fp, FILE *archive, codes_array_t *codes);

void file_read_decrypted_data(FILE *archive, FILE *fp, tree_node_t *root, int64_t data_size);

void write_encrypt(unsigned char *data, int64_t size, codes_array_t codes, FILE *fp);
#endif
