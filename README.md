=====================================================================================
		MAQUINA VIRTUAL, MONTADOR DE DOIS PASSOS E LIGADOR DE DOIS PASSOS
=====================================================================================
	
	----------------------------------------------------------------------
	LINGUAGEM:
	----------------------------------------------------------------------
		Palavras reservadas e instruçoes devem ser
		escritas em maiusculo.

		variaveis são declaradas ao final após a area
		de programa.

		start é um label identificador do inicio.
		Apenas o programa principal deve conter a palavra "start".
		Sub-programas não devem conter start.


	----------------------------------------------------------------------
	COMPILAÇÃO:
	----------------------------------------------------------------------

		Para compilar no linux utilize o arquivo makefile.

		No termnal digite:  make
	
	----------------------------------------------------------------------
	EXECUÇÃO:
	----------------------------------------------------------------------

		nome_programa_saida.txt: esse nome voce mesmo determina, esse programa deve
							     ser executado no Assembler.exe.

		nome_programa.txt: nome do programa escrito em codigo assembly, deve ser executado no Assembler.exe.

		nome_programa_executavel.txt: esse nome voce mesmo determina, o executavel
									  gerado com esse nome deve ser executado na
									  VirtualMachine.exe

		Ligador: ./Ligador.exe nome_programa1.txt nome_programa2.txt nome_programaN.txt nome_programa_saida.txt

		Montador: ./Assembler.exe nome_programa.txt(ou nome_programa_saida.txt) nome_programa_executavel.txt

		Maquina virtuamerl: ./VirtualMachine.exe nome_programa_executavel.txt

	----------------------------------------------------------------------
	IDENTIFICAÇÃO DE ERROS:
	----------------------------------------------------------------------

		Montador: 
			Identifica erros de: 
				*variaveis já declaradas.
				*variaveis não declaradas.
				*instruções invalidas.

	----------------------------------------------------------------------
	PROGRAMAS JA INCLUSOS:
	----------------------------------------------------------------------
		Para usar direto no montador: 
			*maior3numAss.txt
			*somaAss.txt
		
		Para usar no ligador:
			*media1.txt
			*media2.txt
=====================================================================================


