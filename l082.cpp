// cd project8part2
// cmake .
// make
// ./l082

#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <math.h>
#define _USE_MATH_DEFINES

using namespace std;
using namespace cv;

void part1() {
    int videoWidth = 800;
    int videoHeight = 600;
    int fps = 4;
    //int scale = 200;
    int xtranslation = 400;
    int ytranslation = 300;

    VideoWriter writer;
    //writer.open("rotation.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, Size(videoWidth, videoHeight));

    //working
    int fpm = 30;
    writer.open("rotation.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), fpm, Size(videoWidth, videoHeight));
    //Mat frame = Mat(videoHeight, videoWidth, CV_8UC3, Scalar(0, 0, 0));

    ofstream fout;
    fout.open("coordinates.txt");

    //vector<cv::Mat> images;
    //int sz[] = {images[0].rows, images[0].cols, images.size()};
    //cv::Mat cube(3, sz, images[0].type());
    double scale[4][4] = {{100, 0, 0, 0}, {0, 100, 0, 0}, {0, 0, 100, 0}, {0, 0, 0, 1}};
    Mat scalemat = Mat(4 , 4, CV_64FC1, scale);
    
    //store points like this
    double point1[4] = { -1, -1, -1, 1 };
    double point2[4] = { 1, -1, -1, 1 };
    double point3[4] = { 1, 1, -1, 1 };
    double point4[4] = { -1, 1, -1, 1 };
    double point5[4] = { -1, -1, 1, 1 };
    double point6[4] = { 1, -1, 1, 1 };
    double point7[4] = { 1, 1, 1, 1 };
    double point8[4] = { -1, 1, 1, 1 };
    
    cv::Mat p1(4, 1, CV_64FC1, point1);
    cv::Mat p2(4, 1, CV_64FC1, point2);
    cv::Mat p3(4, 1, CV_64FC1, point3);
    cv::Mat p4(4, 1, CV_64FC1, point4);
    cv::Mat p5(4, 1, CV_64FC1, point5);
    cv::Mat p6(4, 1, CV_64FC1, point6);
    cv::Mat p7(4, 1, CV_64FC1, point7);
    cv::Mat p8(4, 1, CV_64FC1, point8);
    
    /*vector <Mat> originalCube;
    originalCube.push_back(p1);
    originalCube.push_back(p2);
    originalCube.push_back(p3);
    originalCube.push_back(p4);
    originalCube.push_back(p5);
    originalCube.push_back(p6);
    originalCube.push_back(p7);
    originalCube.push_back(p8);*/
    
    vector<Mat> newCube;
    newCube.push_back(scalemat * p1);
    newCube.push_back(scalemat * p2);
    newCube.push_back(scalemat * p3);
    newCube.push_back(scalemat * p4);
    newCube.push_back(scalemat * p5);
    newCube.push_back(scalemat * p6);
    newCube.push_back(scalemat * p7);
    newCube.push_back(scalemat * p8);
    
    /*for (int i = 0; i < newCube.size(); i++) {
        double x = newCube[i].at<double>(0, 0);
        double y = newCube[i].at<double>(1, 0);
        double z = newCube[i].at<double>(2, 0);

        std::cout << x << " " << y << " " << z << "\n";
    }*/
    
    fout << "Cube Coordinates (4 frames):" << endl;

    for (int i = 0; i < newCube.size() - 1; i++){
        fout << "(" << newCube[i].at<double>(0, 0) << ", " << newCube[i].at<double>(0, 1) << ", " << newCube[i].at<double>(0, 2) << "), ";
    }
    fout << endl;
    
    for (int angle = 0; angle <= 360; angle+=3) {
        double multfactor = angle * 3.14159265358979323 / 180;
    
        double rotationArrayX[4][4] = {{1, 0, 0, 0}, {0, cos(multfactor), sin(multfactor), 0}, {0, -1*sin(multfactor), cos(multfactor), 0}, {0, 0, 0, 1}}; 
        double rotationArrayY[4][4] = {{cos(multfactor), 0, -1*sin(multfactor), 0}, {0, 1, 0, 0}, {sin(multfactor), 0, cos(multfactor), 0}, {0, 0, 0, 1}}; 
        cv::Mat rotationXmat(4, 4, CV_64FC1, rotationArrayX);
        cv::Mat rotationYmat(4, 4, CV_64FC1, rotationArrayY);
        
        double x, y, z;
        std::vector<Point> points;
        std::vector<std::vector<double>> pointcoordinates;
        
        for (int i = 0; i < newCube.size(); i++) {
            Mat m = rotationXmat * rotationYmat * newCube[i];
            x = m.at<double>(0, 0);
            y = m.at<double>(1, 0);
            z = m.at<double>(2, 0);
            
            //std::cout << x << " " << y << " " << z << "\n";
            points.push_back(Point(x + xtranslation, y + ytranslation));
            pointcoordinates.push_back({x + xtranslation, y + ytranslation, z});
            if(angle < 10 && angle %3 == 0){
                fout << "(" << x << ", " << y << ", " << z << "), ";
            }
            fout << endl;
        }
        
        //std::cout << "\n";
        
        //draw here
        Mat frame = Mat(videoHeight, videoWidth, CV_8UC3, Scalar(0, 0, 0));

        Point p1 = points[0];
        Point p2 = points[1];
        Point p3 = points[2];
        Point p4 = points[3];
        Point p5 = points[4];
        Point p6 = points[5];
        Point p7 = points[6];
        Point p8 = points[7];
                
        for(int k = 0; k < points.size(); k++)
            circle(frame, points[k], 3, Scalar(0, 255, 0), 20);

        line(frame, p1, p2, Scalar(0, 255, 0), 5, LINE_8);
        line(frame, p2, p3, Scalar(0, 255, 0), 5, LINE_8);
        line(frame, p3, p4, Scalar(0, 255, 0), 5, LINE_8);
        line(frame, p4, p1, Scalar(0, 255, 0), 5, LINE_8);
        line(frame, p5, p6, Scalar(0, 255, 0), 5, LINE_8);
        line(frame, p6, p7, Scalar(0, 255, 0), 5, LINE_8);
        line(frame, p7, p8, Scalar(0, 255, 0), 5, LINE_8);
        line(frame, p8, p5, Scalar(0, 255, 0), 5, LINE_8);
        line(frame, p1, p5, Scalar(0, 255, 0), 5, LINE_8);
        line(frame, p2, p6, Scalar(0, 255, 0), 5, LINE_8);
        line(frame, p3, p7, Scalar(0, 255, 0), 5, LINE_8);
        line(frame, p4, p8, Scalar(0, 255, 0), 5, LINE_8);
        
        writer.write(frame);
    }
    fout << endl;

    writer.release();
    destroyAllWindows();
}

void cube(VideoWriter writer){
    int xtranslation = 400;
    int ytranslation = 300;
    
    int eye = 700;
    int plane = 500;
    
    ofstream fout;
    fout.open("coordinates.txt");

    double scale[4][4] = {{200, 0, 0, 0}, {0, 200, 0, 0}, {0, 0, 200, 0}, {0, 0, 0, 1}};
    Mat scalemat = Mat(4 , 4, CV_64FC1, scale);
    
    //store points like this
    double point1[4] = { -1, -1, -1, 1 };
    double point2[4] = { 1, -1, -1, 1 };
    double point3[4] = { 1, 1, -1, 1 };
    double point4[4] = { -1, 1, -1, 1 };
    double point5[4] = { -1, -1, 1, 1 };
    double point6[4] = { 1, -1, 1, 1 };
    double point7[4] = { 1, 1, 1, 1 };
    double point8[4] = { -1, 1, 1, 1 };
    
    cv::Mat p1(4, 1, CV_64FC1, point1);
    cv::Mat p2(4, 1, CV_64FC1, point2);
    cv::Mat p3(4, 1, CV_64FC1, point3);
    cv::Mat p4(4, 1, CV_64FC1, point4);
    cv::Mat p5(4, 1, CV_64FC1, point5);
    cv::Mat p6(4, 1, CV_64FC1, point6);
    cv::Mat p7(4, 1, CV_64FC1, point7);
    cv::Mat p8(4, 1, CV_64FC1, point8);
    
    vector<Mat> newCube;
    newCube.push_back(scalemat * p1);
    newCube.push_back(scalemat * p2);
    newCube.push_back(scalemat * p3);
    newCube.push_back(scalemat * p4);
    newCube.push_back(scalemat * p5);
    newCube.push_back(scalemat * p6);
    newCube.push_back(scalemat * p7);
    newCube.push_back(scalemat * p8);
    
    fout << "Cube Coordinates (4 frames):" << endl;

    for (int i = 0; i < newCube.size() - 1; i++){
        fout << "(" << newCube[i].at<double>(0, 0) << ", " << newCube[i].at<double>(0, 1) << ", " << newCube[i].at<double>(0, 2) << "), ";
    }
    fout << endl;
    
    for (int angle = 0; angle <= 360; angle+=3) {
        double multfactor = angle * 3.14159265358979323 / 180;
    
        double rotationArrayX[4][4] = {{1, 0, 0, 0}, {0, cos(multfactor), sin(multfactor), 0}, {0, -1*sin(multfactor), cos(multfactor), 0}, {0, 0, 0, 1}}; 
        double rotationArrayY[4][4] = {{cos(multfactor), 0, -1*sin(multfactor), 0}, {0, 1, 0, 0}, {sin(multfactor), 0, cos(multfactor), 0}, {0, 0, 0, 1}}; 
        cv::Mat rotationXmat(4, 4, CV_64FC1, rotationArrayX);
        cv::Mat rotationYmat(4, 4, CV_64FC1, rotationArrayY);
        
        double x, y, z;
        std::vector<Point> points;
        std::vector<std::vector<double>> pointcoordinates;
        
        for (int i = 0; i < newCube.size(); i++) {
            Mat m = rotationXmat * rotationYmat * newCube[i];
            x = m.at<double>(0, 0);
            y = m.at<double>(1, 0);
            z = m.at<double>(2, 0);
            
            double t = (plane - eye) / (x - eye);
            double newX = y * t;
            double newY = z * t;
            
            points.push_back(Point(newX + xtranslation, newY + ytranslation));
            pointcoordinates.push_back({newX + xtranslation, newY + ytranslation, z});
            if(angle < 10 && angle %3 == 0){
                fout << "(" << newX << ", " << newY << ", " << z << "), ";
            }
            fout << endl;
        }
                
        //draw here
        Mat frame = Mat(600, 800, CV_8UC3, Scalar(0, 0, 0));

        Point p1 = points[0];
        Point p2 = points[1];
        Point p3 = points[2];
        Point p4 = points[3];
        Point p5 = points[4];
        Point p6 = points[5];
        Point p7 = points[6];
        Point p8 = points[7];
                
        for(int k = 0; k < points.size(); k++)
            circle(frame, points[k], 3, Scalar(0, 255, 0), 20);

        line(frame, p1, p2, Scalar(0, 255, 0), 5, LINE_8);
        line(frame, p2, p3, Scalar(0, 255, 0), 5, LINE_8);
        line(frame, p3, p4, Scalar(0, 255, 0), 5, LINE_8);
        line(frame, p4, p1, Scalar(0, 255, 0), 5, LINE_8);
        line(frame, p5, p6, Scalar(0, 255, 0), 5, LINE_8);
        line(frame, p6, p7, Scalar(0, 255, 0), 5, LINE_8);
        line(frame, p7, p8, Scalar(0, 255, 0), 5, LINE_8);
        line(frame, p8, p5, Scalar(0, 255, 0), 5, LINE_8);
        line(frame, p1, p5, Scalar(0, 255, 0), 5, LINE_8);
        line(frame, p2, p6, Scalar(0, 255, 0), 5, LINE_8);
        line(frame, p3, p7, Scalar(0, 255, 0), 5, LINE_8);
        line(frame, p4, p8, Scalar(0, 255, 0), 5, LINE_8);
        
        writer.write(frame);
    }
    fout << endl;
    
    writer.release();
    destroyAllWindows();
}

void tetrahedron(VideoWriter writer){
    int xtranslation = 400;
    int ytranslation = 300;
    
    int eye = 700;
    int plane = 500;

    double scale[4][4] = {{200, 0, 0, 0}, {0, 200, 0, 0}, {0, 0, 200, 0}, {0, 0, 0, 1}};
    Mat scalemat = Mat(4 , 4, CV_64FC1, scale);
    
    //store points like this
    double point1[4] = { 0, 0, 2/sqrt(6), 1 };
    double point2[4] = { 1/sqrt(3), 0, 0, 1 };
    double point3[4] = { -sqrt(3)/6, 1/2, 0, 1 };
    double point4[4] = { -sqrt(3)/6, -1/2, 0, 1 };
    
    cv::Mat p1(4, 1, CV_64FC1, point1);
    cv::Mat p2(4, 1, CV_64FC1, point2);
    cv::Mat p3(4, 1, CV_64FC1, point3);
    cv::Mat p4(4, 1, CV_64FC1, point4);
    
    vector<Mat> newCube;
    newCube.push_back(scalemat * p1);
    newCube.push_back(scalemat * p2);
    newCube.push_back(scalemat * p3);
    newCube.push_back(scalemat * p4);
    
    for (int angle = 0; angle <= 360; angle+=3) {
        double multfactor = angle * 3.14159265358979323 / 180;
    
        double rotationArrayX[4][4] = {{1, 0, 0, 0}, {0, cos(multfactor), sin(multfactor), 0}, {0, -1*sin(multfactor), cos(multfactor), 0}, {0, 0, 0, 1}};
        double rotationArrayY[4][4] = {{cos(multfactor), 0, -1*sin(multfactor), 0}, {0, 1, 0, 0}, {sin(multfactor), 0, cos(multfactor), 0}, {0, 0, 0, 1}};
        cv::Mat rotationXmat(4, 4, CV_64FC1, rotationArrayX);
        cv::Mat rotationYmat(4, 4, CV_64FC1, rotationArrayY);
        
        double x, y, z;
        std::vector<Point> points;
        std::vector<std::vector<double>> pointcoordinates;
        
        for (int i = 0; i < newCube.size(); i++) {
            Mat m = rotationXmat * rotationYmat * newCube[i];
            x = m.at<double>(0, 0);
            y = m.at<double>(1, 0);
            z = m.at<double>(2, 0);
            
            //double newX = x;
            //double newY = y;
            
            double t = (plane - eye) / (x - eye);
            double newX = y * t;
            double newY = z * t;
            points.push_back(Point(newX + xtranslation, newY + ytranslation));
            pointcoordinates.push_back({newX + xtranslation, newY + ytranslation, z});
        }
                
        //draw here
        Mat frame = Mat(600, 800, CV_8UC3, Scalar(0, 0, 0));

        Point p1 = points[0];
        Point p2 = points[1];
        Point p3 = points[2];
        Point p4 = points[3];
                
        for(int k = 0; k < points.size(); k++)
            circle(frame, points[k], 3, Scalar(0, 255, 0), 20);

        line(frame, p3, p1, Scalar(0, 255, 0), 5, LINE_8);
        line(frame, p3, p2, Scalar(0, 255, 0), 5, LINE_8);
        line(frame, p3, p4, Scalar(0, 255, 0), 5, LINE_8);
        line(frame, p1, p2, Scalar(0, 255, 0), 5, LINE_8);
        line(frame, p2, p4, Scalar(0, 255, 0), 5, LINE_8);
        line(frame, p4, p1, Scalar(0, 255, 0), 5, LINE_8);

        writer.write(frame);
    }
    
    writer.release();
    destroyAllWindows();
}

void part2() {
    VideoWriter writer;
    writer.open("rotation.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 30, Size(800, 600));
    cube(writer);
    tetrahedron(writer);
}

int main(int argc, char* argv[]) {
    //part1();
    part2();
}

// eye = 12 <-  x value
// plane is in between cube and eye -> 5
// double t = (plane - eye) / (x - eye);
// double newX = y * t + xtranslation;
// double newY = z * t + ytranslation;