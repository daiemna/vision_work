# train_cascade.sh
pos_sample_file="pos_samples.vec"
pos_anotation="pos.txt"
neg_anotation="neg.txt"

totalNumPos=1000
totalNumNeg=1000

numPos=50
numNeg=50
numStages=20
image_width=40
image_height=95
minHitRate=0.99
maxFalseAlarmRate=0.1
acceptanceRatioBreakValue=10e-5
numThreads=100
featureType=HAAR
mode=ALL
boostingType=DAB

# ./opencv_createsample -vec $pos_sample_file -info $pos_anotation -num $totalNumPos -w $image_width -h $image_height
# mkdir classifier
./opencv_traincascade -data pedestrian_classifier -bt $boostingType -numThreads $numThreads -vec $pos_sample_file -bg $neg_anotation -numStages $numStages -minHitRate $minHitRate -featureType $featureType -maxFalseAlarmRate $maxFalseAlarmRate -acceptanceRatioBreakValue $acceptanceRatioBreakValue -numPos $numPos -numNeg $numNeg -w $image_width -h $image_height -mode BASIC -precalcValBufSize 1024 -precalcIdxBufSize 1024 -mode $mode
