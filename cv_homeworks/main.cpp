//****************************//
//Author:Song Jin
//Last modify time:2017/9/14
//version:1.0
//License:MIT
//Email:15754603603@163.com
//****************************//
#include <iostream> // for standard I/O
#include <string>   // for strings
#include <opencv2/core.hpp>     // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/imgproc.hpp>  // Gaussian Blur
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>  // OpenCV window I/O
using namespace std;
using namespace cv;
static void help()
{
	cout
		<< "------------------------------------------------------------------------------" << endl
		<< "This program shows how to caputer live video from webcam with OpenCV. In addition," << endl
		<< "it blur the live video and save the processed video to"<< endl
		<< "disk with color and gray-scale formats" << endl
		<< "--------------------------------------------------------------------------" << endl
		<< endl;
}
int main(int argc, char *argv[])
{
	help();
	Mat frame, out_frame, out_frame_grey;
	const int output_FPS = 20;
	//--- INITIALIZE VIDEOCAPTURE 
	VideoCapture inputVideo;
	// open the default camera using default API
	inputVideo.open(0);
	if (!inputVideo.isOpened())
	{
		cout << "Could not open webcam " << endl;
		return -1;
	}
	int ex = VideoWriter::fourcc('P', 'I', 'M', '1');     // Set Codec Type- Int form
	Size S = Size((int)inputVideo.get(CAP_PROP_FRAME_WIDTH),    // Acquire input size
		          (int)inputVideo.get(CAP_PROP_FRAME_HEIGHT));
    // open the output
	const string NAME_COLOR = "processed_video_color.avi";
	const string NAME_GREY = "processed_video_grey.avi";
	//--- INITIALIZE VIDEOOUTPUT
	VideoWriter outputVideo_color(NAME_COLOR, ex, output_FPS, S, true);
	VideoWriter outputVideo_grey(NAME_GREY, ex, output_FPS, S, false);
	if (!outputVideo_color.isOpened() || !outputVideo_grey.isOpened())
	{
		cout << "Could not open the output video for write !!!" << endl;
		return -1;
	}
	//--- GRAB AND WRITE LOOP
	cout << "Start grabbing and process" << endl
		<< "Press any key to terminate" << endl;
	//--- CREAT THE SHOW WINDOWS
	const char* WIN_CAMERA = "Live";
	const char* WIN_PROCESSED = "Processed";
	namedWindow(WIN_CAMERA, WINDOW_AUTOSIZE);
	namedWindow(WIN_PROCESSED, WINDOW_AUTOSIZE);
	moveWindow(WIN_CAMERA, 0, 100);
	moveWindow(WIN_PROCESSED, S.width, 100);
	for (;;)
	{
		// wait for a new frame from camera and store it into 'frame'
		inputVideo.read(frame);
		// check if we succeeded
		if (frame.empty()) {
			cerr << "ERROR! blank frame grabbed\n";
			break;
		}
		// do some operation to the frame
		blur(frame, out_frame, Size(10, 10));
		//erode(frame, out_frame, getStructuringElement(MORPH_RECT, Size(15, 15)));
		// convert it to grey
		cvtColor(out_frame, out_frame_grey, COLOR_BGR2GRAY);
		// show images and wait for a key with timeout long enough to show images
		imshow(WIN_CAMERA, frame);
		imshow(WIN_PROCESSED, out_frame);
		// save the images to file
		outputVideo_color << out_frame;
		outputVideo_grey << out_frame_grey;
		if (waitKey(5) >= 0)
			break;
	}
	outputVideo_color.release();
	outputVideo_grey.release();
	return 0;
}
