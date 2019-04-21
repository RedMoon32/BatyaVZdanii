# Edge detection for PPM images
This project is my solution to OMP test problem
### Description

[Netpbm formats](https://en.wikipedia.org/wiki/Netpbm_format) are image file formats designed to be easily exchanged between platforms: PPM (P3 P6), PGM (P2 P5), PNM (P1 P4). 


[Sobel operator](https://en.wikipedia.org/wiki/Sobel_operator) is special edge detecting algorithm invented by Irwin Sobel and Gary Feldman

### Project overview
This project is implementation of parallel sobel operator for ppm images.
It supports saving/editing/reading of P3/P6 (PPM) file formats, as well as applying sobel operator to them 
and converting rgb to grayscale (or back)

**Libraries**:  
*ppm.h* - reading/editing/saving of PPM images  
*sobel.h* - image processing: converting rgb to grayscale(or back),  
applying sobel operator to grayscale images

### Building & Running
**Requirements**: make, gcc utilites      
**Builing** project is pretty simple: just run `make` in project's root folder, this command wil create executable `netpbm-sobel` in project's root folder.  
**Running** project:
* Configuration for running is `./netpbm-sobel -i [input_file_path] -o [output_file_path] -p [result file mode(either P3 or P6)] -t [number of threads (from 1 to 16)]`  
By default two threads are used  
* **Example** of running the project with engine image from test directory and saving as P6 image:  
`./netpbm-sobel -i ../tests/engine.ppm -o ../tests/result.ppm -t 4 -p P6`   

