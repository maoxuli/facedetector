//***********************************************************************************************
//
// Face detector
// with HAAR cascade classifier in OpenCV v2.4.9 or later (from April 2014)
//
// The program detect faces in input image and mark detected face with a rectangle box
// Detected faces can be displayed or stored into files.
//
// Some parts are based on the tutorial & code by Shervin Emami.
//
//***********************************************************************************************

#include "opencv2/opencv.hpp"
#include <iostream>
#include <string>
#include <sstream>

// Cascade Classifier xml files, used for face detection
// Copy the files from OpenCV data folder (eg: '/OpenCV/data/haarcascades')

// Haar face detector.
const char *sFaceCascadeFile = "cascades/haarcascade_frontalface_alt.xml";

// Name shown in the GUI window.
const char *sWindowName = "Face Detector";

std::string MakeFileName(std::string& fileName, int n)
{
    std::ostringstream oss;
    std::string::size_type pos = fileName.find_last_of('.');
    oss << fileName.substr(0, pos) << "_face_" << n << fileName.substr(pos);
    return oss.str();
}

IplImage* CutImage(IplImage* src, CvRect* rect)
{
    cvSetImageROI(src, *rect);
    IplImage* dst = cvCreateImage(cvSize(rect->width, rect->height),
                                  src->depth,
                                  src->nChannels);
    
    cvCopy(src,dst,0);
    cvResetImageROI(src);
    return dst;
}

int main(int argc, char *argv[])
{
    // Load Haar cascade classifier for face detection
    CvHaarClassifierCascade *pHaarCascade = NULL;
    try
    {
        pHaarCascade = (CvHaarClassifierCascade*)cvLoad(sFaceCascadeFile);
    }
    catch (cv::Exception &e)
    {
        std::cerr <<
        "Cascade classifier " << sFaceCascadeFile <<
        " load exception: " << e.what() << std::endl;
    }
    if ( pHaarCascade == NULL )
    {
        std::cerr << "Cascade classifier load failed: " << sFaceCascadeFile << std::endl;
        std::cerr << "Copy file from OpenCV data folder (eg: '/OpenCV/data/haarcascades')" << std::endl;
        exit(1);
    }
    std::cout << "Loaded face detection cascade classifier: " << sFaceCascadeFile << std::endl;
    
    // Path name of source image file
    std::string sImageFile = "test.jpg"; // Default file name
    if (argc > 1)
    {
        sImageFile = argv[1];
    }
    
    // Load image
    IplImage *pSrcImage = NULL;
    try
    {
        pSrcImage = cvLoadImage(sImageFile.c_str(), CV_LOAD_IMAGE_UNCHANGED);
    }
    catch (cv::Exception &e)
    {
        std::cerr <<
        "Image file " << sImageFile <<
        " load exception: " << e.what() << std::endl;
    }
    if(pSrcImage == NULL)
    {
        std::cerr <<
        "Source image load failed."
        " Please check file : " << sImageFile << std::endl;
        exit(1);
    }
    std::cout << "Loaded source image file: " << sImageFile << std::endl;

    // Convert image to gray for fast face detection
	IplImage *pGrayImage = NULL;
    try
    {
        pGrayImage = cvCreateImage(cvGetSize(pSrcImage), IPL_DEPTH_8U, 1);
        cvCvtColor(pSrcImage, pGrayImage, CV_BGR2GRAY);
    }
	catch (cv::Exception &e)
    {
        std::cerr << "Convert image to gray exception: " << e.what() << std::endl;
        cvReleaseImage(&pSrcImage);
        exit(1);
    }
    assert(pGrayImage != NULL);
    
	// Face detection
    try
    {
        CvMemStorage *pcvMStorage = cvCreateMemStorage(0);
		cvClearMemStorage(pcvMStorage);
        
        // Detect
		CvSeq *pcvSeqFaces = cvHaarDetectObjects(pGrayImage, pHaarCascade, pcvMStorage);
        std::cout << pcvSeqFaces->total << " face is detected." << std::endl;
        
		for(int i = 0; i <pcvSeqFaces->total; i++)
		{
            // Cut and store
            CvRect* pRect = (CvRect*)cvGetSeqElem(pcvSeqFaces, i);
            std::string sFaceImageFile = MakeFileName(sImageFile, i + 1);
            IplImage* pFaceImage = CutImage(pSrcImage, pRect);
            if( !cvSaveImage(sFaceImageFile.c_str(), pFaceImage) )
            {
                std::cerr << "Store detected face image failed: " << sFaceImageFile << std::endl;
            }
            std::cout << "Detected face has been stored into file: " << sFaceImageFile << std::endl;
            cvReleaseImage(&pFaceImage);
            
            // Mark
            CvPoint pt1 = {pRect->x, pRect->y};
            CvPoint pt2 = {pRect->x + pRect->width, pRect->y + pRect->height};
            CvScalar color = {0, 0, 255};
            cvRectangle(pSrcImage, pt1, pt2, color);
		}
        
		cvReleaseMemStorage(&pcvMStorage);
	}
    catch (cv::Exception &e)
    {
        std::cout << "Face detetion exception: " << e.what() << std::endl;
    }
    
    cvShowImage(sWindowName, pSrcImage);
    
    std::cout << "Press any key to quit." << std::endl;
    cvWaitKey(0);
    
	cvDestroyWindow(sWindowName);
    cvReleaseImage(&pGrayImage);
	cvReleaseImage(&pSrcImage);

    return 0;
}
