#include <stdint.h>
#include <stdio.h>

#include "mp5.h"
#include "mp9.h"

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
    if(4 * nnum + 1 >= p->n_nodes){ //meet a leaf node(a vertex in the graph)
        if(in_range(loc, p->node[nnum].x, p->node[nnum].y_left) && vs->count < MAX_IN_VERTEX_SET)
            vs->id[vs->count++] = p->node[nnum].id; //put the vertex in the vs set
        return;
    }
    int32_t dx = loc->x - p->node[nnum].x;
    int32_t dy_left = loc->y - p->node[nnum].y_left, dy_right = loc->y - p->node[nnum].y_right;
    //the following conditional statements are discussing different situations
    //and trying to avoid recursion using the information of locale and x, y_left, y_right
    if(dx >= 0 && loc->range <= dx){
        if(dy_right <= 0 && loc->range <= -dy_right)
            find_nodes(loc, vs, p, 4 * nnum + 2);
        else if(dy_right >= 0 && loc->range <= dy_right)
            find_nodes(loc, vs, p, 4 * nnum + 4);
        else{
            find_nodes(loc, vs, p, 4 * nnum + 2);
            find_nodes(loc, vs, p, 4 * nnum + 4);
        }
    }
    else if(dx <= 0 && loc->range <= -dx){
        if(dy_left <= 0 && loc->range <= -dy_left)
            find_nodes(loc, vs, p, 4 * nnum + 1);
        else if(dy_left >= 0 && loc->range <= dy_left)
            find_nodes(loc, vs, p, 4 * nnum + 3);
        else{
            find_nodes(loc, vs, p, 4 * nnum + 1);
            find_nodes(loc, vs, p, 4 * nnum + 3);
        }
    }
    else{
        if(p->node[nnum].y_left <= p->node[nnum].y_right){

            if(dy_left <= 0){
                if(loc->range <= -dy_left){
                    find_nodes(loc, vs, p, 4 * nnum + 1);
                    find_nodes(loc, vs, p, 4 * nnum + 2);
                }
                else if(loc->range <= -dy_right){
                    find_nodes(loc, vs, p, 4 * nnum + 1);
                    find_nodes(loc, vs, p, 4 * nnum + 2);
                    find_nodes(loc, vs, p, 4 * nnum + 3);
                }
                else{
                    find_nodes(loc, vs, p, 4 * nnum + 1);
                    find_nodes(loc, vs, p, 4 * nnum + 2);
                    find_nodes(loc, vs, p, 4 * nnum + 3);
                    find_nodes(loc, vs, p, 4 * nnum + 4);
                }
            }
       
            else if(dy_right >= 0){
                if(loc->range <= dy_right){
                    find_nodes(loc, vs, p, 4 * nnum + 3);
                    find_nodes(loc, vs, p, 4 * nnum + 4);
                }
                else if(loc->range <= dy_left){
                    find_nodes(loc, vs, p, 4 * nnum + 2);
                    find_nodes(loc, vs, p, 4 * nnum + 3);
                    find_nodes(loc, vs, p, 4 * nnum + 4);
                }
                else{
                    find_nodes(loc, vs, p, 4 * nnum + 1);
                    find_nodes(loc, vs, p, 4 * nnum + 2);
                    find_nodes(loc, vs, p, 4 * nnum + 3);
                    find_nodes(loc, vs, p, 4 * nnum + 4);
                }
            }
            else{
                if(loc->range <= dy_left && loc->range <= -dy_right){
                    find_nodes(loc, vs, p, 4 * nnum + 2);
                    find_nodes(loc, vs, p, 4 * nnum + 3);
                }
                else if(loc->range <= dy_left){
                    find_nodes(loc, vs, p, 4 * nnum + 2);
                    find_nodes(loc, vs, p, 4 * nnum + 3);
                    find_nodes(loc, vs, p, 4 * nnum + 4);
                }
                else if(loc->range <= -dy_right){
                    find_nodes(loc, vs, p, 4 * nnum + 1);
                    find_nodes(loc, vs, p, 4 * nnum + 2);
                    find_nodes(loc, vs, p, 4 * nnum + 3);
                }
                else{
                    find_nodes(loc, vs, p, 4 * nnum + 1);
                    find_nodes(loc, vs, p, 4 * nnum + 2);
                    find_nodes(loc, vs, p, 4 * nnum + 3);
                    find_nodes(loc, vs, p, 4 * nnum + 4);
                }
            }
        }
        else{
            if(dy_right <= 0){
                if(loc->range <= -dy_right){
                    find_nodes(loc, vs, p, 4 * nnum + 1);
                    find_nodes(loc, vs, p, 4 * nnum + 2);
                }
                else if(loc->range <= -dy_left){
                    find_nodes(loc, vs, p, 4 * nnum + 1);
                    find_nodes(loc, vs, p, 4 * nnum + 2);
                    find_nodes(loc, vs, p, 4 * nnum + 4);
                }
                else{
                    find_nodes(loc, vs, p, 4 * nnum + 1);
                    find_nodes(loc, vs, p, 4 * nnum + 2);
                    find_nodes(loc, vs, p, 4 * nnum + 3);
                    find_nodes(loc, vs, p, 4 * nnum + 4);
                }
            }
       
            else if(dy_left >= 0){
                if(loc->range <= dy_left){
                    find_nodes(loc, vs, p, 4 * nnum + 3);
                    find_nodes(loc, vs, p, 4 * nnum + 4);
                }
                else if(loc->range <= dy_right){
                    find_nodes(loc, vs, p, 4 * nnum + 1);
                    find_nodes(loc, vs, p, 4 * nnum + 3);
                    find_nodes(loc, vs, p, 4 * nnum + 4);
                }
                else{
                    find_nodes(loc, vs, p, 4 * nnum + 1);
                    find_nodes(loc, vs, p, 4 * nnum + 2);
                    find_nodes(loc, vs, p, 4 * nnum + 3);
                    find_nodes(loc, vs, p, 4 * nnum + 4);
                }
            }
            else{
                if(loc->range <= -dy_left && loc->range <= dy_right){
                    find_nodes(loc, vs, p, 4 * nnum + 1);
                    find_nodes(loc, vs, p, 4 * nnum + 4);
                }
                else if(loc->range <= -dy_left){
                    find_nodes(loc, vs, p, 4 * nnum + 1);
                    find_nodes(loc, vs, p, 4 * nnum + 2);
                    find_nodes(loc, vs, p, 4 * nnum + 4);
                }
                else if(loc->range <= dy_right){
                    find_nodes(loc, vs, p, 4 * nnum + 1);
                    find_nodes(loc, vs, p, 4 * nnum + 3);
                    find_nodes(loc, vs, p, 4 * nnum + 4);
                }
                else{
                    find_nodes(loc, vs, p, 4 * nnum + 1);
                    find_nodes(loc, vs, p, 4 * nnum + 2);
                    find_nodes(loc, vs, p, 4 * nnum + 3);
                    find_nodes(loc, vs, p, 4 * nnum + 4);
                }
            }
        }
    }
}

