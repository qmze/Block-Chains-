using System;
using System.Collections.Generic;
using System.Security.Cryptography;
using System.Text;

class Transaction
{
    public string Sender { get; }
    public string Recipient { get; }
    public decimal Amount { get; }

    public Transaction(string sender, string recipient, decimal amount)
    {
        Sender = sender;
        Recipient = recipient;
        Amount = amount;
    }

    public override string ToString()
    {
        return $"{Sender} sent {Amount} to {Recipient}";
    }
}

class Block
{
    public int Index { get; set; }
    public DateTime Timestamp { get; set; }
    public List<Transaction> Transactions { get; }
    public string PreviousHash { get; set; }
    public string Hash { get; set; }
    public int Nonce { get; set; }

    public Block(int index, List<Transaction> transactions, string previousHash)
    {
        Index = index;
        Timestamp = DateTime.Now;
        Transactions = transactions;
        PreviousHash = previousHash;
        Nonce = 0;
        MineBlock();
    }

    public void MineBlock()
    {
        do
        {
            Nonce++;
            CalculateHash();
        } while (!Hash.StartsWith("0000"));
    }

    public void CalculateHash()
    {
        using (SHA256 sha256 = SHA256.Create())
        {
            byte[] inputBytes = Encoding.UTF8.GetBytes($"{Index}{Timestamp}{Transactions}{PreviousHash}{Nonce}");
            byte[] hashBytes = sha256.ComputeHash(inputBytes);
            Hash = BitConverter.ToString(hashBytes).Replace("-", "");
        }
    }

    public decimal GetBlockBalance()
    {
        decimal balance = 0;
        foreach (var transaction in Transactions)
        {
            balance += transaction.Amount;
        }
        return balance;
    }

    public override string ToString()
    {
        StringBuilder sb = new StringBuilder();
        sb.AppendLine($"Block #{Index}");
        sb.AppendLine($"Timestamp: {Timestamp}");
        sb.AppendLine("Transactions:");
        foreach (var transaction in Transactions)
        {
            sb.AppendLine($"  {transaction}");
        }
        sb.AppendLine($"Previous Hash: {PreviousHash}");
        sb.AppendLine($"Hash: {Hash}");
        sb.AppendLine($"Nonce: {Nonce}");
        return sb.ToString();
    }
}

class Blockchain
{
    private readonly List<Block> _blocks = new List<Block>();

    public void AddBlock(List<Transaction> transactions)
    {
        int index = _blocks.Count;
        string previousHash = index > 0 ? _blocks[index - 1].Hash : "0";
        Block newBlock = new Block(index, transactions, previousHash);
        _blocks.Add(newBlock);
    }

    public bool IsChainValid()
    {
        for (int i = 1; i < _blocks.Count; i++)
        {
            Block currentBlock = _blocks[i];
            Block previousBlock = _blocks[i - 1];

            if (currentBlock.Hash != currentBlock.GetExpectedHash())
            {
                return false;
            }

            if (currentBlock.PreviousHash != previousBlock.Hash)
            {
                return false;
            }
        }

        return true;
    }

    public void PrintBlockchain()
    {
        foreach (Block block in _blocks)
        {
            Console.WriteLine(block);
            Console.WriteLine();
        }
    }
}

class Program
{
    static void Main()
    {
        Blockchain blockchain = new Blockchain();

        List<Transaction> initialTransactions = new List<Transaction>
        {
            new Transaction("Alice", "Bob", 10),
            new Transaction("Bob", "Charlie", 5),
            new Transaction("Charlie", "David", 2)
        };

        blockchain.AddBlock(initialTransactions);

        List<Transaction> transactions1 = new List<Transaction>
        {
            new Transaction("David", "Alice", 7),
            new Transaction("Alice", "Bob", 3)
        };

        blockchain.AddBlock(transactions1);

        blockchain.PrintBlockchain();

        Console.WriteLine($"Is the blockchain valid? {blockchain.IsChainValid()}");
    }
}
