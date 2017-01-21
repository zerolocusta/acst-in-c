#include <regex.h>
#include "mongoose.h"

typedef struct _rtrie_node{
    const char *uri;
    (sturct _rtrie_node *) next[94];
    regex_t regex;
    mg_event_handler_t event_handler;
} rtire_node_t;


typedef struct _rtrie{
    struct rtrie_node *root;
} rtrie_t;

int trie_create(rtrie_t *rt)
{
    rtrie_node_t root = (rtrie_node_t *) malloc(sizeof(rtrie_node_t));
    if (root = NULL)
        return -1;
    rt->root = root;
    return 0;
}

void add_route()