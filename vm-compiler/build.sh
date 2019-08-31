#remove the vm compiler from programs
sudo rm /bin/dracovmc

g++ -o dracovmc *.cpp *.hpp 

sudo cp dracovmc /bin/

