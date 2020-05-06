#include "stdafx.h"
#include "common.h"
#include <random>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>


using namespace std;
using namespace cv;


default_random_engine gen;
uniform_int_distribution<int> d(0, 255);

Mat_<Vec3b> shapes = imread("Images/Lab4/trasaturi_geom.bmp");
Mat_<uchar> bwshapes = imread("Images/Lab5/diagonal.bmp", CV_LOAD_IMAGE_GRAYSCALE);


void testOpenImage()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src;
		src = imread(fname);
		imshow("image", src);
		waitKey();
	}
}

void testOpenImagesFld()
{
	char folderName[MAX_PATH];
	if (openFolderDlg(folderName) == 0)
		return;
	char fname[MAX_PATH];
	FileGetter fg(folderName, "bmp");
	while (fg.getNextAbsFile(fname))
	{
		Mat src;
		src = imread(fname);
		imshow(fg.getFoundFileName(), src);
		if (waitKey() == 27) //ESC pressed
			break;
	}
}

void testImageOpenAndSave()
{
	Mat src, dst;

	src = imread("Images/Lena_24bits.bmp", CV_LOAD_IMAGE_COLOR);	// Read the image

	if (!src.data)	// Check for invalid input
	{
		printf("Could not open or find the image\n");
		return;
	}

	// Get the image resolution
	Size src_size = Size(src.cols, src.rows);

	// Display window
	const char* WIN_SRC = "Src"; //window for the source image
	namedWindow(WIN_SRC, CV_WINDOW_AUTOSIZE);
	cvMoveWindow(WIN_SRC, 0, 0);

	const char* WIN_DST = "Dst"; //window for the destination (processed) image
	namedWindow(WIN_DST, CV_WINDOW_AUTOSIZE);
	cvMoveWindow(WIN_DST, src_size.width + 10, 0);

	cvtColor(src, dst, CV_BGR2GRAY); //converts the source image to a grayscale one

	imwrite("Images/Lena_24bits_gray.bmp", dst); //writes the destination to file

	imshow(WIN_SRC, src);
	imshow(WIN_DST, dst);

	printf("Press any key to continue ...\n");
	waitKey(0);
}

void testNegativeImage()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		double t = (double)getTickCount(); // Get the current time [s]

		Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		int height = src.rows;
		int width = src.cols;
		Mat dst = Mat(height, width, CV_8UC1);
		// Asa se acceseaaza pixelii individuali pt. o imagine cu 8 biti/pixel
		// Varianta ineficienta (lenta)
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				uchar val = src.at<uchar>(i, j);
				uchar neg = MAX_PATH - val;
				dst.at<uchar>(i, j) = neg;
			}
		}

		// Get the current time again and compute the time difference [s]
		t = ((double)getTickCount() - t) / getTickFrequency();
		// Print (in the console window) the processing time in [ms] 
		printf("Time = %.3f [ms]\n", t * 1000);

		imshow("input image", src);
		imshow("negative image", dst);
		waitKey();
	}
}

void testParcurgereSimplaDiblookStyle()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		int height = src.rows;
		int width = src.cols;
		Mat dst = src.clone();

		double t = (double)getTickCount(); // Get the current time [s]

		// the fastest approach using the “diblook style”
		uchar* lpSrc = src.data;
		uchar* lpDst = dst.data;
		int w = (int)src.step; // no dword alignment is done !!!
		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++) {
				uchar val = lpSrc[i * w + j];
				lpDst[i * w + j] = 255 - val;
			}

		// Get the current time again and compute the time difference [s]
		t = ((double)getTickCount() - t) / getTickFrequency();
		// Print (in the console window) the processing time in [ms] 
		printf("Time = %.3f [ms]\n", t * 1000);

		imshow("input image", src);
		imshow("negative image", dst);
		waitKey();
	}
}

void testColor2Gray()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname);

		int height = src.rows;
		int width = src.cols;

		Mat dst = Mat(height, width, CV_8UC1);

		// Asa se acceseaaza pixelii individuali pt. o imagine RGB 24 biti/pixel
		// Varianta ineficienta (lenta)
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				Vec3b v3 = src.at<Vec3b>(i, j);
				uchar b = v3[0];
				uchar g = v3[1];
				uchar r = v3[2];
				dst.at<uchar>(i, j) = (r + g + b) / 3;
			}
		}

		imshow("input image", src);
		imshow("gray image", dst);
		waitKey();
	}
}

