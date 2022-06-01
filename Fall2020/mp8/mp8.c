#include <stdlib.h>
#include <string.h>

#include "mp8.h"
/*
Introduction: Do flood fill operations using recursion to color an image from a given point with particular colors and various
conditions to stop the recursion.
*/

/*
 * basicFlood -- wrapper for flood filling recursively from a point until 
 *               reaching white or the image border
 * INPUTS: width -- width of the input image
 *         height -- height of the input image
 *         inRed -- pointer to the input red channel (1-D array)
 *         inGreen -- pointer to the input green channel (1-D array)
 *         inBlue -- pointer to the input blue channel (1-D array)
 *         startX -- starting x position of the flood
 *         startY -- starting y position of the flood
 *         floodR -- red component of the flood color
 *         floodG -- green component of the flood color
 *         floodB -- blue component of the flood color
 * OUTPUTS: outRed -- pointer to the output red channel (1-D array)
 *          outGreen -- pointer to the output green channel (1-D array)
 *          outBlue -- pointer to the output blue channel (1-D array)
 * RETURN VALUE: none
 * SIDE EFFECTS: none
 */
void 
basicFlood (int32_t width, int32_t height,
	    const uint8_t* inRed, const uint8_t* inGreen, 
	    const uint8_t* inBlue, 
	    int32_t startX, int32_t startY, 
	    uint8_t floodR, uint8_t floodG, uint8_t floodB,
	    uint8_t* outRed, uint8_t* outGreen, 
	    uint8_t* outBlue)
{
	memset(outRed, 0, width * height);	//clear the marking array
	basicRecurse(width, height, inRed, inGreen, inBlue, startX, startY, outRed);	//do the flood fill process
	for(int32_t x = 0; x < width; x++)
		for(int32_t y = 0; y < height; y++){
			if(outRed[x+y*width]){		//if this pixel is visited, fill it with the flood color
				outRed[x+y*width]   = floodR;
				outGreen[x+y*width] = floodG;
				outBlue[x+y*width]  = floodB;
			}
			else{				//if this pixel is not visited, copy from the origin image
				outRed[x+y*width]   = inRed[x+y*width];
				outGreen[x+y*width] = inGreen[x+y*width];
				outBlue[x+y*width]  = inBlue[x+y*width];
			}
		}
}


/*
 * colorsWithinDistSq -- returns 1 iff two colors are within Euclidean
 *                       distance squared of one another in RGB space
 * INPUTS: r1 -- red component of color 1
 *         g1 -- green component of color 1
 *         b1 -- blue component of color 1
 *         r2 -- red component of color 2
 *         g2 -- green component of color 2
 *         b2 -- blue component of color 2
 *         distSq -- maximum distance squared for the check
 * RETURN VALUE: 1 if the sum of the squares of the differences in the 
 *               three components is less or equal to distSq; 0 otherwise
 * SIDE EFFECTS: none
 */
int32_t
colorsWithinDistSq (uint8_t r1, uint8_t g1, uint8_t b1,
                    uint8_t r2, uint8_t g2, uint8_t b2, uint32_t distSq)
{
	uint32_t R = (int32_t)(r1 - r2) * (int32_t)(r1 - r2);
	uint32_t G = (int32_t)(g1 - g2) * (int32_t)(g1 - g2);
	uint32_t B = (int32_t)(b1 - b2) * (int32_t)(b1 - b2);

	if(R + G + B <= distSq)  return 1;		//calculate the distance squared and compare
	return 0;
}


/*
 * greyFlood -- wrapper for flood filling recursively from a point until 
 *              reaching near-white pixels or the image border
 * INPUTS: width -- width of the input image
 *         height -- height of the input image
 *         inRed -- pointer to the input red channel (1-D array)
 *         inGreen -- pointer to the input green channel (1-D array)
 *         inBlue -- pointer to the input blue channel (1-D array)
 *         startX -- starting x position of the flood
 *         startY -- starting y position of the flood
 *         floodR -- red component of the flood color
 *         floodG -- green component of the flood color
 *         floodB -- blue component of the flood color
 *         distSq -- maximum distance squared between white and boundary
 *                   pixel color
 * OUTPUTS: outRed -- pointer to the output red channel (1-D array)
 *          outGreen -- pointer to the output green channel (1-D array)
 *          outBlue -- pointer to the output blue channel (1-D array)
 * RETURN VALUE: none
 * SIDE EFFECTS: none
 */
