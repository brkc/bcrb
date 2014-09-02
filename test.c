#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bcrb.h"

void test_empty(void);
void test_one(void);
void test_left(void);
void test_right(void);
void test_left_left(void);
void test_left_right(void);
void test_right_left(void);
void test_right_right(void);
void test_left_left_left(void);
void test_left_left_right(void);
void test_left_right_left(void);
void test_left_right_right(void);
void test_right_left_left(void);
void test_right_left_right(void);
void test_right_right_left(void);
void test_right_right_right(void);
void test_big_test(void);

int
main(void)
{
    test_empty();
    test_one();

    test_left();
    test_right();

    test_left_left();
    test_left_right();
    test_right_left();
    test_right_right();

    test_left_left_left();
    test_left_left_right();
    test_left_right_left();
    test_left_right_right();
    test_right_left_left();
    test_right_left_right();
    test_right_right_left();
    test_right_right_right();

    test_big_test();

    return EXIT_SUCCESS;
}

static struct map {
    char *key, *val;
} state_map[] = {
    {"AL", "Alabama"},
    {"AK", "Alaska"},
    {"NC", "North Carolina"},
    {"OH", "Ohio"},
    {"IA", "Iowa"},
    {"NY", "New York"},
    {"MA", "Massachusetts"},
    {"VT", "Vermont"},
    {"ME", "Maine"},
    {"AZ", "Arizona"},
    {"WA", "Washington"},
    {"DC", "District of Columbia"},
    {"HI", "Hawaii"},
    {"TN", "Tennessee"},
    {"FL", "Florida"},
    {"GA", "Georgia"},
    {"SC", "South Carolina"},
    {"TX", "Texas"},
    {"CA", "California"},
    {"WY", "Wyoming"},
    {"CO", "Colorado"},
    {"MT", "Montana"},
    {"MI", "Michigan"},
    {"OR", "Oregon"},
    {"IL", "Illinois"},
    {"NH", "New Hampshire"},
    {"VA", "Virgina"},
    {"WV", "West Virginia"},
    {"MD", "Maryland"},
    {"RI", "Rhode Island"},
    {"NM", "New Mexico"},
    {"OK", "Oklahoma"},
    {"ND", "North Dakota"},
    {"SD", "South Dakota"},
    {"RI", "Rhode Island"},
    {"MO", "Missouri"},
    {"NV", "Nevada"},
    {"UT", "Utah"},
    {"IN", "Indiana"},
    {"LA", "Lousiana"},
    {"ID", "Idaho"},
    {"KY", "Kentucky"},
    {"PA", "Pennsylvania"},
    {"MN", "Minnesota"},
    {"WI", "Wisconsin"},
    {"AR", "Arkansas"},
    {"CT", "Connecticut"},
    {"KS", "Kansas"},
    {"NE", "Nebraska"},
    {"NJ", "New Jersey"},
    {"DE", "Delaware"},
};

int
cmp(const void *left, const void *right)
{
    return strcmp(((struct map *) left)->key, ((struct map *) right)->key);
}

void
test_big_test(void)
{
    bcrb *tree = bcrb_new(bcrb_strcmp);
    size_t i;

    size_t nmems = sizeof(state_map) / sizeof(state_map[0]);
    qsort(state_map, nmems, sizeof(state_map[0]), cmp);

    for (i = 0; i < sizeof(state_map) / sizeof(state_map[0]); i++)
        bcrb_put(tree, state_map[i].key, state_map[i].val);

    for (i = 0; i < sizeof(state_map) / sizeof(state_map[0]); i++)
        assert(!strcmp(state_map[i].val,
                       (char *) bcrb_get(tree, state_map[i].key)));

    bcrb_del(tree, NULL, NULL);
}

void
test_empty(void)
{
    bcrb *tree = bcrb_new(bcrb_strcmp);
    void *ip = bcrb_get(tree, "aardvark");
    assert(ip == NULL);
    bcrb_del(tree, free, free); /* make sure there's nothing in it */
}

void
test_one(void)
{
    bcrb *tree = bcrb_new(bcrb_strcmp);
    bcrb_put(tree, "backwards", "sdrawkcab");
    assert(!strcmp("sdrawkcab", (char *) bcrb_get(tree, "backwards")));
    bcrb_del(tree, NULL, NULL);
}

