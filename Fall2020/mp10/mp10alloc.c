#include <stdint.h>
#include <stdlib.h>

#include "mp10.h"

//the following functions are used to create and free vertex sets and paths
vertex_set_t*
new_vertex_set ()
{
    vertex_set_t *p = malloc(sizeof(*p));
    if(!p)  return NULL;
    p->count = 0;
    p->id_array_size = 100;
    p->id = malloc(100 * sizeof(*(p->id)));
    if(!p->id){
        free(p);
        return NULL;
    }
    return p;
}


void
free_vertex_set (vertex_set_t* vs)
{
    if(vs){
        if(vs->id)  free(vs->id);
        free(vs);
    }
}


path_t*
new_path ()
{
    path_t* p = malloc(sizeof(*p));
    if(!p)  return NULL;
    p->id = malloc(500 * sizeof(*(p->id)));
    if(!p->id){
        free(p);
        return NULL;
    }
    return p;
}


void
free_path (path_t* path)
{
    if(path){
        if(path->id)  free(path->id);
        free(path);
    }
}