/*inputs: graph_t* g, vertex_set_t* vs, locale_t* loc
no output or return value, no side effects
description: see mp9.h
*/
void
trim_nodes (graph_t* g, vertex_set_t* vs, locale_t* loc)
{
    /*vertex_set_t temp = *vs;    //make a copy of the vs set
    vs->count = 0;  //empty the vs set
    for(int32_t i = 0; i < temp.count; i++){
        if(in_range(loc, g->vertex[temp.id[i]].x, g->vertex[temp.id[i]].y))
            vs->id[vs->count++] = temp.id[i];   //if the point is in the range of loc, add it to vs
    }   //use a loop to add all valid points (delete invalid points)
    printf("%d\n", vs->count);
    */
    int32_t count = 0;
    for(int32_t i = 0; i < vs->count; i++){
        if(in_range(loc, g->vertex[vs->id[i]].x, g->vertex[vs->id[i]].y))
            vs->id[count++] = vs->id[i];      //if the point is in the range of loc, retain it in vs
    }   //use a loop to retain all valid points (delete invalid points)
    vs->count = count;      //change vs->count after compressing
    //printf("%d\n", vs->count);
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
                else{
                    heap_up(g, h, v->heap_id);  //heapify the heap if the point is already in the heap
                    heap_down(g, h, v->heap_id);
                }
            }
        }
    }

    path_t temp;
    temp.n_vertices = 0;
    temp.tot_dist = MY_INFINITY;   

    int32_t end = -1;
    
    for(int32_t i = 0; i < dest->count; i++)
        if(temp.tot_dist > g->vertex[dest->id[i]].from_src){
            end = dest->id[i];
            temp.tot_dist = g->vertex[dest->id[i]].from_src;    //find the end point of shortest path
        }
    if(temp.tot_dist == MY_INFINITY)  return 0;     //no path exists, failed

    while(end != -1){   //record the path reversely
        if(temp.n_vertices >= MAX_PATH_LENGTH)  return 0; //path is too long, failed
        temp.id[temp.n_vertices++] = end;
        end = g->vertex[end].pred;  //go backward
    }
    path->n_vertices = temp.n_vertices;
    path->tot_dist = temp.tot_dist;
    for(int32_t i = 0; i < path->n_vertices; i++)
        path->id[i] = temp.id[path->n_vertices-1-i];    //record the path forward
    
    return 1;
}

