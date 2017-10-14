# ExercisesOS_sockets

## Description

Exercises to implement a interface master-slave with sockets and threads

## Compiling
Question 1.
- Master
> gcc questao01_mestre.c -o mestre01
- Slave
> gcc questao01_escravo -o escravo01

Question 2.
- Master
> gcc questao02_mestre.c -o mestre02
- Slave
> gcc questao02_escravo -o escravo02

Question 3.
- Master
> gcc questao03_mestre.c -o mestre03
- Slave
> gcc questao03_escravo -o escravo03 -lpthread

## Executing

Question 1.
- Master 
> ./mestre01 <PORT>
- Slaves, open 3 terminals and execute a one those commands in each one:
* First Slave
> ./escravo01 <PORT>
* Second Slave
> ./escravo01 <PORT+1>
* Third Slave
> ./escravo01 <PORT+2>
  
Question 2.
- Master 
> ./mestre02 PORT NUMBER_OF_SLAVES
- Slaves, open N terminals to slaves and execute in each one
* First Slave
> ./escravo02 PORT
* Second Slave
> ./escravo02 (PORT+1)
* Third Slave
> ./escravo02 (PORT+2)
(...)
* N-th slave
> ./escravo02 (PORT+(N-1))
  
Question 3.
- Master 
> ./mestre03 PORT NUMBER_OF_SLAVES
- Slaves, open N terminals to slaves and execute in each one
> ./escravo02 PORT NUMBER_OF_SLAVES

## Authorship

Developed by Erick de Oliveira Silva (<erickoliveira.eos@gmail.com>), BTI's student in UFRN.

2017.2

&copy; IMD/UFRN 2017.