void
test_left(void)
{
    bcrb *tree = bcrb_new(bcrb_strcmp);

    bcrb_put(tree, "banana", "yellow");
    assert(!strcmp("yellow", (char *) bcrb_get(tree, "banana")));

    bcrb_put(tree, "apple", "red");
    assert(!strcmp("red", (char *) bcrb_get(tree, "apple")));

    assert(!strcmp("red", (char *) bcrb_get(tree, "apple")));
    assert(!strcmp("yellow", (char *) bcrb_get(tree, "banana")));

    bcrb_del(tree, NULL, NULL);
}

void
test_right(void)
{
    bcrb *tree = bcrb_new(bcrb_strcmp);

    bcrb_put(tree, "apple", "red");
    assert(!strcmp("red", (char *) bcrb_get(tree, "apple")));

    bcrb_put(tree, "banana", "yellow");
    assert(!strcmp("yellow", (char *) bcrb_get(tree, "banana")));

    assert(!strcmp("red", (char *) bcrb_get(tree, "apple")));
    assert(!strcmp("yellow", (char *) bcrb_get(tree, "banana")));

    bcrb_del(tree, NULL, NULL);
}

void
test_left_left(void)
{
    bcrb *tree = bcrb_new(bcrb_strcmp);

    bcrb_put(tree, "coconut", "brown");
    assert(!strcmp("brown", (char *) bcrb_get(tree, "coconut")));

    bcrb_put(tree, "banana", "yellow");
    assert(!strcmp("yellow", (char *) bcrb_get(tree, "banana")));

    bcrb_put(tree, "apple", "red");
    assert(!strcmp("red", (char *) bcrb_get(tree, "apple")));

    assert(!strcmp("red", (char *) bcrb_get(tree, "apple")));
    assert(!strcmp("yellow", (char *) bcrb_get(tree, "banana")));
    assert(!strcmp("brown", (char *) bcrb_get(tree, "coconut")));

    bcrb_del(tree, NULL, NULL);
}

void
test_left_right(void)
{
    bcrb *tree = bcrb_new(bcrb_strcmp);

    bcrb_put(tree, "coconut", "brown");
    assert(!strcmp("brown", (char *) bcrb_get(tree, "coconut")));

    bcrb_put(tree, "apple", "red");
    assert(!strcmp("red", (char *) bcrb_get(tree, "apple")));

    bcrb_put(tree, "banana", "yellow");
    assert(!strcmp("yellow", (char *) bcrb_get(tree, "banana")));

    assert(!strcmp("red", (char *) bcrb_get(tree, "apple")));
    assert(!strcmp("yellow", (char *) bcrb_get(tree, "banana")));
    assert(!strcmp("brown", (char *) bcrb_get(tree, "coconut")));

    bcrb_del(tree, NULL, NULL);
}

void
test_right_left(void)
{
    bcrb *tree = bcrb_new(bcrb_strcmp);

    bcrb_put(tree, "apple", "red");
    assert(!strcmp("red", (char *) bcrb_get(tree, "apple")));

    bcrb_put(tree, "coconut", "brown");
    assert(!strcmp("brown", (char *) bcrb_get(tree, "coconut")));

    bcrb_put(tree, "banana", "yellow");
    assert(!strcmp("yellow", (char *) bcrb_get(tree, "banana")));

    assert(!strcmp("red", (char *) bcrb_get(tree, "apple")));
    assert(!strcmp("yellow", (char *) bcrb_get(tree, "banana")));
    assert(!strcmp("brown", (char *) bcrb_get(tree, "coconut")));

    bcrb_del(tree, NULL, NULL);
}

void
test_right_right(void)
{
    bcrb *tree = bcrb_new(bcrb_strcmp);

    bcrb_put(tree, "apple", "red");
    assert(!strcmp("red", (char *) bcrb_get(tree, "apple")));

    bcrb_put(tree, "banana", "yellow");
    assert(!strcmp("yellow", (char *) bcrb_get(tree, "banana")));

    bcrb_put(tree, "coconut", "brown");
    assert(!strcmp("brown", (char *) bcrb_get(tree, "coconut")));

    assert(!strcmp("red", (char *) bcrb_get(tree, "apple")));
    assert(!strcmp("yellow", (char *) bcrb_get(tree, "banana")));
    assert(!strcmp("brown", (char *) bcrb_get(tree, "coconut")));

    bcrb_del(tree, NULL, NULL);
}

