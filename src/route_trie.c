#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include "route_trie.h"

rtrie_node_t *rtrie_node_create()
{
    rtrie_node_t *rtn = (rtrie_node_t *)malloc(sizeof(rtrie_node_t));
    if (rtn == NULL)
        return rtn;
    bzero(rtn, sizeof(rtrie_node_t));
    return rtn;
}

int rtrie_init(rtrie_t *rt)
{
    rtrie_node_t *root = rtrie_node_create();
    if (root == NULL)
        return -1;
    rt->root = root;
    return 0;
}

pcre2_code *parse_uri_regex(const struct mg_str *uri)
{
    int urindex, regindex;
    PCRE2_SIZE erroffset;
    int errorcode;
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
    pcre2_code *ret = pcre2_compile((PCRE2_SPTR)regex, PCRE2_ZERO_TERMINATED, 0, &errorcode, &erroffset, NULL);
    if (ret == NULL)
    {
        PCRE2_UCHAR8 errmsg[120];
        pcre2_get_error_message(errorcode, errmsg, 120);
        printf("pcre2_compile failed (offset: %zu), %s\n", erroffset, errmsg);
        exit(-1);
    }
    free(regex);
    return ret;
}

/* "/api/news/:([0-9]+)/comment/:([0-9]*)" will be reduce into "/api/news/:/comment/:"*/
int reduce_uri(const struct mg_str *uri, char *result)
{
    uint32_t urindex, rindex;
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
        if (rtrie_init(rt) < 0)
            return -1;
    }
    rtrie_node_t *current = rt->root;
    char *ruri = (char *)malloc(uri->len + 1);
    reduce_uri(uri, ruri);
    int len = strlen(ruri);
    int i = 0;
    /* searching last node for last character in uri->p */
    while (i < len)
    {
        int ch_index = FIND_INDEX(ruri[i]);

        if (ch_index < 0 || ch_index > 93)
            return -1;

        if (current->p[ch_index] == NULL)
        {
            current->p[ch_index] = rtrie_node_create();
            if (current->p[ch_index] == NULL)
                return -1;
        }
        i++;
        current = current->p[ch_index];
    }

    current->uri = mg_mk_str_n(ruri, len);
    current->event_handler = ev_handler;
    current->regex = parse_uri_regex(uri);
    return 0;
}

int matching_route_regex(pcre2_code *re, const struct mg_str *str, size_t *matchstr_range, size_t matchstr_range_size)
{
    size_t *ovector;
    pcre2_match_data *match_data = pcre2_match_data_create_from_pattern(re, NULL);
    int rc = pcre2_match(re, (PCRE2_SPTR)str->p, str->len, 0, 0, match_data, NULL);
    if (rc <= 0)
    {
        pcre2_match_data_free(match_data);
        return -1;
    }
    else
    {   
        ovector = pcre2_get_ovector_pointer(match_data);
        memcpy(matchstr_range, ovector + 2, matchstr_range_size);
        pcre2_match_data_free(match_data);
        return 0;
    }
}

int matching_route(rtrie_t *rt, const struct mg_str *uri, mg_event_handler_t *event_handler,
                   pcre2_code **re)
{
    rtrie_node_t *current_node = rt->root;
    int urindex = 0;
    int colon_index = FIND_INDEX(':');
    while (urindex < uri->len && current_node != NULL)
    {
        /* if fonud ':' in current node, it means we reach a match pattern, jump it*/
        if (current_node->p[colon_index] != NULL)
        {
            while (urindex < uri->len && uri->p[urindex] != '/')
                urindex++;
            if (urindex == uri->len)
            {
                current_node = current_node->p[colon_index];
                break;
            }
        }

        char current_char = uri->p[urindex++];
        int ch_index = FIND_INDEX(current_char);

        if (ch_index < 0 || ch_index > 93)
            return -1;

        current_node = current_node->p[ch_index];
    }
    if (current_node == NULL)
        return -1;

    if (event_handler != NULL)
        *event_handler = current_node->event_handler;
    if (re != NULL)
        *re = current_node->regex;
    return 0;
}