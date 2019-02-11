# Jillian Khoo - jkhoo@usc.edu

# Installation instructions  
# Download vagrant here:  
https://www.vagrantup.com/downloads.html
# Follow these instructions for Project Setup for your vagrant vm:   https://www.vagrantup.com/intro/getting-started/project_setup.html
# Install the box:  
Use this box: https://app.vagrantup.com/generic/boxes/ubuntu1804
`vagrant box add generic/ubuntu1804`  
Open the Vagrantfile and change its contents to this:  
`Vagrant.configure("2") do |config|
  config.vm.box = "hashicorp/precise64"
	config.vm.synced_folder ".", "/vagrant"
 	config.vm.provision :shell, path: "bootstrap.sh"
end`  
# Create the Shell Script bootstrap.sh  
Create the following shell script and save it as bootstrap.sh in the same directory as your Vagrantfile  
`apt-get update
apt-get install -y apache2
if ! [ -L /var/www ]; then
  rm -rf /var/www
  ln -fs /vagrant /var/www
fi`  
# Run `vagrant up` then `vagrant ssh`  
# Install grpc and protobuf  
Use these instructions to install grpc and protobuf: https://github.com/grpc/grpc/blob/master/BUILDING.md  
Go to grpc/third_party/protobuf in the grpc repository and run `./autogen.sh`, `./configure`, `make && sudo make install`  
Go to the root directory of the grpc repository and run `sudo make install`  
# Install gtest  
`sudo apt-get install libgtest-dev`  
`sudo apt-get install cmake
cd /usr/src/gtest
sudo cmake CMakeLists.txt
sudo make
sudo cp *.a /usr/lib`
