for fname in linkline waypoint
do
    # multipliers in order L,S,H
    convert  ../$fname.png  -modulate 130,95,100  ../${fname}_inactive.png
done
