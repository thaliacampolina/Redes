=============================================
			TP1 Redes
=============================================


Autores:	Marcelo Augusto Andrade Penna (prof Luiz Felipe)		marceloaapenna@gmail.com
			Alysson Alves de Oliveira (prof. Daniel)				alysson_alves@yahoo.com.br
		 				
-----------------------------------------------

!!Importante!!: 1. Tenha certeza de que a porta 9999 ou a que você escolheu esteja aberta;
				2.A porta não está configurada para 25 e sim 9999, para testes
				
-----------------------------------------------

Compilando:
	Cliente:
		- Entre no diretório "cliente"
		- Execute: make

	Servidor:
		- Entre no diretório "servidor"
		- Execute: make
	
-----------------------------------------------

Executando:
	Cliente:
		./cliente <ip servidor smtp> <e-mail remetente> <e-mail destinatário> <nome do arquivo anexado(Opcional)>
	
	Servidor:
		./servidor

-----------------------------------------------

Erros de execução:
	- Cliente:
		1. Anexar arquivo e ele não está no mesmo diretório do cliente;
				Resultado: envia o e-mail mas na anvia o arquivo. Não ocorre nenhum aviso de erro.
	- Servidor:
		1. Ao ser conectado via telnet não se espera pelo "." para terminar a mensagem, ao primeiro ENTER ele considera a mensagem enviada;

-----------------------------------------------

Testes:
	- Cliente: testado no servdiro SMTP do Linux;
	- Servidor: testado com o cliente SMTP e com o telnet do linux;

-----------------------------------------------
