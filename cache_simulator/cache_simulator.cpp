#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <algorithm>

struct MemoryBlock {
    unsigned int tag;
    bool valid;
    int frequency;
    std::vector<unsigned int> values;
};

struct CacheSet {
    std::vector<MemoryBlock> blocks;
};

void printCacheSet(const CacheSet& set) {
    std::cout << "Estado atual do conjunto da cache:\n";
    for (const auto& block : set.blocks) {
        if (block.valid) {
            std::cout << "Tag: " << block.tag << " Frequencia: " << block.frequency << " Valores: [ ";
            for (auto value : block.values) std::cout << value << " ";
            std::cout << "]\n";
            break;
        }
        else {
            std::cout << "Bloco invalido\n";
        }
    }
}

int main() {
    std::string filename;
    std::cout << "Digite o nome do arquivo de entrada: ";
    std::cin >> filename;

    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "Erro ao abrir o arquivo!" << std::endl;
        return 1;
    }

    int mpSize, wordsPerBlock, cacheSize, linesPerSet;
    inputFile >> mpSize >> wordsPerBlock >> cacheSize >> linesPerSet;
    inputFile.close();

    if (mpSize <= 0 || wordsPerBlock <= 0 || cacheSize <= 0 || linesPerSet <= 0) {
        std::cerr << "Valores de configuracao invalidos!" << std::endl;
        return 1;
    }

    int w = log2(wordsPerBlock);
    int d = log2((cacheSize * 1024) / (linesPerSet * wordsPerBlock * 4));
    int s = log2((mpSize * 1024) / (wordsPerBlock * 4));
    int tag = s - d;

    std::vector<MemoryBlock> mainMemory((mpSize * 1024) / (wordsPerBlock * 4));
    srand(time(NULL));
    for (auto& block : mainMemory) {
        block.values.resize(wordsPerBlock);
        for (auto& value : block.values) {
            value = rand();
        }
    }

    std::vector<CacheSet> cache((cacheSize * 1024) / (linesPerSet * wordsPerBlock * 4));
    for (auto& set : cache) {
        set.blocks.resize(linesPerSet);
        for (auto& block : set.blocks) {
            block.valid = false;
            block.frequency = 0;
            block.values.resize(wordsPerBlock);
        }
    }

    std::cout << "\nMemoria Principal e Cache inicializadas.\n";
    std::cout << "Tamanho da Memoria Principal: " << mpSize << " KB" << std::endl;
    std::cout << "Quantidade de palavras por bloco na MP: " << wordsPerBlock << std::endl;
    std::cout << "Tamanho da cache: " << cacheSize << " KB" << std::endl;
    std::cout << "Numero de linhas por conjunto da cache: " << linesPerSet << std::endl;
    std::cout << "w = " << w << ", d = " << d << ", s = " << s << ", tag = " << tag << std::endl;

    unsigned int cacheHits = 0, cacheMisses = 0, cacheReplacements = 0;
    int option;
    bool running = true;

    while (running) {
        std::cout << "\nMenu:\n";
        std::cout << "1. Entrar com um endereco de MP para acesso\n";
        std::cout << "2. Ler um arquivo de enderecos de MP\n";
        std::cout << "3. Sair\n";
        std::cout << "Escolha uma opcao: ";
        std::cin >> option;

        switch (option) {
        case 1: {
            unsigned int address;
            std::cout << "Digite o endereco da MP que deseja acessar: ";
            std::cin >> address;

            int blockSize = wordsPerBlock * 4;

            int numLines = ((cacheSize * 1024) / 4) / wordsPerBlock;
            int numSets = numLines / linesPerSet;

            unsigned int blockIndex = address / blockSize;

            unsigned int setIndex = blockIndex % numSets;
            unsigned int calculatedTag = blockIndex / numSets;

            bool found = false;
            for (auto& block : cache[setIndex].blocks) {
                if (block.valid && block.tag == calculatedTag) {
                    found = true;
                    block.frequency++;
                    cacheHits++;
                    unsigned int value = block.values[address & ((1 << w) - 1)];
                    std::cout << "Cache hit! Palavra lida da cache: " << value << std::endl;
                    std::cout << "Localizado no conjunto " << setIndex << ", linha da cache: " << &block - &cache[setIndex].blocks[0] << std::endl;
                    printCacheSet(cache[setIndex]);
                    break;
                }
            }

            if (!found) {
                cacheMisses++;
                auto minFreqBlock = std::min_element(cache[setIndex].blocks.begin(), cache[setIndex].blocks.end(),
                    [](const MemoryBlock& a, const MemoryBlock& b) {
                        return a.frequency < b.frequency || (!a.valid && b.valid);
                    });

                if (minFreqBlock->valid) {
                    cacheReplacements++;
                }
                minFreqBlock->valid = true;
                minFreqBlock->tag = calculatedTag;
                minFreqBlock->values = mainMemory[blockIndex].values;
                minFreqBlock->frequency = 1;

                std::cout << "Cache miss! Bloco lido da MP e colocado na cache. ";
                std::cout << "Localizado no conjunto " << setIndex << ", linha da cache substituida: " << std::distance(cache[setIndex].blocks.begin(), minFreqBlock) << std::endl;
                printCacheSet(cache[setIndex]);
            }
            break;
        }
        case 2: {
            std::string addressFilename;
            std::cout << "Digite o nome do arquivo de enderecos de MP: ";
            std::cin >> addressFilename;

            std::ifstream addressFile(addressFilename);
            if (!addressFile.is_open()) {
                std::cerr << "Erro ao abrir o arquivo de enderecos!" << std::endl;
                break;
            }

            unsigned int address;
            while (addressFile >> address) {
                int blockSize = wordsPerBlock * 4;
                int numLines = ((cacheSize * 1024) / 4) / wordsPerBlock;
                int numSets = numLines / linesPerSet;
                unsigned int blockIndex = address / blockSize;
                unsigned int setIndex = blockIndex % numSets;
                unsigned int calculatedTag = blockIndex / numSets;

                bool found = false;
                for (auto& block : cache[setIndex].blocks) {
                    if (block.valid && block.tag == calculatedTag) {
                        found = true;
                        block.frequency++;
                        cacheHits++;
                        unsigned int value = block.values[address & ((1 << w) - 1)];
                        std::cout << "Cache hit! Palavra lida da cache: " << value << std::endl;
                        std::cout << "Localizado no conjunto " << setIndex << ", linha da cache: " << &block - &cache[setIndex].blocks[0] << std::endl;
                        printCacheSet(cache[setIndex]);
                        break;
                    }
                }

                if (!found) {
                    cacheMisses++;
                    auto minFreqBlock = std::min_element(cache[setIndex].blocks.begin(), cache[setIndex].blocks.end(),
                        [](const MemoryBlock& a, const MemoryBlock& b) {
                            return a.frequency < b.frequency || (!a.valid && b.valid);
                        });

                    if (minFreqBlock->valid) {
                        cacheReplacements++;
                    }
                    minFreqBlock->valid = true;
                    minFreqBlock->tag = calculatedTag;
                    minFreqBlock->values = mainMemory[blockIndex].values;
                    minFreqBlock->frequency = 1;

                    std::cout << "Cache miss! Bloco lido da MP e colocado na cache. ";
                    std::cout << "Localizado no conjunto " << setIndex << ", linha da cache substituida: " << std::distance(cache[setIndex].blocks.begin(), minFreqBlock) << std::endl;
                    printCacheSet(cache[setIndex]);
                }
            }

            addressFile.close();
            break;
        }
        case 3:
            std::cout << "Saindo do programa...\n";
            running = false;
            break;
        default:
            std::cout << "Opcao invalida. Tente novamente.\n";
            break;
        }
    }

    std::cout << "Estatisticas finais: Hits: " << cacheHits << ", Misses: " << cacheMisses << ", Substituicoes: " << cacheReplacements << std::endl;

    return 0;
}