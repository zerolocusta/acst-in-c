#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include "route_trie.h"

trie_node_t *trie_node_create()
{
    rtrie_node_t *rtn = (rtrie_node_t *)malloc(sizeof(rtrie_node_t));
    if (rtn == NULL)
        return rtn;
    bzero(rtn, sizeof(rtire_node_t));
    for (int i = 0; i < 94; i++)
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
    char *regex = (char *)malloc(uri->len + 1);
    if (regex == NULL)
    {
        printf("malloc fail, no more memory for regex");
        exit(-1);
    }
    for (urindex = 0, regindex = 0; urindex < uri->len; urindex++)
    {
        /* when find ':', jump it*/
        if (uri->p[urindex] == ':')
            continue;
        regex[regindex++] = uri->p[urindex];
    }
    regex[regindex] = '\0';
    pcre *ret = pcre_compile(regex, 0, &error, &erroffset, 0);
    if (ret == NULL)
    {
        printf("pcre_compile failed (offset: %d), %s\n", erroffset, error);
        exit(-1);
    }
    free(regex);
    return ret;
}

/* "/api/news/:([0-9]+)/comment/:([0-9]*)" will be reduce into "/api/news/:/comment/:"*/
int reduce_uri(const struct mg_str *uri, char *result)
{
    int urindex, rindex;
    for (urindex = 0, rindex = 0; urindex < uri->len;)
    {
        result[rindex++] = uri->p[urindex];
        if (uri->p[urindex] == ':')
        {
            while (urindex < uri->len && uri->p[urindex] != '/')
                urindex++;
            continue;
        }
        urindex++;
    }
    result[rindex] = '\0';
    return 0;
}

int add_route(rtrie_t *rt, const struct mg_str *uri, mg_event_handler_t ev_handler)
{
    if (rt->root == NULL)
    {
        if (trie_init(rt) < 0)
            return -1;
    }
    rtrie_node_t *current = rt->root;
    int i = 0;
    /* searching last node for last character in uri->p */
    while (i < uri->len)
    {
        int ch_index = FIND_INDEX(uri->p[i]);

        if (ch_index < '!' || ch_index > '~')
            return -1;

        if (current->p[ch_index] == NULL)
        {
            current->p[ch_index] = trie_node_create();
            if (current->p[ch_index] == NULL)
                return -1;
        }
        i++;
        current = current->p[ch_index];
    }
    char *ruri = (char *)malloc(uri->len + 1);
    reduce_uri(uri, ruri);
    current->uri = mg_mk_str(ruri);
    current->event_handler = ev_handler;
    current->regex = parse_uri_regex(uri);
    return 0;
}

int matching_route_regex(pcre *re, const struct mg_str *str, int *matchstr_range, int matchstr_range_size)
{
    int *ovector = (int *)malloc(matchstr_range_size + 2);
    int rc = pcre_exec(regex, 0, str, len, 0, 0, ovector, matchstr_range_size + 2);
}

int matching_route(rtrie_t *rt, const struct mg_str *uri, mg_event_handler_t event_handler,
                   int *matchstr_range, int matchstr_range_size)
{
    rtrie_node_t *current_node = rt->root;
    int urindex = 0;
    while (urindex < uri->len && current_node != NULL && current_node->next != NULL)
    {
        /* if fonud ':' in current node, it means we reach a match pattern, jump it*/
        if (current_node->p[':'] != NULL)
        {
            while (urindex < uri->len && uri->p[urindex] != '/')
                urindex++;
            if (urindex == uri->len)
                break;
        }

        char current_char = uri->p[urindex++];
        int ch_index = FIND_INDEX(current_char);

        if (ch_index < '!' || ch_index > '~')
            return -1;

        current_node = current_node->p[ch_index];
    }
    if (current_node == NULL)
        return -1;

    event_handler = current_node->event_handler;
}