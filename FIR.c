
#include "includes.h"
#include "FIR.h"

#define SAMPLE  double      /* define the type used for data samples */
#define NTAPSMAX 71

int NTAPS;
static double z[NTAPSMAX];
static char filter_enable;
const double *h; // pointer to selecter FIR filter
/*









#include <iostream>
#include <vector>
#include <cmath>

// Function to implement a FIR low-pass filter
std::vector<double> firLowPassFilter(const std::vector<double>& inputSignal, double samplingRate, double cutoffFrequency) {
    int filterLength = 31; // Filter length (adjustable)
    std::vector<double> outputSignal(inputSignal.size(), 0.0);
    std::vector<double> filterCoefficients(filterLength, 0.0);

    // Calculate normalized cutoff frequency
    double omegaC = 2.0 * M_PI * cutoffFrequency / samplingRate;

    // Calculate filter coefficients using Hamming window
    for (int n = 0; n < filterLength; ++n) {
        if (n != (filterLength - 1) / 2) {
            filterCoefficients[n] = sin(omegaC * (n - (filterLength - 1) / 2)) / (M_PI * (n - (filterLength - 1) / 2));
        } else {
            filterCoefficients[n] = omegaC / M_PI;
        }
        filterCoefficients[n] *= 0.54 - 0.46 * cos(2.0 * M_PI * n / (filterLength - 1)); // Apply Hamming window
    }

    // Apply the filter
    int N = inputSignal.size();
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < filterLength; ++j) {
            if ((i - j) >= 0) {
                outputSignal[i] += inputSignal[i - j] * filterCoefficients[j];
            }
        }
    }

    return outputSignal;
}

int main() {
    // Sample input signal (replace with your own data)
    std::vector<double> inputSignal = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};

    // Sampling rate (samples per second)
    double samplingRate = 15.0; // Hz

    // Cutoff frequency (Hz)
    double cutoffFrequency = 3.0; // Hz

    // Apply FIR low-pass filter
    std::vector<double> outputSignal = firLowPassFilter(inputSignal, samplingRate, cutoffFrequency);

    // Print the filtered output signal
    std::cout << "Filtered Output Signal:" << std::endl;
    for (double value : outputSignal) {
        std::cout << value << std::endl;
    }

    return 0;
}

*/
// Fir filter
/*

// Function to implement a simple FIR low-pass filter
std::vector<double> firLowPassFilter(const std::vector<double>& inputSignal, double samplingRate, double cutoffFrequency) {
    std::vector<double> outputSignal(inputSignal.size(), 0.0);

    // Calculate filter coefficients
    double nyquistFrequency = samplingRate / 2.0;
    double cutoffNormalized = cutoffFrequency / nyquistFrequency;

    // Filter coefficients using rectangular window
    int filterLength = 21; // Filter length (adjustable)
    std::vector<double> filterCoefficients(filterLength, 0.0);
    for (int i = 0; i < filterLength; ++i) {
        if (i == filterLength / 2) {
            filterCoefficients[i] = 2.0 * cutoffNormalized;
        } else {
            filterCoefficients[i] = sin(2.0 * M_PI * cutoffNormalized * (i - filterLength / 2)) / (M_PI * (i - filterLength / 2));
        }
    }

    // Apply the filter
    int N = inputSignal.size();
    for (int n = 0; n < N; ++n) {
        for (int k = 0; k < filterLength; ++k) {
            if ((n - k) >= 0) {
                outputSignal[n] += inputSignal[n - k] * filterCoefficients[k];
            }
        }
    }

    return outputSignal;
}

int main() {
    // Sample input signal (replace with your own data)
    std::vector<double> inputSignal = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};

    // Sampling rate (in Hz)
    double samplingRate = 100.0; // Hz

    // Cutoff frequency (in Hz)
    double cutoffFrequency = 3.0; // Hz

    // Apply FIR low-pass filter
    std::vector<double> outputSignal = firLowPassFilter(inputSignal, samplingRate, cutoffFrequency);

    // Print the filtered output signal
    std::cout << "Filtered Output Signal:" << std::endl;
    for (double value : outputSignal) {
        std::cout << value << std::endl;
    }

    return 0;
}

Filter Coefficients
k       Coefficient
---------------------
0       0.0
1       -0.0333148
2       -0.0335646
3       -0.0338154
4       -0.0340673
5       -0.0343202
6       -0.0345742
7       -0.0348292
8       -0.0350853
9       -0.0353424
10      -0.0356007
11      -0.0358600
12      0.6641400
13      -0.0358600
14      -0.0356007
15      -0.0353424
16      -0.0350853
17      -0.0348292
18      -0.0345742
19      -0.0343202
20      -0.0340673



*/
// 3Hz filter
const double h1[17] = {//  17 tap filter  1Hz passband, -80db stopband @ 15Hz, -3db point is 3 Hz
    0.000848541262,
    0.004194910476,
    0.012618547643,
    0.028553070144,
    0.052694857292,
    0.082580884320,
    0.112411023273,
    0.134649112086,
    0.142898107008,
    0.134649112086,
    0.112411023273,
    0.082580884320,
    0.052694857292,
    0.028553070144,
    0.012618547643,
    0.004194910476,
    0.000848541262
};