void testBGR2HSV()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname);
		int height = src.rows;
		int width = src.cols;

		// Componentele d eculoare ale modelului HSV
		Mat H = Mat(height, width, CV_8UC1);
		Mat S = Mat(height, width, CV_8UC1);
		Mat V = Mat(height, width, CV_8UC1);

		// definire pointeri la matricele (8 biti/pixeli) folosite la afisarea componentelor individuale H,S,V
		uchar* lpH = H.data;
		uchar* lpS = S.data;
		uchar* lpV = V.data;

		Mat hsvImg;
		cvtColor(src, hsvImg, CV_BGR2HSV);

		// definire pointer la matricea (24 biti/pixeli) a imaginii HSV
		uchar* hsvDataPtr = hsvImg.data;

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				int hi = i * width * 3 + j * 3;
				int gi = i * width + j;

				lpH[gi] = hsvDataPtr[hi] * 510 / 360;		// lpH = 0 .. 255
				lpS[gi] = hsvDataPtr[hi + 1];			// lpS = 0 .. 255
				lpV[gi] = hsvDataPtr[hi + 2];			// lpV = 0 .. 255
			}
		}

		imshow("input image", src);
		imshow("H", H);
		imshow("S", S);
		imshow("V", V);

		waitKey();
	}
}

void testResize()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src;
		src = imread(fname);
		Mat dst1, dst2;
		//without interpolation
		resizeImg(src, dst1, 320, false);
		//with interpolation
		resizeImg(src, dst2, 320, true);
		imshow("input image", src);
		imshow("resized image (without interpolation)", dst1);
		imshow("resized image (with interpolation)", dst2);
		waitKey();
	}
}

void testCanny()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src, dst, gauss;
		src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		double k = 0.4;
		int pH = 50;
		int pL = (int)k * pH;
		GaussianBlur(src, gauss, Size(5, 5), 0.8, 0.8);
		Canny(gauss, dst, pL, pH, 3);
		imshow("input image", src);
		imshow("canny", dst);
		waitKey();
	}
}

void testVideoSequence()
{
	VideoCapture cap("Videos/rubic.avi"); // off-line video from file
	//VideoCapture cap(0);	// live video from web cam
	if (!cap.isOpened()) {
		printf("Cannot open video capture device.\n");
		waitKey(0);
		return;
	}

	Mat edges;
	Mat frame;
	char c;

	while (cap.read(frame))
	{
		Mat grayFrame;
		cvtColor(frame, grayFrame, CV_BGR2GRAY);
		Canny(grayFrame, edges, 40, 100, 3);
		imshow("source", frame);
		imshow("gray", grayFrame);
		imshow("edges", edges);
		c = cvWaitKey(0);  // waits a key press to advance to the next frame
		if (c == 27) {
			// press ESC to exit
			printf("ESC pressed - capture finished\n");
			break;  //ESC pressed
		};
	}
}


void testSnap()
{
	VideoCapture cap(0); // open the deafult camera (i.e. the built in web cam)
	if (!cap.isOpened()) // openenig the video device failed
	{
		printf("Cannot open video capture device.\n");
		return;
	}

	Mat frame;
	char numberStr[256];
	char fileName[256];

	// video resolution
	Size capS = Size((int)cap.get(CV_CAP_PROP_FRAME_WIDTH),
		(int)cap.get(CV_CAP_PROP_FRAME_HEIGHT));

	// Display window
	const char* WIN_SRC = "Src"; //window for the source frame
	namedWindow(WIN_SRC, CV_WINDOW_AUTOSIZE);
	cvMoveWindow(WIN_SRC, 0, 0);

	const char* WIN_DST = "Snapped"; //window for showing the snapped frame
	namedWindow(WIN_DST, CV_WINDOW_AUTOSIZE);
	cvMoveWindow(WIN_DST, capS.width + 10, 0);

	char c;
	int frameNum = -1;
	int frameCount = 0;

	for (;;)
	{
		cap >> frame; // get a new frame from camera
		if (frame.empty())
		{
			printf("End of the video file\n");
			break;
		}

		++frameNum;

		imshow(WIN_SRC, frame);

		c = cvWaitKey(10);  // waits a key press to advance to the next frame
		if (c == 27) {
			// press ESC to exit
			printf("ESC pressed - capture finished");
			break;  //ESC pressed
		}
		if (c == 115) { //'s' pressed - snapp the image to a file
			frameCount++;
			fileName[0] = NULL;
			sprintf(numberStr, "%d", frameCount);
			strcat(fileName, "Images/A");
			strcat(fileName, numberStr);
			strcat(fileName, ".bmp");
			bool bSuccess = imwrite(fileName, frame);
			if (!bSuccess)
			{
				printf("Error writing the snapped image\n");
			}
			else
				imshow(WIN_DST, frame);
		}
	}

}

