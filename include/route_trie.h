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

int trie_init(rtrie_t *);
int add_route(rtrie_t *, const char *);
