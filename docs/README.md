# Para Desenvolvedores

## Requisitos

- LibreOffice 7: uso da planilha
- PDFLatex: conversão dos resultados obtidos pela heurística em representação gráfica.
- Código-fonte da heurística executável.
- [curl](https://curl.se/) (Apenas Windows)
- [wget](https://www.gnu.org/software/wget/) (Apenas Linux)

## Compilação do Instalador

Na pasta [instalador](https://github.com/thuzax/Projeto-Ceramica-Dev/blob/main/instalador/) é possível encontrar o código-fonte do instalador.

A compilação do códgo fonte já foi feita para alguns sistemas operacionais. Caso seu sistema operacional seja um dos listados, basta executá-lo como administrador que o download dos arquivos e a instalação do PDFLatex serão realizados. **Caso contrário será necessária a compilação do código-fonte do instalador.** Para a compilação do código fonte do instalador recomenda-se o uso do compilador GCC/G++. Em sistemas Windows esse compilador pode ser obtido através da instalação do [MinGW](https://www.mingw-w64.org/). Para sua instalação, recomenda-se [este guia](https://terminalroot.com.br/2022/12/como-instalar-gcc-gpp-mingw-no-windows.html). 

Para a compilação do código fonte utilize o seguinte comando no CMD(Windows)/Terminal(Linux) na pasta onde o código fonte do instalador foi salvo:

> g++ -Wall -Wconversion -o instalador_versao_so installer.cpp

Para usuários do Windows, [este guia](https://www.softdownload.com.br/como-abrir-prompt-comando-windows-pasta-especifica.html) pode auxiliar no processo de abrir o CMD em uma pasta específica.

Uma vez que o instalador esteja compilado, basta executá-lo com privilégios de administrador que o projeto será baixado e estará pronto para uso.

O download dos arquivos é feito na pasta *Documentos* para Windows e no diretório do usuário para Linux.

## Compilação do Código-fonte da Heurística

O código fonte da heurística está compilado para os sistemas *Ubuntu 22* e *Windows 11*. Se seu sistema operacional não for compatível ele deverá ser recompilado.

A compilação é feita utilizando este [Makefile](solver/Makefile). Para Windows é necessário fazer a instalação de um programa externo para a execução do Makefile. Também é necessário ter instalado o compilador GCC/G++. Recomenda-se a instalação do [MinGW](https://www.mingw-w64.org/) através [deste guia](https://terminalroot.com.br/2022/12/como-instalar-gcc-gpp-mingw-no-windows.html), caso o compilador não esteja presente Para sua instalação. Para a instalação do comando de execução do Makefile, recomenda-se [este guia](https://coffops.com/usando-comando-make-projetos-windows/).


Para compilar o programa em **Windows** basta abrir o CMD(Windows) e executar o seguinte comando:

> cd %USERPROFILE%\Documents\Projeto-Ceramica\solver && make

Para compilar o programa em **Linux** basta abrir o terminal e digitar o seguinte comando:

> cd ~/Projeto-Ceramica/solver/ && make


## Links para guias

- Guia de Instalação do MinGw: https://terminalroot.com.br/2022/12/como-instalar-gcc-gpp-mingw-no-windows.html
- Guia de "instalação do make": https://coffops.com/usando-comando-make-projetos-windows/
- Guia de como abrir o CMD em um diretório específico: https://www.softdownload.com.br/como-abrir-prompt-comando-windows-pasta-especifica.html