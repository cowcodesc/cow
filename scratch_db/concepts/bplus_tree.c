//TODO:Change BNode->data type to vector
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include "../../stdcow/dynamic_arrays/bytes.h"

#define BNODE_NODE 1
#define BNODE_LEAF 1

#define HEADER               4
#define BTREE_PAGE_SIZE      4096
#define BTREE_MAX_KEY_SIZE   1000
#define BTREE_MAX_VALUE_SIZE 3000
#define NODE_MAX_SIZE        (HEADER + 8 + 2 + 4 + BTREE_MAX_KEY_SIZE + BTREE_MAX_VALUE_SIZE)

/*
 * NODE FORMAT
 * 1. fixed sized header containing type of the node : leaf node or internal node
 *    it also contains the number of keys
 * 2. list of pointers to child nodes (used by internal nodes)
 * 3. list of offset pointing to each K-V (key-value) pair : offset is relative to 1st K-V pair
 * 4. packed K-V pairs
 *
 * Node format
 * | type | nkeys |  pointers | offsets  | K-V pairs |
 * |  2B  |  2B   |  nkeys*8B | nkeys*2B |   ...     |
 *
 * K-V pair format :: given below is a single pair
 * | klen | vlen | key | value |
 * |  2B  |  2B  | ... |  ...  |
 *
 * internal and leaf nodes have same format
*/

typedef struct BNode
{
    bytes data;
} BNode;

typedef struct BTree
{
    uint64_t root;
    //helper functions
    struct BNode *(*get)(uint64_t);
    uint64_t (*new)(BNode *);
    void (*del)(uint64_t);
} BTree;

uint64_t get_little_endian(uint8_t *data, size_t n)
{
    uint64_t value = 0;
    for (size_t i = 0; i < n; i++)
        value |= (data[i] << i * 8);
    return value;
}
void set_little_endian(uint8_t *dst, uint64_t data)
{
    size_t ind = 0;
    while (data)
    {
        dst[ind++] = data & 0x00FF;
        data >>= 8;
    }
}

uint16_t btype(BNode *node) { return (uint16_t)get_little_endian(node->data, 4); }
uint16_t nkeys(BNode *node) { return (uint16_t)get_little_endian(&(node->data[2]), 4); }
void set_header(BNode *node, uint16_t btype, uint16_t nkeys)
{
    set_little_endian(node->data, btype);
    set_little_endian(&(node->data[2]), nkeys);
}

uint64_t get_ptr(BNode *node, uint16_t idx)
{
    assert(idx < nkeys(node));
    const uint16_t offset = HEADER + 8 * idx;
    return get_little_endian(&(node->data[offset]), 8);
}
void set_ptr(BNode *node, uint16_t idx, uint64_t val)
{
    assert(idx < nkeys(node));
    const uint16_t offset = HEADER + 8 * idx;
    set_little_endian(&(node->data[offset]), val);
}

//get address of offset :: 1 based indexing
uint16_t get_offset_addr(BNode *node, uint16_t idx)
{
    assert (1 <= idx && idx <= nkeys(node));
    return HEADER +  8 * nkeys(node) + 2 * (idx - 1);
}
uint16_t get_offset(BNode *node, uint16_t idx)
{
    if (idx == 0) return 0;
    return (uint16_t)get_little_endian(&(node->data[get_offset_addr(node, idx)]), 2);
}
void set_offset(BNode *node, uint16_t idx, uint16_t offset)
{
    set_little_endian(&(node->data[get_offset_addr(node, idx)]), offset);
}

//get KV pair address using offset
uint16_t get_KV_addr(BNode *node, uint16_t idx)
{
    assert(idx < nkeys(node));
    return HEADER + 8 * nkeys(node) + 2 * nkeys(node) + get_offset(node, idx);
}
uint8_t *get_key(BNode *node, uint16_t idx, uint16_t *klen)
{
    assert(idx < nkeys(node));
    const uint16_t addr = get_KV_addr(node, idx);
    *klen = (uint16_t)get_little_endian(&(node->data[addr]), 2);
    return &(node->data[addr + 4]);
}
uint8_t *get_val(BNode *node, uint16_t idx, uint16_t *vlen)
{
    assert(idx < nkeys(node));
    const uint16_t addr = get_KV_addr(node, idx);
    const uint16_t klen = (uint16_t)get_little_endian(&(node->data[addr]), 2);
    *vlen = (uint16_t)get_little_endian(&(node->data[addr + 2]), 2);
    return &(node->data[addr + 4 + klen]);
}

//get size of the node
uint16_t nbytes(BNode *node) { return get_KV_addr(node, nkeys(node)); }

uint16_t lookup_node(BNode *node, uint8_t *key)
{
    const uint16_t n_keys = nkeys(node);
    uint16_t found = 0;

    uint16_t klen;
    uint8_t *_key;
    for (uint16_t i = 0; i < n_keys; i++)
    {
        _key = get_key(node, i, &klen);
        int cmp = memcmp(_key, key, klen);
        if (cmp <= 0) found = i;
        if (cmp >= 0) break;
    }
    return found;
}

void node_append_range(BNode *dst, BNode *src, uint16_t dst_ind, uint16_t src_ind, uint16_t n)
{
    assert(dst_ind + n < nkeys(dst));
    assert(src_ind + n < nkeys(src));

    if (n == 0) return;
    for (uint16_t i = 0; i < n; i++)
        set_ptr(dst, dst_ind + i, get_ptr(src, src_ind + i));

    uint16_t dst_offset = get_offset(dst, dst_ind);
    uint16_t src_offset = get_offset(src, src_ind);

    for (uint16_t i = 0; i < n; i++)
    {
        uint16_t offset = dst_offset + get_offset(src, src_ind + i) - src_offset;
        set_offset(dst, dst_ind + i, offset);
    }

    //copy key-value pairs
    uint16_t dst_KV_begin = get_KV_addr(src, src_ind);
    uint16_t src_KV_begin = get_KV_addr(src, src_ind);
    memcpy(&dst->data[dst_KV_begin], &src->data[src_KV_begin], n);
}

void node_append_KV(BNode *dst, uint16_t idx, uint64_t ptr, uint8_t *key, uint8_t *val)
{
    set_ptr(dst, ptr);
    uint16_t KV_addr = get_KV_addr(dst, idx);

    set_little_endian(&dst->data[KV_addr], (uint16_t)len(key));
    set_little_endian(&dst->data[KV_addr+2], (uint16_t)len(val));
    memcpy(&dst->data[KV_addr+4], key, len(key));
    memcpy(&dst->data[KV_addr+4+len(key)], val, len(val));
    set_offset(dst, idx+1, get_offset(idx) + 4 + len(key) + len(val));
}

void insert_leaf(BNode *dst, BNode *src, uint16_t idx, uint8_t *key, uint8_t *val)
{
    set_header(dst, BNODE_LEAF, nkeys(src) + 1);
    node_append_range(dst, src, 0, 0, idx);
    node_append_KV(dst, idx, 0, key, val);
    node_append_range(dst, src, idx + 1, idx, nkeys(src) - idx);
}

BNode *tree_insert(BTree *tree, BNode *node, uint8_t *key, uint8_t *val)
{
    //create a result Node
    /*(BNode *)malloc(****);*/
    uint16_t idx = lookup_node(new_node, key);
}

void init()
{
    assert(NODE_MAX_SIZE <= BTREE_PAGE_SIZE);
}
