# Scoreboarding</h1>
Este projeto implementa um simulador de Scoreboarding. Apresentado e desenvolvido durante a disciplina de Arquitetura e Organização de Computadores II.

# Comandos suportados
Atualmente, o Scoreboarding suporta as seguintes operações:
* li
* move
* add
* addi
* sub
* or
* ori
* and
* andi
* slt
* mult
* div

Os comandos li e move são comandos extras colocados no simulador. Eles seguem a sintáxe descrita no [GreenCard](https://inst.eecs.berkeley.edu/~cs61c/resources/MIPS_Green_Sheet.pdf).

# Como usar
Para executar o simulador, são necessários os parâmetros ***-c configFile***, ***-o outputFile***, ***-p programFile*** e ***-m tamanhoMemoria***.

ConfigFile representa o arquivo contendo as latências de cada uma das instruções (todos as instruções devem ter suas latências declaradas aqui).

OutputFile representa o nome do arquivo de saída, onde será impresso o resultado *clock a clock* da simulação.

ProgramFile representa o arquivo contendo o código em Assembly que será lido pelo simulador. Este deve respeitas as convenções do MIPS32, descritas em [GreenCard](https://inst.eecs.berkeley.edu/~cs61c/resources/MIPS_Green_Sheet.pdf), e estar de acordo com os comandos que são usados aqui.

TamanhoMemoria representa o tamanho que será alocado para o programa que será executado. O tamanho é um número inteiro e deve ser igual ou menor à quantidade de linhas no código Assembly.