/*									tab:8
 *
 * main.c - skeleton source file for ECE220 picture drawing program
 *
 * "Copyright (c) 2018 by Charles H. Zega, and Saransh Sinha."
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose, without fee, and without written agreement is
 * hereby granted, provided that the above copyright notice and the following
 * two paragraphs appear in all copies of this software.
 * 
 * IN NO EVENT SHALL THE AUTHOR OR THE UNIVERSITY OF ILLINOIS BE LIABLE TO 
 * ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL 
 * DAMAGES ARISING OUT  OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, 
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
 * Author:	    Charles Zega, Saransh Sinha
 * Version:	    1
 * Creation Date:   12 February 2018
 * Filename:	    mp5.h
 * History:
 *	CZ	1	12 February 2018
 *		First written.
 */
#include "mp5.h"

/*
	You must write all your code only in this file, for all the functions!
*/



/* 
 *  near_horizontal
 *	 
 *	 
 *	
 *	
 * INPUTS: x_start,y_start -- the coordinates of the pixel at one end of the line
 * 	   x_end, y_end    -- the coordinates of the pixel at the other end
 * OUTPUTS: draws a pixel to all points in between the two given pixels including
 *          the end points
 * RETURN VALUE: 0 if any of the pixels drawn are out of bounds, otherwise 1
 * SIDE EFFECTS: none
 */


/*
	Introduction: this program is used to draw lines, rectangles, circles, triangles,
	parallelograms and make a simple gradient of colors. You can invoke functions to draw simple 
	shapes and draw_picture make a simple ugly picture.
*/
int32_t
near_horizontal(int32_t x_start, int32_t y_start, int32_t x_end, int32_t y_end){
	/* Your code goes here! */
	int32_t x1, y1, x2, y2;
	if(x_start < x_end){	//let x1 < x2
		x1 = x_start;  x2 = x_end;
		y1 = y_start;  y2 = y_end;
	}
	else{
		x2 = x_start;  x1 = x_end;
		y2 = y_start;  y1 = y_end;
	}
	int32_t sgn = 0, flag = 1, dx = x2 - x1, dy = y2 - y1;
	if(dy > 0)  sgn = 1;
	if(dy < 0)  sgn = -1;		// get difference and sgn for next

	for(int32_t x = x1; x <= x2; x++){
		int32_t y = (2 * dy * (x - x1) + sgn * dx) / (2 * dx) + y1;
		flag = draw_dot(x, y) && flag;	// draw each pixel
	}
	if(flag)  return 1;
	return 0;
}


/* 
 *  near_vertical
 *	 
 *	 
 *	
 *	
 * INPUTS: x_start,y_start -- the coordinates of the pixel at one end of the line
 * 	   x_end, y_end    -- the coordinates of the pixel at the other end
 * OUTPUTS: draws a pixel to all points in between the two given pixels including
 *          the end points
 * RETURN VALUE: 0 if any of the pixels drawn are out of bounds, otherwise 1
 * SIDE EFFECTS: none
 */

int32_t
near_vertical(int32_t x_start, int32_t y_start, int32_t x_end, int32_t y_end){
	/* Your code goes here! */
	int32_t x1, y1, x2, y2;
	if(y_start < y_end){	//let y1 < y2
		x1 = x_start;  x2 = x_end;
		y1 = y_start;  y2 = y_end;
	}
	else{
		x2 = x_start;  x1 = x_end;
		y2 = y_start;  y1 = y_end;
	}
	int32_t sgn = 0, flag = 1, dx = x2 - x1, dy = y2 - y1;
	if(dx > 0)  sgn = 1;
	if(dx < 0)  sgn = -1;		// get the difference and the sgn

	if(dx == 0 && dy == 0){		// special case
		flag = draw_dot(x1, y1) && flag;
		return flag;
	}
	for(int32_t y = y1; y <= y2; y++){
		int32_t x = (2 * dx * (y - y1) + sgn * dy) / (2 * dy) + x1;
		flag = draw_dot(x, y) && flag;	// draw each pixel
	}
	if(flag)  return 1;
	return 0;
}

/* 
 *  draw_line
 *	 
 *	 
 *	
 *	
 * INPUTS: x_start,y_start -- the coordinates of the pixel at one end of the line
 * 	   x_end, y_end    -- the coordinates of the pixel at the other end
 * OUTPUTS: draws a pixel to all points in between the two given pixels including
 *          the end points
 * RETURN VALUE: 0 if any of the pixels drawn are out of bounds, otherwise 1
 * SIDE EFFECTS: none
 */

