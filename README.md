# Color Calibration

## Algorithm for color calibrating images.

The goal is to have the same color representation on each image, regardless of lighting conditions, camera models, etc.
This can be achieved by using color calibration chart with known values (https://en.wikipedia.org/wiki/ColorChecker).
Colors on the chart will be different on each image. This values are then used to calculate a mapping function between measured and true color values.
This mapping function is then used to color calibrate entire image.

## How it works?

<p>Mapping function is represented as a 3x3 matrix:</p>
<p>
  M=<br>
  [p1 p2 p3]<br>
  [p4 p5 p6]<br>
  [p7 p8 p9]
 </p>

<p>To color calibrate image, each pixel P is multiplied with mapping function M to get new pixel P'</p>

P = [R G B]<br>
P' = [R' G' B']<br>
P' = M * P

<p>The main question is: How to obtain the mapping function?</b>
The simplest solution is to use color chart with 3 known colors, each consiting of 3 values for red, green and blue channel.
Mapping parameters for each channel can be computed separately, having 3 unknowns and 3 equations.


## Example


<p>Original, altered and calibrated image</p>

<img src="https://github.com/adrianzgaljic/ColorCalibration/blob/master/cmake-build-debug/original.jpg" alt="drawing" width="200"/>   <img src="https://github.com/adrianzgaljic/ColorCalibration/blob/master/cmake-build-debug/processed_image.jpg" alt="drawing" width="200"/>  <img src="https://github.com/adrianzgaljic/ColorCalibration/blob/master/cmake-build-debug/calibrated.jpg" alt="drawing" width="200"/>

<img src="https://github.com/adrianzgaljic/ColorCalibration/blob/master/cmake-build-debug/example.png" alt="drawing" width="400"/>


## Next steps

### Using a neural network 

Neural netwrok seems to produce fair results but is unacceptably slow because it needs to be trained in real time.
Project can be found here: https://github.com/adrianzgaljic/ColorCalibratorNN
Here are results using 3, 4 and 5 colors:


<img src="https://github.com/adrianzgaljic/ColorCalibration/blob/master/cmake-build-debug/image_fixed_3_colors.jpg" alt="drawing" width="200"/>   <img src="https://github.com/adrianzgaljic/ColorCalibration/blob/master/cmake-build-debug/image_fixed_4_colors.jpg" alt="drawing" width="200"/>  <img src="https://github.com/adrianzgaljic/ColorCalibration/blob/master/cmake-build-debug/image_fixed_5_colors.jpg" alt="drawing" width="200"/>


### Using color chart with more than 3 colors

With more than 3 colors, we would have a linear system with the number of equations more than the number of variables.
This can be solved with some kind of regression algorithm. Least squares, polynomial regression, neural network?
It is also important to choose colors that give best result with color scales used in test strips.

### Using other color spaces

Using color spaces other than RGB, like YUV or HSV, could produce better results?

## Links
https://stackoverflow.com/questions/35785663/map-one-cameras-colour-profile-to-another
https://blog.francium.tech/using-machine-learning-for-color-calibration-with-a-color-checker-d9f0895eafdb
https://www.learnopencv.com/color-spaces-in-opencv-cpp-python/
https://sci-hub.tw/10.1109/TIP.2015.2405336
https://sci-hub.tw/https://doi.org/10.3390/s120607063





            
          
