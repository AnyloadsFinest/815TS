#include "includes.h"

//----------------------------------------------------------------------------
// Caculate graduated weight
// This functions calculates the weight as a float and rounds it to an integer.

//#include <iostream>
//#include <cmath>
/*
// Structure to represent a point (x, y)
struct Point {
    double x;
    double y;
};

// Function to solve a second-degree polynomial using three sample points
void solveSecondDegreePolynomial(const Point& p1, const Point& p2, const Point& p3) {
    double x1 = p1.x, x2 = p2.x, x3 = p3.x;
    double y1 = p1.y, y2 = p2.y, y3 = p3.y;

    // Using the formula for the coefficients of a second-degree polynomial
    double a = ((y3 - y1) * (x2 - x1) - (y2 - y1) * (x3 - x1)) / ((x3 * x3 - x1 * x1) * (x2 - x1) - (x2 * x2 - x1 * x1) * (x3 - x1));
    double b = ((y2 - y1) - a * (x2 * x2 - x1 * x1)) / (x2 - x1);
    double c = y1 - a * x1 * x1 - b * x1;

    // Print the coefficients of the polynomial
    std::cout << "Coefficients of the second-degree polynomial: " << std::endl;
    std::cout << "a = " << a << ", b = " << b << ", c = " << c << std::endl;
}

int main() {
    // Sample points
    const Point p1 = {1.0, 2.0};
    const Point p2 = {2.0, 3.0};
    const Point p3 = {3.0, 5.0};

    // Solve the second-degree polynomial
    solveSecondDegreePolynomial(p1, p2, p3);

    return 0;
}
*/
// CURVE FIITING 
/*
#include <iostream>
#include <vector>
#include <cmath>

// Function to perform polynomial curve fitting using the least squares method
std::vector<double> polynomialCurveFit(const std::vector<double>& xData, const std::vector<double>& yData, int degree) {
    int n = xData.size();
    int m = degree + 1;
    int np = 2 * m - 1;
    int nm = np - 1;

    std::vector<double> a(np, 0.0);
    std::vector<double> b(nm, 0.0);

    std::vector<double> yFit(n, 0.0);

    // Compute the coefficients of the polynomial
    for (int i = 0; i < n; ++i) {
        double temp = yData[i];
        for (int j = 0; j < m; ++j) {
            a[j] += temp;
            temp *= xData[i];
        }
        for (int j = m; j < np; ++j) {
            a[j] += temp;
            temp *= xData[i];
            b[j - m] += temp;
        }
    }

    // Solving the linear system using Gaussian elimination
    for (int i = 0; i < m; ++i) {
        int ij = i + i * np;
        double temp = a[i];
        for (int j = i + 1; j < np; ++j) {
            a[j] -= a[i] * b[j - m] / temp;
        }
        a[ij] /= temp;
        for (int j = i + 1; j < m; ++j) {
            int ik = i + j * np;
            double temp = a[ij];
            for (int k = i + 1; k < np; ++k) {
                a[k + j * np] -= temp * a[k + i * np];
            }
            a[ik] /= temp;
        }
    }

    // Backsubstitution
    for (int i = 0; i < n; ++i) {
        yFit[i] = a[0];
        double xPower = xData[i];
        for (int j = 1; j < np; ++j) {
            yFit[i] += a[j] * xPower;
            xPower *= xData[i];
        }
    }

    return yFit;
}

int main() {
    // Sample data
    std::vector<double> xData = {0.0, 1.0, 2.0, 3.0, 4.0};
    std::vector<double> yData = {1.0, 3.0, 5.0, 4.0, 2.0};

    // Degree of the polynomial to fit
    int degree = 2;

    // Perform curve fitting
    std::vector<double> yFit = polynomialCurveFit(xData, yData, degree);

    // Print the fitted curve
    std::cout << "Fitted curve:" << std::endl;
    for (int i = 0; i < xData.size(); ++i) {
        std::cout << "x = " << xData[i] << ", y = " << yFit[i] << std::endl;
    }

    return 0;
}
*/

