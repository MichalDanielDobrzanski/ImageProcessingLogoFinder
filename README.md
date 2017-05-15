# ImageProcessingLogoFinder
*C++* project with *OpenCV library* for Logo recognition in colored images.

Project realized for Warsaw University of Technology.

It does not yield the 100% right results and this project serves more as a **reference** for implemented approach and functionalities.

Suggested approach calculates **image moments** for image **sub-regions** 
and checks whther they are in certain range of values in order to
make a decision about the presence of Logo or Text.

Runs with CMake. Created in CLion.

## Realized functionality (implemented algorithms):

- implemented *flood_fill algorithm*
- finding *regions of interest* which match the desired criteria
- creating *moving window* for processing *sub-regions*
- conversion from *RGB* to *HSV*
- extaction of *Hue* and *Saturation* channel
- *logical AND operation* for two *binary images*
- *erosion*
- *dilation*
- *median* filtering with adjustable window size and selecting the 'median' element by index

## Sample results:

Black rectangle = Logo found in this region
Blue rectangle = Text found in this region

![Logo and text found](https://github.com/MichalDanielDobrzanski/ImageProcessingLogoFinder/blob/master/out_data/recognized/image_1.jpg)


![Logo mismatch and no text found](https://github.com/MichalDanielDobrzanski/ImageProcessingLogoFinder/blob/master/out_data/recognized/image_3.jpg)
