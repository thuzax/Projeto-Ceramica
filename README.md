# Projeto Cerâmica

Este projeto foi desenvolvido com o apoio da Fundação de Amparo à Pesquisa do Estado de São Paulo (FAPESP). Seu intuito de criar uma interface para o uso da heurística proposta por [Belleboni,2016](#1) para o problema de empacotamento de peças de cerâmica em fornos tridimensionais. A interface é uma planilha a ser utilizada pelo *software* [LibreOffice](#2) versão 7.

O trabalho original de [Belleboni,2016](#1) teve como objetivo auxiliar a alocação de peças de cerâmica em um forno tridimensional. Nota-se que devido a característica do problema foi considerado que o forno poderia ser dividido em andares, de forma que o problema pudesse ser modelado como Nestings bidimensionais. Assim foi possível resolver o problema adaptando uma heurística *bottom-left*. A implementação da heurística se encontra [neste diretório](solver/bottom-left-heuristic-master/).


# Instalação

Foi criado um instalador que realiza o download dos arquivos necessários para o uso do projeto. A seguir explica-se o que deve ser feito para a instalação.

## Instalação do LibreOffice

Primeiramente é necessário ter a suíte de escritódio LibreOffice 7 instalado. Caso não possua em seu sistema operacional [este guia](https://pt-br.libreoffice.org/ajuda/instrucoes-de-instalacao/) pode auxiliar na instalação. **Caso o LibreOffice 7 não esteja instalado, o download do projeto não será realizado**.

Após o download do LibreOffice 7 é preciso dar permissão para que macros sejam executadas no *LibreOffice Calc*.  Abra o LibreOffice Calc e no menu superior escolha a opção "Ferramentas", em seguida "Opções...". Na nova janela, no canto esquerdo, selecione no menu com título "LibreOffice" a opção "Segurança". Aperte o botão "Segurança de macros...", escolha a opção "Baixa" e confirme apertando o botão "OK".


## Instalação da Planilha

### Windows

Para o Sistema Operacional WINDOWS 11, faça o download do instalador clicando <a id="raw-url" href="https://raw.githubusercontent.com/thuzax/Projeto-Ceramica-Dev/main/instalador/instalador_win/INSTALDOR_WINDOWS_11.exe">neste link</a>. Após isso execute a aplicação como administrador e a instalação será feita (clique com o botão direito no arquivo e vá na opção "Executar como administrador").

Se necessário será solicitado permissão para instalação de *softwares* que são utilizados pela aplicação. Confirme caso deseje que a instalação prossiga.

A pasta do *software* ficará é salvo em *Documentos* e um atalho de nome "forno.ods" será criado na Área de Trabalho.

Uma vez finalizada a instalação é possível inicializar a aplicação ao abrir o atalho com a aplicação LibreOffice Calc (clique com o botão direito no arquivo, vá na opção "Abrir com..." e selecione o LibreOffice Calc).


### Linux

Para o Sistema Operacional UBUNTU 22 ou 23, faça o download do instalador contido  <a id="raw-url" href="https://raw.githubusercontent.com/thuzax/Projeto-Ceramica-Dev/main/instalador/instalador_ubuntu/INSTALADOR_UBUNTU_22">neste link</a> e execute o instalador via terminal.

Se necessário será solicitado permissão para instalação de *softwares* que são utilizados pela aplicação. Confirme caso deseje que a instalação prossiga.

O *software* é salvo na pasta pessoal.


## Softwares de Terceiros
- Para a instalação do PDFLatex em sistemas Windows é necessário o download do *software* [MikTex](https://miktex.org/). O download e instalação deste *software* é realizado pelo instalador.
- No sistema Ubuntu é necessária a instalação do pacote [PDFLatex](https://linux.die.net/man/1/pdflatex) e do pacote [UnZip](https://infozip.sourceforge.net/UnZip.html). O download e instalação deste pacote é realizado pelo instalador.


# *Disclaimer*

## Códigos de terceiros

Este projeto se utiliza dos seguintes programas terceiros sem nenhuma modificação de seus códigos-fonte:

- PDFLatex
- MikTex
- curl
- wget
- UnZip

Por fim, este projeto também se utiliza do trabalho de [Belleboni,2016](#1) com modificações em seu código-fonte.

# Licença

A licença de uso deste projeto pode ser encontrada [aqui](LICENSE).

# Atualizações e Correções

Este projeto não prevê garantias de atualizações após sua conclusão. Os autores se isentam de qualquer responsabilidade de atualizar o Software, podendo ou não, de acordo com seu desejo, realizar modificações.


# Para Desenvolvedores

Para um guia de compilação do código-fonte ou para detalhes para desenvolvedores, acesse [este diretorio](https://github.com/thuzax/Projeto-Ceramica-Dev/blob/main/docs/).


# Links para guias

- Guia de instalação do LibreOffice: https://pt-br.libreoffice.org/ajuda/instrucoes-de-instalacao/

# Referências

<a id="1">Belleboni, Matheus G. S., 2016. </a> Aplicação web para empacotamento de peças de cerâmica em fornos tridimensionais. TCC (Graduação) - Bacharel em Ciência da Computação, Universidade de São Paulo, São Carlos - SP.

<a id="2">Foundation, T.D., 2023</a> LibreOffice Calc. Disponível em: https://hub.libreoffice.org/credits/ (Acessado em 04/01/2024).
