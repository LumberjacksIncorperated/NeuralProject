
#NeuralProject: NeuralNetwork.c NeuralProjectDriver.c
#	gcc NeuralProject NeuralNetwork.c NeuralProjectDriver.c -I.



#NeuralProject: $(wildcard *.o)
#	$(CC) $^ -o $@

#%.o: %.c %.h
#	$(CC) -I. -c $< -o $@

NeuralProject: NeuralNetwork.o NeuralProjectDriver.o
	gcc -o NeuralProject NeuralNetwork.o NeuralProjectDriver.o

NeuralProjectDriver.o: NeuralProjectDriver.c NeuralNetwork.h
	gcc -c NeuralProjectDriver.c

NeuralNetwork.o: NeuralNetwork.c NeuralNetwork.h
	gcc -c NeuralNetwork.c




clean:
	$(RM) NeuralProject *.o
