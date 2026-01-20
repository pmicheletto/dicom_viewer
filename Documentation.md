1.  INSIGHTS

Para desenvolver a visualização DICOM vou iniciar olhando para as bibliotecas necessárias, para a tela eu vou seguir com a raylib.h e a razão é pessoal, estou estudando essa lib para aplicar em projetos com sistemas embarcados.

    1.1 O usuário vai precisar selecionar um arquivo: a raylib tem um método nativo que detecta
    um arquivo arrastado, mas o requisito tecnico exige que o usuario passe o arquivo pelo sistema de arquivo. Dessa forma, vou usar a tinyfiledialogs, que é uma lib que chama a janela de arquivos nativa do sistema operacional.

    1.2 Vou desenvolver uma interface nível desenvolvedor, expondo informações do funcionamento.


2. DESENVOLVIMENTO

- Primeiro teste: com a tinyfiledialogs gera um erro do sistema operacional "is not responding" isso acontece porque ir para o sistema de arquivo "pausa" o ciclo while da main() e o Ubuntu(meu caso) entende que o programa travou. Para resolver isso seria necessário utilizar threads, tenho que nao é necessário explorar esse ponto.
- Distribuir classes de acordo com as responsabilidades para facilitar manutenção.
- Segundo commit: responsabilidades organizadas!
- Com uma interface funcional e emitindo logs agora vou seguir com estudo do padrão DICOM e como trabalhar com ele.
    - O arquivo DICOM é como um envelope com metadados e a imagem bruta(Pixel Data);
    - A biblioteca mais utilizada para trabalhar com arquivos DICOM em C++ é a DCMTK;
    - Pesquisei se me causaria prejuizos na visualização usando o raylib e descobri que o DICOM trabalha com 12 ou 16bit então para qualquer interpreção em display haverá a necessidade de converter para 8bits para a visualização. Entao o certo é armazenar o buffer original e o buffer para a exibição na tela ser temporario.
- Atenção para usar smart points e nao precisar ficar dando remove.
- Necessário: sudo apt install libdcmtk-dev dcmtk
- O leitor de imagem DICOM não está implementado com suporte para outras compressões além de ~*JPEG*.
- O requisito "Window Level/Width" foi implementado como automático e nao como interativo
- Para 'suporte a imagens RGB' eu adicionei o laço:

        if (dcm_image->isMonochrome()) {
            result.image.format = PIXELFORMAT_UNCOMPRESSED_GRAYSCALE;
        } else {
            result.image.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8;
        }

mas não consegui testar o funcionamento 

3. CONCLUSÃO

    A versão final cumpre com os requisitos, sendo esses testados com o arquivo `anonymized_mamo.dcm`. 
    É importante repetir o que já existe no README.md: as instruções do build em ambiente Windows foi gerado por IA e não foi testado! Toda implementação foi feita em ambiente Linux - Ubuntu 24.04.2 LTS
    Gostaria que a main.cpp tivesse ficado um pouco mais limpa, RunSelectionState e RunLoadingState não deveriam ser sua responsabilidade, mas para um caso de teste achei tranquilo manter.
    A implementação da interface foi toda hardcodada e utilizando números mágicos, seria ideal separa-los e criar uma classe config_view
    Todo o codigo foi organizado em Google Style
    Não me preocupei em verificar se a "cascata" de includes para otimização de memória

Obs: 
    Atualização de última hora (20/01 - 10h): eu estava achando o contraste ruim, até que encontrei que com dcm_image->setWindow(0) eu consigo puxar a config de contraste que vem dentro do dicom
    