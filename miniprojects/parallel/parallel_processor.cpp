#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <omp.h>
#include <sys/stat.h>

using namespace cv;
using namespace std;

void createFolder(const string& path) {
    mkdir(path.c_str(), 0777);
}

Mat adjustBrightness(const Mat& frame, double alpha, int beta) {
    Mat adjusted;
    frame.convertTo(adjusted, -1, alpha, beta);
    return adjusted;
}

Mat colorCorrection(const Mat& frame) {
    Mat result;
    Mat lab;
    cvtColor(frame, lab, COLOR_BGR2Lab);
    
    vector<Mat> channels;
    split(lab, channels);
    equalizeHist(channels[0], channels[0]);
    merge(channels, lab);
    cvtColor(lab, result, COLOR_Lab2BGR);
    
    return result;
}

Mat createThumbnail(const Mat& frame, int width, int height) {
    Mat thumbnail;
    resize(frame, thumbnail, Size(width, height));
    return thumbnail;
}

int main() {
    string inputFolder = "frames";
    int numFrames = 185;  // CHANGE THIS to match your frame count
    
    createFolder("processed_bright");
    createFolder("processed_color");
    createFolder("thumbnails");
    
    cout << "Processing " << numFrames << " frames with " << omp_get_max_threads() << " threads..." << endl;
    
    double startTime = omp_get_wtime();
    
    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < numFrames; i++) {
        string framePath = inputFolder + "/frame_" + to_string(i) + ".jpg";
        Mat frame = imread(framePath);
        
        if (frame.empty()) {
            continue;
        }
        
        Mat brightened = adjustBrightness(frame, 1.2, 30);
        string brightPath = "processed_bright/bright_" + to_string(i) + ".jpg";
        imwrite(brightPath, brightened);
        
        Mat corrected = colorCorrection(frame);
        string colorPath = "processed_color/color_" + to_string(i) + ".jpg";
        imwrite(colorPath, corrected);
        
        Mat thumbnail = createThumbnail(frame, 160, 120);
        string thumbPath = "thumbnails/thumb_" + to_string(i) + ".jpg";
        imwrite(thumbPath, thumbnail);
    }
    
    double endTime = omp_get_wtime();
    double totalTime = endTime - startTime;
    
    cout << "Complete! Time: " << totalTime << " sec | Speed: " << (numFrames / totalTime) << " fps" << endl;
    
    return 0;
}

