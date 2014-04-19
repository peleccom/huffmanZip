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
	if (node->is_leaf){
		char *str = malloc((strlen(path)+ 1) * sizeof(char));
		strcpy(str, path);
		if (strlen(str) == 0)
			{
				FREE(str);
				str = malloc(2 * sizeof(char));
				strcpy(str, "0");
			}
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
	FREE(node);
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

void free_codes(codes_array_t* codes){
	int i;
	for(i=0;i<256;i++)
		//free strings
		FREE(codes->code[i])
}
void free_tree_node(tree_node_t* node){
	if (!(node->is_leaf))
	{
		if (node->left)
			free_tree_node(node->left);
		if (node->right)
			free_tree_node(node->right);
	}
	FREE(node);
}

void free_tree(tree_node_t* root){
	free_tree_node(root);
}

codes_array_t* generate_codes(frequency_array_t *frequency_array){
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
		FREE(node);
	}
	sorted_queue_free(queue);
	return codes;
}


tree_node_t* generate_tree(codes_array_t* codes){
    int i;
    int j;
    int l;
    tree_node_t *root;
    tree_node_t *cur_node;
    tree_node_t *new_node;
    char *s;
    root = malloc(sizeof(tree_node_t));
    root->is_leaf = 1;
    root->left = NULL;
    root->right = NULL;
    root->data = 0;
    cur_node = root;
    for(i=0; i<256; i++){
		if (!codes->code[i]) continue;
		cur_node = root;
		s = codes->code[i];
		l = strlen(s);
		for(j=0; j< l; j++){
			if (cur_node->is_leaf){
				cur_node->is_leaf = 0;
				cur_node->left = NULL;
				cur_node->right = NULL;
				new_node = malloc(sizeof(tree_node_t));
				new_node->is_leaf = 1;	   
				if (s[j] == '1')
					cur_node->right = new_node;
				else
					cur_node->left = new_node;
				cur_node = new_node;
			}
			else
			{
				if (s[j] == '1')
				{
					if(cur_node->right)
						cur_node = cur_node->right;
					else{
						new_node = malloc(sizeof(tree_node_t));
						new_node->is_leaf = 1;
						cur_node->right = new_node;
						cur_node = new_node;
					}
				}
				else{
					if (cur_node->left)
						cur_node = cur_node->left;
					else{
						new_node = malloc(sizeof(tree_node_t));
						new_node->is_leaf = 1;
						cur_node->left = new_node;
						cur_node = new_node;
					}
				}
			}
	 
		}    
		cur_node->is_leaf = 1;
		cur_node->data = i; 
    }
    return root;
	
}
