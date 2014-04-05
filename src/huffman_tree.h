#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>


#define MAX_CODE_SIZE 100;
typedef struct frequency_array_t {
    uint64_t freq[256];
} frequency_array_t;


typedef struct codes_array_t {
    char* code[256];
} codes_array_t;


typedef struct tree_node_t{
	struct tree_node_t *left;
	struct tree_node_t *right;
	uint64_t weight;
	short is_leaf;
	unsigned char data;
} tree_node_t;

codes_array_t* generate_codes(frequency_array_t *frequency_array);
void free_codes(codes_array_t* codes);
void free_tree(tree_node_t *root);
/**
 * generate huffman tree from codes map
 */
tree_node_t* generate_tree(codes_array_t* codes);
#endif
