#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <openssl/sha.h>

struct Transaction {
    char sender[50];
    char recipient[50];
    double amount;
};

struct Block {
    int index;
    time_t timestamp;
    struct Transaction transactions[50];
    char previousHash[65];
    char hash[65];
    int nonce;
};

struct Blockchain {
    struct Block blocks[50];
    int blockCount;
};

void calculateHash(struct Block *block);
void mineBlock(struct Block *block);
double getBlockBalance(struct Block *block);
void printBlock(struct Block *block);
int isChainValid(struct Blockchain *blockchain);
void addBlock(struct Blockchain *blockchain, struct Transaction *transactions);

int main() {
    struct Blockchain blockchain;
    blockchain.blockCount = 0;

    struct Transaction initialTransactions[] = {
        {"Alice", "Bob", 10},
        {"Bob", "Charlie", 5},
        {"Charlie", "David", 2}
    };

    addBlock(&blockchain, initialTransactions);

    struct Transaction transactions1[] = {
        {"David", "Alice", 7},
        {"Alice", "Bob", 3}
    };

    addBlock(&blockchain, transactions1);

    for (int i = 0; i < blockchain.blockCount; i++) {
        printBlock(&blockchain.blocks[i]);
        printf("\n");
    }

    printf("Is the blockchain valid? %s\n", isChainValid(&blockchain) ? "Yes" : "No");

    return 0;
}

void calculateHash(struct Block *block) {
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, &block->index, sizeof(block->index));
    SHA256_Update(&sha256, &block->timestamp, sizeof(block->timestamp));
    SHA256_Update(&sha256, block->transactions, sizeof(struct Transaction) * 50);
    SHA256_Update(&sha256, block->previousHash, sizeof(block->previousHash));
    SHA256_Update(&sha256, &block->nonce, sizeof(block->nonce));
    SHA256_Final(block->hash, &sha256);
}

void mineBlock(struct Block *block) {
    do {
        block->nonce++;
        calculateHash(block);
    } while (block->hash[0] != '0' || block->hash[1] != '0' || block->hash[2] != '0' || block->hash[3] != '0');
}

double getBlockBalance(struct Block *block) {
    double balance = 0;
    for (int i = 0; i < 50; i++) {
        balance += block->transactions[i].amount;
    }
    return balance;
}

void printBlock(struct Block *block) {
    printf("Block #%d\n", block->index);
    printf("Timestamp: %s", ctime(&block->timestamp));
    printf("Transactions:\n");
    for (int i = 0; i < 50; i++) {
        printf("  %s sent %.2f to %s\n", block->transactions[i].sender,
               block->transactions[i].amount, block->transactions[i].recipient);
    }
    printf("Previous Hash: %s\n", block->previousHash);
    printf("Hash: %s\n", block->hash);
    printf("Nonce: %d\n", block->nonce);
}

int isChainValid(struct Blockchain *blockchain) {
    for (int i = 1; i < blockchain->blockCount; i++) {
        struct Block *currentBlock = &blockchain->blocks[i];
        struct Block *previousBlock = &blockchain->blocks[i - 1];

        calculateHash(currentBlock);

        if (strcmp(currentBlock->hash, currentBlock->hash) != 0) {
            return 0;
        }

        if (strcmp(currentBlock->previousHash, previousBlock->hash) != 0) {
            return 0;
        }
    }
    return 1;
}

void addBlock(struct Blockchain *blockchain, struct Transaction *transactions) {
    struct Block newBlock;
    newBlock.index = blockchain->blockCount;
    newBlock.timestamp = time(NULL);
    memcpy(newBlock.transactions, transactions, sizeof(struct Transaction) * 50);

    if (newBlock.index > 0) {
        struct Block *previousBlock = &blockchain->blocks[newBlock.index - 1];
        strcpy(newBlock.previousHash, previousBlock->hash);
    } else {
        strcpy(newBlock.previousHash, "0");
    }

    mineBlock(&newBlock);

    blockchain->blocks[blockchain->blockCount] = newBlock;
    blockchain->blockCount++;
}
