###########################################################################################
#
# AUTHOR
# ------
# Lumberjacks Incorperated (2018)
#
###########################################################################################

NeuralProject: NeuralNetwork.o NeuralProjectDriver.o
	gcc -o NeuralProject NeuralNetwork.o NeuralProjectDriver.o

NeuralProjectDriver.o: NeuralProjectDriver.c NeuralNetwork.h
	gcc -c NeuralProjectDriver.c

NeuralNetwork.o: NeuralNetwork.c NeuralNetwork.h
	gcc -c NeuralNetwork.c

clean:
	$(RM) NeuralProject *.o