int32_t
draw_line(int32_t x_start, int32_t y_start, int32_t x_end, int32_t y_end){
	/* Your code goes here! */
	int32_t x1, y1, x2, y2;
	if(x_start < x_end){	//let x1 < x2
		x1 = x_start;  x2 = x_end;
		y1 = y_start;  y2 = y_end;
	}
	else{
		x2 = x_start;  x1 = x_end;
		y2 = y_start;  y1 = y_end;
	}
	int32_t flag = 1, dx = x2 - x1, dy = y2 - y1;
	if(dx == dy){		// special case
		flag = near_vertical(x1, y1, x2, y2) && flag;
		return flag;
	}
	if(-dx <= dy && dy <= dx)		// get the slope and invoke proper function
		flag = near_horizontal(x1, y1, x2, y2) && flag;
	else
		flag = near_vertical(x1, y1, x2, y2) && flag;

	if(flag)  return 1;
	return 0;
}


/* 
 *  draw_rect
 *	 
 *	 
 *	
 *	
 * INPUTS: x,y -- the coordinates of the of the top-left pixel of the rectangle
 *         w,h -- the width and height, respectively, of the rectangle
 * OUTPUTS: draws a pixel to every point of the edges of the rectangle
 * RETURN VALUE: 0 if any of the pixels drawn are out of bounds, otherwise 1
 * SIDE EFFECTS: none
 */

int32_t
draw_rect(int32_t x, int32_t y, int32_t w, int32_t h){
	/* Your code goes here! */
	if(w <= 0 || h < 0)  return 0;		// special case

	int32_t flag = 1;
	flag = draw_line(x, y, x + w, y) && flag;
	flag = draw_line(x, y, x, y + h) && flag;
	flag = draw_line(x + w, y, x + w, y + h) && flag;
	flag = draw_line(x, y + h, x + w, y + h) && flag;	// draw 4 edges
	if(flag)  return 1;
	return 0;
}


/* 
 *  draw_triangle
 *	 
 *	 
 *	
 *	
 * INPUTS: x_A,y_A -- the coordinates of one of the vertices of the triangle
 *         x_B,y_B -- the coordinates of another of the vertices of the triangle
 *         x_C,y_C -- the coordinates of the final of the vertices of the triangle
 * OUTPUTS: draws a pixel to every point of the edges of the triangle
 * RETURN VALUE: 0 if any of the pixels drawn are out of bounds, otherwise 1
 * SIDE EFFECTS: none
 */

int32_t
draw_triangle(int32_t x_A, int32_t y_A, int32_t x_B, int32_t y_B, int32_t x_C, int32_t y_C){
	/* Your code goes here! */

	int32_t flag = 1;
	flag = draw_line(x_A, y_A, x_B, y_B) && flag;
	flag = draw_line(x_A, y_A, x_C, y_C) && flag;
	flag = draw_line(x_B, y_B, x_C, y_C) && flag;	// draw 3 edges
	if(flag)  return 1;
	return 0;
}

/* 
 *  draw_parallelogram
 *	 
 *	 
 *	
 *	
 * INPUTS: x_A,y_A -- the coordinates of one of the vertices of the parallelogram
 *         x_B,y_B -- the coordinates of another of the vertices of the parallelogram
 *         x_C,y_C -- the coordinates of another of the vertices of the parallelogram
 * OUTPUTS: draws a pixel to every point of the edges of the parallelogram
 * RETURN VALUE: 0 if any of the pixels drawn are out of bounds, otherwise 1
 * SIDE EFFECTS: none
 */

int32_t
draw_parallelogram(int32_t x_A, int32_t y_A, int32_t x_B, int32_t y_B, int32_t x_C, int32_t y_C){
	/* Your code goes here! */
	int32_t flag = 1, dx = x_C - x_B, dy = y_C - y_B;
	int32_t x_D = x_A + dx, y_D = y_A + dy;	// get the fourth point
	flag = draw_line(x_A, y_A, x_B, y_B) && flag;
	flag = draw_line(x_A, y_A, x_D, y_D) && flag;
	flag = draw_line(x_B, y_B, x_C, y_C) && flag;
	flag = draw_line(x_C, y_C, x_D, y_D) && flag;	// draw 4 edges
	if(flag)  return 1;
	return 0;
}


