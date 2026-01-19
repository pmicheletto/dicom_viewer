1.  INSIGHTS

Para desenvolver a visualização DICOM vou iniciar olhando para as bibliotecas necessárias, para a tela eu vou seguir com a raylib.h e a razão é pessoal, estou estudando essa lib para aplicar em projetos com sistemas embarcados.

    1.1 O usuário vai precisar selecionar um arquivo: a raylib tem um método nativo que detecta
    um arquivo arrastado, mas o requisito tecnico exige que o usuario passe o arquivo pelo sistema de arquivo. Dessa forma, vou usar a tinyfiledialogs, que é uma lib que chama a janela de arquivos nativa do sistema operacional.

    1.2 Vou desenvolver uma interface nível desenvolvedor, expondo informações do funcionamento.


2. DESENVOLVIMENTO

- O primeiro teste com a tinyfiledialogs gera um erro do sistema operacional "is not responding" isso acontece porque ir para o sistema de arquivo "pausa" o ciclo while da main() e o Ubuntu(meu caso) entende que o programa travou. Para resolver isso seria necessário utilizar threads, tenho que nao é necessário explorar esse ponto.
- Distribuir classes de acordo com as responsabilidades para facilitar manutenção.
- Segundo commit: responsabilidades organizadas!