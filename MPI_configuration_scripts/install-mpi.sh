#!
echo "-----Instalacja MPI"
if [ ! -d "/home/mpiuser" ]; then
	echo "-----Konfiguracja uzytkownika mpiuser"
	useradd -m mpiuser
	echo mpiuser:mpiuser | chpasswd
fi
echo "-----Instalacja G++"
apt-get -q=2 install g++
cd /home/mpiuser
echo "-----Pobieranie MPICH2"
wget -q http://archive.ubuntu.com/ubuntu/pool/universe/m/mpich/mpich_3.2.orig.tar.gz
echo "-----Wypakowywanie MPICH2"
tar -xzf mpich_3.2.orig.tar.gz
cd /home/mpiuser/mpich-3.2
echo "-----Konfiguracja MPICH2"
./configure -q --disable-fortran
echo "-----Instalacja MPICH2"
make -s; make -s install
echo "-----Instalacja MPI zakonczona"
echo "-----Uruchom komputer ponownie przed uzywaniem MPI"
