#!
echo "-----Konfigurowanie komputera jako MPI slave"
echo "Podaj IP mastera: "
read ip_master
echo "Podaj IP tego komputera: "
read ip_slave
echo "Podaj którym slavem ma byc ten komputer: "
read nr_slave
echo "Podales: \n master: $ip_master \n slave$nr_slave: $ip_slave"
echo "-----Instalacja OpenSSH-server"
apt-get -q=2 install openssh-server
echo "-----Instalacja NFS-common"
apt-get -q=2 install nfs-common
echo "-----Konfiguracja pliku /etc/hosts"
echo "$ip_master\tmaster" >> /etc/hosts
echo "$ip_slave\tslave$nr_slave" >> /etc/hosts
if [ ! -d "/home/mpiuser" ]; then
	echo "-----Konfiguracja uzytkownika mpiuser"
	useradd -m mpiuser
	echo mpiuser:mpiuser | chpasswd
fi
echo "-----Konfiguracja NFS"
if [ ! -d "/home/mpiuser/mpi-cloud" ]; then
	mkdir -m 0777 /home/mpiuser/mpi-cloud
fi
mount -t nfs master:/home/mpiuser/mpi-cloud /home/mpiuser/mpi-cloud
echo "#MPI CLUSTER SETUP\nmaster:/home/mpiuser/mpi-cloud /home/mpiuser/mpi-cloud nfs" >> /etc/fstab
echo "-----Konfiguracja komputera jak MPI slave zakonczona"
echo "-----Uruchom komputer ponownie przed uzywaniem MPI"
