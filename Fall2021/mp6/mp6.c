/*									tab:8
 *
 * main.c - skeleton source file for ECE220 image processing MP6
 *
 * "Copyright (c) 2021 by Steven S. Lumetta
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose, without fee, and without written agreement is
 * hereby granted, provided that the above copyright notice and the following
 * two paragraphs appear in all copies of this software.
 * 
 * IN NO EVENT SHALL THE AUTHOR OR THE UNIVERSITY OF ILLINOIS BE LIABLE TO 
 * ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL 
 * DAMAGES ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, 
 * EVEN IF THE AUTHOR AND/OR THE UNIVERSITY OF ILLINOIS HAS BEEN ADVISED 
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * THE AUTHOR AND THE UNIVERSITY OF ILLINOIS SPECIFICALLY DISCLAIM ANY 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE 
 * PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND NEITHER THE AUTHOR NOR
 * THE UNIVERSITY OF ILLINOIS HAS ANY OBLIGATION TO PROVIDE MAINTENANCE, 
 * SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS."
 *
 * Author:	    Steven S. Lumetta
 * Version:	    1
 * Creation Date:   19 July 2021
 * Filename:	    mp6.h
 * History:
 *	SSL	1	19 July 2021
 *		First written.
 */

#include <math.h>
#include "mp6.h"

/*
Introduction:
The following code is used to implement a few basic image processing techniques on arrays of pixels. 
Specifically, the code performs conversion from RGB (red, green, and blue) pixels to HSL 
(hue, saturation, and luma/luminance) and back, edge detection using convolution with Sobel kernels, 
and histogram equalization to balance the distribution of brightness across the image.
*/

//
// convert_RGB_to_HSL -- convert red, green, and blue image data into
//   HSL channels.  See documentation for specific equations to be used.
//
//   INPUTS: height -- number of rows in image
//           width -- number of columns in image
//           red -- red pixels (0-255) in column-major order
//           green -- green pixels (0-255) in column-major order
//           blue -- blue pixels (0-255) in column-major order
//   OUTPUTS: H -- hue (0-49151) in column-major order
//            S -- saturation (0-32768) in column-major order
//            L -- luma (0 to 510) in column-major order
//   RETURN VALUE: none
//   SIDE EFFECTS: none
//
void
convert_RGB_to_HSL (int32_t height, int32_t width, const uint8_t* red, 
		    const uint8_t* green, const uint8_t* blue,
		    uint16_t* H, uint16_t* S, uint16_t* L)
{
	for(int32_t index = 0; index < height * width; index ++){
		int32_t R = red[index], G = green[index], B = blue[index];
		int32_t M = fmax(R, fmax(G, B));
		int32_t N = fmin(R, fmin(G, B));
		int32_t C = M - N;		// calculate related variables

		L[index] = M + N;
		if(M == 0 || N == 255)  
			S[index] = 0;
		else if(0 < L[index] && L[index] <= 255)  
			S[index] = 0x8000 * C / L[index];
		else                    
			S[index] = 0x8000 * C / (510 - L[index]);	// find S

		if(C == 0)	
			H[index] = 0;
		else if(C > 0 && M == R)  
			H[index] = 0x2000 * (C + G - B) / C;
		else if(C > 0 && M > R && M == G)
			H[index] = 0x2000 * (3 * C + B - R) / C;
		else	
			H[index] = 0x2000 * (5 * C + R - G) / C;	// find H
	}
}

