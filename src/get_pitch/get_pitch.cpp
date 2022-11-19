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
    -m REAL, --umaxnorm=REAL  Umbral del maximo de la autocorrelación [default: 0.378] 
    -z REAL, --minZcr=REAL  Umbral del minimo de la tasa de cruces por cero [default: 3445.0] 
    -u REAL, --u1norm=REAL  Umbral  de la autocorrelación [default: 0.9765] 
    -p REAL, --maxpot=REAL  Umbral del maximo de la potencia [default: -46.35] 
    -a REAL, --alpha=REAL  Umbral central clipping [default: 0.003] 
    -x REAL, --th1=REAL  Threshold post-processing [default: 1.6] 
    -y REAL, --th2=REAL  Factor post-processing [default: 0.9] 

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
  float minZcr = std::stof(args["--minZcr"].asString());
  float u1norm = std::stof(args["--u1norm"].asString());
  float maxpot = std::stof(args["--maxpot"].asString());
  float alpha = std::stof(args["--alpha"].asString());
  float th1 = std::stof(args["--th1"].asString());
  float th2 = std::stof(args["--th2"].asString());


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
  PitchAnalyzer analyzer(n_len, rate, PitchAnalyzer::RECT, 50, 500, umaxnorm, u1norm, maxpot, minZcr);

  /// \TODO
  /// Preprocess the input signal in order to ease pitch estimation. For instance,
  /// central-clipping or low pass filtering may be used.
  /// \DONE

  

  // Iterate for each frame and save values in f0 vector
  vector<float>::iterator iX;
  vector<float> f0;
  float f = 0.0;
  // float alpha = 0.00049;
  for (iX = x.begin(); iX < x.end(); iX++) {

    if (*iX < alpha && *iX > -alpha){ 
      *iX = 0.0;
    }
    // printf("%f ",*iX);
  }

  for (iX = x.begin(); iX + n_len < x.end(); iX = iX + n_shift) {
    f = analyzer(iX, iX + n_len);
    f0.push_back(f);
  }

  /// \TODO
  /// Postprocess the estimation in order to supress errors. For instance, a median filter
  /// or time-warping may be used.
  /// \DONE

  float avgPitch = 0;
  int numPitch = 0;


  // filter that eliminates the single voiced frames
  vector<float>::iterator iF0;

  for (iF0 = f0.begin(); iF0 < f0.end(); iF0++) {
    if (*iF0 != 0.0) {
      avgPitch += *iF0;
      numPitch++;
      if (iF0 == f0.begin()) {
        if (*(iF0 + 1) == 0.0) {
          *iF0 = 0.0;
        }
      } else if (iF0 == f0.end() - 1) {
        if (*(iF0 - 1) == 0.0) {
          *iF0 = 0.0;
        }
      } else {
        if (*(iF0 - 1) == 0.0 && *(iF0 + 1) == 0.0) {
          *iF0 = 0.0;
        }
      }
    }
  }

  // filter that fills the single unvoiced frames
  for (iF0 = f0.begin(); iF0 < f0.end(); iF0++) {
    if (*iF0 == 0.0) {
      if (iF0 == f0.begin()) {
        if (*(iF0 + 1) != 0.0) {
          *iF0 = *(iF0 + 1);
        }
      } else if (iF0 == f0.end() - 1) {
        if (*(iF0 - 1) != 0.0) {
          *iF0 = *(iF0 - 1);
        }
      } else {
        if (*(iF0 - 1) != 0.0 && *(iF0 + 1) != 0.0) {
          *iF0 = (*(iF0 - 1) + *(iF0 + 1)) / 2;
        }
      }
    }
  }

  // filter that fills double zero holes
  for (iF0 = f0.begin(); iF0 < f0.end()-3; iF0++) {
    if (*iF0 != 0.0 && *(iF0 + 1) == 0.0 && *(iF0 + 2) == 0.0 && *(iF0 + 3) != 0.0) {
      *(iF0 + 1) = (*(iF0) + *(iF0 + 3)) / 2;
      *(iF0 + 2) = (*(iF0) + *(iF0 + 3)) / 2;
    }
  }


  avgPitch = avgPitch / numPitch;


  for (iF0 = f0.begin(); iF0 < f0.end(); iF0++) {
    if (*iF0 >= avgPitch*th1){
      *iF0 = avgPitch*th2;
    }
  }

  //Write f0 contour into the output file
  ofstream os(output_txt);
  if (!os.good()) {
    cerr << "Error reading output file " << output_txt << " (" << strerror(errno) << ")\n";
    return -3;
  }

  os << 0 << '\n'; //pitch at t=0
  for (iX = f0.begin(); iX != f0.end(); ++iX) 
    os << *iX << '\n';
  os << 0 << '\n';//pitch at t=Dur

  return 0;
}
