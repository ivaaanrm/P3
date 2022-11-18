/// @file

#include <iostream>
#include <stdio.h>
#include <math.h>
#include "pitch_analyzer.h"

using namespace std;


/// Name space of UPC
namespace upc {
  void PitchAnalyzer::autocorrelation(const vector<float> &x, vector<float> &r) const {

    for (unsigned int l = 0; l < r.size(); ++l) {
  		/// \TODO Compute the autocorrelation r[l]
      /// \DONE
      r[l]= 0.0f;
      
      for(unsigned int n = l; n<x.size();n++){
        r[l] += x[n]*x[n-l];
      }
      r[l] = r[l] / x.size();
    }

    if (r[0] == 0.0F) //to avoid log() and divide zero 
      r[0] = 1e-10; 
  }

  int sgn(float x) {
    if (x > 0.0) return 1;
    if (x < 0.0) return -1;
    return 0;
  }

  float PitchAnalyzer::compute_zcr(std::vector<float> & x) const {
    int N = x.size();
    float fm = samplingFreq;
    float sum = 0.0;
    for (int i = 0; i < N; i++){
        if (sgn(x[i]) != sgn(x[i-1])){
            sum++;
        }
    }
    return fm/(2*(N-1))*sum;
  }

  void PitchAnalyzer::set_window(Window win_type) {
    if (frameLen == 0)
      return;

    window.resize(frameLen);

    switch (win_type) {
    case HAMMING:
      /// \TODO Implement the Hamming window
      /// \DONE
      for(unsigned int n = 0; n<frameLen;n++){
        window[n] = 0.54 - 0.46*cos(2*M_PI*n/(frameLen-1));
      }
      break;
    case RECT:
    default:
      window.assign(frameLen, 1);
    }
  }

  void PitchAnalyzer::set_f0_range(float min_F0, float max_F0) {
    npitch_min = (unsigned int) samplingFreq/max_F0;
    if (npitch_min < 2)
      npitch_min = 2;  // samplingFreq/2

    npitch_max = 1 + (unsigned int) samplingFreq/min_F0;

    //frameLen should include at least 2*T0
    if (npitch_max > frameLen/2)
      npitch_max = frameLen/2;
  }

  bool PitchAnalyzer::unvoiced(float pot, float r1norm, float rmaxnorm, float zcr) const {
    /// \TODO Implement a rule to decide whether the sound is voiced or not.
    /// * You can use the standard features (pot, r1norm, rmaxnorm),
    ///   or compute and use other ones.
    /// \DONE
    if((pot < maxpot && r1norm < u1norm) || (rmaxnorm < umaxnorm || zcr > minZcr)){
    // if(rmaxnorm < umaxnorm || zcr > minZcr){
      return true;
    } else {
      return false;
    }
  }

  float PitchAnalyzer::compute_pitch(vector<float> & x) const {
    if (x.size() != frameLen)
      return -1.0F;

    //Window input frame
    for (unsigned int i=0; i<x.size(); ++i)
      x[i] *= window[i];

    vector<float> r(npitch_max);

    //Compute correlation
    autocorrelation(x, r);

    vector<float>::const_iterator iR = r.begin(), iRMax = iR;

    /// \TODO
    /// \DONE 
    /// Localizar máximo de la autocorrelation
	/// Find the lag of the maximum value of the autocorrelation away from the origin.<br>
	/// Choices to set the minimum value of the lag are:
	///    - The first negative value of the autocorrelation.
	///    - The lag corresponding to the maximum value of the pitch.
    ///	   .
	/// In either case, the lag should not exceed that of the minimum value of the pitch.

    /// GET lag of the maximum value of the autocorrelation away from the origin
    for (iR = iRMax = r.begin() + npitch_min; iR < r.begin() + npitch_max; iR++) {
      if (*iRMax < *iR) iRMax = iR;
    }

    unsigned int lag = iRMax - r.begin();

    float pot = 10 * log10(r[0]);

    // zcr 
    float zcr = compute_zcr(x);

    //You can print these (and other) features, look at them using wavesurfer
    //Based on that, implement a rule for unvoiced
    //change to #if 1 and compile
#if 0
    if (r[0] > 0.0F)
    printf("%4.4f\t%4.4f\t%4.4f\t%4.4f\n", pot, r[1]/r[0], r[lag]/r[0], zcr);
    // printf("%4.4f", pot);
      // cout << pot << '\t' << r[1]/r[0] << '\t' << r[lag]/r[0] << '\t' << zcr << endl;


#endif

    if (unvoiced(pot, r[1]/r[0], r[lag]/r[0], zcr)){
      return 0;
      }else{
      return (float) samplingFreq/(float) lag;
      }
  }
}
