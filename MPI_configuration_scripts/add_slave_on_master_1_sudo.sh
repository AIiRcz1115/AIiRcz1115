#!
echo "-----Dodawanie slava"
echo "Podaj IP slava: "
read ip_slave
echo "Podaj numer slava: "
read nr_slave
echo "Podales: \n slave$nr_slave: $ip_slave"
echo "-----Konfiguracja pliku /etc/hosts"
echo "$ip_slave\tslave$nr_slave" >> /etc/hosts
echo "-----Przeloguj sie na mpiuser i wykonaj drugi skrypt"
