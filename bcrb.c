#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "bcrb.h"

struct bcrb_node {
    struct bcrb_node *parent, *left, *right;
    void *key, *val;
    enum color {red, black} color;
};

struct bcrb {
    struct bcrb_node *root;
    bcrb_cmpfunc cmp;
};

static void
node_del(struct bcrb_node *node, bcrb_freefunc keyfree, bcrb_freefunc valfree);

static struct bcrb_node *
bcrb_node_get(struct bcrb_node *node, bcrb_cmpfunc cmp, const void *key);

static void
bcrb_node_preorder(struct bcrb_node *node, bcrb_applyfunc apply);

static void
bcrb_node_inorder(struct bcrb_node *node, bcrb_applyfunc apply);

static void
bcrb_node_postorder(struct bcrb_node *node, bcrb_applyfunc apply);

static void
bcrb_fix(struct bcrb *tree, struct bcrb_node *node);

static void
bcrb_rotate_up(struct bcrb *tree, struct bcrb_node *node);

struct bcrb *
bcrb_new(bcrb_cmpfunc cmp)
{
    struct bcrb *tree;
    assert(cmp);

    tree = malloc(sizeof(*tree));
    assert(tree);
    tree->root = NULL;
    tree->cmp = cmp;
    return tree;
}

static void
node_del(struct bcrb_node *node, bcrb_freefunc keyfree, bcrb_freefunc valfree)
{
    if (!node)
        return;
    node_del(node->left, keyfree, valfree);
    node_del(node->right, keyfree, valfree);
    if (keyfree)
        keyfree(node->key);
    if (valfree)
        valfree(node->val);
    free(node);
}

void
bcrb_del(struct bcrb *tree, bcrb_freefunc keyfree, bcrb_freefunc valfree)
{
    assert(tree);
    node_del(tree->root, keyfree, valfree);
    free(tree);
}

void
bcrb_put(struct bcrb *tree, const void *key, const void *val)
{
    struct bcrb_node *parent, *child, *newnode;
    bcrb_cmpfunc cmp;
    assert(tree && key && val);

    child = tree->root;
    cmp = tree->cmp;
    assert(cmp);

    while (child) {
        parent = child;
        if (cmp(key, parent->key) < 0)
            child = parent->left;
        else
            child = parent->right;
    }

    newnode = malloc(sizeof(*newnode));
    newnode->key = (void *) key;
    newnode->val = (void *) val;
    newnode->left = NULL;
    newnode->right = NULL;
    newnode->parent = parent;
    newnode->color = red;

    if (!tree->root) {
        tree->root = newnode;
        tree->root->parent = NULL;
        bcrb_fix(tree, tree->root);
        return;
    }

    if (cmp(key, parent->key) < 0)
        parent->left = newnode;
    else
        parent->right = newnode;

    bcrb_fix(tree, newnode);
}

static void
bcrb_fix(struct bcrb *tree, struct bcrb_node *node)
{
    struct bcrb_node *parent;
    struct bcrb_node *grandparent;

    parent = node->parent;
    if (!parent) {
        node->color = black;
        return;
    }

    if (parent->color == black)
        return;

    grandparent = parent->parent;
    if (grandparent) {
        struct bcrb_node *uncle;

        if (grandparent->left == parent)
            uncle = grandparent->right;
        else
            uncle = grandparent->left;

        if (uncle && uncle->color == red) {
            grandparent->color = red;
            parent->color = black;
            uncle->color = black;
            bcrb_fix(tree, grandparent);
            return;
        }

        if ((parent->left == node && grandparent->right == parent)
                || (parent->right == node && grandparent->left == parent)) {
            bcrb_rotate_up(tree, node);
            node = parent;
            parent = parent->parent;
        }

        parent->color = black;
        grandparent->color = red;
        bcrb_rotate_up(tree, parent);
    }

}

struct bcrb *
bcrb_get(struct bcrb *tree, const void *key)
{
    struct bcrb_node *node;
    assert(tree && key);

    if (!tree->root)
        return NULL;

    node = bcrb_node_get(tree->root, tree->cmp, key);
    return node ? node->val : NULL;
}

struct bcrb_node *
bcrb_node_get(struct bcrb_node *node, bcrb_cmpfunc cmp, const void *key)
{
    int cmpres;
    assert(node && cmp && key);
    cmpres = cmp(key, node->key);

    if (cmpres < 0 && node->left)
        return bcrb_node_get(node->left, cmp, key);
    else if (cmpres > 0 && node->right)
        return bcrb_node_get(node->right, cmp, key);
    else if (cmpres == 0)
        return node;
    return NULL;
}

void
bcrb_preorder(struct bcrb *tree, bcrb_applyfunc apply)
{
    assert(tree && apply);
    bcrb_node_preorder(tree->root, apply);
}

static void
bcrb_node_preorder(struct bcrb_node *node, bcrb_applyfunc apply)
{
    assert(apply);
    if (!node)
        return;
    apply(node->key, node->val);
    bcrb_node_preorder(node->left, apply);
    bcrb_node_preorder(node->right, apply);
}

void
bcrb_inorder(struct bcrb *tree, bcrb_applyfunc apply)
{
    assert(tree && apply);
    bcrb_node_inorder(tree->root, apply);
}

static void
bcrb_node_inorder(struct bcrb_node *node, bcrb_applyfunc apply)
{
    assert(apply);
    if (!node)
        return;
    bcrb_node_inorder(node->left, apply);
    apply(node->key, node->val);
    bcrb_node_inorder(node->right, apply);
}

void
bcrb_postorder(struct bcrb *tree, bcrb_applyfunc apply)
{
    assert(tree && apply);
    bcrb_node_postorder(tree->root, apply);
}

static void
bcrb_node_postorder(struct bcrb_node *node, bcrb_applyfunc apply)
{
    assert(apply);
    if (!node)
        return;
    bcrb_node_postorder(node->left, apply);
    bcrb_node_postorder(node->right, apply);
    apply(node->key, node->val);
}

static void
bcrb_rotate_up(struct bcrb *tree, struct bcrb_node *node)
{
    struct bcrb_node *subtree;
    struct bcrb_node *parent;
    struct bcrb_node *grandparent;
    assert(node);

    /* Initialize variables after node cannot be NULL. */
    parent = node->parent;
    if (!parent)
        return;
    grandparent = parent->parent;

    if (parent->left == node) {
        subtree = node->right;
        node->right = parent;
        parent->left = subtree;
    } else {
        subtree = node->left;
        node->left = parent;
        parent->right = subtree;
    }

    /* Make node's old parent its new right subtree. */
    parent->parent = node;

    /* Make node's old right subtree parent's new left subtree (if it exists). */
    if (subtree)
        subtree->parent = parent;

    /* Link node to grandparent (if it exists). */
    if (!grandparent) {
        node->parent = NULL;
        tree->root = node;
        return;
    }
    if (grandparent->left == parent)
        grandparent->left = node;
    else
        grandparent->right = node;
    node->parent = grandparent;
}

int
bcrb_strcmp(const void *left, const void *right)
{
    return strcmp((const char *) left, (const char *) right);
}
