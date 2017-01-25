#ifndef ACST_CONFIG_H_
#define ACST_CONFIG_H_

#include "mongoose.h"
int s_sig_num = 0;
void *s_db_handle = NULL;
struct mg_serve_http_opts s_http_server_opts;
const char *s_http_port = "8000";
const char *s_db_path = "acst.db";
const struct mg_str s_get_method = MG_MK_STR("GET");
const struct mg_str s_post_method = MG_MK_STR("POST");
const struct mg_str s_put_method = MG_MK_STR("PUT");
const struct mg_str s_delele_method = MG_MK_STR("DELETE");

#endif /* ACST_CONFIG_H_ */