#include <opencv2/opencv.hpp>
#include <iostream>
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
    Mat result, lab;
    cvtColor(frame, lab, COLOR_BGR2Lab);
    vector<Mat> channels;
    split(lab, channels);
    equalizeHist(channels[0], channels[0]);
    merge(channels, lab);
    cvtColor(lab, result, COLOR_Lab2BGR);
    return result;
}

Mat createThumbnail(const Mat& frame, int w, int h) {
    Mat thumbnail;
    resize(frame, thumbnail, Size(w, h));
    return thumbnail;
}

double sequential(int numFrames) {
    createFolder("seq_bright");
    createFolder("seq_color");
    createFolder("seq_thumb");
    
    double start = omp_get_wtime();
    
    for (int i = 0; i < numFrames; i++) {
        Mat frame = imread("frames/frame_" + to_string(i) + ".jpg");
        if (frame.empty()) continue;
        
        imwrite("seq_bright/bright_" + to_string(i) + ".jpg", adjustBrightness(frame, 1.2, 30));
        imwrite("seq_color/color_" + to_string(i) + ".jpg", colorCorrection(frame));
        imwrite("seq_thumb/thumb_" + to_string(i) + ".jpg", createThumbnail(frame, 160, 120));
    }
    
    double time = omp_get_wtime() - start;
    cout << "Sequential: " << time << " sec | " << (numFrames / time) << " fps" << endl;
    return time;
}

double parallel(int numFrames) {
    createFolder("par_bright");
    createFolder("par_color");
    createFolder("par_thumb");
    
    double start = omp_get_wtime();
    
    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < numFrames; i++) {
        Mat frame = imread("frames/frame_" + to_string(i) + ".jpg");
        if (frame.empty()) continue;
        
        imwrite("par_bright/bright_" + to_string(i) + ".jpg", adjustBrightness(frame, 1.2, 30));
        imwrite("par_color/color_" + to_string(i) + ".jpg", colorCorrection(frame));
        imwrite("par_thumb/thumb_" + to_string(i) + ".jpg", createThumbnail(frame, 160, 120));
    }
    
    double time = omp_get_wtime() - start;
    cout << "Parallel (" << omp_get_max_threads() << " threads): " << time << " sec | " << (numFrames / time) << " fps" << endl;
    return time;
}

int main() {
    int numFrames = 185;
    int numThreads = omp_get_max_threads();
    
    cout << "Frames: " << numFrames << endl;
    cout << "Threads: " << numThreads << endl;
    cout << endl;
    
    double seqTime = sequential(numFrames);
    cout << endl;
    
    double parTime = parallel(numFrames);
    cout << endl;
    
    double speedup = seqTime / parTime;
    double efficiency = (speedup / numThreads) * 100.0;
    double overhead = parTime - (seqTime / numThreads);
    
    cout << "Performance Metrics:" << endl;
    cout << "Speedup: " << speedup << "x" << endl;
    cout << "Efficiency: " << efficiency << "%" << endl;
    cout << "Overhead: " << overhead << " sec" << endl;
    
    return 0;
}

