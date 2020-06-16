# bitmap-filter

[![License](http://img.shields.io/:license-gpl3-blue.svg)](http://www.gnu.org/licenses/gpl-3.0.html)

Filters for 24 bit uncompressed bitmap images. Available filters are:

- **Grayscale** - Converts the image into a single sample containing only intensity information.

- **Reflect**  - Converts the image into its mirror image.

- **Blur** - Blurs the image.

- **Edge** - Highlights the edges in the image. It uses basic [sobel operator](https://en.wikipedia.org/wiki/Sobel_operator) to detect the edges.

# Installation 

- Clone the repository.
```
$ git clone https://github.com/sachinkumarsingh092/bitmap-filter.git
```
- Change the current directory to `bitmap-filter`
```
$ cd bitmap-filter
```
- Use **`make`** to build the executable.
```
$ make
```
  If you want to remove the executable, use the `clean` target in `make`
  ```
  $ make clean
  ```
You will need to install **`clang`** if not already available. To use **`gcc`** compiler make changes in **Makefile** accordingly.

# Usage

**Original Image:**

<img src="https://www.the.me/wp-content/uploads/2014/08/story_lena_lenna_1.jpg" width="412" height="412" />

### Grayscale
```
./filter -g input.bmp output.bmp
```
<img src="https://www.cosy.sbg.ac.at/~pmeerw/Watermarking/lena_gray.gif" width="412" height="412" />

### Refect 
```
$ ./filter -r input.bmp output.bmp
```
<img src="https://lh4.googleusercontent.com/SDNLFgcN-ZoyGITdJ5Gsb3zull4_qN7T4wNd2cXVRHFbj1gpCOHf5iYb0VGGVAmIlBPSHuCnmTe0lCrPKHvm7MH-AY18SxajMxLEZySjr5VggCXTfjJLfxcvjga3lPj1HQ=s412" width="412" height="412" />

### Blur
```
$ ./filter -b input.bmp output.bmp
```
<img src="https://www.researchgate.net/profile/Nir_Sochen/publication/51386844/figure/fig3/AS:394043197542405@1470958627655/Lena-image-blurred-by-7-2-7-out-of-focus-kernel.png" width="412" height="412" />

### Edge
```
$ ./filter -e input.bmp output.bmp
```
<img src="https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcSsKGDM6Z7268QnNfrSNhoyscuOE1KDG7BYSpqxksP_AGxvLj-A&s" width="412" height="412" />

# To-Do

- [ ] Write a help option.
- [ ] Write a function/script to check the compatibility of the image to be converted.

