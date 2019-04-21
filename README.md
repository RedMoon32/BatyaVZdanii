# Edge detection for PPM images

### Description

[Netpbm formats](https://en.wikipedia.org/wiki/Netpbm_format) are image file formats designed to be easily exchanged between platforms: PPM (P3 P6), PGM (P2 P5), PNM (P1 P4). 


[Sobel operator](https://en.wikipedia.org/wiki/Sobel_operator) is special edge detecting algorithm invented by Irwin Sobel and Gary Feldman

### Project overview
This project is implementation of parallel sobel operator for ppm images.
Program reads PPM (P3/P6) image from file, applies sobel and saves it in PPM or PGM(P2/P5) format

**Libraries**:  
*ppm.h* - image structs & file operations: read PPM image, save to PPM/PGM  
*sobel.h* - image processing: converting rgb to grayscale(and back), applying sobel operator to grayscale images

### Building & Running
**Requirements**: make, gcc utilites      
**Builing** project is pretty simple: first create build directory(`mkdir build`)  ,
then run `make` in project's root folder - this command wil create executable `netpbm-sobel` in `build` folder.  
**Running** project:
1) Go to build directory: `cd build`
2) Configuration for running is `./netpbm-sobel -i [input_file_path] -o [output_file_path] (-s [result file mode(P2/P3/P5/P6)] -t [number of threads (from 1 to 16)])`  
By default one thread is used and saving mode is P2 (PGM)
* **Example** of running the project with engine image from test directory and saving it as P5 grayscale image:  
`./netpbm-sobel -i ../tests/engine.ppm -o ../tests/result.ppm -s P5 -t 4`   

### Notes
* Please ensure that #comments are absent in input image file
