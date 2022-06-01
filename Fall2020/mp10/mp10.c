#include <stdint.h>
#include <stdio.h>

#include "mp10.h"

void mark_down(graph_t* g, pyr_tree_t* p, int32_t nnum, int32_t anc){
    if(4 * nnum + 1 >= p->n_nodes){
        g->vertex[p->node[nnum].id].mm_bit = anc;
        return;
    }
    mark_down(g, p, 4 * nnum + 1, anc);
    mark_down(g, p, 4 * nnum + 2, anc);
    mark_down(g, p, 4 * nnum + 3, anc);
    mark_down(g, p, 4 * nnum + 4, anc);
}

int32_t 
mark_vertex_minimap (graph_t* g, pyr_tree_t* p)
{
    if(g->n_vertices <= 64){
        for(int32_t i = 0; i < g->n_vertices; i++)
            g->vertex[i].mm_bit = i;
    }
    else{
        for(int32_t i = 0; i < 64; i++)
            mark_down(g, p, i + 21, i);
    }
    return 1;
}


void 
build_vertex_set_minimap (graph_t* g, vertex_set_t* vs)
{
    vs->minimap = 0ULL;
    for(int32_t i = 0; i < vs->count; i++)
        vs->minimap |= (1ULL << g->vertex[vs->id[i]].mm_bit);
}


void 
build_path_minimap (graph_t* g, path_t* p)
{
    p->minimap = 0ULL;
    for(int32_t i = 0; i < p->n_vertices; i++)
        p->minimap |= (1ULL << g->vertex[p->id[i]].mm_bit);
}


int32_t
merge_vertex_sets (const vertex_set_t* v1, const vertex_set_t* v2,
		   vertex_set_t* vint)
{
    vint->count = 0;
    for(int32_t i = 0, j = 0;;){
        if(i == v1->count || j == v2->count)  break;
        if(v1->id[i] == v2->id[j]){
            vint->id[vint->count++] = v1->id[i];
            i ++;
            j ++;
        }
        else if(v1->id[i] < v2->id[j])  i ++;
        else  j ++;
    }
    if(vint->count)  return 1;
    return 0;
}

