#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "mp5.h"
#include "mp10.h"

/************************************************
*   Introduction:
*   This week's program reads a file consisting of many requests. For each request, this code attempts to
*   find a matching request from among those requests that have not already been matched. If a match is found,
*   the code finds a path as before and record the trip information in a list of matched requests. If no
*   match is found, the code adds the request to the list of unmatched requests.
*************************************************/

//
// These variables hold the heads of two singly-linked lists of 
// requests.  
//
// The avaialble list consists of unpaired requests: partner 
// should be NULL, and next is used to form the list.   
// 
// The shared list consists of groups with non-empty start and end 
// vertex intersections.  Only one of the requests in the group is in 
// the list.  The others are linked through the first's partner field 
// and then through the next field of the others in the group.  The 
// next field of the first request in a group is used to form the
// shared list.
//
// Note: for MP2, you should only build groups of two in the shared
// list.
//

static request_t* available = NULL;
static request_t* shared = NULL;

/* handle_request
  * Two file-scope lists are used for recording requests: available is the singly-linked list of unpaired
  * requests. Any unmatched request is added to this list. The shared list is the list of paired requests. Initially,
  * of course, both lists are empty. 
  *==INPUT==
  * graph_t* g
  * pyr_tree_t* p
  * heap_t* h
  * request_t* r

  *==OUTPUT==
  * 0 on failure (of any allocations)
  * 1 on success
  *==EFFEFT== 
  * 1. If no match is found, add the request to the available list and return 1 for success. Again, be
sure to free any dynamically allocated structures (specifically, those used for testing matches).
  * 2. If a matching request is found, remove the matching request from the available list, free both
requests vertex sets (these should be replaced with the intersected vertex sets), and make both
requests point to the intersected vertex sets and the path filled in by dijkstra. Be sure to build
minimaps for both intersected vertex sets and for the path. Link the two requests as described
previously into the shared list and return 1 for success. 
 */ 
int32_t
handle_request (graph_t* g, pyr_tree_t* p, heap_t* h, request_t* r)
{   
    
    r->src_vs = new_vertex_set();
    if(!r->src_vs)  return 0;

    r->dst_vs = new_vertex_set();
    if(!r->dst_vs){
        free_vertex_set(r->src_vs);
        return 0;
    }       // create vertex sets

    r->path = new_path();
    if(!r->path){
        free_vertex_set(r->src_vs);
        free_vertex_set(r->dst_vs);
        return 0;
    }       // create path

    find_nodes(&(r->from), r->src_vs, p, 0);
    find_nodes(&(r->to), r->dst_vs, p, 0);
    //printf("%d %d %d\n", r->uid, r->src_vs->count, r->dst_vs->count);
    if(!r->src_vs->count || !r->dst_vs->count){
        free_vertex_set(r->src_vs);
        free_vertex_set(r->dst_vs);
        free_path(r->path);
        return 0;
    }   // if either set is empty, free them all
    
    build_vertex_set_minimap(g, r->src_vs);
    build_vertex_set_minimap(g, r->dst_vs);

    vertex_set_t* vint1 = new_vertex_set();
    if(!vint1){
        free_vertex_set(r->src_vs);
        free_vertex_set(r->dst_vs);
        free_path(r->path);
        return 0;
    }

    if(r->src_vs->count > vint1->id_array_size){
        int32_t* temp = realloc(vint1->id, r->src_vs->count * sizeof(*(vint1->id)));
        if(!temp){
            free_vertex_set(r->src_vs);
            free_vertex_set(r->dst_vs);
            free_path(r->path);
            free_vertex_set(vint1);
            return 0;
        }
        vint1->id = temp;
        vint1->id_array_size = r->src_vs->count;
    }

    vertex_set_t* vint2 = new_vertex_set();
    if(!vint2){
        free_vertex_set(r->src_vs);
        free_vertex_set(r->dst_vs);
        free_path(r->path);
        free_vertex_set(vint1);
        return 0;
    }
    if(r->dst_vs->count > vint2->id_array_size){
        int32_t* temp = realloc(vint2->id, r->dst_vs->count * sizeof(*(vint2->id)));
        if(!temp){
            free_vertex_set(r->src_vs);
            free_vertex_set(r->dst_vs);
            free_path(r->path);
            free_vertex_set(vint1);
            free_vertex_set(vint2);
            return 0;
        }
        vint2->id = temp;
        vint2->id_array_size = r->dst_vs->count;
    }       // build intersection sets for both requests

    request_t* last;
    for(request_t* t = available; t; last = t, t = t->next)
        if((r->src_vs->minimap & t->src_vs->minimap) && (r->dst_vs->minimap & t->dst_vs->minimap)){
            if(!merge_vertex_sets(r->src_vs, t->src_vs, vint1))  continue;
            if(!merge_vertex_sets(r->dst_vs, t->dst_vs, vint2))  continue;
            if(dijkstra(g, h, vint1, vint2, r->path)){  // find a path
                if(t == available)  available = t->next;
                else  last->next = t->next;
                free_vertex_set(r->src_vs);
                free_vertex_set(r->dst_vs);
                free_vertex_set(t->src_vs);
                free_vertex_set(t->dst_vs);     // free all the sets

                build_vertex_set_minimap(g, vint1);
                build_vertex_set_minimap(g, vint2);
                build_path_minimap(g, r->path);     //build minmap
                r->src_vs = t->src_vs = vint1;
                r->dst_vs = t->dst_vs = vint2;
                t->path = r->path;
                r->partner = t;
                t->partner = t->next = NULL;
                r->next = shared;   // insert into share list
                shared = r;
                return 1;
            }
        }
    free_vertex_set(vint1);
    free_vertex_set(vint2);
    free_path(r->path);
    r->partner = NULL;
    r->path = NULL;
    r->next = available;
    available = r;  // insert into available list
    return 1;
}


