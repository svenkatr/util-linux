/*
 * symbols.c - routines for symbol handling
 *
 * Copyright (C) 2014 Ondrej Oprala <ooprala@redhat.com>
 * Copyright (C) 2016 Igor Gnatenko <i.gnatenko.brain@gmail.com>
 *
 * This file may be redistributed under the terms of the
 * GNU Lesser General Public License.
 */

/**
 * SECTION: symbols
 * @title: Symbols
 * @short_description: allows to overwrite default output chars (for ascii art)
 *
 * An API to access and modify data and information per symbol/symbol group.
 */


#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "smartcolsP.h"

/**
 * scols_new_symbols:
 *
 * Returns: a pointer to a newly allocated struct libscols_symbols instance.
 */
struct libscols_symbols *scols_new_symbols(void)
{
	struct libscols_symbols *sy = calloc(1, sizeof(struct libscols_symbols));

	if (!sy)
		return NULL;
	sy->refcount = 1;
	return sy;
}

/**
 * scols_ref_symbols:
 * @sy: a pointer to a struct libscols_symbols instance
 *
 * Increases the refcount of @sy.
 */
void scols_ref_symbols(struct libscols_symbols *sy)
{
	if (sy)
		sy->refcount++;
}

/**
 * scols_unref_symbols:
 * @sy: a pointer to a struct libscols_symbols instance
 *
 * Decreases the refcount of @sy.
 */
void scols_unref_symbols(struct libscols_symbols *sy)
{
	if (sy && --sy->refcount <= 0) {
		free(sy->branch);
		free(sy->vert);
		free(sy->right);
		free(sy->title_padding);
		free(sy);
	}
}

/**
 * scols_symbols_set_branch:
 * @sb: a pointer to a struct libscols_symbols instance
 * @str: a string which will represent the branch part of a tree output
 *
 * Returns: 0, a negative value in case of an error.
 */
int scols_symbols_set_branch(struct libscols_symbols *sb, const char *str)
{
	return strdup_to_struct_member(sb, branch, str);
}

/**
 * scols_symbols_set_vertical:
 * @sb: a pointer to a struct libscols_symbols instance
 * @str: a string which will represent the vertical part of a tree output
 *
 * Returns: 0, a negative value in case of an error.
 */
int scols_symbols_set_vertical(struct libscols_symbols *sb, const char *str)
{
	return strdup_to_struct_member(sb, vert, str);
}

/**
 * scols_symbols_set_right:
 * @sb: a pointer to a struct libscols_symbols instance
 * @str: a string which will represent the right part of a tree output
 *
 * Returns: 0, a negative value in case of an error.
 */
int scols_symbols_set_right(struct libscols_symbols *sb, const char *str)
{
	return strdup_to_struct_member(sb, right, str);
}

/**
 * scols_symbols_set_title_padding:
 * @sb: a pointer to a struct libscols_symbols instance
 * @str: a string which will represent the symbols which fill title output
 *
 * The current implementation uses only the first byte from the padding string.
 * A multibyte chars are not supported yet.
 *
 * Returns: 0, a negative value in case of an error.
 *
 * Since: 2.28
 */
int scols_symbols_set_title_padding(struct libscols_symbols *sb, const char *str)
{
	return strdup_to_struct_member(sb, title_padding, str);
}

/**
 * scols_symbols_set_cell_padding:
 * @sb: a pointer to a struct libscols_symbols instance
 * @str: a string which will represent the symbols which fill cells
 *
 * The padding char has to take up just one cell on the terminal.
 *
 * Returns: 0, a negative value in case of an error.
 *
 * Since: 2.29
 */
int scols_symbols_set_cell_padding(struct libscols_symbols *sb, const char *str)
{
	return strdup_to_struct_member(sb, cell_padding, str);
}

/**
 * scols_copy_symbols:
 * @sb: a pointer to a struct libscols_symbols instance
 *
 * Returns: a newly allocated copy of the @sb symbol group or NULL in case of an error.
 */
struct libscols_symbols *scols_copy_symbols(const struct libscols_symbols *sb)
{
	struct libscols_symbols *ret;
	int rc;

	assert(sb);
	if (!sb)
		return NULL;

	ret = scols_new_symbols();
	if (!ret)
		return NULL;

	rc = scols_symbols_set_branch(ret, sb->branch);
	if (!rc)
		rc = scols_symbols_set_vertical(ret, sb->vert);
	if (!rc)
		rc = scols_symbols_set_right(ret, sb->right);
	if (!rc)
		rc = scols_symbols_set_title_padding(ret, sb->title_padding);
	if (!rc)
		rc = scols_symbols_set_cell_padding(ret, sb->cell_padding);
	if (!rc)
		return ret;

	scols_unref_symbols(ret);
	return NULL;

}
