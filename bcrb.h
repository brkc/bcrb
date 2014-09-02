#ifndef BCBST_H
#define BCBST_H 1

typedef int(*bcrb_cmpfunc)(const void *left, const void *right);
typedef void(*bcrb_freefunc)(void *);
typedef void(*bcrb_applyfunc)(void *key, void *val);

typedef struct bcrb bcrb;
bcrb * bcrb_new(bcrb_cmpfunc cmp);
void   bcrb_del(bcrb *tree, bcrb_freefunc keyfree, bcrb_freefunc valfree);
int    bcrb_strcmp(const void *left, const void *right);
void   bcrb_put(bcrb *tree, const void *key, const void *val);
bcrb * bcrb_get(bcrb *tree, const void *key);

void bcrb_preorder (bcrb *tree, bcrb_applyfunc apply);
void bcrb_inorder  (bcrb *tree, bcrb_applyfunc apply);
void bcrb_postorder(bcrb *tree, bcrb_applyfunc apply);

#endif /* ifndef BCBST_H */
