cd tests
echo 'Clear test .o files'
Rm *.o
cd ..
echo 'Clear raster .o files'
Rm *.o
Rm *.tos
echo 'Building test driver'
make