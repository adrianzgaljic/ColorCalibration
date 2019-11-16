# Color Calibration

## Algorithm for color calibrating images.

The goal is to have the same color representation of each image, regardless of lighting conditions, camera models, etc.
This can be achieved by using a color calibration chart with known values (https://en.wikipedia.org/wiki/ColorChecker).
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

<p>To color calibrate the image, each pixel P is multiplied with mapping function M to get new pixel P'</p>

P = [R G B]<br>
P' = [R' G' B']<br>
P' = M * P

## Example

### Using 3 colors to calculate the mapping function

The simplest solution is to use color chart with 3 known colors, each consisting of 3 values for red, green and blue channel.
Mapping parameters for each channel can be computed separately, having 3 unknowns and 3 equations.
This system has the same number of equations and unknowns and has a single unique solution: mapping function M.


<p>Original, altered and calibrated image</p>

<img src="https://github.com/adrianzgaljic/ColorCalibration/blob/master/cmake-build-debug/original_with_3_colors.jpg" alt="drawing" width="200"/>   <img src="https://github.com/adrianzgaljic/ColorCalibration/blob/master/cmake-build-debug/processed_with_3_colors.jpg" alt="drawing" width="200"/>  <img src="https://github.com/adrianzgaljic/ColorCalibration/blob/master/cmake-build-debug/calibrated_with_3_colors.jpg" alt="drawing" width="200"/>

<img src="https://github.com/adrianzgaljic/ColorCalibration/blob/master/cmake-build-debug/example.png" alt="drawing" width="400"/>

### Using more colors to calculate mapping function

<p>The previous solution will work well for the 3 colors that were used to calculate the mapping function, but will fail to map other colors correctly. If more colors are used for calculation of the mapping function, this is a system with more equations than unknowns and there is no solution. This means that mapping function has to be estimated to optimally work with all the colors. This is done by defining the calibration error:</p>
<p>Error = (Original color - Calibrated color)^2</p>
<p>And the overall error is the sum of errors for each color.</p>
<p>Mapping function is than calculated by finding the minimum of overall error function.</p>

<p>Five photos of color chart were taken under different lightning conditions to test the algorithm,</p>
<p>here are examples before and after the calibration:</p>

<img src="https://github.com/adrianzgaljic/ColorCalibration/blob/master/cmake-build-debug/calibration_before.gif" alt="drawing" width="200"/>   <img src="https://github.com/adrianzgaljic/ColorCalibration/blob/master/cmake-build-debug/calibration_after_all.gif" alt="drawing" width="200"/> <img src="https://github.com/adrianzgaljic/ColorCalibration/blob/master/cmake-build-debug/calibration_changes.gif" alt="drawing" width="200"/>

<p>Overall error for each image before calibration:</p>
<p>Image 1 error: <strong>2765.99</strong></p>
<p>Image 2 error: <strong>24468.1</strong></p>
<p>Image 3 error: <strong>9771.38</strong></p>
<p>Image 4 error: <strong>7792.67</strong></p>
<p>Image 5 error: <strong>1262.42</strong></p>

<p>Overall error for each image after calibration:</p>
<p>Image 1 error: <strong>55.9662</strong></p>
<p>Image 2 error: <strong>410.455</strong></p>
<p>Image 3 error: <strong>193.925</strong></p>
<p>Image 4 error: <strong>102.541</strong></p>
<p>Image 5 error: <strong>137.367</strong></p>







## Next steps

### Using a neural network 

Neural network seems to produce fair results, but is unacceptably slow because it needs to be trained in real time.
The project can be found here: https://github.com/adrianzgaljic/ColorCalibratorNN
Here are results using 3, 4 and 5 colors:


<img src="https://github.com/adrianzgaljic/ColorCalibration/blob/master/cmake-build-debug/image_fixed_3_colors.jpg" alt="drawing" width="200"/>   <img src="https://github.com/adrianzgaljic/ColorCalibration/blob/master/cmake-build-debug/image_fixed_4_colors.jpg" alt="drawing" width="200"/>  <img src="https://github.com/adrianzgaljic/ColorCalibration/blob/master/cmake-build-debug/image_fixed_5_colors.jpg" alt="drawing" width="200"/>


### Using other color spaces

Could using color spaces other than RGB, like YUV or HSV, produce better results?

## Links
https://stackoverflow.com/questions/35785663/map-one-cameras-colour-profile-to-another
https://blog.francium.tech/using-machine-learning-for-color-calibration-with-a-color-checker-d9f0895eafdb
https://www.learnopencv.com/color-spaces-in-opencv-cpp-python/
https://sci-hub.tw/10.1109/TIP.2015.2405336
https://sci-hub.tw/https://doi.org/10.3390/s120607063





            
          
