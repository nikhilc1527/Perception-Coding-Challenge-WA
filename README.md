# Perception Coding Challenge

## My solution

![](https://github.com/nikhilc1527/Perception-Coding-Challenge-WA/blob/main/static/my_output.png)

## Methodology

Firstly, the cones needed to be detected and all other noise needed to be removed. Since the cones are bright orange, I first slightly blurred the image and then filtered the image based on a very high saturation value. This gave me a black and white image with the silhouettes of the cones as white. Then, I used the simpleblobdetector to detect the positions of each of the cones in the image, and got a list of points from that. To distinguish the left side from the right side, I found the position in the list of points where the slope changed, and used that to separate the points. After that, I had a list of points for the left and right sides, and ran the least squares algorithm on each of them to get the final lines.

# What did I try and what did not work

I tried detecting the cones using a triangular kernel passed over the image. This did not provide nearly good enough results, because the size, orientation, and shape of each of the cones was different, and the size and shape of the exit sign was not different enough. I realized that I do not have to filter just by the triangular shape of the cones, but rather just the bright color that they have.

# What libraries are used

I used just the opencv library, version 4.7.0
