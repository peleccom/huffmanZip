#include <string.h>
#include "my_utils.h"
#include "huffman_tree.h"
#include "sorted_queue.h"


int cmp(const void *a, const void *b){
	uint64_t w_a = ((tree_node_t *)a)->weight;
	uint64_t w_b = ((tree_node_t *)b)->weight;
	if (w_a > w_b) return 1;
	if (w_a < w_b) return -1;
	return 0;
}

void rebuild_tree(sorted_queue_t queue){
	tree_node_t *node1;
	tree_node_t *node2;
	tree_node_t *root_node;
	while(sorted_queue_count(queue) > 1)
	{
		node1 = sorted_queue_pop(queue);
		node2 = sorted_queue_pop(queue);
		root_node = malloc(sizeof(tree_node_t));
		CHKPTR(root_node);
		root_node->left = node1;
		root_node->right = node2;
		root_node->is_leaf = 0;
		root_node->weight = node1->weight  +node2->weight;
		sorted_queue_append(queue, root_node);
	}
}

void walk(char *path, tree_node_t *node, codes_array_t *codes){
	printf("%s\n",path);
	if (node->is_leaf){
		char *str = malloc((strlen(path)+ 1) * sizeof(char));
		strcpy(str, path);
		codes->code[node->data] = str;
		FREE(node);
		return;
	}
	char *left_path, *right_path;
	if (node->left){
		left_path = malloc((strlen(path)+ 2) * sizeof(char));
		strcpy(left_path, path);
		strcat(left_path, "0");
		walk(left_path, node->left, codes);
		FREE(left_path);
	}
	if (node->right){
		right_path = malloc((strlen(path)+ 2) * sizeof(char));
		strcpy(right_path, path);
		strcat(right_path, "1");
		walk(right_path, node->right, codes);
		FREE(right_path);
	}
}

codes_array_t* generate_codes_map(sorted_queue_t queue){
	int i;
	codes_array_t *codes;
	codes = malloc(sizeof(codes_array_t));
	for(i=0;i<256;i++)
		codes->code[i] = NULL;
	tree_node_t *root = sorted_queue_pop(queue);
	walk("", root, codes);
	return codes;
}

void free_codes_map(codes_array_t* codes){
	int i;
	for(i=0;i<256;i++)
		FREE(codes->code[i])
	FREE(codes);
}


void generate_tree(frequency_array_t *frequency_array){
	int i;
	sorted_queue_t queue;
	tree_node_t *node;
	queue = sorted_queue_new(cmp);
	for (i=0;i<256;i++){
		if (frequency_array->freq[i] == 0)
			continue;
		node = malloc(sizeof(tree_node_t));
		node->left = NULL;
		node->right = NULL;
		node->is_leaf = 1;
		node->data = i;
		node->weight = frequency_array->freq[i];
		sorted_queue_append(queue, node);
	}
	rebuild_tree(queue);
	codes_array_t* codes = generate_codes_map(queue);
	while(sorted_queue_count(queue)){
		node  = sorted_queue_pop(queue);
		printf("%"PRIu64"\n" , node->weight);
		FREE(node);
	}
	
	for(i=0;i<256;i++)
	{
		
		if (codes->code[i])
			printf("%i - %s\n",i, codes->code[i]);
	}
	sorted_queue_free(queue);
}