// 2Hz filter
const double h2[29] = {//  29 tap filter  1Hz passband, -80db stopband @ 9Hz, -3db point is 2 Hz
    0.000241864769,
    0.000812631321,
    0.002031220885,
    0.004228206915,
    0.007755515592,
    0.012910434559,
    0.019854140529,
    0.028534481540,
    0.038633054973,
    0.049554621965,
    0.060468545092,
    0.070402047111,
    0.078372840664,
    0.083537394456,
    0.085325999256,
    0.083537394456,
    0.078372840664,
    0.070402047111,
    0.060468545092,
    0.049554621965,
    0.038633054973,
    0.028534481540,
    0.019854140529,
    0.012910434559,
    0.007755515592,
    0.004228206915,
    0.002031220885,
    0.000812631321,
    0.000241864769
};





// 1.2Hz filter
const double h3[55] = {//  55 tap filter  1Hz passband, -80db stopband @ 5Hz, -3db point is 1.2 Hz
    0.000026478296,
    0.000122891618,
    0.000221373289,
    0.000418588783,
    0.000704245893,
    0.001118798513,
    0.001689114287,
    0.002448387059,
    0.003427321690,
    0.004654623179,
    0.006153712624,
    0.007940979423,
    0.010023414507,
    0.012396598035,
    0.015043447133,
    0.017933073171,
    0.021020611002,
    0.024247859995,
    0.027544628217,
    0.030830975652,
    0.034020025938,
    0.037021649691,
    0.039746316641,
    0.042109148918,
    0.044034063494,
    0.045457389822,
    0.046331308327,
    0.046625949605,
    0.046331308327,
    0.045457389822,
    0.044034063494,
    0.042109148918,
    0.039746316641,
    0.037021649691,
    0.034020025938,
    0.030830975652,
    0.027544628217,
    0.024247859995,
    0.021020611002,
    0.017933073171,
    0.015043447133,
    0.012396598035,
    0.010023414507,
    0.007940979423,
    0.006153712624,
    0.004654623179,
    0.003427321690,
    0.002448387059,
    0.001689114287,
    0.001118798513,
    0.000704245893,
    0.000418588783,
    0.000221373289,
    0.000122891618,
    0.000026478296
};



// 0.5Hz filter
const double h4[71] = {//  71 tap filter  1Hz passband, -80db stopband @ 5Hz, -3db point is 1.2 Hz
    0.000026065172,
    0.000068135960,
    0.000105774243,
    0.000191703372,
    0.000297029247,
    0.000454516258,
    0.000659026053,
    0.000931353832,
    0.001276790249,
    0.001711333961,
    0.002244197721,
    0.002888375881,
    0.003653071008,
    0.004547722214,
    0.005578628137,
    0.006750149981,
    0.008063169708,
    0.009515359732,
    0.011100391969,
    0.012808028336,
    0.014623774705,
    0.016529092672,
    0.018501520622,
    0.020514980984,
    0.022540332673,
    0.024545984604,
    0.026498622832,
    0.028364082467,
    0.030108276508,
    0.031698140823,
    0.033102627725,
    0.034293591462,
    0.035246700348,
    0.035942202792,
    0.036365468773,
    0.036507553944,
    0.036365468773,
    0.035942202792,
    0.035246700348,
    0.034293591462,
    0.033102627725,
    0.031698140823,
    0.030108276508,
    0.028364082467,
    0.026498622832,
    0.024545984604,
    0.022540332673,
    0.020514980984,
    0.018501520622,
    0.016529092672,
    0.014623774705,
    0.012808028336,
    0.011100391969,
    0.009515359732,
    0.008063169708,
    0.006750149981,
    0.005578628137,
    0.004547722214,
    0.003653071008,
    0.002888375881,
    0.002244197721,
    0.001711333961,
    0.001276790249,
    0.000931353832,
    0.000659026053,
    0.000454516258,
    0.000297029247,
    0.000191703372,
    0.000105774243,
    0.000068135960,
    0.000026065172
};


//-------------------------------------------------------------------------------
// FIR filter
//   These functions use most or all of the following input parameters:
//
//       input    - the input sample data
//       ntaps    - the number of taps in the filter
//       h[]      - the FIR coefficient array
//       z[]      - the FIR delay line array
//-------------------------------------------------------------------------------
 double fir_filter(long input)
 {
    int ii;
    double accum;

 if (filter_enable==0)  // if not enabled by pass filter
     {
    //  return(input);
     } // 


    z[0] = input;   /* store input at the beginning of the delay line */

    // calc FIR
    accum = 0;  // clear accumalator
    for (ii = 0; ii < NTAPS; ii++) // loop from beginning to length of filter array
     {//1
        accum =  accum +  (h[ii] * z[ii]) ;    //multiply each tap in the FIR filter with filter coeff, and sum all the taps
     }//1


    // shift delay line
    for (ii = NTAPS - 2; ii >= 0; ii--)   // loop through and shift the delay line to the right
     {//2
        z[ii + 1] = z[ii];
     }//2

 return (accum);

 }  // end of function




//--------------------------------------------------------------------------------------
// This function must be called to initialize the FIR filter before starting
//--------------------------------------------------------------------------------------
void initialize_FIR(char select)
{

  switch (select)
  {


     case 1:
        {
        filter_enable=1;
        h = &h4[0];  // select 0.5 hz filter
         NTAPS=71;
         break;
        }


      case 2:
        {
        filter_enable=1;
        h = &h3[0];  // select 1 hz filter
         NTAPS=55;
        break;
        }



      case 3:
        {
        filter_enable=1;
        h = &h2[0]; // select 2 hz filter
         NTAPS=29;
        break;
        }

      case 4:
        {
        filter_enable=1;  
        h = &h1[0]; // select 3 hz filter
         NTAPS=17;
        break;
        }
  
     


    } //end of switch


// z


} // end of function

