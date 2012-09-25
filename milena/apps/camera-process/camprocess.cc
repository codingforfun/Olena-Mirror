// Copyright (C) 2012 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

/// \file
/// \brief Frame-by-frame processing of a camera stream.
///
/// Inspired by:
///  http://www.geckogeek.fr/lire-le-flux-dune-webcam-camera-video-avec-opencv.html
///
/// Iplimage format description:
///  http://opencv.willowgarage.com/documentation/basic_structures.html#iplimage
///
/// Hints on manipulating OpenCV:
///  http://www.cs.iit.edu/~agam/cs512/lect-notes/opencv-intro/opencv-intro.html
///

#include <opencv/highgui.h>
#include <opencv/cv.h>

#include "process.hh"

int main()
{
  // Capture camera stream.
  CvCapture* capture = cvCreateCameraCapture(CV_CAP_ANY);  
  if (!capture)
    {
      std::cerr << "Cannot open camera stream." << std::endl;
      exit(1);
    }

  // Create a GUI window.
  cvNamedWindow("Window", CV_WINDOW_AUTOSIZE);

  // Loop until `q' or `Q' is pressed.
  char key; 
  while(key != 'q' && key != 'Q')
    {
      // Grab a frame from the stream...
      IplImage* image = cvQueryFrame(capture);
      // ...process it with Milena...
      process(image);
      // ...and display it.
      cvShowImage("Window", image);
 
      // Wait for the user to press a key for 10 ms.
      key = cvWaitKey(10);
    }

  // Release ressources.
  cvReleaseCapture(&capture);
  cvDestroyWindow("Window");
}
