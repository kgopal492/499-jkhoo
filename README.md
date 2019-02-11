# Jillian Khoo - jkhoo@usc.edu

## Installation
### Initial Setup:
#### Download vagrant here:  
https://www.vagrantup.com/downloads.html
#### Follow these instructions for Project Setup for your vagrant vm:  
https://www.vagrantup.com/intro/getting-started/project_setup.html
#### Install the box:  
Use this box: https://app.vagrantup.com/generic/boxes/ubuntu1804
vagrant box add generic/ubuntu1804  
Open the Vagrantfile and change its contents to this:  
`Vagrant.configure("2") do |config|`   
  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`config.vm.box = "hashicorp/precise64"`  
	&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`config.vm.synced_folder ".", "/vagrant"`   
 	&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`config.vm.provision :shell, path: "bootstrap.sh" `   
`end`    
#### Create the Shell Script bootstrap.sh  
Create the following shell script and save it as bootstrap.sh in the same directory as your Vagrantfile  
`apt-get update`  
`apt-get install -y apache2`  
`if ! [ -L /var/www ]; then`  
  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`rm -rf /var/www`  
  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`ln -fs /vagrant /var/www`  
`fi`  
#### Run `vagrant up` then `vagrant ssh`  
#### Install grpc and protobuf  
Use these instructions to install grpc and protobuf: https://github.com/grpc/grpc/blob/master/BUILDING.md  
Go to grpc/third_party/protobuf in the grpc repository and run `./autogen.sh`, `./configure`, `make && sudo make install`  
Go to the root directory of the grpc repository and run `sudo make install`  
#### Install gtest  
`sudo apt-get install libgtest-dev`  
`sudo apt-get install cmake
cd /usr/src/gtest
sudo cmake CMakeLists.txt
sudo make
sudo cp *.a /usr/lib`  

## Compiling and running the code:  
Go to 499-jkhoo/cpp and run `./compile_clean` to compile everything except tests  
Start the Key Value store with `./backend_store`  
Start the Service Layer with `./service_layer`  
Run the client layer with `./client_layer` and any appropriate flags  

## Compiling and running the tests:  
Go to 499-jkhoo/tests and run `./test_compile_clean`  
Run the key value store tests with `./key_value_store_service_impl_tests`  
Run the service layer tests with `./service_layer_service_impl_tests`
