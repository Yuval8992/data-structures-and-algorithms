#include <stddef.h> /*size_t*/
#include <stdint.h> /*uint32_t*/

typedef struct trie trie_t; 

trie_t *TrieCreate(size_t height);

/*Occupy given path, returned value: 1 - success, 0 - failure*/
int TrieOccupyPath(trie_t *trie, size_t bit_path);

/*Find and occupy the smallest free path, returned value: 1 - success, 0 - failure*/
int TrieOccupyNextFreePath(trie_t *trie, uint32_t *path);

/*Release given path*/
void TrieReleasePath(trie_t *trie, uint32_t path);

void TrieDestroy(trie_t *trie);

