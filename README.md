# Simulador de Memória Cache
Este projeto implementa um simulador de memória cache que permite configurar e testar o desempenho de uma cache em relação a uma memória principal. A simulação inclui a inicialização da memória, leitura de endereços de memória e avaliação do desempenho da cache através de estatísticas de hits, misses e substituições.


## Instruções!!

### 1. Preparação do Arquivo de Configuração

Antes de iniciar o programa, prepare um arquivo de texto que contenha as configurações iniciais da simulação. O arquivo deve ter os seguintes parâmetros, cada um em uma nova linha:

Tamanho da Memória Principal (KB): O tamanho total da memória principal em kilobytes.
Palavras por Bloco: O número de palavras que cada bloco de memória contém.
Tamanho da Cache (KB): O tamanho total da cache em kilobytes.
Linhas por Conjunto: O número de linhas (ou blocos) em cada conjunto da cache.

## Exemplo de Arquivo de Configuração (config.txt):

```
256
2
32
2
```

### 2. Execução do Programa

Compile o programa (se necessário):
```
g++ -o cache_simulator cache_simulator.cpp
```

Execute o programa:
```
./cache_simulator
```

Digite o nome do arquivo de configuração: O programa solicitará o nome do arquivo de configuração. Digite o nome do arquivo preparado no Passo 1 e pressione Enter. O programa lerá as configurações e inicializará a memória principal e a cache com base nesses parâmetros.

## 3. Menu Interativo
Após a inicialização, você será apresentado a um menu interativo com as seguintes opções:

Entrar com um endereço de MP para acesso: Permite que você digite manualmente um endereço de memória principal para acessar e verificar se os dados estão na cache.
Ler um arquivo de endereços de MP: Permite que você especifique um arquivo que contém uma lista de endereços de memória principal a serem acessados sequencialmente.
Sair: Encerra o programa.

### 4. Acessando um Endereço de Memória Principal

Se você escolher a opção 1, o programa solicitará que você insira um endereço de memória. Após fornecer um endereço válido, o programa verificará se este endereço resulta em um hit ou miss na cache, atualizando as estatísticas e exibindo os resultados.

### 5. Usando um Arquivo de Endereços

Se você escolher a opção 2, precisará fornecer o nome de um arquivo que contém vários endereços de memória, um por linha. O programa processará cada endereço sequencialmente, atualizando as estatísticas de acesso e exibindo os resultados após cada acesso.

Exemplo de Arquivo de Endereços (addresses.txt):
```
1024
2048
3072
...
```

## 6.Visualizando Estatísticas e Saindo

### Prepare os Arquivos de Endereço Antecipadamente: Para facilitar os testes, prepare arquivos de endereços com antecedência, especialmente se planeja realizar múltiplas simulações.
### Entenda os Resultados: Familiarize-se com o significado de "hit" e "miss" na cache para melhor interpretar os resultados e entender o desempenho da simulação.

## Exemplo de Execução
```
$ ./cache_simulator
Digite o nome do arquivo de entrada: config.txt

Memória Principal e Cache inicializadas.
Tamanho da Memória Principal: 256 KB
Quantidade de palavras por bloco na MP: 2
Tamanho da cache: 32 KB
Número de linhas por conjunto da cache: 2
w = 1, d = 11, s = 15, tag = 4

Menu:
1. Entrar com um endereço de MP para acesso
2. Ler um arquivo de endereços de MP
3. Sair
Escolha uma opção: 2
Digite o nome do arquivo de endereços de MP: addresses.txt
Cache miss! Bloco lido da MP e colocado na cache. Localizado no conjunto 128, linha da cache substituída: 0
Estado atual do conjunto da cache:
Tag: 0 Frequência: 1 Valores: [ 18109 11549 ]
...
Estatísticas finais: Hits: 0, Misses: 10, Substituições: 0
```
