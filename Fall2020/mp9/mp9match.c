#include <stdint.h>
#include <stdio.h>

#include "mp5.h"
#include "mp9.h"

/*Introduction:
This function is to find graph vertices within range of the starting and ending locales
for two requests and find a shortest path between any vertex in the starting set and any 
vertex in the ending set. The shortest path will be written in the parameter path.
return 1 on success, or 0 on failure
*/
int32_t
match_requests (graph_t* g, pyr_tree_t* p, heap_t* h,
		request_t* r1, request_t* r2,
		vertex_set_t* src_vs, vertex_set_t* dst_vs, path_t* path)
{
	//find vertices within one starting locale and remove those points outside the other locale
	src_vs->count= 0;
	find_nodes(&(r1->from), src_vs, p, 0);//printf("%d\n", src_vs->count);
	trim_nodes(g, src_vs, &(r2->from));
	
	//find vertices within one ending locale and remove those points outside the other locale
	dst_vs->count= 0;
	find_nodes(&(r1->to), dst_vs, p, 0);//printf("%d\n", dst_vs->count);   
	trim_nodes(g, dst_vs, &(r2->to));
	//printf("%d %d\n", src_vs->count, dst_vs->count);
	//printf("%d\n",find_nodes_inst(&(r1->to), dst_vs, p, 0));
	if(!src_vs->count || !dst_vs->count)  return 0;  //one set is empty, failed
	return dijkstra(g, h, src_vs, dst_vs, path); //find the shortest path
}
