seeds=(123 862 53)

for i in `seq 0 1 2`
do
    convert -size 800x600 xc:   -seed ${seeds[$i]} +noise Random   random.png
    convert random.png -virtual-pixel tile  -blur 0x50 -auto-level  random_10.png
    convert random_10.png  -channel G  -separate   random_10_gray.png
    convert random_10_gray.png  -ordered-dither threshold,10 random_10_blobs.png
    # convert temp.png -define h:format=gray +antialias temp.h
    
    convert -sample 80x60 random_10_blobs.png -define h:format=gray -depth 8 test.h
    sed "s/MagickImage/level$(($i + 1))_array/" test.h > ../../level$((i + 1)).h
    rm test.h
    
    convert -sample 80x60 random_10_blobs.png test.png
    
    convert random_10_blobs.png +level-colors \#031a91,\#b4fdf7 ../level$(($i + 1)).png
    
    rm random.png
    rm random_10.png
    rm random_10_gray.png
    rm random_10_blobs.png
done
