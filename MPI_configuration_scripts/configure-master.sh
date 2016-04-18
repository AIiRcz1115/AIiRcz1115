#!
echo "-----Konfigurowanie komputera jako MPI master"
echo "Podaj IP tego komputera: "
read ip_master
echo "Podales: \n master: $ip_master"
echo "-----Instalacja OpenSSH-server"
apt-get -q=2 install openssh-server
echo "-----Instalacja NFS-Kernel-Server"
apt-get -q=2 install nfs-kernel-server
echo "-----Konfiguracja pliku /etc/hosts"
echo "$ip_master\tmaster" >> /etc/hosts
if [ ! -d "/home/mpiuser" ]; then
	echo "-----Konfiguracja uzytkownika mpiuser"
	useradd -m mpiuser
	echo mpiuser:mpiuser | chpasswd
fi
echo "-----Konfiguracja NFS"
if [ ! -d "/home/mpiuser/mpi-cloud" ]; then
	mkdir -m 0777 /home/mpiuser/mpi-cloud
fi
echo "/home/mpiuser/mpi-cloud *(rw,sync,no_root_squash,no_subtree_check)" >> /etc/exports
exportfs -a
service nfs-kernel-server restart
echo "-----Konfiguracja komputera jak MPI master zakonczona"
echo "-----Teraz mozesz dodac MPI slavy"
echo "-----Uruchom komputer ponownie przed uzywaniem MPI"
