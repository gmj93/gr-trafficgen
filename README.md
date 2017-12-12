# gr-trafficgen 

#### Traffic generation blocks for GNU Radio  

This project contains blocks for generating traffic in GNURadio applications. Users can choose to generate Constant Bit Rate traffic (using the CBR Transmitter), or Variable Bit Rate traffic (using the VBR Transmitter). Other components include: blocks for number generation following probabilistic distributions (Poisson, Weibull, Uniform), trigger blocks (user defined and timer) and a Receiver block.  
## Dependencies  
To install the required dependencies for importing and running this block, run the following command:  
```  
sudo apt install libboost-all-dev  gnuradio swig  
```  
__IMPORTANT__: There are known issues with compiling this block using other systems. For safety, we recommend using ubuntu as the dependencies installation in this system is straightforward.  

## Installation  
To install the block and import it to gnuradio, run the following commands:  
```  
mkdir build  
cd build  
cmake ../  
make  
sudo make install  
```  
After this step, the block should show up on `gnuradio-companion`'s sidebar.  