//----------------------------------------------------------------------------
void get_weight(void)
{

float ad;
float weight;
long dweight;
float fraction;
long gradweight;
float fpiece;
long dpiece;

ad = SCALE.AD - SCALE.ZERO; // get the ad converter value and subtract from zero
weight= ad * CAL.SPAN;  // calculate scale weight (float)
// 
if(CAL.GFORCE == 1)
             {
            weight = weight * CAL.GFSPAN;  // calculate scale weight (float)
             }      

weight= process_peakhold(weight);  // peak detect task
SCALE.GROSSWEIGHT=weight;
format_weight(); // from the grossweight, do all the unit convertion and weight formatting
// round up float to a graduated weight
// weight value is stripped of it's  sign bit
if(weight < -0.5) // check if value is negative	
 {
	weight=weight * -1;  //take absolute value of weight removing the negative sign bit
 }
dweight=(long) weight;   //typecast as integer strips integer value from float
fraction=weight-dweight; //get the remaining fractional value from float
//fraction now holds the fractional part of the weight
if (fraction> 0.5) dweight++;  //if the fractional value is > 0.5 then round up the integer value
// dweight now holds a rounded integer value of the weight
SCALE.GRADWEIGHT=dweight; // update gradweight
//----------- scale over detection is done here -----------------------------------------------
/*
fpiece = ad * CAL.PIECE_AVG;
if(fpiece < -0.5) // check if value is negative	
 {
	fpiece= 0; //fpiece * -1;  //take absolute value of weight removing the negative sign bit
 }
dpiece=(long) fpiece;   //typecast as integer strips integer value from float
fraction=fpiece-dpiece; //get the remaining fractional value from float
//fraction now holds the fractional part of the weight
if (fraction> 0.5) dpiece++;  //if the fractional value is > 0.5 then round up the integer value
// dweight now holds a rounded integer value of the weight
SCALE.DIS_COUNT=dpiece; // update gradweight
// we have to get the actual displayed weight and graduations
// this is to solve the problem when in kgs, switching to lbs and not detecting scale over properly
*/

gradweight = SCALE.DIS_GROSSWEIGHT / SCALE.DIS_GRAD; // get actual displayed number of graduations 

if (SCALE.GRADWEIGHT > SCALE.SCALEOVER)  // check if scale is over capacity
  {
    SCALE.OVER=1;   // set scale over flag
  }
  else
  {
    SCALE.OVER=0;   // clear scale over flag
  }

} // end of function
  // 
//--------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------
void get_weight_linear(void)
{
float ad;
float weight;
long dweight;
float fraction;
long gradweight;
float fpiece;
long dpiece;

 ad = SCALE.AD - SCALE.ZERO; // get the ad converter value and subtract from zero
 
 weight= ad * CAL.SPAN;  // calculate scale weight (float)
 
 if( SCALE.AD <= CAL.AD3 )               //calibration span for scale
 {
   weight= ad * CAL.SPAN3;  // calculate scale weight (float)
 }
 
  if( SCALE.AD <= CAL.AD2 )               //calibration span for scale
 {
   weight= ad * CAL.SPAN2;  // calculate scale weight (float)
 }
  
  if( SCALE.AD <= CAL.AD1 )               //calibration span for scale
 {
   weight= ad * CAL.SPAN1;  // calculate scale weight (float)
 }     
 
 
if(CAL.GFORCE == 1)
             {
            weight = weight * CAL.GFSPAN;  // calculate scale weight (float)
             }      

weight= process_peakhold(weight);  // peak detect task
SCALE.GROSSWEIGHT=weight;
format_weight(); // from the grossweight, do all the unit convertion and weight formatting
// round up float to a graduated weight
// weight value is stripped of it's  sign bit
if(weight < -0.5) // check if value is negative	
 {
	weight=weight * -1;  //take absolute value of weight removing the negative sign bit
 }
dweight=(long) weight;   //typecast as integer strips integer value from float
fraction=weight-dweight; //get the remaining fractional value from float
//fraction now holds the fractional part of the weight
if (fraction> 0.5) dweight++;  //if the fractional value is > 0.5 then round up the integer value
// dweight now holds a rounded integer value of the weight
SCALE.GRADWEIGHT=dweight; // update gradweight
//----------- scale over detection is done here -----------------------------------------------
/*
fpiece = ad * CAL.PIECE_AVG;
if(fpiece < -0.5) // check if value is negative	
 {
	fpiece= 0; //fpiece * -1;  //take absolute value of weight removing the negative sign bit
 }
dpiece=(long) fpiece;   //typecast as integer strips integer value from float
fraction=fpiece-dpiece; //get the remaining fractional value from float
//fraction now holds the fractional part of the weight
if (fraction> 0.5) dpiece++;  //if the fractional value is > 0.5 then round up the integer value
// dweight now holds a rounded integer value of the weight
SCALE.DIS_COUNT=dpiece; // update gradweight
// we have to get the actual displayed weight and graduations
// this is to solve the problem when in kgs, switching to lbs and not detecting scale over properly
*/

gradweight = SCALE.DIS_GROSSWEIGHT / SCALE.DIS_GRAD; // get actual displayed number of graduations 

if (SCALE.GRADWEIGHT > SCALE.SCALEOVER)  // check if scale is over capacity
  {
    SCALE.OVER=1;   // set scale over flag
  }
  else
  {
    SCALE.OVER=0;   // clear scale over flag
  }

} // end of function

