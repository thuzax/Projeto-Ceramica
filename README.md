# Projeto Cerâmica

Este projeto tem o intuito de criar uma interface para o uso da heurística proposta por [Belleboni,2016](#1) para o problema de empacotamento de peças de cerâmica em fornos tridimensionais. A interface é uma planilha a ser utilizada pelo *software* [LibreOffice](#2) versão 7.

O trabalho original de [Belleboni,2016](#1) teve como objetivo auxiliar a alocação de peças de cerâmica em um forno tridimensional. Nota-se que devido a característica do problema foi considerado que o forno poderia ser dividido em andares, de forma que o problema pudesse ser modelado como Nestings bidimensionais. Assim foi possível resolver o problema adaptando uma heurística *bottom-left*. A implementação da heurística se encontra [neste diretório](solver/bottom-left-heuristic-master/)


## *Disclaimer*

### Códigos de terceiros

Este projeto se utiliza dos seguintes programas terceiros sem nenhuma modificação de seus códigos-fonte:

- PDFLatex
- MikTex
- curl
- wget
- UnZip

Por fim, este projeto também se utiliza do trabalho de [Belleboni,2016](#1) com modificações em seu código-fonte.


### Atualizações e Correções

Este projeto não prevê garantias de atualizações após sua conclusão. Os autores se isentam de qualquer responsabilidade de atualizar o Software, podendo ou não, de acordo com seu desejo, realizar modificações.

### Licença

A licença de uso deste projeto pode ser encontrada [aqui](LICENSE).


## Requisitos

- LibreOffice 7: uso da planilha
- PDFLatex: conversão dos resultados obtidos pela heurística em representação gráfica.
- Código-fonte da heurística executável.
- [curl](https://curl.se/) (Apenas Windows)
- [wget](https://www.gnu.org/software/wget/) (Apenas Linux)

## Instalação

Primeiramente é necessário ter o LibreOffice 7 instalado. Caso não possua em seu sistema operacional [este guia](https://pt-br.libreoffice.org/ajuda/instrucoes-de-instalacao/) pode auxiliar na instalação.

Um instalador foi criado para auxiliar no download da aplicação. A pasta [instalador](instalador/) é possível encontrar o código-fonte do instalador. **Caso o LibreOffice 7 não esteja instalado, o download do projeto não será realizado**.

### Utilizando o instalador

A compilação do códgo fonte já foi feita para alguns sistemas operacionais. Caso seu sistema operacional seja um dos listados, basta executá-lo como administrador que o download dos arquivos e a instalação do PDFLatex serão realizados. **Caso contrário será necessária a compilação do código-fonte do instalador.** Para a compilação do código fonte do instalador recomenda-se o uso do compilador GCC/G++. Em sistemas Windows esse compilador pode ser obtido através da instalação do [MinGW](https://www.mingw-w64.org/). Para sua instalação, recomenda-se [este guia](https://terminalroot.com.br/2022/12/como-instalar-gcc-gpp-mingw-no-windows.html). 

Para a compilação do código fonte utilize o seguinte comando no CMD(Windows)/Terminal(Linux) na pasta onde o código fonte do instalador foi salvo:

> g++ -Wall -Wconversion -o instalador_versao_so installer.cpp

Para usuários do Windows, [este guia](https://www.softdownload.com.br/como-abrir-prompt-comando-windows-pasta-especifica.html) pode auxiliar no processo de abrir o CMD em uma pasta específica.

Uma vez que o instalador esteja compilado, basta executá-lo com privilégios de administrador que o projeto será baixado e estará pronto para uso.

O download dos arquivos é feito na pasta *Documentos* para Windows e no diretório do usuário para Linux.

**Observações:**
- Para a instalação do PDFLatex em sistemas Windows é necessário o download do *software* [MikTex](https://miktex.org/). O download e instalação deste *software* é realizado pelo instalador.
- Para a instalação do PDFLatex em sistemas Linux é necessária a instalação do pacote [UnZip](https://infozip.sourceforge.net/UnZip.html). O download e instalação deste pacote é realizado pelo instalador.

### Código-fonte da heurística

O código fonte da heurística está compilado para os sistemas *Ubuntu 22.04.03* e *Windows 11 Pro para Educação*. Se seu sistema operacional não for compatível ele deverá ser recompilado.

A compilação é feita utilizando este [Makefile](solver/Makefile). Para Windows é necessário fazer a instalação de um programa externo para a execução do Makefile. Também é necessário ter instalado o compilador GCC/G++. Recomenda-se a instalação do [MinGW](https://www.mingw-w64.org/) através [deste guia](https://terminalroot.com.br/2022/12/como-instalar-gcc-gpp-mingw-no-windows.html), caso o compilador não esteja presente Para sua instalação. Para a instalação do comando de execução do Makefile, recomenda-se [este guia](https://coffops.com/usando-comando-make-projetos-windows/).


Para compilar o programa em **Windows** basta abrir o CMD(Windows) e executar o seguinte comando:

> cd %USERPROFILE%\Documents\Projeto-Ceramica\solver && make

Para compilar o programa em **Linux** basta abrir o terminal e digitar o seguinte comando:

> cd ~/Projeto-Ceramica/solver/ && make


## Links para guias

- Guia de instalação do LibreOffice: https://pt-br.libreoffice.org/ajuda/instrucoes-de-instalacao/
- Guia de Instalação do MinGw: https://terminalroot.com.br/2022/12/como-instalar-gcc-gpp-mingw-no-windows.html
- Guia de "instalação do make": https://coffops.com/usando-comando-make-projetos-windows/
- Guia de como abrir o CMD em um diretório específico: https://www.softdownload.com.br/como-abrir-prompt-comando-windows-pasta-especifica.html

## Referências

<a id="1">Belleboni, Matheus G. S., 2016. </a> Aplicação web para empacotamento de peças de cerâmica em fornos tridimensionais. TCC (Graduação) - Bacharel em Ciência da Computação, Universidade de São Paulo, São Carlos - SP.

<a id="2">Foundation, T.D., 2023</a> LibreOffice Calc. Disponível em: https://hub.libreoffice.org/credits/ (Acessado em 04/01/2024).