void
print_results ()
{
    request_t* r;
    request_t* prt;

    printf ("Matched requests:\n");
    for (r = shared; NULL != r; r = r->next) {
        printf ("%5d", r->uid);
	for (prt = r->partner; NULL != prt; prt = prt->next) {
	    printf (" %5d", prt->uid);
	}
	printf (" src=%016lX dst=%016lX path=%016lX\n", r->src_vs->minimap,
		r->dst_vs->minimap, r->path->minimap);
    }

    printf ("\nUnmatched requests:\n");
    for (r = available; NULL != r; r = r->next) {
        printf ("%5d src=%016lX dst=%016lX\n", r->uid, r->src_vs->minimap,
		r->dst_vs->minimap);
    }
}


int32_t
show_results_for (graph_t* g, int32_t which)
{
    request_t* r;
    request_t* prt;

    // Can only illustrate one partner.
    for (r = shared; NULL != r; r = r->next) {
	if (which == r->uid) {
	    return show_find_results (g, r, r->partner);
	}
	for (prt = r->partner; NULL != prt; prt = prt->next) {
	    if (which == prt->uid) {
		return show_find_results (g, prt, r);
	    }
	}
    }

    for (r = available; NULL != r; r = r->next) {
        if (which == r->uid) {
	    return show_find_results (g, r, r);
	}
    }
    return 0;
}


static void
free_request (request_t* r)
{
    free_vertex_set (r->src_vs);
    free_vertex_set (r->dst_vs);
    if (NULL != r->path) {
	free_path (r->path);
    }
    free (r);
}

void
free_all_data ()
{
    request_t* r;
    request_t* prt;
    request_t* next;

    // All requests in a group share source and destination vertex sets
    // as well as a path, so we need free those elements only once.
    for (r = shared; NULL != r; r = next) {
	for (prt = r->partner; NULL != prt; prt = next) {
	    next = prt->next;
	    free (prt);
	}
	next = r->next;
	free_request (r);
    }

    for (r = available; NULL != r; r = next) {
	next = r->next;
	free_request (r);
    }
}


