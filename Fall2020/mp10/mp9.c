#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "mp5.h"
#include "mp10.h"

#define MY_INFINITY   1000000000

/*
Introduction: 
This code includes several functions to implement requests for walking partners.
Specifically, it finds vertices in a given locale by a structure called pyramid tree,
and remove points that are not in another locale to find the intersection of two locales.
Finally, it used dijkstra algorithm to find the shortest path between all starting ad ending
points with a heap to reduce its time complexity and store the path.
*/

/*inputs: locale_t* loc, vertex_set_t* vs, pyr_tree_t* p, int32_t nnum
no output or return value, no side effects
description: see mp9.h
*/
void
find_nodes (locale_t* loc, vertex_set_t* vs, pyr_tree_t* p, int32_t nnum)
{
    // Do not modify the following line nor add anything above in the function.
    record_fn_call ();
    //printf("%d\n", nnum);
    if(4 * nnum + 1 >= p->n_nodes){ //meet a leaf node(a vertex in the graph)
        if(in_range(loc, p->node[nnum].x, p->node[nnum].y_left)){
            if(vs->count == vs->id_array_size){
                int32_t *temp = realloc(vs->id, vs->id_array_size * 2 * sizeof(*(vs->id)));
                if(temp){printf("1\n");
                    vs->id = temp;
                    vs->id_array_size *= 2;
                }
                else{printf("2\n");
                    temp = realloc(vs->id, (vs->id_array_size + 1) * sizeof(*(vs->id)));
                    if(temp){
                        vs->id = temp;
                        vs->id_array_size ++;
                    }
                }
            }
            for(int32_t i = vs->count++;; i--){
                if(!i || vs->id[i-1] < p->node[nnum].id){
                    vs->id[i] = p->node[nnum].id;
                    break;
                }
                vs->id[i] = vs->id[i-1];
            }
        }
        return;
    }
    int32_t left = loc->x - loc->range, right = loc->x + loc->range;
    int32_t up = loc->y - loc->range, down = loc->y + loc->range;
    //the following conditional statements are discussing different situations
    //and trying to avoid recursion using the information of locale and x, y_left, y_right
    if(left <= p->node[nnum].x && up <= p->node[nnum].y_left)
        find_nodes(loc, vs, p, 4 * nnum + 1);
    if(right >= p->node[nnum].x && up <= p->node[nnum].y_right)
        find_nodes(loc, vs, p, 4 * nnum + 2);
    if(left <= p->node[nnum].x && down >= p->node[nnum].y_left)
        find_nodes(loc, vs, p, 4 * nnum + 3);
    if(right >= p->node[nnum].x && down >= p->node[nnum].y_right)
        find_nodes(loc, vs, p, 4 * nnum + 4);
}

/*inputs: graph_t* g, vertex_set_t* vs, locale_t* loc
no output or return value, no side effects
description: see mp9.h
*/
void
trim_nodes (graph_t* g, vertex_set_t* vs, locale_t* loc)
{
    vertex_set_t temp = *vs;    //make a copy of the vs set
    vs->count = 0;  //empty the vs set
    for(int32_t i = 0; i < temp.count; i++){
        if(in_range(loc, g->vertex[temp.id[i]].x, g->vertex[temp.id[i]].y))
            vs->id[vs->count++] = temp.id[i];   //if the point is in the range of loc, add it to vs
    }   //use a loop to add all valid points (delete invalid points)
}
//this function is to swap two values using pointers
/*inputs: int32_t *a, int32_t *b
no output or return value, no side effects
*/
void swap(int32_t *a, int32_t *b){
    int32_t c = *a;
    *a = *b;
    *b = c;
}
//this function is to heapify the heap downward from node x (shift down)
/*inputs: graph_t* g, heap_t* h, int32_t x
no output or return value, no side effects
*/
void heap_down(graph_t* g, heap_t* h, int32_t x){

    int32_t least = x; 
    int32_t lson = (x << 1) + 1, rson = (x << 1) + 2;
    //if the value of x is larger than its son with smaller value, then swap them
    if(lson < h->n_elts && g->vertex[h->elt[lson]].from_src < g->vertex[h->elt[least]].from_src)
        least = lson;
 
    if(rson < h->n_elts && g->vertex[h->elt[rson]].from_src < g->vertex[h->elt[least]].from_src)
        least = rson;
 
    if(least != x){
        swap(h->elt + x, h->elt + least);   //swap the value in heap
        g->vertex[h->elt[x]].heap_id = x;   
        g->vertex[h->elt[least]].heap_id = least;   //update the heap_id
        heap_down(g, h, least); //heapify downward
    }

}
//this function is to heapify the heap upward from node x (shift up)
/*inputs: graph_t* g, heap_t* h, int32_t x
no output or return value, no side effects
*/
void heap_up(graph_t* g, heap_t* h, int32_t x){
    int32_t fa = (x - 1) >> 1;
    if(fa >= 0 && g->vertex[h->elt[x]].from_src < g->vertex[h->elt[fa]].from_src){
        swap(h->elt + x, h->elt + fa);  //if the fa of x has a larger value, swap x with its fa
        g->vertex[h->elt[x]].heap_id = x;
        g->vertex[h->elt[fa]].heap_id = fa; //update the heap_id
        heap_up(g, h, fa);  //heapify upward
    }
}
//this function is to build a heap of the points in the set src
/*inputs: graph_t* g, heap_t* h, vertex_set_t* src
no output or return value, no side effects
*/
void heap_initialize(graph_t* g, heap_t* h, vertex_set_t* src){
    h->n_elts = src->count; //all the points are in the heap first
    for(int32_t i = 0; i < h->n_elts; i++){  
        h->elt[i] = src->id[i];
        g->vertex[h->elt[i]].heap_id = i;   //initialize the heap and heap_id
    }
    for(int32_t i = h->n_elts/2-1; i >= 0; i--) //heapify downward from bottom to top, ignore leaves
        heap_down(g, h, i);
}

