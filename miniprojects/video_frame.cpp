#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <sys/stat.h>

using namespace cv;
using namespace std;

void createFolder(const string& path) {
    mkdir(path.c_str(), 0777);
}

int main() {
    // ---- EDIT THIS PATH TO YOUR VIDEO FILE ----
    string inputpath = "video.mp4";
    string outputpath = "frames";
    
    VideoCapture cap(inputpath);
    if (!cap.isOpened()) {
        cerr << "Error: Cannot open video file: " << inputpath << endl;
        return -1;
    }
    
    createFolder(outputpath);
    
    int totalFrames = cap.get(CAP_PROP_FRAME_COUNT);
    double fps = cap.get(CAP_PROP_FPS);
    int width = cap.get(CAP_PROP_FRAME_WIDTH);
    int height = cap.get(CAP_PROP_FRAME_HEIGHT);
    
    cout << "========== Video Info ==========" << endl;
    cout << "Total Frames: " << totalFrames << endl;
    cout << "FPS: " << fps << endl;
    cout << "Resolution: " << width << "x" << height << endl;
    cout << "================================" << endl;
    
    int frameCount = 0;
    Mat frame;
    
    cout << "Starting frame extraction..." << endl;
    
    while (cap.read(frame)) {
        string filename = outputpath + "/frame_" + to_string(frameCount) + ".jpg";
        imwrite(filename, frame);
        
        frameCount++;
        if (frameCount % 10 == 0) {
            cout << "Extracted: " << frameCount << "/" << totalFrames << " frames" << endl;
        }
    }
    
    cout << "\n========== Extraction Complete! ==========" << endl;
    cout << "Total frames extracted: " << frameCount << endl;
    cout << "Output directory: " << outputpath << "/" << endl;
    
    cap.release();
    
    return 0;
}

