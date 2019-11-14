# Maquina virtual, montador de dois passos e ligador de dois passos

	
## Linguagem:

* Palavras reservadas e instruçoes devem ser
escritas em maiusculo.

* Variaveis são declaradas ao final após a area
de programa.

* start é um label identificador do inicio.
* Apenas o programa principal deve conter a palavra "start".
* Sub-programas não devem conter start.



## Compilação:

* Para compilar no linux utilize o arquivo makefile.

* No termnal digite:  make


## Execução:

* nome_programa_saida.txt: Esse nome voce mesmo determina, esse programa deve ser executado no Assembler.exe.

* nome_programa.txt: Nome do programa escrito em codigo assembly, deve ser executado no Assembler.exe.

* nome_programa_executavel.txt: Esse nome voce mesmo determina, o executavel gerado com esse nome deve ser executado na VirtualMachine.exe

* Ligador: ./Ligador.exe nome_programa1.txt nome_programa2.txt nome_programaN.txt nome_programa_saida.txt

* Montador: ./Assembler.exe nome_programa.txt(ou nome_programa_saida.txt) nome_programa_executavel.txt

* Maquina virtuamerl: ./VirtualMachine.exe nome_programa_executavel.txt

## Identificação de erros:

* Montador: 
	* Identifica erros de: 
		* Variaveis já declaradas.
		* Variaveis não declaradas.
		* Instruções invalidas.

## Programas já inclusos:

* Para usar direto no montador: 
	* maior3numAss.txt
	* somaAss.txt

* Para usar no ligador:
	* media1.txt
	* media2.txt



