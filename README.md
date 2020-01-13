## ! Требуется CMake версии 3.5 и выше. 
## Для обновления на CentOS необходимо выполнить следующие шаги:
* sudo yum remove cmake -y
*  cd /packages
* sudo wget https://cmake.org/files/v3.6/cmake-3.6.2.tar.gz
* sudo tar -zxvf cmake-3.6.2.tar.gz
* cd cmake-3.6.2
* sudo ./bootstrap --prefix=/usr/local
* sudo make
* sudo make install
* vi ~/.bash_profile
PATH=/usr/local/bin:$PATH:$HOME/bin
* cmake --version


# Собрать библиотеку командами:
* cd /libraries/rnnnoise_stt_preprocessing_snatchbot/audiocodec
* cmake -F CMakeLists.txt
* sudo yum install yasm
* make
* make install
