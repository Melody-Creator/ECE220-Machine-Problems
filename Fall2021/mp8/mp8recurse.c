#include <stdlib.h>
#include <string.h>

#include "mp8.h"

/*
Introduction: Do flood fill operations using recursion to color an image from a given point with particular colors and various
conditions to stop the recursion.
*/

/*
 * basicRecurse -- flood fill recursively from a point until reaching
 *                 white or the image border
 * INPUTS: width -- width of the input image
 *         height -- height of the input image
 *         inRed -- pointer to the input red channel (1-D array)
 *         inGreen -- pointer to the input green channel (1-D array)
 *         inBlue -- pointer to the input blue channel (1-D array)
 *         x -- current x position of the flood
 *         y -- current y position of the flood
 *         marking -- array used to mark seen positions (indexed
 *                    in the same way as the RGB input arrays)
 * OUTPUTS: marking -- marked with flooded pixels
 * RETURN VALUE: none
 * SIDE EFFECTS: none
 */
void 
basicRecurse (int32_t width, int32_t height,
	      const uint8_t* inRed, const uint8_t* inGreen, 
	      const uint8_t* inBlue, 
	      int32_t x, int32_t y, 
	      uint8_t* marking)
{
	if(x < 0 || x >= width || y < 0 || y >= height || marking[x+y*width])  return;	//out of boundary or visited before
	if(inRed[x+y*width] == 0xFF && inGreen[x+y*width] == 0xFF && inBlue[x+y*width] == 0xFF)  return;	//meet a white pixel
	marking[x+y*width] = 1;		//mark this pixel, it's visited now
	basicRecurse(width, height, inRed, inGreen, inBlue, x, y-1, marking);	//search four directions from this pixel
	basicRecurse(width, height, inRed, inGreen, inBlue, x+1, y, marking);
	basicRecurse(width, height, inRed, inGreen, inBlue, x, y+1, marking);
	basicRecurse(width, height, inRed, inGreen, inBlue, x-1, y, marking);
}


/*
 * greyRecurse -- flood fill recursively from a point until reaching
 *                near-white pixels or the image border
 * INPUTS: width -- width of the input image
 *         height -- height of the input image
 *         inRed -- pointer to the input red channel (1-D array)
 *         inGreen -- pointer to the input green channel (1-D array)
 *         inBlue -- pointer to the input blue channel (1-D array)
 *         x -- current x position of the flood
 *         y -- current y position of the flood
 *         distSq -- maximum distance squared between white and boundary
 *                   pixel color
 *         marking -- array used to mark seen positions (indexed
 *                    in the same way as the RGB input arrays)
 * OUTPUTS: marking -- marked with flooded pixels
 * RETURN VALUE: none
 * SIDE EFFECTS: none
 */
void 
greyRecurse (int32_t width, int32_t height,
	     const uint8_t* inRed, const uint8_t* inGreen, 
	     const uint8_t* inBlue, 
	     int32_t x, int32_t y, uint32_t distSq, 
	     uint8_t* marking)
{
	if(x < 0 || x >= width || y < 0 || y >= height || marking[x+y*width])  return;	//out of boundary or visited before
	uint8_t R = inRed[x+y*width], G = inGreen[x+y*width], B = inBlue[x+y*width];
	if(colorsWithinDistSq(R, G, B, 0xFF, 0xFF, 0xFF, distSq))  return;	//stop at a grey pixel
	marking[x+y*width] = 1;		//mark this pixel, it's visited now
	greyRecurse(width, height, inRed, inGreen, inBlue, x, y-1, distSq, marking);	//search four directions from this pixel
	greyRecurse(width, height, inRed, inGreen, inBlue, x+1, y, distSq, marking);
	greyRecurse(width, height, inRed, inGreen, inBlue, x, y+1, distSq, marking);
	greyRecurse(width, height, inRed, inGreen, inBlue, x-1, y, distSq, marking);
}


/*
 * limitedRecurse -- flood fill recursively from a point until reaching
 *                   pixels too different (in RGB color) from the color at
 *                   the flood start point, too far away (> 35 pixels), or
 *                   beyond the image border
 * INPUTS: width -- width of the input image
 *         height -- height of the input image
 *         inRed -- pointer to the input red channel (1-D array)
 *         inGreen -- pointer to the input green channel (1-D array)
 *         inBlue -- pointer to the input blue channel (1-D array)
 *         origX -- starting x position of the flood
 *         origY -- starting y position of the flood
 *         x -- current x position of the flood
 *         y -- current y position of the flood
 *         distSq -- maximum distance squared between pixel at origin 
 *                   and boundary pixel color
 *         marking -- array used to mark seen positions (indexed
 *                    in the same way as the RGB input arrays)
 * OUTPUTS: marking -- marked with flooded pixels
 * RETURN VALUE: none
 * SIDE EFFECTS: none
 */
void 
limitedRecurse (int32_t width, int32_t height,
	        const uint8_t* inRed, const uint8_t* inGreen, 
	        const uint8_t* inBlue, 
	        int32_t origX, int32_t origY, int32_t x, int32_t y, 
		uint32_t distSq, uint8_t* marking)
{
	uint32_t dX, dY;
	if(x >= origX)  dX = x - origX;
	else  dX = origX - x;
	if(y >= origY)  dY = y - origY;
	else  dY = origY - y;
	if(dX * dX + dY * dY > 35 * 35)  return;	//stop when going more than 35 pixels
	if(x < 0 || x >= width || y < 0 || y >= height || marking[x+y*width])  return;	//out of boundary or visited before
	uint8_t R1 = inRed[x+y*width], G1 = inGreen[x+y*width], B1 = inBlue[x+y*width];
	uint8_t R2 = inRed[origX+origY*width], G2 = inGreen[origX+origY*width], B2 = inBlue[origX+origY*width];
	if(!colorsWithinDistSq(R1, G1, B1, R2, G2, B2, distSq))  return;	//meet the condition to stop
	marking[x+y*width] = 1;		//mark this pixel, it's visited now
	limitedRecurse(width, height, inRed, inGreen, inBlue, origX, origY, x, y-1, distSq, marking);	//search four directions from this pixel
	limitedRecurse(width, height, inRed, inGreen, inBlue, origX, origY, x+1, y, distSq, marking);
	limitedRecurse(width, height, inRed, inGreen, inBlue, origX, origY, x, y+1, distSq, marking);
	limitedRecurse(width, height, inRed, inGreen, inBlue, origX, origY, x-1, y, distSq, marking);
}