void
test_left_left_left(void)
{
    bcrb *tree = bcrb_new(bcrb_strcmp);

    bcrb_put(tree, "dragon", "green");
    assert(!strcmp("green", (char *) bcrb_get(tree, "dragon")));

    bcrb_put(tree, "coconut", "brown");
    assert(!strcmp("brown", (char *) bcrb_get(tree, "coconut")));

    bcrb_put(tree, "banana", "yellow");
    assert(!strcmp("yellow", (char *) bcrb_get(tree, "banana")));

    bcrb_put(tree, "apple", "red");
    assert(!strcmp("red", (char *) bcrb_get(tree, "apple")));

    assert(!strcmp("red", (char *) bcrb_get(tree, "apple")));
    assert(!strcmp("yellow", (char *) bcrb_get(tree, "banana")));
    assert(!strcmp("brown", (char *) bcrb_get(tree, "coconut")));
    assert(!strcmp("green", (char *) bcrb_get(tree, "dragon")));

    bcrb_del(tree, NULL, NULL);
}

void
test_left_left_right(void)
{
    bcrb *tree = bcrb_new(bcrb_strcmp);

    bcrb_put(tree, "dragon", "green");
    assert(!strcmp("green", (char *) bcrb_get(tree, "dragon")));

    bcrb_put(tree, "coconut", "brown");
    assert(!strcmp("brown", (char *) bcrb_get(tree, "coconut")));

    bcrb_put(tree, "apple", "red");
    assert(!strcmp("red", (char *) bcrb_get(tree, "apple")));

    bcrb_put(tree, "banana", "yellow");
    assert(!strcmp("yellow", (char *) bcrb_get(tree, "banana")));

    assert(!strcmp("red", (char *) bcrb_get(tree, "apple")));
    assert(!strcmp("yellow", (char *) bcrb_get(tree, "banana")));
    assert(!strcmp("brown", (char *) bcrb_get(tree, "coconut")));
    assert(!strcmp("green", (char *) bcrb_get(tree, "dragon")));

    bcrb_del(tree, NULL, NULL);
}

void
test_left_right_left(void)
{
    bcrb *tree = bcrb_new(bcrb_strcmp);

    bcrb_put(tree, "banana", "yellow");
    assert(!strcmp("yellow", (char *) bcrb_get(tree, "banana")));

    bcrb_put(tree, "apple", "red");
    assert(!strcmp("red", (char *) bcrb_get(tree, "apple")));

    bcrb_put(tree, "dragon", "green");
    assert(!strcmp("green", (char *) bcrb_get(tree, "dragon")));

    bcrb_put(tree, "coconut", "brown");
    assert(!strcmp("brown", (char *) bcrb_get(tree, "coconut")));

    assert(!strcmp("red", (char *) bcrb_get(tree, "apple")));
    assert(!strcmp("yellow", (char *) bcrb_get(tree, "banana")));
    assert(!strcmp("brown", (char *) bcrb_get(tree, "coconut")));
    assert(!strcmp("green", (char *) bcrb_get(tree, "dragon")));

    bcrb_del(tree, NULL, NULL);
}

void
test_left_right_right(void)
{
    bcrb *tree = bcrb_new(bcrb_strcmp);

    bcrb_put(tree, "dragon", "green");
    assert(!strcmp("green", (char *) bcrb_get(tree, "dragon")));

    bcrb_put(tree, "apple", "red");
    assert(!strcmp("red", (char *) bcrb_get(tree, "apple")));

    bcrb_put(tree, "banana", "yellow");
    assert(!strcmp("yellow", (char *) bcrb_get(tree, "banana")));

    bcrb_put(tree, "coconut", "brown");
    assert(!strcmp("brown", (char *) bcrb_get(tree, "coconut")));

    assert(!strcmp("red", (char *) bcrb_get(tree, "apple")));
    assert(!strcmp("yellow", (char *) bcrb_get(tree, "banana")));
    assert(!strcmp("brown", (char *) bcrb_get(tree, "coconut")));
    assert(!strcmp("green", (char *) bcrb_get(tree, "dragon")));

    bcrb_del(tree, NULL, NULL);
}

