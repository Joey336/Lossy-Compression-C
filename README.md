# Lossy-Compression-C

This lossy image compression program will compress any image that has been converted to a .pnm file.

This algorithm works by reading the image as 2x2 blocks of pixels, converting these blocks into component video color format then packing each block into 32 bit words using bitpacking.

-Download or clone my repository

Program can be compiled by running: ```bash compile```

You can compress one of the example files by running: ```./image -c example/<filename> > <outputFilename>```

You can decompress a compressed file by running: ```./image -d <compressedFile> > <outputFilename.ppm>```

Running ```convert example.ppm example.jpg``` can be used to convert any before/after .ppm file to see its quality in .jpg format
