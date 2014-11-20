/* File: Plotstream.cxx
 *
 * Implementation class Plotstream
 * A plotstream opens a window displays a data plot, then closes
 * the window.
 *
 * left-clicking the mouse in the window displays the current plot coordinates of
 * the cursor. 
 *
 * This file hides all the fiddly bits about plotting a function.
 *
 * Author: 	jlk
 * Version:	1.2
 * Date:	January 2006
 *
 * This file is in the public domain and can be used without
 * any restriction.
 */
#include <ostream>
#include <iomanip>
#include <string>
#include <cfloat>

#include "Plotstream.h"
#include <stdexcept>


namespace Plot {


// Constants used exclusively in this file

static const int border_height	= 20;
static const int border_width	= 20;
static const int left_border	= 70;
static const int mark_length	= 4;
static const int max_height		= 600;
static const int min_height 	= 200;
static const Color graph_color    = GREEN; // Default only, can be changed
static const Color point_color    = graph_color; // will be XORed
static const Color marker_color	= CYAN; // Will give red in XOR mode over white
static const float chouia		= 0.0001; // an invisible difference (in drawing terms)
static const double epsilon		= DBL_EPSILON * 1000; // Extremely small difference
static const bool ERASE			= true;

// Mouse events variables used exclusively in this file
static bool left_clicked = false;
static int cursorX;
static int cursorY;


// Local function hidden at file scope
// dtoa safely converts a double to the equivalent char *
// It is the responsibility of the calling program to delete the returned string
static char * dtoa(double val)
{
	std::ostringstream ostr;
	ostr.precision(3);
	ostr << val;
	
	char * ret = new char[ostr.str().size() + 1];
	strcpy(ret, ostr.str().c_str());
	return ret;
}


// Local function hidden at file scope
// pow10l() does not exist in mingw (redefine as inline wrapper here)
static inline double pow10l(double x)
{
	return powl(10.0, x);
}

/* Local function hidden at file scope
 * Rounds double value by discarding very small decimals.
 * (necessary to deal with previous small floating point errors)
 * @param val 		value to round
 * @param sigDigits number of significant digits.
 * @param intrep    all significant digits as a long int 
 * @return			truncated value as a double (may be inexact)
 */
static double floatRound(double val, int & sigDigits, long int &intrep)
{ 
	bool neg = false;
	int decPos, i, j;
	int leading0s = 0;
	std::ostringstream ostr;
	double res;
	
	ostr.setf(std::ios::floatfield);
	ostr.unsetf(std::ios::showpoint);
	ostr << val;
	
	std::string str = ostr.str();
	// Remove negative sign if any
	if (str[0] == '-')
	{
		neg = true;
		str = str.substr(1);
	}
	   
	// find position of dot if any
	decPos = str.find('.');
	if(unsigned(decPos) == std::string::npos) // then it's an integer, return now
	{
		sigDigits = str.size();
		intrep = static_cast<long int>(val);
		return val;
	}
			
	// find the 1st non 0 digit
	for(i = 0; unsigned(i) < str.size() && (str[i] == '0' || str[i] == '.'); i++);	  
	// cut the string there
	if (i > 0)
	{
		str = str.substr(i);
		leading0s = i - 1;
	}
		
	// remove decimal point if still there
	i = str.find('.');
	if (unsigned(i) != std::string::npos)
	{
		std::string head = str.substr(0, i);
		str = head.append(str.substr(i + 1));
	}
	
	// find first incidence of "000" or "999" if any in string
	i = str.find("000");
	j = str.find("999");
	// if i < j, then truncate at that point
	if (unsigned(i) != std::string::npos)
	{
		if ( unsigned(j) == std::string::npos || i < j)
			str = str.substr(0, i);
	}
	// else round the number: Insert '.' before the 1st 9
	// then round up to an integer
	else if (unsigned(j) != std::string::npos)
	{
		double roundnum;
		
		str = str.insert(j, 1, '.');
		std::istringstream istr(str);
		istr >> roundnum;
		roundnum = round(roundnum);
		
		ostr.str(""); // Empty the ostringstream
		str = ostr.str();
		ostr << int(roundnum); // insert number as integer
		str = ostr.str();
		
		if (j == 0) // the decimal point has moved bcos of rounding up
			decPos++;
	}

	while((unsigned)decPos > str.size()) // The rounding up occurred to the
		str += '0'; 		   // left of the decimal point. Add 0s
			
	sigDigits = str.size();
	{
		
		std::istringstream istr(str);
		istr >> intrep;
	}
	
	// re-insert the leading 0s, decimal point & sign
	if(leading0s > 0)
		str.insert(0, leading0s, '0');
	   str.insert(decPos, 1, '.');
	if(neg)
		str.insert(0, 1, '-');
	std::istringstream istr(str);
	
	istr >> res;

	return res;
}	 

/* Local function hidden at file scope
 * Obtains a practical y axis range with "round" numbers at both ends
 * Params hold the min and max values in plot data on entry and
 * pass back the practical values on output.
 */
static void getNearest( double & y_min, double & y_max)
{
	int sigdigits; // Holds number of significant digits
	long int intrep; // the integer representation

	double min = y_min, max = y_max, dif;
	
	min = floatRound(min, sigdigits, intrep);
	max = floatRound(max, sigdigits, intrep);
	
	dif = max - min - (y_max - y_min);
	
	y_max = floatRound(y_max + dif / 2, sigdigits, intrep);
	
	y_min = floatRound(y_min - dif / 2, sigdigits, intrep);
}

/*
 * Local function hidden at file scope
 * Attempt to guess a convenient divisor for grid along the X axis 
 * If 0 is in the range, it should in many cases be on the grid.
 * Otherwise, divide the x range by 10, or 6, or 7, or 5, or 8, or 3,
 * or 4, in this order if divisible, by 10 otherwise.
 */
static int getXDivisor(double lo, double hi, int plotWidth)
{
	int sigdigits, pquo, div = 0;
	long int intVal;
    double delta = (hi - lo) / plotWidth; // Ignore 1 pixel inaccuracies
    double range = hi - lo;

	floatRound(range, sigdigits, intVal);

	// If 0 is on the axis attempt to place it on the grid
	if(lo < 0 && hi > 0) // 0 is part of the range
	{
		if(fabsl(lo) < hi)
			div = int(round(hi / fabsl(lo)));
		else
			div = int(round(fabs(lo) / hi));

		if (div > 12){
			div = 12;
		}

		if (div > 0 && div <= 9)
        {
			div += 1; 
            
            // If too small number of divisions, try multiples
            if(div == 2) // See if divisible by 12, 10, 6, 8, or 4
            {            
        		if (remquol(intVal, 12, & pquo) < delta)
        			div = 12;
        	   	else if (remquol(intVal, 10, & pquo) < delta)
        			div = 10;
        		else if (remquol(intVal, 6, & pquo) < delta)
        			div = 6;
                else if (remquol(intVal, 8, & pquo) < delta)
        			div = 8;
        		else if (remquol(intVal, 4, & pquo) < delta)
        			div = 4;
            }
            else if (div == 3) // See if divisible by 12, 9 or 6
            {
        		if (remquol(intVal, 12, & pquo) < delta)
        			div = 12;
        	   	else if (remquol(intVal, 9, & pquo) < delta)
        			div = 9;
        		else if (remquol(intVal, 6, & pquo) < delta)
        			div = 6;
            }
            return div;
        }             	          	     	    	
	}
	
	// If not, then attempt a reasonable division of the range.
	// Try dividing by  12, or 10, or 5, or 6, or 8,  or 3, or 4	
	else
	{
		if (remquol(intVal, 12, & pquo) < delta)
			div = 12;
	   	else if (remquol(intVal, 10, & pquo) < delta)
			div = 10;
		else if (remquol(intVal, 7, & pquo) < delta)
			div = 7;
    	else if (remquol(intVal, 6, & pquo) < delta)
			div = 6;
	    else if (remquol(intVal, 5, & pquo) < delta)
			div = 5;
		else if (remquol(intVal, 8, & pquo) < delta)
			div = 8;
		else if (remquol(intVal, 3, & pquo) < delta)
			div = 3;
		else if (remquol(intVal, 4, & pquo) < delta)
			div = 4;
		else // We are running out of common divisors
			div = 10;
	}
	return div;
}

/*
 * Local function hidden at file scope
 * Attempt to guess a convenient divisor for grid along the Y axis 
 * If 0 is in the range, it should in many cases be on the grid.
 * Otherwise, divide the y range by 5 or 3 or 4 in this order if
 * divisible, by 5 otherwise.
 */
static int getYDivisor(double lo, double hi, int plotHeight)
{
	int sigdigits, pquo, div = 0;
	long int intVal;
    double delta = (hi - lo) / plotHeight; // Ignore 1 pixel inaccuracies

	// If 0 is on the axis attempt to place it on the grid
	if(lo < 0 && hi > 0) // 0 is part of the range
	{
		if(fabsl(lo) < hi)
			div = int(round(hi / fabsl(lo)));
		else
			div = int(round(fabs(lo) / hi));
			
		if (div > 10) div = 10;

		if (div > 0 && div < 6)
        {
			div += 1; 
            
            // If too small number of divisions, try multiples
            if(div == 2) // See if divisible by 6, 4
            {            
        		if (remquol(intVal, 6, & pquo) < delta)
        			div = 6;
        		else if (remquol(intVal, 4, & pquo) < delta)
        			div = 4;
             }
            else if (div == 3) // See if divisible 6
            {
        		if (remquol(intVal, 6, & pquo) < delta)
        			div = 6;
            }
            return div;
        }             	          	     	    	
	}

	
	// if not, then attempt a reasonable division of the range 
	double range = hi - lo;
	floatRound(range, sigdigits, intVal);
	   
	// If 0 is in the middle try dividing by 4	  
	if (div == 1)
	{
		if (remquol(intVal, 4, & pquo) < delta)
			div = 4;
		else // We are running out of common divisors
			div = 2;
	}
	// else try dividing by  3, or 4, or 5, or 2	
	else
	{
		if (remquol(intVal, 5, & pquo) < delta)
			div = 5;
	   	if (remquol(intVal, 3, & pquo) < delta)
			div = 3;
		else if (remquol(intVal, 4, & pquo) < delta)
			div = 4;
		else if (remquol(intVal, 2, & pquo) < delta)
			div = 2;
	   	else // We are running out of common divisors
			div = 5;
	}
	return div;
}



	
/************************* CLASS FUNCTIONS ***************************/

Plotstream::Plotstream(int width, int height)
: winWidth(width), winHeight(height), plotStarted(false)
{
	bgColor = Color(255,255,255);

	marked = false;
    colour = lastColour = graph_color;

    color2Code(GREEN);

 // Reset file-scope var... don't like this, but necessary for event handling	
	left_clicked = false;
}


Canvas Plotstream::plot(const Plotdata & x, const Plotdata & y, Color color)
{
    colour = color;
	// Need 3 points minimum to do a plot
	if(x.size() < 3){
		throw std::runtime_error("not enough plotting points");
	}
	
	// Need as many y values as x values to do a plot	 
	if(x.size() > y.size()){
		throw std::runtime_error("plotting size missmatch");
	}
		


	// Store the hi and lo points of the axes
    Plotdata::min(x, y, lo_x, lo_y);
    Plotdata::max(x, y, hi_x, hi_y);
    // Set Y values to the nearest "round" numbers
	getNearest(lo_y, hi_y);
	
	x_range = hi_x - lo_x;
	y_range = hi_y - lo_y;
	
	double x2y = x_range / (hi_y - lo_y);
	plotWidth = winWidth - border_width - left_border;
	plotHeight = winHeight - 2 *  border_height;
	
	// if window height is not specified, adjust as proportion of axes data
	if(winHeight == 0)
	{
		// Start with plot height based on the same scale as x axis.
		plotHeight = int(plotWidth / x2y * 3 / 4);
		
		// Coerce the plot-height within practical limits
		if(plotHeight > max_height)
			plotHeight = max_height;
		else if(plotHeight < min_height)
			plotHeight = min_height;
			
		winHeight = plotHeight + 2 * border_height;
	}
	x_scale = x_range / plotWidth;
	y_scale = y_range / plotHeight;



	canvas = Canvas(cv::Size(winWidth, winHeight), cv::Vec3b(255,255,255));


	cd.canvas = canvas;


	/* Set background colour */
	//setbkcolor(WHITE); // or COLOR(248,248,248) if a light grey is preferred
	//cleardevice();

	/* Show empty white page while we draw in the background */	       
	//synchronize();
		      	   
	drawAxes();
	

	/* Draw curve */
	drawFunc(x.getData(), y.getData());
	
	//std::cout << "final canvas " << canvas.cols << ":" << canvas.rows << std::endl;

    /* Now bring plot to foreground */
   return canvas;
	//cv::imshow(winTitle, canvas);
   //cv::waitKey(0);
}

/* Convert graph x value to screen coordinate */
int Plotstream::X(double x) const
{
	return int((x - lo_x) / x_scale + left_border);
}

/* Convert graph y value to screen coordinate */
int Plotstream::Y(double y) const
{
	return int(winHeight - border_height - (y - lo_y) / y_scale);
}

/* Convert screen coordinate to graph x value */
double Plotstream::plotX(int screenX) const
{
	return (screenX - left_border) * x_scale + lo_x;
}

/* Convert screen coordinate to graph y value */
double Plotstream::plotY(int screenY) const
{
	return (winHeight - border_height - screenY) * y_scale + lo_y;
}

/* @return  true if given location is within x and y axes ranges */
bool Plotstream::withinRange( double x, double y) const
{
	return	x + x_scale >= lo_x && x - x_scale <= hi_x
		&&
			y + y_scale >= lo_y && y - y_scale <= hi_y;
}

/* Round(est) double within one pixel in X 
 * 
 * @param theVal the value to be rounded, passed back rounded to caller
 * @param direction the requested direction of rounding UP, DOWN or ANY
 * @return the direction the rounding took place.
 */
Rounding Plotstream::nearRoundX(double & val, Rounding direction) const
{
	int valSigdigits;
	int upSigdigits;
	int downSigdigits;
	long int intrep;
	double theVal, upVal, downVal;
	
	theVal = floatRound(val, valSigdigits, intrep);
	
	if (direction == UP) // If a higher or equal value has been requested
	{
		upVal = floatRound(val + x_scale, upSigdigits, intrep);
		if (theVal < 0 && upVal > 0) // Takes care of 0
		{
			val = 0;
			return UP;
		}
		else if (fabs(trunc(theVal)) < fabs(trunc(upVal))) // takes care of integers
		{
			val = trunc(upVal);
			return UP;
		}
		else if (valSigdigits > upSigdigits)
		{
			val = upVal;
			return UP;
		}
		else
			return ANY;
	}
	else if (direction == DOWN) // If lower or equal value is requested
	{
		downVal = floatRound(val - x_scale, downSigdigits, intrep);
		if (theVal > 0 && downVal < 0) // Takes care of 0
		{
			val = 0;
			return ANY;
		}
		else if (fabs(trunc(theVal)) > fabs(trunc(downVal))) // takes care of integers
		{
			val = trunc(theVal);
			return ANY;
		}
	    else if (valSigdigits > downSigdigits)
		{
			val = downVal;
			return DOWN;
		}
		else
			return ANY;
	}
	else // May round in any direction
	{
		upVal = floatRound(val + x_scale, upSigdigits, intrep);
		downVal = floatRound(val - x_scale, downSigdigits, intrep);
		if (theVal < 0 && upVal > 0)  // Takes care of 0
		{
			val = 0;
			return ANY;
		}
		else if (theVal > 0 && downVal < 0) // Takes care of 0
		{
			val = 0;
			return ANY;
		}
	   	else if (fabs(trunc(theVal)) < fabs(trunc(upVal)))
		{
			val = trunc(upVal);
			return UP;
		}
		else if (fabs(trunc(theVal)) > fabs(trunc(downVal))) // takes care of integers
		{
			val = trunc(theVal);
			return ANY;
		}
		
		int minimum = std::min(valSigdigits, upSigdigits);
		minimum = std::min (minimum, downSigdigits);
		
		if(minimum ==  valSigdigits)
		{
			val = theVal;
			return ANY;
		}
		else if (minimum == upSigdigits)
		{
			val = upVal;
			return UP;
		}
		else
		{
			val = downVal;
			return DOWN;
		}
	}	    	      
}

/* Round(est) double within one pixel in Y */
Rounding Plotstream::nearRoundY(double & val, Rounding direction) const
{
	int valSigdigits;
	int upSigdigits;
	int downSigdigits;
	long int intrep;
	double theVal, upVal, downVal;
	
	theVal = floatRound(val, valSigdigits, intrep);
	
	if (direction == UP) // If a higher or equal value has been requested
	{
		upVal = floatRound(val + y_scale, upSigdigits, intrep);
		if (theVal < 0 && upVal > 0) // Takes care of 0
		{
			val = 0;
			return ANY;
		}
		else if (fabs(trunc(theVal)) < fabs(trunc(upVal))) // takes care of integers
		{
			val = trunc(upVal);
			return UP;
		}
		else if (valSigdigits > upSigdigits)
		{
			val = upVal;
			return UP;
		}
		else
			return ANY;
	}
	else if (direction == DOWN) // If lower or equal value is requested
	{
		downVal = floatRound(val - y_scale, downSigdigits, intrep);
		if (theVal > 0 && downVal < 0) // Takes care of 0
		{
			val = 0;
			return DOWN;
		}
		else if (fabs(trunc(theVal)) > fabs(trunc(downVal))) // takes care of integers
		{
			val = trunc(theVal);
			return ANY;
		}
	    else if (valSigdigits > downSigdigits)
		{
			val = downVal;
			return DOWN;
		}
		else
			return ANY;
	}
	else // May round in any direction
	{
		upVal = floatRound(val + y_scale, upSigdigits, intrep);
		downVal = floatRound(val - y_scale, downSigdigits, intrep);
		if (theVal < 0 && upVal > 0)  // Takes care of 0
		{
			val = 0;
			return ANY;
		}
		if (theVal > 0 && downVal < 0) // Takes care of 0
		{
			val = 0;
			return DOWN;
		}
	   	   else if (fabs(trunc(theVal)) < fabs(trunc(upVal)))
		{
			val = trunc(upVal);
			return UP;
		}
		else if (fabs(trunc(theVal)) > fabs(trunc(downVal))) // takes care of integers
		{
			val = trunc(theVal);
			return ANY;
		}
		
		int minimum = std::min(valSigdigits, upSigdigits);
		minimum = std::min (minimum, downSigdigits);
		
		if(minimum ==  valSigdigits)
		{
			val = theVal;
			return ANY;
		}
		else if (minimum == upSigdigits)
		{
			val = upVal;
			return UP;
		}
		else
		{
			val = downVal;
			return DOWN;
		}
	}	    
}

/* Draw the axes */
void Plotstream::drawAxes()
{
	int xDivs;				// number of x divisions
	int yDivs;				// number of y divisions
	int divLength;	 	 	// length (in pixels) of a division
	int sigdigits;
	long int intVal;
	      	   


	// draw the rectangle
	cd.setcolor(DARKGRAY);
	cd.rectangle(left_border - 1,   // -1 to fix small discrepancy on screen
				border_height - 1, // Probably due to line width.  
				left_border  + plotWidth,
				border_height + plotHeight);


	// Attempt to guess a reasonable number of grid divisions for x and y
	xDivs = getXDivisor(lo_x, hi_x, plotWidth);
	// If y axis is large, divide in the same manner as x axis
	if(winHeight > winWidth * 3 / 5.0){
		yDivs = getXDivisor(lo_y, hi_y, plotHeight);
	} else {
		yDivs = getYDivisor(lo_y, hi_y, plotHeight);
	}



	// draw the grid
	//setlinestyle(USERBIT_LINE,0x0303 , NORM_WIDTH);
	cd.setcolor(LIGHTGRAY);


	// Horizontal grid
	for (int i = yDivs - 1; i > 0; i--)
	{

    	cd.moveto(left_border, border_height + plotHeight * i / yDivs);
    	cd.lineto(winWidth - border_width, border_height + plotHeight * i / yDivs);
	}


	// Vertical grid
	for (int i = xDivs - 1; i > 0; i--)
	{
		cd.moveto(left_border + 1 + plotWidth * i / xDivs, border_height);
		cd.lineto(left_border + 1 + plotWidth * i / xDivs, winHeight - border_height);
	}


	// Draw Axes markers
	//setlinestyle(SOLID_LINE, 0, NORM_WIDTH);
	cd.setcolor(DARKGRAY);

	// Y axis
	for (int i = yDivs - 1; i > 0; i--)
	{

		cd.moveto(left_border, border_height + plotHeight * i / yDivs);
		cd.linerel(mark_length, 0);

		cd.moveto(winWidth - border_width, border_height + plotHeight * i / yDivs);
		cd.linerel(-mark_length, 0);
	}

	// X axis
	for (int i = xDivs - 1; i > 0; i--)
	{
		cd.moveto(left_border + 1 + plotWidth * i / xDivs, winHeight - border_height);
		cd.linerel(0, -mark_length);

		cd.moveto(left_border + 1 + plotWidth * i / xDivs, border_height);
		cd.linerel(0, mark_length);
	}



	// Number the axes
	cd.setcolor(BLACK);

	// Y axis
	//settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 0);
	//setusercharsize(1, 2, 1, 2);
	//settextjustify(RIGHT_TEXT, CENTER_TEXT);
	divLength = int(plotHeight / yDivs);
	int xpos = left_border - 3;
	int ypos = border_height;
	double divVal = floatRound((lo_y - hi_y) / yDivs, sigdigits, intVal);
	
	for (int i = 0;  i <= yDivs; i++)
	{
		double val = floatRound(hi_y + divVal * i, sigdigits, intVal);
		if(fabs(val) < chouia * (hi_y - lo_y))
			val = 0;
		char * asciival = dtoa(val);
		cd.outtextxy(xpos, ypos + divLength * i, asciival);
		delete [] asciival;
	}


	// X axis
	//settextjustify(CENTER_TEXT, CENTER_TEXT);
	ypos = winHeight - border_height/2 + 5;
	xpos = left_border - 3;
	divVal = floatRound((hi_x - lo_x) / xDivs, sigdigits, intVal);
	for (int i = 0;  i <= xDivs; i++)
	{
		double val = floatRound(lo_x + divVal * i, sigdigits, intVal);
		if(fabs(val) < chouia * (hi_x - lo_x))
			val = 0;
		char * asciival = dtoa(val);
		cd.outtextxy(xpos + plotWidth * i / xDivs, ypos, asciival);
		delete [] asciival;
	}
}

void Plotstream::drawFunc(const Plotdata & x, const Plotdata & y)
{
	std::vector<double>::const_iterator x_it  = x.getData().begin();
	std::vector<double>::const_iterator y_it  = y.getData().begin();
	std::vector<double>::const_iterator x_end = x.getData().end();

	cd.setcolor(colour);
	   
	if(std::isfinite(*y_it) && std::isfinite(*x_it))
	{
	   	cd.moveto(X(*x_it), Y(*y_it));
		plotStarted = true;
	}
    // Else if both x and y are NAN, then it could be either a color change
    // request or a single point drawing request.
    else
    {

         if(!std::isfinite(*y_it) && !std::isfinite(*x_it))
            if(x_it + 1 != x_end && x_it + 2 != x_end)
                handleCommand(x_it, y_it);
    }        
	
	while( ++x_it != x_end)
	{
		if(std::isfinite(*(++y_it)) && std::isfinite(*x_it))
		{
			if(plotStarted)
				cd.lineto(X(*x_it), Y(*y_it), 2);
	   	   	else 
			{
	   	   		cd.moveto(X(*x_it), Y(*y_it));
				plotStarted = true;
			}
		}
        else
        {
  	  	  	  plotStarted = false;

            // If both x and y are NAN, then it could be either a colour
            // change request or a single point drawing request.
             if(!std::isfinite(*y_it) && !std::isfinite(*x_it))
                if(x_it + 1 != x_end && x_it + 2 != x_end)
                    handleCommand(x_it, y_it);
        }
    }
}


// Ancillary function used locally by drawMarker()
// draws the marker shape in X and Y.
static inline void drawMarkShape(CanvasDraw & cd, int x, int y)
{
	// Horz
	cd.moveto(x - 4, y); 		cd.lineto(x + 4, y); 		// centre line
	cd.moveto(x - 5, y - 1);	cd.lineto(x - 3, y - 1);
	cd.moveto(x - 5, y + 1);	cd.lineto(x - 3, y + 1);	// side lines
	cd.moveto(x + 5, y - 1);	cd.lineto(x + 3, y - 1);
	cd.moveto(x + 5, y + 1);	cd.lineto(x + 3, y + 1);
	// Vert
	cd.moveto(x	, y - 4); 		cd.lineto(x	, y + 4);	// centre line
	cd.moveto(x - 1, y - 5);	cd.lineto(x - 1, y - 3);
	cd.moveto(x + 1, y - 5);	cd.lineto(x + 1, y - 3);	// side lines
	cd.moveto(x - 1, y + 5);	cd.lineto(x - 1, y + 3);
	cd.moveto(x + 1, y + 5);	cd.lineto(x + 1, y + 3);
}	    
		
// Draw a marker at the current cursor position
// Will erase only if erase is true
void Plotstream::drawMarker(bool erase)
{
	/* select XOR drawing mode and marker colour */
    //setwritemode(XOR_PUT);
    setFgColor(marker_color);

   	if (marked) // then erase existing marker
	{
		drawMarkShape(cd, lastX, lastY);
		marked = false;
    }
   
    if(!erase)
	{
	    drawMarkShape(cd, cursorX, cursorY);
		lastX = cursorX;
		lastY = cursorY;
	    marked = true;
	   }
	
	/* Restore drawing mode */
    //setwritemode(COPY_PUT);
    resetFgColor();
}

// Ancillary function used locally by drawSinglePoint()
// draws the point shape in X and Y.
static void drawPointShape(CanvasDraw & cd, int x, int y)
{
	// Horz
	cd.moveto(x - 1, y - 2); 	cd.lineto(x + 1, y - 2);   // top line
	cd.moveto(x - 1, y + 2);	cd.lineto(x + 1, y + 2);	// bottom line
	// Vert
	cd.moveto(x - 2, y - 1); 	cd.lineto(x - 2, y + 1);	// left line
	cd.moveto(x + 2, y - 1);	cd.lineto(x + 2, y + 1);	// right line
}

// Draw a single point at the given coordinates
void Plotstream::drawSinglePoint(double xCoord, double yCoord)
{
    drawPointShape(cd, X(xCoord) + 1, Y(yCoord));
}

// Set new foreground colour, remember last colour
void Plotstream::setFgColor(Color fgColour)
{
    lastColour = colour;
    colour = fgColour;
    cd.setcolor(fgColour);
}

// Reset foreground colour to last used colour
void Plotstream::resetFgColor(void)
{
    setFgColor(lastColour);
}

/*
 * Check whether the stream holds a request at this point.
 * and handle the request if it does.
 * A request can be a colour change, or a point marker.
 * 
 * Pre-condition:   The data vectors hold at least the 3 data points
 *                  in x and y necessary to hold a command request.
 * Post-condition:  On exit the iterators are unchanged if the data is not
 *                  a known command. They will point to the last data point
 *                  in the request sequence otherwise.
 */
void Plotstream::handleCommand( dataIterator &x_it, dataIterator &y_it)
{
    // Check if command is a colour change
    int newColour = Plotdata::colorChange(x_it, y_it);
    if(Plotdata::isColor(newColour))
    {
        if(newColour == RESETCOLOR)
            setFgColor(lastColour);
        else
            setFgColor(code2Color(newColour));
    }
    // else, draw a single point if requested
    else
    {


        double xCoord, yCoord;
        if(Plotdata::singlePoint(xCoord, yCoord, x_it, y_it))
            drawSinglePoint(xCoord, yCoord);
    }       
}

}
