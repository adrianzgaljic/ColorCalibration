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

Original and altered image

<img src="https://github.com/adrianzgaljic/ColorCalibration/blob/master/cmake-build-debug/original.jpg" alt="drawing" width="200"/>   <img src="https://github.com/adrianzgaljic/ColorCalibration/blob/master/cmake-build-debug/processed_image.jpg" alt="drawing" width="200"/>  <img src="https://github.com/adrianzgaljic/ColorCalibration/blob/master/cmake-build-debug/examplee.png" alt="drawing" width="200"/>


            
          