void 
greyFlood (int32_t width, int32_t height,
	   const uint8_t* inRed, const uint8_t* inGreen, 
	   const uint8_t* inBlue, 
	   int32_t startX, int32_t startY, 
	   uint8_t floodR, uint8_t floodG, uint8_t floodB, uint32_t distSq,
	   uint8_t* outRed, uint8_t* outGreen, 
	   uint8_t* outBlue)
{
	memset(outRed, 0, width * height);	//clear the marking array
	greyRecurse(width, height, inRed, inGreen, inBlue, startX, startY, distSq, outRed);	//do the flood fill process
	for(int32_t x = 0; x < width; x++)
		for(int32_t y = 0; y < height; y++){
			if(outRed[x+y*width]){		//if this pixel is visited, fill it with the flood color
				outRed[x+y*width]   = floodR;
				outGreen[x+y*width] = floodG;
				outBlue[x+y*width]  = floodB;
			}
			else{				//if this pixel is not visited, copy from the origin image
				outRed[x+y*width]   = inRed[x+y*width];
				outGreen[x+y*width] = inGreen[x+y*width];
				outBlue[x+y*width]  = inBlue[x+y*width];
			}
		}
}


/*
 * limitedFlood -- wrapper for flood filling recursively from a point until 
 *                 reaching pixels too different (in RGB color) from the 
 *                 color at the flood start point, too far away 
 *                 (> 35 pixels), or beyond the image border
 * INPUTS: width -- width of the input image
 *         height -- height of the input image
 *         inRed -- pointer to the input red channel (1-D array)
 *         inGreen -- pointer to the input green channel (1-D array)
 *         inBlue -- pointer to the input blue channel (1-D array)
 *         startX -- starting x position of the flood
 *         startY -- starting y position of the flood
 *         floodR -- red component of the flood color
 *         floodG -- green component of the flood color
 *         floodB -- blue component of the flood color
 *         distSq -- maximum distance squared between pixel at origin 
 *                   and boundary pixel color
 * OUTPUTS: outRed -- pointer to the output red channel (1-D array)
 *          outGreen -- pointer to the output green channel (1-D array)
 *          outBlue -- pointer to the output blue channel (1-D array)
 * RETURN VALUE: none
 * SIDE EFFECTS: none
 */
void 
limitedFlood (int32_t width, int32_t height,
	      const uint8_t* inRed, const uint8_t* inGreen, 
	      const uint8_t* inBlue, 
	      int32_t startX, int32_t startY, 
	      uint8_t floodR, uint8_t floodG, uint8_t floodB, uint32_t distSq,
	      uint8_t* outRed, uint8_t* outGreen, 
	      uint8_t* outBlue)
{
	memset(outRed, 0, width * height);	//clear the marking array
	limitedRecurse(width, height, inRed, inGreen, inBlue, startX, startY, startX, startY, distSq, outRed);	//do the flood fill process
	for(int32_t x = 0; x < width; x++)
		for(int32_t y = 0; y < height; y++){
			if(outRed[x+y*width]){		//if this pixel is visited, fill it with the flood color
				outRed[x+y*width]   = floodR;
				outGreen[x+y*width] = floodG;
				outBlue[x+y*width]  = floodB;
			}
			else{				//if this pixel is not visited, copy from the origin image
				outRed[x+y*width]   = inRed[x+y*width];
				outGreen[x+y*width] = inGreen[x+y*width];
				outBlue[x+y*width]  = inBlue[x+y*width];
			}
		}
}