void MyCallBackFunc(int event, int x, int y, int flags, void* param)
{
	//More examples: http://opencvexamples.blogspot.com/2014/01/detect-mouse-clicks-and-moves-on-image.html
	Mat* src = (Mat*)param;
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		printf("Pos(x,y): %d,%d  Color(RGB): %d,%d,%d\n",
			x, y,
			(int)(*src).at<Vec3b>(y, x)[2],
			(int)(*src).at<Vec3b>(y, x)[1],
			(int)(*src).at<Vec3b>(y, x)[0]);
	}
}

void testMouseClick()
{
	Mat src;
	// Read image from file 
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		src = imread(fname);
		//Create a window
		namedWindow("My Window", 1);

		//set the callback function for any mouse event
		setMouseCallback("My Window", MyCallBackFunc, &src);

		//show the image
		imshow("My Window", src);

		// Wait until user press some key
		waitKey(0);
	}
}

/* Histogram display function - display a histogram using bars (simlilar to L3 / PI)
Input:
name - destination (output) window name
hist - pointer to the vector containing the histogram values
hist_cols - no. of bins (elements) in the histogram = histogram image width
hist_height - height of the histogram image
Call example:
showHistogram ("MyHist", hist_dir, 255, 200);
*/
void showHistogram(const std::string& name, int* hist, const int  hist_cols, const int hist_height)
{
	Mat imgHist(hist_height, hist_cols, CV_8UC3, CV_RGB(255, 255, 255)); // constructs a white image

	//computes histogram maximum
	int max_hist = 0;
	for (int i = 0; i < hist_cols; i++)
		if (hist[i] > max_hist)
			max_hist = hist[i];
	double scale = 1.0;
	scale = (double)hist_height / max_hist;
	int baseline = hist_height - 1;

	for (int x = 0; x < hist_cols; x++) {
		Point p1 = Point(x, baseline);
		Point p2 = Point(x, baseline - cvRound(hist[x] * scale));
		line(imgHist, p1, p2, CV_RGB(255, 0, 255)); // histogram bins colored in magenta
	}

	imshow(name, imgHist);
}


///##########################################################################################################################################Project begin

void writeInFile(ofstream &fi, bool x, int n, int bytes) {
	fi.write((char*)&x, sizeof(char));						//Write the color (Black or White)
	if (bytes == 4)
		fi.write((char*)&n, sizeof(int));					//Write the number of occurences (int)
	else if (bytes == 2)
		fi.write((char*)&n, sizeof(unsigned short int));	//Write the number of occurences (unsigned short int)
	else
		fi.write((char*)&n, sizeof(char));					//Write the number of occurences (char)
}

string getCompressionUrl(string imagename, int bytes) {		//Returns a string that is the path to the compression file according to the image name and the number of bytes
	return (bytes == 4) ? "Images/Comp/" + imagename + "_i.dat" : (bytes == 2) ? "Images/Comp/" + imagename + "_sui.dat" : "Images/Comp/" + imagename + "_c.dat";
}

int imageWidth(vector<unsigned char> buffer, int bytes) {				//Computes the width by adding all the occurences until the first end of line in the compression file
	int nr, w = 0;
	for (int i = 0;i < buffer.size() && buffer[i] <= 1;i += bytes+1) {	//Travers until the end of buffer or the first end of line
		nr = 0;
		for (int j = bytes;j >= 1;j--) 
			nr = nr << 8 | static_cast<unsigned char>(buffer[i + j]);	//Get the number of occurences
		w += nr;														//Add the occurences to the width
	}
	return w;
}

int imageHeight(vector<unsigned char> buffer, int bytes) {		//Counts the lines of the compressed file and computes the image height
	int h = 0;
	for (int i = 0;i < buffer.size();i += bytes+1)
		if (buffer[i] == '\n') {
			i -= bytes;
			h++;
		}
	return h;
}

