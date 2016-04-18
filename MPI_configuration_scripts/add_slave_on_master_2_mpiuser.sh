#!
echo "Podaj numer slava: "
read nr_slave
if [ ! -d "/home/mpiuser/.ssh" ]; then
	echo "-----Generowanie klucza RSA"
	ssh-keygen -t rsa
fi
echo "-----Kopiowanie klucza RSA do slava"
echo mpiuser | ssh-copy-id "slave$nr_slave"
echo "-----Zakonczono dodawanie slava"