//
// convert_HSL_to_RGB -- convert HSL channel data into red, green, and 
//   blue image data.  See documentation for specific equations to be used.
//
//   INPUTS: height -- number of rows in image
//           width -- number of columns in image
//           H -- hue (0-49151) in column-major order
//           S -- saturation (0-32768) in column-major order
//           L -- luma (0 to 510) in column-major order
//   OUTPUTS: red -- red pixels (0-255) in column-major order
//            green -- green pixels (0-255) in column-major order
//            blue -- blue pixels (0-255) in column-major order
//   RETURN VALUE: none
//   SIDE EFFECTS: none
//
void
convert_HSL_to_RGB (int32_t height, int32_t width, const uint16_t* H, 
		    const uint16_t* S, const uint16_t* L, 
		    uint8_t* red, uint8_t* green, uint8_t* blue)
{
	for(int32_t index = 0; index < height * width; index ++){
		int32_t C, N, M;
		if(L[index] <= 255)
			C = (int32_t)S[index] * L[index] / 0x8000;
		else
			C = (int32_t)S[index] * (510 - L[index]) / 0x8000;
		N = (L[index] - C) / 2;
		M = N + C;
		int32_t H_maj = H[index] / 0x2000, H_min = H[index] & 0x3FFF, T;
		if(H_min >= 0x2000)
			T = N + C * (H_min - 0x2000) / 0x2000;
		else
			T = N + C * (0x2000 - H_min) / 0x2000;		// find related variables
		switch(H_maj){
			case 0:
				red[index] = M;
				green[index] = N;
				blue[index] = T;
				break;
			case 1:
				red[index] = M;
				green[index] = T;
				blue[index] = N;
				break;
			case 2:
				red[index] = T;
				green[index] = M;
				blue[index] = N;
				break;
			case 3:
				red[index] = N;
				green[index] = M;
				blue[index] = T;
				break;
			case 4:
				red[index] = N;
				green[index] = T;
				blue[index] = M;
				break;
			default:
				red[index] = T;
				green[index] = N;
				blue[index] = M;
		}							// use switch to find (R, G, B)
	}
}

//
// compute_sobel_kernels -- compute X and Y Sobel edge detection kernels
//   using convolution.  Edge pixels may be ignored, and kernel outputs
//   for these pixels left undefined.
//
//   INPUTS: height -- number of rows in image
//           width -- number of columns in image
//           L -- luma (0-510) in column-major order
//   OUTPUTS: Gx -- Sobel X kernel pixels in column-major order
//            Gy -- Sobel Y kernel pixels in column-major order
//   RETURN VALUE: none
//   SIDE EFFECTS: none
//
void
compute_sobel_kernels (int32_t height, int32_t width, const uint16_t* L, 
                       int32_t* Gx, int32_t* Gy)
{
	int32_t Kx[3][3] = {{1, 0, -1}, {2, 0, -2}, {1, 0, -1}};
	int32_t Ky[3][3] = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};	// initialize Kx, Ky

	for(int32_t i = 1; i < height-1; i++)
		for(int32_t j = 1; j < width-1; j++){	// omit the edge by restrict i, j
			int32_t resx = 0, resy = 0;
			for(int32_t dx = -1; dx <= 1; dx++)
				for(int32_t dy = -1; dy <= 1; dy++){	// walk through the kernel
					resx += Kx[1+dx][1+dy] * L[(i+dx)*width+j+dy];
					resy += Ky[1+dx][1+dy] * L[(i+dx)*width+j+dy];	// find the position in 1-D array
				}
			Gx[i*width+j] = resx;
			Gy[i*width+j] = resy;	// fill in Gx, Gy by the result of convolution
		}
}

//
// equalize_intensities -- perform histogram equalization to re-map image
//   intensities.
//
//   INPUTS: height -- number of rows in image
//           width -- number of columns in image
//           L -- luma (0-510) in column-major order
//   OUTPUTS: L -- modified luma (0-510) in column-major order
//   RETURN VALUE: none
//   SIDE EFFECTS: none
//
void
equalize_intensities (int32_t height, int32_t width, uint16_t* L)
{
	int32_t H[666], K[666], X[666], N = height * width;		// total pixels named N
	for(int32_t i = 0; i <= 510; i++)  H[i] = K[i] = 0;		// initialize histogram
	for(int32_t index = 0; index < N; index ++)  H[L[index]] ++;	// compute histogram
	K[0] = H[0];
	for(int32_t i = 1; i <= 510; i++)  K[i] = K[i-1] + H[i];
	for(int32_t i = 0; i <= 510; i++)  X[i] = (511 * (int64_t)K[i] + N - 1) / N - 1;	//build look-up table
	for(int32_t index = 0; index < N; index ++)  L[index] = X[L[index]];	// update luma
}
