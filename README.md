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
-
- PDFLatex: conversão dos resultados obtidos pela heurística em representação gráfica.
- Código-fonte da heurística executável.
- [curl](https://curl.se/) (Apenas Windows)
- [wget](https://www.gnu.org/software/wget/) (Apenas Linux)

## Instalação

Primeiramente é necessário ter o LibreOffice 7 instalado. Caso não possua em seu sistema operacional [este guia](https://pt-br.libreoffice.org/ajuda/instrucoes-de-instalacao/) pode auxiliar na instalação.

Um instalador foi criado para auxiliar no download da aplicação. A pasta [instalador](instalador/) é possível encontrar o código-fonte do instalador. **Caso o LibreOffice 7 não esteja instalado, o download do projeto não será realizado**.

### Utilizando o instalador

Para o Sistema Operacional WINDOWS 11, faça o download do instalador contido [nesta pasta](https://github.com/thuzax/Projeto-Ceramica-Dev/blob/main/instalador/instalador_win/). Após isso execute a aplicação como administrador e a instalação será feita.


### Para Desenvolvedores ou Compilação do Código-Fonte

Para um guia de compilação do código-fonte ou para detalhes para desenvolvedores, acesse [este diretorio](https://github.com/thuzax/Projeto-Ceramica-Dev/blob/main/docs/).

Se necessário será solicitado permissão para instalação de *softwares* que são utilizados pela aplicação. Confirme caso deseje que a instalação prossiga.

No Windows *software* é salvo na pasta *Documentos* e um atalho é criado na Área de Trabalho. No Linux, o *software* é salvo na pasta pessoal.


**Observações:**
- Para a instalação do PDFLatex em sistemas Windows é necessário o download do *software* [MikTex](https://miktex.org/). O download e instalação deste *software* é realizado pelo instalador.
- Para a instalação do PDFLatex em sistemas Linux é necessária a instalação do pacote [UnZip](https://infozip.sourceforge.net/UnZip.html). O download e instalação deste pacote é realizado pelo instalador.



## Referências

<a id="1">Belleboni, Matheus G. S., 2016. </a> Aplicação web para empacotamento de peças de cerâmica em fornos tridimensionais. TCC (Graduação) - Bacharel em Ciência da Computação, Universidade de São Paulo, São Carlos - SP.

<a id="2">Foundation, T.D., 2023</a> LibreOffice Calc. Disponível em: https://hub.libreoffice.org/credits/ (Acessado em 04/01/2024).