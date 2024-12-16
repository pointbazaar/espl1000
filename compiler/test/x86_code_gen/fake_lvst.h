#pragma once

#include "tables/lvst/lvst.h"

#include "util/ctx.h"

void sd_uc_fake_lvst(struct Ctx* ctx, size_t fake_lvst_size, size_t stackframe_nargs);

struct Type* fake_uint64_type();