/*inputs: graph_t* g, heap_t* h, vertex_set_t* src, vertex_set_t* dest, path_t* path
no output, return 1 or 0 for success or failure, no side effects
description: see mp9.h*/
int32_t make_path(graph_t* g, path_t* path, int32_t now, int32_t length){
    if(now == -1){
        if(length > 500){
            int32_t *temp = realloc(path->id, length * sizeof(*(path->id)));
            if(!temp)  return 0;
            path->id = temp;
        }
        return 1;
    }

    int32_t res = make_path(g, path, g->vertex[now].pred, length + 1);
    if(res)  path->id[path->n_vertices++] = now;
    return res;
}

/*inputs: graph_t* g, heap_t* h, vertex_set_t* src, vertex_set_t* dest, path_t* path
no output, return 1 or 0 for success or failure, no side effects
description: see mp9.h
*/
int32_t
dijkstra (graph_t* g, heap_t* h, vertex_set_t* src, vertex_set_t* dest,
          path_t* path)
{
    for(int32_t i = 0; i < g->n_vertices; i++){  //initialize the distances of all points from src to INF
        g->vertex[i].from_src = MY_INFINITY;
        g->vertex[i].heap_id = -1;  //all points are not in the heap
    }
    for(int32_t i = 0; i < src->count; i++){  
        g->vertex[src->id[i]].from_src = 0; //the distances in src is zero
        g->vertex[src->id[i]].pred = -1;    //all possible starting points have a pred value of -1
    }

    heap_initialize(g, h, src); //put points in src into the heap
    while(h->n_elts){   //the heap is not empty, keep iteration
        int32_t uid = h->elt[0];
        vertex_t* u = &(g->vertex[uid]);    //get the vertex that has smallest from_src in the heap
        swap(h->elt, h->elt + h->n_elts - 1);
        h->n_elts --;
        heap_down(g, h, 0); //remove the root of heap and heapify downward from root
        for(int32_t i = 0; i < u->n_neighbors; i++){    //search its neighbors
            int32_t vid = u->neighbor[i];
            vertex_t* v = &(g->vertex[vid]);
            if(u->from_src + u->distance[i] < v->from_src){ //relaxation
                v->from_src = u->from_src + u->distance[i];
                v->pred = uid;  //record the pre vertex in the path
                if(v->heap_id == -1){   //insert the point in the heap and heapify
                    v->heap_id = h->n_elts ++;
                    h->elt[v->heap_id] = vid;
                    heap_up(g, h, v->heap_id);
                }
                else  heap_up(g, h, v->heap_id);  //heapify the heap if the point is already in the heap
            }
        }
    }

    int32_t end = -1, dist = MY_INFINITY;
    for(int32_t i = 0; i < dest->count; i++)
        if(dist >= g->vertex[dest->id[i]].from_src){
            end = dest->id[i];
            dist = g->vertex[dest->id[i]].from_src;    //find the end point of shortest path
        }
    if(dist == MY_INFINITY)  return 0;
    path->n_vertices = 0;
    path->tot_dist = dist;
    return make_path(g, path, end, 0);
}

