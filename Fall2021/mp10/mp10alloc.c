#include <stdint.h>
#include <stdlib.h>

#include "mp10.h"

//the following functions are used to create and free vertex sets and paths
/* new_vertex_t()
 *==INPUT==
 *NONE
 *==OUTPUT==
 *a pointer with type vertex_set_t*
 *==EFFECT==
 * the function allocates a new vertex set and returns the allocated pointer
 */
vertex_set_t*
new_vertex_set ()
{
    vertex_set_t *p = malloc(sizeof(*p));
    if(!p)  return NULL;
    p->count = 0;
    p->id_array_size = 100;
    p->id = malloc(100 * sizeof(*(p->id)));
    if(!p->id){         // no memory, free p
        free(p);
        return NULL;
    }
    return p;
}

/* free_vertex_set()
 *==INPUT==
 *vertex_set_t* vs
 *==OUTPUT==
 *NONE
 *==EFFECT==
 * the function frees vs (and all pointers inside, if any)
 */
void
free_vertex_set (vertex_set_t* vs)
{
    if(vs){
        if(vs->id)  free(vs->id);
        free(vs);
    }
}

/* new_path()
 *==INPUT==
 *NONE
 *==OUTPUT==
 *a pointer with type path_t*
 *==EFFECT==
 * the function allocates a new path and returns the allocated pointer
 */
path_t*
new_path ()
{
    path_t* p = malloc(sizeof(*p));
    if(!p)  return NULL;
    p->id = malloc(500 * sizeof(*(p->id)));
    if(!p->id){
        free(p);        // no memory, free p
        return NULL;
    }
    return p;
}

/* free_path_set()
 *==INPUT==
 *path_t* path
 *==OUTPUT==
 *NONE
 *==EFFECT==
 * the function frees path (and all pointers inside, if any)
 */
void
free_path (path_t* path)
{
    if(path){
        if(path->id)  free(path->id);
        free(path);
    }
}

