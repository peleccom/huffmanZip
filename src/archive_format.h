#ifndef ARCHIVE_FORMAT_H
#define ARCHIVE_FORMAT_H
#include "huffman_tree.h"
#include <stdio.h>
#include <string.h>
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

void write_encrypt(unsigned char *data, int64_t size, codes_array_t codes, FILE *fp);
#endif
