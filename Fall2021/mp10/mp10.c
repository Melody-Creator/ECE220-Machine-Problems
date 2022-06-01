#include <stdint.h>
#include <stdio.h>

#include "mp10.h"

/*
*==INPUT==
 * graph_t* g
 * pyr_tree_t* p
   int32_t nnum
   int32_t anc
 *==OUTPUT==
 * NONE
 *==EFFECT==
 * build the minimap when # of vertices >= 64
*/
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

/* mark_vertex_minimap 
 *If the graph has 64 vertices or fewer, simply use each vertex's array index for the vertex's mm_bit field.
Otherwise, for each graph vertex, your function must identify the pyramid tree node corresponding to each
vertex, then find the ancestor node with index from 21 to 84 (these are the nodes at the fourth level of the
pyramid tree). Finally, set the vertex's mm_bit to the ancestor's node index minus 21. 
 *==INPUT==
 * graph_t* g
 * pyr_tree_t* p
 *==OUTPUT==
 * 1
 *==EFFECT==
 * the function assign a value mm_bit to all vertices in the graph according to their ancestor in the 4-th level of pry tree 
 */
int32_t 
mark_vertex_minimap (graph_t* g, pyr_tree_t* p)
{
    if(g->n_vertices <= 64){    //less than 64, handle it separately
        for(int32_t i = 0; i < g->n_vertices; i++)
            g->vertex[i].mm_bit = i;
    }
    else{
        for(int32_t i = 0; i < 64; i++) //avoid searching the whole tree
            mark_down(g, p, i + 21, i);
    }
    return 1;
}

/* build_vertex_set_minimap
 *given a set, the function calculates its minimap
 *==INPUT==
 * graph_t* g
 * vertex_set_t* vs
 *==OUTPUT==
 * NONE
 *==EFFECT==
 * the function assign a value minimap by ORing the mm_bit field of vertices in the vertex set
 */
void 
build_vertex_set_minimap (graph_t* g, vertex_set_t* vs)
{
    vs->minimap = 0ULL;
    for(int32_t i = 0; i < vs->count; i++)
        vs->minimap |= (1ULL << g->vertex[vs->id[i]].mm_bit);
}

/* build_path_minimap
 *given a set, the function calculates its minimap
 *==INPUT==
 * graph_t* g
 * path_t* p
 *==OUTPUT==
 * NONE
 *==EFFECT==
 * the function assign a value minimap by ORing the mm_bit field of vertices in the path
 */
void 
build_path_minimap (graph_t* g, path_t* p)
{
    p->minimap = 0ULL;
    for(int32_t i = 0; i < p->n_vertices; i++)
        p->minimap |= (1ULL << g->vertex[p->id[i]].mm_bit);
}

/* merge_vertex_sets
  * merges two vertex sets into a third set. This function is used to check for
overlap between the vertex sets of two requests
  *==INPUT==
  *const vertex_set_t* v1
  *const_vertex_Set_t* v2
  *vertex_set_t* vint
  *==OUTPUT==
  * 0 on no intersection is found
  * 1 on intersections can be found
  *==EFFECT==
  *This function finds the intersection of two vertex sets v1 and v2 and writes the results into vint. The
function should assume that the id array in vint has enough space to hold the intersection of the sets (as
mentioned below, your handle_request function must guarantee this property). If the resulting intersection
is non-empty, the function returns 1. Otherwise, it returns 0. 

*/
int32_t
merge_vertex_sets (const vertex_set_t* v1, const vertex_set_t* v2,
		   vertex_set_t* vint)
{
    vint->count = 0;
    for(int32_t i = 0, j = 0;;){    //merge two sorted list with two pointers
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

