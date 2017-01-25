#ifndef ACST_ROUTE_TRIE_H
#define ACST_ROUTE_TRIE_H

#include <pcre.h>
#include "mongoose.h"

#define FIND_INDEX(ch) ((ch) - '!')

typedef struct _rtrie_node
{
    struct mg_str uri;
    /* start from '!' to '~'*/
    struct _rtrie_node *p[94];
    /* using ':' to indicate start of regex pattern (just like compojure)*/
    pcre *regex;
    mg_event_handler_t event_handler;
} rtrie_node_t;

typedef struct _rtrie
{
    rtrie_node_t *root;
} rtrie_t;

rtrie_node_t *rtrie_node_create();
int rtrie_init(rtrie_t *);
pcre *parse_uri_regex(const struct mg_str *);
int reduce_uri(const struct mg_str *, char *);
int add_route(rtrie_t *, const struct mg_str *, mg_event_handler_t);
int matching_route_regex(pcre *, const struct mg_str *, int *, int);
int matching_route(rtrie_t *, const struct mg_str *, mg_event_handler_t *, pcre **);

#endif