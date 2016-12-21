/**
 * @file parser.h
 * @author Radek Krejci <rkrejci@cesnet.cz>
 * @brief Parsers for libyang
 *
 * Copyright (c) 2015 CESNET, z.s.p.o.
 *
 * This source code is licensed under BSD 3-Clause License (the "License").
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://opensource.org/licenses/BSD-3-Clause
 */

#ifndef LY_PARSER_H_
#define LY_PARSER_H_

#include <pcre.h>

#include "libyang.h"
#include "tree_schema.h"
#include "tree_internal.h"

/**
 * @defgroup yin YIN format support
 * @{
 */
struct lys_module *yin_read_module(struct ly_ctx *ctx, const char *data, const char *revision, int implement);
struct lys_submodule *yin_read_submodule(struct lys_module *module, const char *data,struct unres_schema *unres);

/**@} yin */

/**
 * @defgroup xmldata XML data format support
 * @{
 */
struct lyd_node *xml_read_data(struct ly_ctx *ctx, const char *data, int options);

/**@} xmldata */

/**
 * @defgroup jsondata JSON data format support
 * @{
 */
struct lyd_node *lyd_parse_json(struct ly_ctx *ctx, const char *data, int options, const struct lyd_node *rpc_act,
                                const struct lyd_node *data_tree);

/**@} jsondata */

enum LY_IDENT {
    LY_IDENT_SIMPLE,   /* only syntax rules */
    LY_IDENT_FEATURE,
    LY_IDENT_IDENTITY,
    LY_IDENT_TYPE,
    LY_IDENT_NODE,
    LY_IDENT_NAME,     /* uniqueness across the siblings */
    LY_IDENT_PREFIX
};

struct lys_module *lyp_search_file(struct ly_ctx *ctx, struct lys_module *module, const char *name,
                                   const char *revision, int implement, struct unres_schema *unres);

struct lys_type *lyp_get_next_union_type(struct lys_type *type, struct lys_type *prev_type, int *found);

struct lys_type *lyp_parse_value(struct lys_type *type, const char **value_, struct lyxml_elem *xml,
                                         struct lyd_node *tree, struct lyd_node_leaf_list *leaf,
                                         int store, int resolvable, int dflt);

int lyp_check_length_range(const char *expr, struct lys_type *type);

int lyp_check_pattern(const char *pattern, pcre **pcre_precomp);

int fill_yin_type(struct lys_module *module, struct lys_node *parent, struct lyxml_elem *yin, struct lys_type *type,
                  int tpdftype, struct unres_schema *unres);

int lyp_check_status(uint16_t flags1, struct lys_module *mod1, const char *name1,
                     uint16_t flags2, struct lys_module *mod2, const char *name2,
                     const struct lys_node *node);

int dup_typedef_check(const char *type, struct lys_tpdf *tpdf, int size);

int dup_identities_check(const char *id, struct lys_module *module);

/**
 * @brief Get know if the node is part of the RPC/action's input/output
 *
 * @param node Schema node to be examined.
 * @return 1 for true, 0 for false
 */
int lyp_is_rpc_action(struct lys_node *node);

/**
 * @brief Check validity of parser options.
 *
 * @param options Parser options to be checked.
 * @return 0 for ok, 1 when multiple data types bits are set, or incompatible options are used together.
 */
int lyp_check_options(int options);

int lyp_check_identifier(const char *id, enum LY_IDENT type, struct lys_module *module, struct lys_node *parent);
int lyp_check_date(const char *date);
int lyp_check_mandatory_augment(struct lys_node_augment *node, const struct lys_node *target);
int lyp_check_mandatory_choice(struct lys_node *node);

int lyp_check_include(struct lys_module *module, struct lys_submodule *submodule, const char *value,
                      struct lys_include *inc, struct unres_schema *unres);

int lyp_check_import(struct lys_module *module, const char *value, struct lys_import *imp);

/**
 * @brief Propagate imports and includes into the main module
 *
 * @param module Main module
 * @param inc Filled include structure
 * @param line
 * @return 0 for success, 1 for failure
 */
int lyp_propagate_submodule(struct lys_module *module, struct lys_include *inc);

int lyp_ctx_add_module(struct lys_module **module);

/**
 * Store UTF-8 character specified as 4byte integer into the dst buffer.
 * Returns number of written bytes (4 max), expects that dst has enough space.
 *
 * UTF-8 mapping:
 * 00000000 -- 0000007F:    0xxxxxxx
 * 00000080 -- 000007FF:    110xxxxx 10xxxxxx
 * 00000800 -- 0000FFFF:    1110xxxx 10xxxxxx 10xxxxxx
 * 00010000 -- 001FFFFF:    11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
 *
 */
unsigned int pututf8(char *dst, int32_t value);
unsigned int copyutf8(char *dst, const char *src);

#endif /* LY_PARSER_H_ */
