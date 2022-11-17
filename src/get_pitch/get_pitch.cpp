/// @file

#include <iostream>
#include <fstream>
#include <string.h>
#include <errno.h>

#include "wavfile_mono.h"
#include "pitch_analyzer.h"

#include "docopt.h"

#define FRAME_LEN   0.030 /* 30 ms. */
#define FRAME_SHIFT 0.015 /* 15 ms. */

using namespace std;
using namespace upc;

static const char USAGE[] = R"(
get_pitch - Pitch Estimator 

Usage:
    get_pitch [options] <input-wav> <output-txt>
    get_pitch (-h | --help)
    get_pitch --version

Options:
    -h, --help  Show this screen
    --version   Show the version of the project
    -m REAL, --umaxnorm=REAL  Umbral del maximo de la autocorrelación [default: 0.59] 
    -u REAL, --u1norm=REAL  Umbral  de la autocorrelación [default: 0.96] 
    -p REAL, --maxpot=REAL  Umbral del maximo de la potencia [default: -33] 

Arguments:
    input-wav   Wave file with the audio signal
    output-txt  Output file: ASCII file with the result of the estimation:
                    - One line per frame with the estimated f0
                    - If considered unvoiced, f0 must be set to f0 = 0
)";

int main(int argc, const char *argv[]) {
	/// \TODO 
	///  Modify the program syntax and the call to **docopt()** in order to
	///  add options and arguments to the program.
  /// \DONE
    std::map<std::string, docopt::value> args = docopt::docopt(USAGE,
        {argv + 1, argv + argc},	// array of arguments, without the program name
        true,    // show help if requested
        "2.0");  // version string

	std::string input_wav = args["<input-wav>"].asString();
	std::string output_txt = args["<output-txt>"].asString();
  
  float umaxnorm = std::stof(args["--umaxnorm"].asString());
  float u1norm = std::stof(args["--u1norm"].asString());
  float maxpot = std::stof(args["--maxpot"].asString());


  // Read input sound file
  unsigned int rate;
  vector<float> x;
  if (readwav_mono(input_wav, rate, x) != 0) {
    cerr << "Error reading input file " << input_wav << " (" << strerror(errno) << ")\n";
    return -2;
  }

  int n_len = rate * FRAME_LEN;
  int n_shift = rate * FRAME_SHIFT;

  // Define analyzer
  PitchAnalyzer analyzer(n_len, rate, PitchAnalyzer::HAMMING, 50, 500, umaxnorm, u1norm, maxpot);

  /// \TODO
  /// Preprocess the input signal in order to ease pitch estimation. For instance,
  /// central-clipping or low pass filtering may be used.
float alpha = 0.0042;

  // Iterate for each frame and save values in f0 vector
  vector<float>::iterator iX;
  vector<float> f0;
  float f = 0.0;
  for (iX = x.begin(); iX + n_len < x.end(); iX = iX + n_shift) {
      if (*iX < alpha && *iX > -alpha){ 
        f = 0.0;
      } else {
        f = analyzer(iX, iX + n_len);
      }
    f0.push_back(f);
  }

  /// \TODO
  /// Postprocess the estimation in order to supress errors. For instance, a median filter
  /// or time-warping may be used.

  /// Postprocess the estimation in order to supress errors. For instance, a median filter
  /// or time-warping may be used.
  vector<float>::iterator iF0;
  vector<float> f0_post;
  for (iF0 = f0.begin(); iF0 < f0.end(); iF0++) {
    if (*iF0 == 0) {
      f0_post.push_back(0);
    } else {
      float f = *iF0;
      float f1 = *(iF0 + 1);
      float f2 = *(iF0 + 2);
      if (f1 == 0) {
        f0_post.push_back(f);
      } else if (f2 == 0) {
        f0_post.push_back((f + f1) / 2);
      } else {
        f0_post.push_back((f + f1 + f2) / 3);
      }
    }
  }


  // Write f0 contour into the output file
  ofstream os(output_txt);
  if (!os.good()) {
    cerr << "Error reading output file " << output_txt << " (" << strerror(errno) << ")\n";
    return -3;
  }

  os << 0 << '\n'; //pitch at t=0
  for (iF0 = f0_post.begin(); iF0 != f0_post.end(); ++iF0) 
    os << *iF0 << '\n';
  os << 0 << '\n';//pitch at t=Dur

  // // Write f0 contour into the output file
  // ofstream os(output_txt);
  // if (!os.good()) {
  //   cerr << "Error reading output file " << output_txt << " (" << strerror(errno) << ")\n";
  //   return -3;
  // }

  // os << 0 << '\n'; //pitch at t=0
  // for (iX = f0.begin(); iX != f0.end(); ++iX) 
  //   os << *iX << '\n';
  // os << 0 << '\n';//pitch at t=Dur

  return 0;
}
