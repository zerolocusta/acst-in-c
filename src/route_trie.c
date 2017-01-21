#include <strings.h>
#include "route_trie.h"

int trie_init(rtrie_t *rt)
{
    rtrie_node_t *root = (rtrie_node_t *) malloc(sizeof(rtrie_node_t));
    bzero(root, sizeof(rtire_node_t));
    if (root = NULL)
        return -1;
    rt->root = root;
    return 0;
}



int add_route(rtrie_t *rt, const char *uri)
{
    if (rt->root == NULL)
    {
        if (trie_init(rt) < 0)
            return -1;
    }
}
