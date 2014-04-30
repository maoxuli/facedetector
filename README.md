Face Detector
=============

This program is a face detector based on OpenCV. It detects faces in input image file and store the face region into another image file. 

### Developement and Running Platform

It is currently compiled and tested on:
* OpenCV 2.4.9 
* Xcode 5.1.1
* Mac OS X Mavericks (10.9.2) 

It may be compiled on other versions of OpenCV, Xcode, and Mac OS X. 

It will be compiled and tested on Windows and Linux later as well.

### Compile on Xcode and Mac OS X

Open the Xcode project file in root folder and do compilation. The output executable file will be copied to "bin folder automatically. 

The compilation depends on OpenCV installed on the system. By default, the project search OpenCV header files in "/usr/local/include" and OpenCV libraries in "/usr/local/lib".

### Running and Testing

Running of the program depends on OpenCV libraries installed in "/usr/local/lib". In addition, the program loads a default HAAR cascade classifier xml file from folder "bin/cascades/". The file name is "haarcascade_frontalface_alt.xml". Please copy the file from OpenCV data folder, it is "OpenCV/data/haarcascades" by default. 

#### Running program with no argument: 

./facedetector

The program will load a default image in current folder with name "test.jpg". Please check and make sure the file is exist. If both face detection cascade classifier and image are loaded successfully, the program detects faces in the image, mark the face region with a red rectangle box, and store the face region into another file. Suppose the source image file name is "name.jpg", the detected face image file will be "name_face_n.jpg", where n is the sequence number of faces detected in the image. 

##### Running program with one argument: 

./facedetector test.jpg

The program will load the image given by argument.  