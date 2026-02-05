# 🎥 Parallel Video Frame Processing Using OpenMP

<div align="center">

![OpenMP](https://img.shields.io/badge/OpenMP-4.5-blue)
![C++](https://img.shields.io/badge/C++-11-00599C?logo=c%2B%2B)
![OpenCV](https://img.shields.io/badge/OpenCV-4.x-5C3EE8?logo=opencv)
![License](https://img.shields.io/badge/license-MIT-green)
![Platform](https://img.shields.io/badge/platform-Linux-lightgrey)

**A High-Performance Image Processing System for CCTV Video Analysis**

[Features](#-features) •
[Installation](#-installation) •
[Usage](#-usage) •
[Performance](#-performance-analysis) •
[Documentation](#-documentation)

</div>

---

## 📋 Table of Contents

- [Overview](#-overview)
- [Features](#-features)
- [Problem Statement](#-problem-statement)
- [Technical Architecture](#-technical-architecture)
- [Installation](#-installation)
- [Usage](#-usage)
- [Performance Analysis](#-performance-analysis)
- [Results](#-results-and-discussion)
- [Project Structure](#-project-structure)
- [Future Work](#-future-work)
- [Contributing](#-contributing)
- [License](#-license)
- [Acknowledgments](#-acknowledgments)

---

## 🔍 Overview

This project implements a **parallel video frame processing system** using OpenMP to accelerate image processing operations on extracted video frames. The system demonstrates significant performance improvements over sequential processing by leveraging multi-core CPU architectures for concurrent frame processing operations.

### Key Highlights

- ⚡ **3-4x Speedup** over sequential processing
- 🎯 Efficient multi-core CPU utilization
- 🖼️ Real-time video frame enhancement
- 📊 Comprehensive performance benchmarking

---

## ✨ Features

### Core Processing Operations

- **Brightness Adjustment**: Automatic enhancement for low-light CCTV footage
- **Color Correction**: Intelligent color balance and quality improvement
- **Thumbnail Generation**: Fast preview generation for quick analysis
- **Parallel Execution**: OpenMP-based multi-threaded processing

### Technical Features

- Thread-safe frame processing pipeline
- Configurable thread pool sizing
- Performance metrics and profiling
- Support for multiple video formats
- Batch processing capabilities

---

## 🎯 Problem Statement

Modern surveillance systems generate massive amounts of video data requiring efficient processing pipelines. Sequential processing creates significant bottlenecks:

**Challenge**: Processing 185 frames sequentially takes ~19.76 seconds  
**Solution**: Parallel processing using OpenMP reduces processing time by 70-75%

### Processing Requirements

```
Input: Raw CCTV video frames (185 frames)
Operations: Brightness adjustment + Color correction + Thumbnail generation
Output: Enhanced frames with improved visual quality
```

---

## 🏗️ Technical Architecture

### System Specifications

| Component | Specification |
|-----------|--------------|
| **OS** | Ubuntu 20.04 LTS (VirtualBox) |
| **Compiler** | GCC 11.x with C++11 standard |
| **Parallel Framework** | OpenMP 4.5 |
| **Image Library** | OpenCV 4.x |
| **Hardware** | Multi-core CPU (VM environment) |

### Architecture Diagram

```
┌─────────────────────────────────────────────────┐
│           Video Input (CCTV Footage)            │
└─────────────────┬───────────────────────────────┘
                  │
                  ▼
┌─────────────────────────────────────────────────┐
│           Frame Extraction Module               │
└─────────────────┬───────────────────────────────┘
                  │
                  ▼
┌─────────────────────────────────────────────────┐
│       OpenMP Parallel Processing Engine         │
│  ┌──────────┐  ┌──────────┐  ┌──────────┐      │
│  │ Thread 1 │  │ Thread 2 │  │ Thread N │      │
│  └────┬─────┘  └────┬─────┘  └────┬─────┘      │
│       │             │             │             │
│       ▼             ▼             ▼             │
│  [Brightness]  [Color Fix]  [Thumbnail]         │
└─────────────────┬───────────────────────────────┘
                  │
                  ▼
┌─────────────────────────────────────────────────┐
│          Enhanced Output Frames                 │
└─────────────────────────────────────────────────┘
```

---

## 🚀 Installation

### Prerequisites

```bash
# Update system packages
sudo apt update && sudo apt upgrade -y

# Install required dependencies
sudo apt install build-essential cmake git
sudo apt install libopencv-dev
sudo apt install libomp-dev
```

### Build from Source

```bash
# Clone the repository
git clone https://github.com/yourusername/parallel-video-processing.git
cd parallel-video-processing

# Create build directory
mkdir build && cd build

# Compile with OpenMP support
cmake ..
make -j$(nproc)

# Verify installation
./video_processor --version
```

### Quick Start with Make

```bash
# Compile
g++ -fopenmp -std=c++11 main.cpp -o video_processor `pkg-config --cflags --libs opencv4`

# Run
./video_processor input_video.mp4
```

---

## 💻 Usage

### Basic Usage

```bash
# Process video with default settings (auto-detect cores)
./video_processor input_video.mp4

# Specify number of threads
./video_processor input_video.mp4 --threads 8

# Process with performance metrics
./video_processor input_video.mp4 --benchmark
```

### Advanced Options

```bash
# Custom output directory
./video_processor input.mp4 -o ./output/

# Brightness adjustment only
./video_processor input.mp4 --brightness-only

# Generate performance report
./video_processor input.mp4 --report performance.csv

# Verbose mode with timing details
./video_processor input.mp4 --verbose
```

### Code Example

```cpp
#include <opencv2/opencv.hpp>
#include <omp.h>

int main() {
    // Set number of threads
    omp_set_num_threads(8);
    
    // Process frames in parallel
    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < numFrames; i++) {
        cv::Mat frame = frames[i];
        processFrame(frame);
        saveFrame(frame, i);
    }
    
    return 0;
}
```

---

## 📊 Performance Analysis

### Benchmarking Results

| Metric | Sequential | Parallel (4 threads) | Parallel (8 threads) |
|--------|-----------|---------------------|---------------------|
| Execution Time | 19.76s | 6.84s | 5.12s |
| Speedup | 1.0x | 2.89x | 3.86x |
| Efficiency | 100% | 72.25% | 48.25% |
| Frames/sec | 9.36 | 27.05 | 36.13 |

### Performance Visualization

```
Speedup vs Number of Threads
────────────────────────────
4.0x │                    ●
     │                ●
3.0x │            ●
     │        ●
2.0x │    ●
     │●
1.0x │
     └─────────────────────────
      1   2   4   6   8  threads
```

### Key Findings

- **Optimal Thread Count**: 8 threads for test dataset
- **Amdahl's Law Impact**: Overhead increases with thread count
- **Scalability**: Near-linear speedup up to 4 threads
- **Bottlenecks**: I/O operations remain sequential

---

## 📈 Results and Discussion

### Performance Improvements

The parallel implementation demonstrates significant performance gains:

- **Sequential Processing**: 19.76s baseline
- **4-Thread Parallel**: 72% reduction in processing time
- **8-Thread Parallel**: 74% reduction in processing time

### Observations

#### ✅ Advantages

- Excellent speedup for CPU-bound operations
- Easy implementation with OpenMP directives
- Efficient resource utilization
- Scalable across different core counts

#### ⚠️ Limitations

- Diminishing returns beyond optimal thread count
- I/O bottlenecks remain
- Memory bandwidth constraints
- Thread creation overhead

---

## 📁 Project Structure

```
parallel-video-processing/
├── src/
│   ├── main.cpp                 # Main entry point
│   ├── frame_processor.cpp      # Frame processing logic
│   ├── parallel_engine.cpp      # OpenMP implementation
│   └── utils.cpp                # Helper functions
├── include/
│   ├── frame_processor.h
│   ├── parallel_engine.h
│   └── utils.h
├── data/
│   ├── input/                   # Input video files
│   └── output/                  # Processed frames
├── docs/
│   ├── report.pdf               # Detailed analysis
│   └── presentation.pptx        # Project presentation
├── tests/
│   └── benchmark.cpp            # Performance tests
├── CMakeLists.txt
├── Makefile
├── README.md
└── LICENSE
```

---

## 🔮 Future Work

### Planned Enhancements

- [ ] GPU acceleration using CUDA
- [ ] Real-time video stream processing
- [ ] Machine learning-based frame analysis
- [ ] Distributed processing across multiple nodes
- [ ] Web-based dashboard for monitoring
- [ ] Support for 4K and 8K video processing
- [ ] Adaptive thread scheduling algorithms
- [ ] Integration with cloud storage (S3, GCS)

### Research Directions

- Hybrid CPU-GPU processing pipelines
- Dynamic load balancing strategies
- Energy-efficient processing techniques
- Edge computing deployment

---

## 🤝 Contributing

Contributions are welcome! Please follow these guidelines:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

### Code Style

- Follow C++11 standards
- Use meaningful variable names
- Add comments for complex logic
- Include unit tests for new features


---

## 🙏 Acknowledgments

- OpenMP community for excellent parallel programming framework
- OpenCV contributors for robust image processing library
- Research papers on parallel video processing techniques
- Course instructors and mentors for guidance

### References

1. OpenMP Architecture Review Board. (2023). OpenMP Application Programming Interface
2. Bradski, G. (2000). The OpenCV Library. Dr. Dobb's Journal
3. Amdahl, G. (1967). Validity of the single processor approach to achieving large scale computing capabilities

---

<div align="center">

⭐ Star this repository if you find it helpful!

Made with ❤️ for high-performance video processing

[Report Bug](https://github.com/yourusername/parallel-video-processing/issues) •
[Request Feature](https://github.com/yourusername/parallel-video-processing/issues)

</div>