void
test_right_left_left(void)
{
    bcrb *tree = bcrb_new(bcrb_strcmp);

    bcrb_put(tree, "coconut", "brown");
    assert(!strcmp("brown", (char *) bcrb_get(tree, "coconut")));

    bcrb_put(tree, "dragon", "green");
    assert(!strcmp("green", (char *) bcrb_get(tree, "dragon")));

    bcrb_put(tree, "banana", "yellow");
    assert(!strcmp("yellow", (char *) bcrb_get(tree, "banana")));

    bcrb_put(tree, "apple", "red");
    assert(!strcmp("red", (char *) bcrb_get(tree, "apple")));

    assert(!strcmp("red", (char *) bcrb_get(tree, "apple")));
    assert(!strcmp("yellow", (char *) bcrb_get(tree, "banana")));
    assert(!strcmp("brown", (char *) bcrb_get(tree, "coconut")));
    assert(!strcmp("green", (char *) bcrb_get(tree, "dragon")));

    bcrb_del(tree, NULL, NULL);
}

void
test_right_left_right(void)
{
    bcrb *tree = bcrb_new(bcrb_strcmp);

    bcrb_put(tree, "coconut", "brown");
    assert(!strcmp("brown", (char *) bcrb_get(tree, "coconut")));

    bcrb_put(tree, "dragon", "green");
    assert(!strcmp("green", (char *) bcrb_get(tree, "dragon")));

    bcrb_put(tree, "apple", "red");
    assert(!strcmp("red", (char *) bcrb_get(tree, "apple")));

    bcrb_put(tree, "banana", "yellow");
    assert(!strcmp("yellow", (char *) bcrb_get(tree, "banana")));

    assert(!strcmp("red", (char *) bcrb_get(tree, "apple")));
    assert(!strcmp("yellow", (char *) bcrb_get(tree, "banana")));
    assert(!strcmp("brown", (char *) bcrb_get(tree, "coconut")));
    assert(!strcmp("green", (char *) bcrb_get(tree, "dragon")));

    bcrb_del(tree, NULL, NULL);
}

void
test_right_right_left(void)
{
    bcrb *tree = bcrb_new(bcrb_strcmp);

    bcrb_put(tree, "apple", "red");
    assert(!strcmp("red", (char *) bcrb_get(tree, "apple")));

    bcrb_put(tree, "banana", "yellow");
    assert(!strcmp("yellow", (char *) bcrb_get(tree, "banana")));

    bcrb_put(tree, "dragon", "green");
    assert(!strcmp("green", (char *) bcrb_get(tree, "dragon")));

    bcrb_put(tree, "coconut", "brown");
    assert(!strcmp("brown", (char *) bcrb_get(tree, "coconut")));

    assert(!strcmp("red", (char *) bcrb_get(tree, "apple")));
    assert(!strcmp("yellow", (char *) bcrb_get(tree, "banana")));
    assert(!strcmp("brown", (char *) bcrb_get(tree, "coconut")));
    assert(!strcmp("green", (char *) bcrb_get(tree, "dragon")));

    bcrb_del(tree, NULL, NULL);
}

void
test_right_right_right(void)
{
    bcrb *tree = bcrb_new(bcrb_strcmp);

    bcrb_put(tree, "apple", "red");
    assert(!strcmp("red", (char *) bcrb_get(tree, "apple")));

    bcrb_put(tree, "banana", "yellow");
    assert(!strcmp("yellow", (char *) bcrb_get(tree, "banana")));

    bcrb_put(tree, "coconut", "brown");
    assert(!strcmp("brown", (char *) bcrb_get(tree, "coconut")));

    bcrb_put(tree, "dragon", "green");
    assert(!strcmp("green", (char *) bcrb_get(tree, "dragon")));

    assert(!strcmp("red", (char *) bcrb_get(tree, "apple")));
    assert(!strcmp("yellow", (char *) bcrb_get(tree, "banana")));
    assert(!strcmp("brown", (char *) bcrb_get(tree, "coconut")));
    assert(!strcmp("green", (char *) bcrb_get(tree, "dragon")));

    bcrb_del(tree, NULL, NULL);
}
