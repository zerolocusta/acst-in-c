#include <string.h>
#include <stdlib.h>
#include "route_trie.h"

trie_node_t *trie_node_create(){
    rtrie_node_t *rtn = (rtrie_node_t *) malloc(sizeof(rtrie_node_t));
    if(rtn == NULL)
        return rtn;
    bzero(rtn, sizeof(rtire_node_t));
    for(int i = 0; i < 94; i++)
        rtn->p[i] = NULL;
    return rtn;
}

int trie_init(rtrie_t *rt)
{
    rtrie_node_t *root = trie_node_create();
    if (root = NULL)
        return -1;
    rt->root = root;
    return 0;
}

pcre *parse_uri_regex(const struct mg_str *uri)
{
    int urindex, regindex;
    int erroffset;
    const char *error;
    char *regex = (char *) malloc(uri->len);
    for(urindex = 0, regindex = 0; urindex < uri->len; urindex++)
    {
        /* when find ':', jump it*/
        if (uri->p[urindex] == ':')
            continue;
        regex[regindex++] = uri->p[urindex];
    }
    pcre *ret = pcre_compile(regex, 0, &error, &erroffset, 0);
    if (ret == NULL)
    {
        printf("pcre_compile failed (offset: %d), %s\n", erroffset, error);
        exit(-1);
    }
    free(regex);
    return ret;
}

int add_route(rtrie_t *rt, const struct mg_str *uri, mg_event_handler_t ev_handler)
{
    if (rt->root == NULL)
    {
        if (trie_init(rt) < 0)
            return -1;
    }
    rtrie_node_t *next = rt->root;
    int i = 0;
    /* searching last node for last character in uri->p */
    while(i < uri->len)
    {
        int ch_index = FIND_INDEX(uri->p[i]);
        if (next->p[ch_index] == NULL)
        {
            next->p[ch_index] = trie_node_create();
            if(next->p[ch_index] == NULL)
                return -1;
        }
        i++;
        next = next->p[ch_index];
    }
    next->uri = uri;
    next->event_handler = ev_handler;
    next->regex = parse_uri_regex(uri);
    return 0;
}

