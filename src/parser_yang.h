/**
 * @file parser_yang.h
 * @author Pavol Vican
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

#ifndef LY_PARSER_YANG_H_
#define LY_PARSER_YANG_H_

#include <stdlib.h>
#include <string.h>

#include "tree_schema.h"
#include "resolve.h"
#include "common.h"
#include "context.h"

#define LYS_SYSTEMORDERED 0x40
#define LYS_ORDERED_MASK 0xC0
#define LYS_MIN_ELEMENTS 0x01
#define LYS_MAX_ELEMENTS 0x02
#define LYS_RPC_INPUT 0x01
#define LYS_RPC_OUTPUT 0x02
#define LYS_DATADEF 0x04
#define LYS_TYPE_DEF 0x08
#define CONFIG_INHERIT_DISABLE 0x00
#define CONFIG_INHERIT_ENABLE  0x01
#define CONFIG_IGNORE 0x02
#define CONFIG_MASK 0x03
#define LYS_CHOICE_DEFAULT 0x10
#define LYS_NO_ERASE_IDENTITY 0x20
#define LY_YANG_ARRAY_SIZE 8

struct type_node {
    union {
        struct lys_node_leaflist *ptr_leaflist;
        struct lys_node_list *ptr_list;
        struct lys_node_leaf *ptr_leaf;
        struct lys_tpdf *ptr_tpdf;
        struct lys_node_anydata *ptr_anydata;
        struct lys_node_rpc_action *ptr_rpc;
        struct lys_node_choice *ptr_choice;
    };
    uint flag;
};

struct yang_parameter {
    struct lys_module *module;
    struct lys_submodule *submodule;
    struct unres_schema *unres;
    struct lys_node **node;
    char **value;
    void **data_node;
    void **actual_node;
    uint8_t remove_import;
    uint8_t exist_module;
};

struct yang_type {
    char flags;       /**< this is used to distinguish lyxml_elem * from a YANG temporary parsing structure */
    LY_DATA_TYPE base;
    const char *name;
    struct lys_type *type;
};

#include "parser_yang_bis.h"

char *yang_read_string(const char *input, char *output, int size, int offset, int indent);

int yang_read_common(struct lys_module *module,char *value, enum yytokentype type);

int yang_read_prefix(struct lys_module *module, struct lys_import *imp, char *value);

int yang_check_version(struct lys_module *module, struct lys_submodule *submodule, char *value, int repeat);

int yang_check_imports(struct lys_module *module, struct unres_schema *unres);

int yang_read_description(struct lys_module *module, void *node, char *value, char *where, enum yytokentype type);

int yang_read_reference(struct lys_module *module, void *node, char *value, char *where, enum yytokentype type);

void yang_read_revision(struct lys_module *module, char *value, struct lys_revision *retval);

int yang_read_message(struct lys_module *module,struct lys_restr *save,char *value, char *what, int message);

int yang_read_presence(struct lys_module *module, struct lys_node_container *cont, char *value);

int yang_read_config(void *node, int value, enum yytokentype type);

int store_flags(struct lys_node *node, uint8_t flags, int config_opt);

void *yang_read_when(struct lys_module *module, struct lys_node *node, enum yytokentype type, char *value);

/**
 * @brief Allocate memory for node and add to the tree
 *
 * @param[in/out] node Pointer to the array.
 * @param[in] parent Pointer to the parent.
 * @param[in] root Pointer to the root of schema tree.
 * @param[in] value Name of node
 * @param[in] nodetype Type of node
 * @param[in] sizeof_struct Size of struct
 * @return Pointer to the node, NULL on error.
*/
void *yang_read_node(struct lys_module *module, struct lys_node *parent, struct lys_node **root,
                     char *value, int nodetype, int sizeof_struct);

int yang_read_default(struct lys_module *module, void *node, char *value, enum yytokentype type);

int yang_read_units(struct lys_module *module, void *node, char *value, enum yytokentype type);

int yang_read_key(struct lys_module *module, struct lys_node_list *list, struct unres_schema *unres);

int yang_read_unique(struct lys_module *module, struct lys_node_list *list, struct unres_schema *unres);

void *yang_read_type(struct lys_module *module, void *parent, char *value, enum yytokentype type);

void *yang_read_length(struct lys_module *module, struct yang_type *typ, char *value);

int yang_check_type(struct lys_module *module, struct lys_node *parent, struct yang_type *typ, struct lys_type *type, int tpdftype, struct unres_schema *unres);

void yang_free_type_union(struct ly_ctx *ctx, struct lys_type *type);

int yang_read_leafref_path(struct lys_module *module, struct yang_type *stype, char *value);

int yang_read_require_instance(struct yang_type *stype, int req);

int yang_read_pattern(struct lys_module *module, struct lys_restr *pattern, char *value, char modifier);

void *yang_read_range(struct  lys_module *module, struct yang_type *typ, char *value);

int yang_read_fraction(struct yang_type *typ, uint32_t value);

int yang_read_enum(struct lys_module *module, struct yang_type *typ, struct lys_type_enum *enm, char *value);

int yang_check_enum(struct yang_type *typ, struct lys_type_enum *enm, int64_t *value, int assign);

int yang_read_bit(struct lys_module *module, struct yang_type *typ, struct lys_type_bit *bit, char *value);

int yang_check_bit(struct yang_type *typ, struct lys_type_bit *bit, int64_t *value, int assign);

void *yang_read_typedef(struct lys_module *module, struct lys_node *parent, char *value);

int yang_read_augment(struct lys_module *module, struct lys_node *parent, struct lys_node_augment *aug, char *value);

void *yang_read_deviate(struct lys_deviation *dev, LYS_DEVIATE_TYPE mod);

int yang_read_deviate_unsupported(struct lys_deviation *dev);

int yang_fill_unique(struct lys_module *module, struct lys_node_list *list, struct lys_unique *unique, char *value, struct unres_schema *unres);

int yang_use_extension(struct lys_module *module, struct lys_node *data_node, void *actual, char *value);

int yang_check_flags(uint16_t *flags, uint16_t mask, char *what, char *where, uint16_t value, int shortint);

/* **
 * @brief Parse YANG from in-memory string
 *
 * yang parser expected at the end of the input string 2 zero byte
 *
 * @param[in] module Pointer to the libyang module.
 * @param[in] submodule Pointer to the libyang submodule.
 * @param[in] unres Pointer to a unres_schema
 * @param[in] data Pointer to a NULL-terminated string containing YANG data to parse.
 * @param[in] size_data Size of input string
 * @param[in/out] node Pointer to node
 * @return 0 on success, -1 on error, 1 on module is already in context.
 */
int yang_parse_mem(struct lys_module *module, struct lys_submodule *submodule, struct unres_schema *unres,
                   const char *data, unsigned int size_data, struct lys_node **node);

struct lys_module *yang_read_module(struct ly_ctx *ctx, const char* data, unsigned int size, const char *revision, int implement);

struct lys_submodule *yang_read_submodule(struct lys_module *module, const char *data, unsigned int size, struct unres_schema *unres);

#endif /* LY_PARSER_YANG_H_ */