/* 
 *  draw_circle
 *	 
 *	 
 *	
 *	
 * INPUTS: x,y -- the center of the circle
 *         inner_r,outer_r -- the inner and outer radius of the circle
 * OUTPUTS: draws a pixel to every point whose distance from the center is
 * 	    greater than or equal to inner_r and less than or equal to outer_r
 * RETURN VALUE: 0 if any of the pixels drawn are out of bounds, otherwise 1
 * SIDE EFFECTS: none
 */

int32_t
draw_circle(int32_t x, int32_t y, int32_t inner_r, int32_t outer_r){
	/* Your code goes here!*/
	if(inner_r < 0 || inner_r > outer_r)  return 0; // invalid case
	int32_t flag = 1;	
	// divide the region into 4 parts to fill the pixels
	// the most outside pixel is <= outer_r, inner is >= inner_r / 2
	for(int32_t xi = x - outer_r; xi <= x + outer_r; xi++)
		for(int32_t yi = y - outer_r; yi <= y - (inner_r / 2); yi++){
			int32_t dis = (xi - x) * (xi - x) + (yi - y) * (yi - y);
			if(dis >= inner_r * inner_r && dis <= outer_r * outer_r)
				flag = draw_dot(xi, yi) && flag;
		}
	for(int32_t xi = x - outer_r; xi <= x + outer_r; xi++)
		for(int32_t yi = y + (inner_r / 2); yi <= y + outer_r; yi++){
			int32_t dis = (xi - x) * (xi - x) + (yi - y) * (yi - y);
			if(dis >= inner_r * inner_r && dis <= outer_r * outer_r)
				flag = draw_dot(xi, yi) && flag;
		}
	for(int32_t xi = x - outer_r; xi <= (x - inner_r / 2); xi++)
		for(int32_t yi = y - (inner_r / 2); yi <= y + (inner_r / 2); yi++){
			int32_t dis = (xi - x) * (xi - x) + (yi - y) * (yi - y);
			if(dis >= inner_r * inner_r && dis <= outer_r * outer_r)
				flag = draw_dot(xi, yi) && flag;
		}
	for(int32_t xi = (x + inner_r / 2); xi <= x + outer_r; xi++)
		for(int32_t yi = y - (inner_r / 2); yi <= y + (inner_r / 2); yi++){
			int32_t dis = (xi - x) * (xi - x) + (yi - y) * (yi - y);
			if(dis >= inner_r * inner_r && dis <= outer_r * outer_r)
				flag = draw_dot(xi, yi) && flag;
		}
	if(flag)  return 1;
	return 0;
}


/* 
 *  rect_gradient
 *	 
 *	 
 *	
 *	
 * INPUTS: x,y -- the coordinates of the of the top-left pixel of the rectangle
 *         w,h -- the width and height, respectively, of the rectangle
 *         start_color -- the color of the far left side of the rectangle
 *         end_color -- the color of the far right side of the rectangle
 * OUTPUTS: fills every pixel within the bounds of the rectangle with a color
 *	    based on its position within the rectangle and the difference in
 *          color between start_color and end_color
 * RETURN VALUE: 0 if any of the pixels drawn are out of bounds, otherwise 1
 * SIDE EFFECTS: none
 */

int32_t
rect_gradient(int32_t x, int32_t y, int32_t w, int32_t h, int32_t start_color, int32_t end_color){
	/* Your code goes here! */
	if(h < 0 || w < 1)  return 0;	// invalid

	// extract each level of colors by bit operations
	int32_t level_r1 = ((start_color & 0x00FF0000) >> 16), level_r2 = ((end_color & 0x00FF0000) >> 16);
	int32_t level_g1 = ((start_color & 0x0000FF00) >> 8), level_g2 = ((end_color & 0x0000FF00) >> 8);
	int32_t level_b1 = (start_color & 0x000000FF), level_b2 = (end_color & 0x000000FF);
	int32_t dr = level_r2 - level_r1, dg = level_g2 - level_g1, db = level_b2 - level_b1;
	int32_t sgnr = 0, sgng = 0, sgnb = 0;
	if(dr > 0)  sgnr = 1;
	if(dr < 0)  sgnr = -1;
	if(dg > 0)  sgng = 1;
	if(dg < 0)  sgng = -1;
	if(db > 0)  sgnb = 1;
	if(db < 0)  sgnb = -1;	//get difference and sgn

	int32_t flag = 1;
	for(int32_t xi = x; xi <= x + w; xi++){
		int32_t level_r = (2 * (xi - x) * dr + sgnr * w) / (2 * w) + level_r1;
		int32_t level_g = (2 * (xi - x) * dg + sgng * w) / (2 * w) + level_g1;
		int32_t level_b = (2 * (xi - x) * db + sgnb * w) / (2 * w) + level_b1;
		int32_t color = (level_r << 16) + (level_g << 8) + level_b;
		// rebuild the color use 3 levels
		set_color(color);
		for(int32_t yi = y; yi <= y + h; yi++)
			flag = draw_dot(xi, yi) && flag;	//fill each pixel
	}
	if(flag)  return 1;	
	return 0;
}


