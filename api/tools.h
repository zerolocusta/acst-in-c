#ifndef ACST_TOOLS_H_
#define ACST_TOOLS_H_

#include "mongoose.h"

int has_prefix(const struct mg_str *, const struct mg_str *);

int is_equal(const struct mg_str *, const struct mg_str *);

#endif /* ACST_TOOLS_H_ */