void compressRLE(string imagename, int bytes) {
	string imageurl = "Images/" + imagename + ".bmp";
	string compressionurl = getCompressionUrl(imagename, bytes);		//Get the path to the compression of the image
	Mat_<uchar> input = imread(imageurl, CV_LOAD_IMAGE_GRAYSCALE);

	ofstream fout;
	fout.open(compressionurl, ios::binary | ios::out);					//Open or create the file of the compression

	bool lastLabel;
	int lastLabelNr;
	char buffer = '\n';

	int compSize = 0;

	for (int i = 0;i < input.rows;i++) {
		lastLabel = (int)(input(i, 0)) % 2;
		lastLabelNr = 1;
		for (int j = 1;j < input.cols;j++) {							//Search on every row
			if ((int)(input(i, j)) % 2 == lastLabel)	
				lastLabelNr++;											//If the color is the same as the previous one just increment the number of occurences
			else {
				writeInFile(fout, lastLabel, lastLabelNr, bytes);		//Else write the current information in the compression file and continue with the new color
				compSize += bytes + 1;
				lastLabel = input(i, j);
				lastLabelNr = 1;
			}
		}
		writeInFile(fout, lastLabel, lastLabelNr, bytes);				//When it reaches the end it writes the last information in the compression file
		fout.write(&buffer, sizeof(char));
		compSize += bytes + 2;
	}

	fout.close();
	compSize *= 8;
	cout << "Original: " << input.rows * input.cols << " b / " << input.rows * input.cols / 8 <<" B / "<< input.rows * input.cols / 8192 << " KB\n";
	cout << "Compressed: " << compSize << " b / " << compSize / 8 << " B / " << compSize / 8192 << " KB\n";
	cout << "Compression ratio: " << (100.0 * (float)compSize) / ((float)(input.rows) * (float)(input.cols)) << "%\n";
}

void decompressRLE(string imagename, int bytes) {  
	string compressionurl = getCompressionUrl(imagename, bytes);
	ifstream fin;
	fin.open(compressionurl, ios::binary | ios::in);												//Open the compression file

	vector<unsigned char> buffer(istreambuf_iterator<char>(fin), {});								//Read the file into a buffer
	int ih = imageHeight(buffer, bytes);															//Compute the image height
	int iw = imageWidth(buffer, bytes);																//Compute the image width
	Mat_<uchar> output(ih, iw);

	int nr, pos = 0;
	for (int i = 0;i < buffer.size();i += bytes+1) {												//Traverse the buffer
		if (buffer[i] < 2) {																		//If the current byte represents a color continue
			nr = 0;
			for (int j = bytes;j >= 1;j--)
				nr = nr << 8 | static_cast<unsigned char>(buffer[i + j]);							//Convert the occurences to int

			for (int j = 0;j < nr;j++)																//Color the pixels 
				output((pos + j) / iw, (pos + j) % iw) = (buffer[i] == 0) ? (uchar)0 : (uchar)255;
			pos += nr;
		}
		else
			i -= bytes;
	}
	fin.close();

	imshow("decompressed", output);
	waitKey(0);

}

void showBinFile(string imagename, int bytes) {
	string compressionurl = getCompressionUrl(imagename, bytes);
	ifstream fin;
	fin.open(compressionurl, ios::binary | ios::in);						//Open the compression file
	vector<unsigned char> buffer(istreambuf_iterator<char>(fin), {});		//Read the file into a buffer
	int nr;
	for (int i = 0;i < buffer.size();i += bytes+1) {						//Traverse the buffer
		if (buffer[i] > 1) {												//If we reached the end of a line 
			cout << '\n';
			i -= bytes;
		}
		else {																//else print the color and the occurences
			nr = 0;
			for (int j = bytes;j >= 1;j--)
				nr = nr << 8 | static_cast<unsigned char>(buffer[i + j]);	//Convert the occurences into int
			cout << "(" << (int)buffer[i] << "," << nr << ")";				//Print
		}
	}
	fin.close();
}

///##########################################################################################################################################Project end

void menu()
{
	int n;
	string str;
	cout << "1: Compress" << endl;
	cout << "2: Decompress" << endl;
	cout << "3: Read bin file" << endl;

	cin >> n;

	switch (n)
	{
	case 1:
		cout << "Image name: ";
		cin >> str;
		cout << "Nr of bytes for occurences (1-char, 2-unsigned short int, 4-int): ";
		cin >> n;
		compressRLE(str, n);
		break;
	case 2:
		cout << "Image name: ";
		cin >> str;
		cout << "Nr of bytes for occurences (1-char, 2-unsigned short int, 4-int): ";
		cin >> n;
		decompressRLE(str, n);
		break;
	case 3:
		cout << "Image name: ";
		cin >> str;
		cout << "Nr of bytes for occurences (1-char, 2-unsigned short int, 4-int): ";
		cin >> n;
		showBinFile(str, n);
		break;
	default:
		break;
	}
}

int main()
{
	while (true)
		menu();

	return 0;
}