// 
//  
//
//--------------------------------------------------------------------------------------------
void format_weight(void)
{
  
  //SCALE.MULT_GRAMS=0;  // this globle variable is used for a special condition for scale over monitoring for grams
   switch (SCALE.DIS_UNITS)
    {
    case _KG: { // we are displaying kgs

              SCALE.DIS_GROSSWEIGHT = display_kilos(SCALE.GROSSWEIGHT);  // format output for kilograms
              SCALE.DIS_TAREWEIGHT = display_kilos(SCALE.TAREWEIGHT);  // convert to tareweight
              SCALE.DIS_NETWEIGHT  = SCALE.DIS_GROSSWEIGHT - SCALE.DIS_TAREWEIGHT;
              
              if (SCALE.NETMODE) // check if we are in netmode
              {//2         

                SCALE.DIS_WEIGHT=SCALE.DIS_NETWEIGHT;  // displayed weight is final netweight

              }//2
              else
              {//3
                 SCALE.DIS_WEIGHT=SCALE.DIS_GROSSWEIGHT ;   // else displayed weight is gross weight
                
              }//3

             break;
            }

    case _LB: { // we are displaying lbs

              SCALE.DIS_GROSSWEIGHT = display_pounds(SCALE.GROSSWEIGHT);  // format output for kilograms
              SCALE.DIS_TAREWEIGHT = display_pounds(SCALE.TAREWEIGHT);  // convert to tareweight
              SCALE.DIS_NETWEIGHT  = SCALE.DIS_GROSSWEIGHT - SCALE.DIS_TAREWEIGHT;
               
              if (SCALE.NETMODE) // check if we are in netmode
              {        

                SCALE.DIS_WEIGHT=SCALE.DIS_NETWEIGHT;
              }
              else
              {
                SCALE.DIS_WEIGHT=SCALE.DIS_GROSSWEIGHT;
              
              } // 

              break;
            }

    case _G: { // we are displaying grams  (kilograms converted to grams)
              
              SCALE.DIS_GROSSWEIGHT = display_grams(SCALE.GROSSWEIGHT);           // format output for kilograms
              SCALE.DIS_TAREWEIGHT =  display_grams(SCALE.TAREWEIGHT);            // convert to tareweight                      
              SCALE.DIS_NETWEIGHT  = SCALE.DIS_GROSSWEIGHT - SCALE.DIS_TAREWEIGHT;  
                       
              if (SCALE.NETMODE) // check if we are in netmode
              {//2
                                     
                SCALE.DIS_WEIGHT=SCALE.DIS_NETWEIGHT;  // displayed weight is final netweight
              }//2
              else
              {//3
                 SCALE.DIS_WEIGHT=SCALE.DIS_GROSSWEIGHT  ;   // else displayed weight is gross weight
        
              }//3           
              //           
             break;
            } //

          case _OZ: { // we are displaying oz ---------------------------------------------------------------

              SCALE.DIS_GROSSWEIGHT = display_ounce(SCALE.GROSSWEIGHT);  // format output for kilograms
              SCALE.DIS_TAREWEIGHT = display_ounce(SCALE.TAREWEIGHT);  // convert to tareweight
              SCALE.DIS_NETWEIGHT  = SCALE.DIS_GROSSWEIGHT - SCALE.DIS_TAREWEIGHT;

              if (SCALE.NETMODE) // check if we are in netmode
              {          

                SCALE.DIS_WEIGHT=SCALE.DIS_NETWEIGHT;
              }
              else
              {
                SCALE.DIS_WEIGHT=SCALE.DIS_GROSSWEIGHT;              
              } // 

              break;
            }

   
     case _TON: { // we are displaying grams  (kilograms converted to grams)              
             
         //     SCALE.DIS_GROSSWEIGHT = display_ton(gross);           // format output for kilograms
        //      SCALE.DIS_TAREWEIGHT =  display_ton(tare);            // convert to tareweight           
              
              SCALE.DIS_GROSSWEIGHT = display_ton(SCALE.GROSSWEIGHT);           // format output for kilograms
              SCALE.DIS_TAREWEIGHT =  display_ton(SCALE.TAREWEIGHT);            // convert to tareweight    
                                                  
              SCALE.DIS_NETWEIGHT  = SCALE.DIS_GROSSWEIGHT - SCALE.DIS_TAREWEIGHT;  
                       
              if (SCALE.NETMODE) // check if we are in netmode
              {//2
                                     
                SCALE.DIS_WEIGHT=SCALE.DIS_NETWEIGHT;  // displayed weight is final netweight

              }//2
              else
              {//3
                 SCALE.DIS_WEIGHT=SCALE.DIS_GROSSWEIGHT  ;   // else displayed weight is gross weight
          
              }//3             
              //       
             break;
            }

     case _STON: { // we are displaying grams  (kilograms converted to grams)
              
              SCALE.DIS_GROSSWEIGHT = display_short_ton(SCALE.GROSSWEIGHT);           // format output for kilograms
              SCALE.DIS_TAREWEIGHT =  display_short_ton(SCALE.TAREWEIGHT);            // convert to tareweight                      
              SCALE.DIS_NETWEIGHT  = SCALE.DIS_GROSSWEIGHT - SCALE.DIS_TAREWEIGHT;  
                       
              if (SCALE.NETMODE) // check if we are in netmode
              {//2
                                     
                SCALE.DIS_WEIGHT=SCALE.DIS_NETWEIGHT;  // displayed weight is final netweight

              }//2
              else
              {//3
                 SCALE.DIS_WEIGHT=SCALE.DIS_GROSSWEIGHT  ;   // else displayed weight is gross weight
          
              }//3             
              //               
             break;
            } // 
            
   
        case _LTON: { // we are displaying grams  (kilograms converted to grams)
              
              SCALE.DIS_GROSSWEIGHT = display_long_ton(SCALE.GROSSWEIGHT);           // format output for kilograms
              SCALE.DIS_TAREWEIGHT =  display_long_ton(SCALE.TAREWEIGHT);            // convert to tareweight                      
              SCALE.DIS_NETWEIGHT  = SCALE.DIS_GROSSWEIGHT - SCALE.DIS_TAREWEIGHT;  
                       
              if (SCALE.NETMODE) // check if we are in netmode
              {//2
                                     
                SCALE.DIS_WEIGHT=SCALE.DIS_NETWEIGHT;  // displayed weight is final netweight

              }//2
              else
              {//3
                 SCALE.DIS_WEIGHT=SCALE.DIS_GROSSWEIGHT  ;   // else displayed weight is gross weight
          
              }//3             
              //               
             break;
            } // 
    
    
    } // end of switch
   
              // add deadzero(s) for grad size of 10,20,50,100
              if (SCALE.DIS_DEADZERO==1)
              {
                SCALE.DIS_WEIGHT=SCALE.DIS_WEIGHT*10;  //x10 muliplier for grad size of 10,20,50
              }
              else
              if (SCALE.DIS_DEADZERO==2)
              {
                SCALE.DIS_WEIGHT=SCALE.DIS_WEIGHT*100;  // x100 multiplier for grad size of 100
              }

               // check if weight is negative, strip sign bit and set negative flag
               if (SCALE.DIS_WEIGHT < 0)
               {
                      SCALE.DIS_WEIGHT=SCALE.DIS_WEIGHT*-1;
                      SCALE.NEGATIVE=1;  // set negative flag
               }
               else
               {
                    SCALE.NEGATIVE=0;  // else clear flag
               }    //   
  
  // SCALE.MULT=mult;  
 
} // end of function format_weight

//-----------------------------------------------------------------------------------------------
// calculate scale over value as set in calibration
//----------------------------------------------------------------------------------------------

void calc_scaleover(void)
{
long cap;
long sover;

cap=CAL.SCALECAP/CAL.GRAD;   // remove dead zeros and convert to 1d grad size

if (CAL.DEADZERO==1)
{
 cap= cap /  10;
}
else if (CAL.DEADZERO==2)
{
 cap= cap / 100;
} 

if (CAL.SCLOVER > 9)  // if greater than 3, select 2% of cap
{
sover= (long) (cap*0.02); // 2% of capacity
}
else
{
sover= CAL.SCLOVER;
}

SCALE.SCALEOVER=sover+cap;
}  // end of function


