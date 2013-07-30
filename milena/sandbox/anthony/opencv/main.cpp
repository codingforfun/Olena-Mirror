#include <iostream>
#include <vector>

#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"


// MAIN ENTRY POINT
int main(int argc, char* argv[])
{
  const cv::Mat input = cv::imread("../images/keith.pbm");
  cv::SiftFeatureDetector detector;
  std::vector<cv::KeyPoint> keypoints;
  detector.detect(input, keypoints);

  cv::Mat output;
  cv::drawKeypoints(input, keypoints, output);
  cv::imwrite("sift.jpg", output);

  return 0;



  // std::string inputPath, inputPath2, toolchainPath, outputPath;
  // std::deque<ourfilters::eFilter> toolchain;
  // bool* multithread = new bool(true);

  // int validCommandLine = getCommandLine(argc, argv, inputPath,
  //                                                   inputPath2,
  //                                                   toolchainPath,
  //                                                   outputPath,
  //                                                   multithread);

  // bool twoSources = inputPath2.length() > 0;

  // if (!validCommandLine)
  //   return errors::invalidCommandLine();

  // parser::getSteps(toolchainPath, toolchain);

  // if (!(*multithread))
  // {
  //   std::cout << "single-threader" << std::endl;
  //   tbb::task_scheduler_init intit(1);
  // }

  // tbb::pipeline pipeline;

  // Picker picker(inputPath, inputPath2, twoSources);
  // pipeline.add_filter(picker);

  // ApplyFilters applicator(toolchain);
  // pipeline.add_filter(applicator);

  // Writer writer(outputPath,
  //     picker.get_codec(),
  //     picker.get_fps(),
  //     picker.get_dimensions());
  // pipeline.add_filter(writer);

  // pipeline.run(128);

  // return 0;
}
