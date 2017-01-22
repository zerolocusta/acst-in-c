#ifndef ACST_ROUTE_TRIE_H
#define ACST_ROUTE_TRIE_H
#include <pcre.h>
#include "mongoose.h"

#define FIND_INDEX (ch) ((ch) - '!') 

typedef struct _rtrie_node{
    const char *uri;
    /* start from '!' to '~'*/
    sturct _rtrie_node *p[94];
    /* using ':' to indicate start of regex pattern (just like compojure)*/
    pcre *regex;
    mg_event_handler_t event_handler;
} rtire_node_t;


typedef struct _rtrie{
    struct rtrie_node *root;
} rtrie_t;

int trie_init(rtrie_t *);
int add_route(rtrie_t *, const char *);
pcre *parse_uri_regex(const struct mg_str);

#endif