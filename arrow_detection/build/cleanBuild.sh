# cleans and builds Cmake porject

sudo rm Main Makefile CMakeCache.txt cmake_install.cmake
sudo rm -r CMakeFiles
cmake ..
make VERBOSE=1

# sudo ./Main arrow_test croped_dataset
# sudo ./Main dataset/train_2 dataset/arrow_2
# sudo ./Main arrow_test/test_1.jpg
# sudo ./Main dataset/train_arrows/Clip_02_Left.jpg
# sudo ./Main train_arrows arrow_features.csv
# sudo ./Main arrow_features.csv 
# sudo ./Main arrow_features.csv dataset/train/IMG_20141216_202652.JPG
# sudo ./Main arrow_features.csv arrow_features_test.csv class_output.csv