/* 
 *  draw_picture
 *	 
 *	 
 *	
 *	
 * INPUTS: none
 * OUTPUTS: alters the image by calling any of the other functions in the file
 * RETURN VALUE: 0 if any of the pixels drawn are out of bounds, otherwise 1
 * SIDE EFFECTS: none
 */

/*
	This function draws ZJU-INTL "kuaitao" (A simple picture)
	Since I do not spend enough time on mp5, I draw a simple and ugly picture...
*/
int32_t
draw_picture(){
	/* Your code goes here! */
	
	int32_t flag = 1;

	

	flag = rect_gradient(205-160, 160-160, 160*2, 160*2, 0x0000FFFF, 0x00FFFF00);
	set_color(255);
	flag = draw_circle(205, 160, 150, 160) && flag;//fill the background

	set_color(0x00FF0000);
	//draw the English characters
	flag = draw_line(100, 60, 120, 60) && flag;
	flag = draw_line(130, 60, 150, 60) && flag;
	flag = draw_line(100, 130, 120, 130) && flag;
	flag = draw_line(130, 130, 140, 130) && flag;
	flag = draw_line(120, 60, 100, 130) && flag;
	flag = draw_line(140, 60, 140, 130) && flag;

	flag = draw_line(160, 60, 160, 130) && flag;
	flag = draw_line(160, 130, 180, 130) && flag;
	flag = draw_line(180, 60, 180, 130) && flag;

	flag = draw_line(190, 95, 210, 95) && flag;
	flag = draw_line(220, 60, 240, 60) && flag;
	flag = draw_line(220, 130, 240, 130) && flag;
	flag = draw_line(230, 60, 230, 130) && flag;

	flag = draw_line(250, 60, 250, 130) && flag;
	flag = draw_line(270, 60, 270, 130) && flag;
	flag = draw_line(250, 60, 270, 130) && flag;

	flag = draw_line(280, 60, 300, 60) && flag;
	flag = draw_line(290, 60, 290, 130) && flag;

	flag = draw_line(310, 60, 310, 130) && flag;
	flag = draw_line(310, 130, 330, 130) && flag;

	set_color(0);
	//draw Chinese characters
	flag = draw_line(120, 150, 120, 250) && flag;
	flag = draw_line(110, 180, 100, 210) && flag;
	flag = draw_line(130, 180, 140, 210) && flag;


	flag = draw_line(140, 180, 170, 180) && flag;
	flag = draw_line(140, 200, 180, 200) && flag;
	flag = draw_line(160, 150, 160, 250) && flag;
	flag = draw_line(170, 180, 170, 200) && flag;

	flag = draw_line(160, 200, 180, 250) && flag;

	flag = draw_line(200, 180, 230, 180) && flag;
	flag = draw_line(200, 220, 230, 220) && flag;
	flag = draw_line(200, 250, 300, 250) && flag;
	flag = draw_line(230, 180, 200, 220) && flag;
	flag = draw_line(230, 220, 200, 250) && flag;
	flag = draw_line(210, 150, 220, 160) && flag;

	flag = draw_line(250, 150, 250, 230) && flag;
	flag = draw_line(270, 150, 270, 230) && flag;
	flag = draw_line(270, 230, 290, 230) && flag;

	flag = draw_line(240, 180, 250, 200) && flag;
	flag = draw_line(240, 210, 250, 210) && flag;
	flag = draw_line(270, 200, 280, 180) && flag;
	flag = draw_line(270, 210, 280, 210) && flag;
	//all only use lines to write character, simple ways
	if(flag)  return 1;


	return 0;
}
