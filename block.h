#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <ctime>
#include <chrono>
#include "stdafx.h"

using namespace std;

/*
- register_node: Add a new node to the list of nodes (Node: another datacenter(usually computer) to store Blocks)
- valid_chain: Determine if a given blockchain is valid
- resolve_conflicts: consensus
- new_block: Create a new Block in the Blockchain
	+ 1 block có giới hạn tối đa 1.000.000.000 bytes, nếu như trong thời gian 1 block đang add trans mà PoW đã solved xong thì block đó đóng lại
	+ So the number of transactions in a block is actually a function of the number of transactions being generated over a time period and the time taken to solve a given block.
	+ 1 miner nếu giải dc PoW mà block đó dù chỉ có 1 trans thì những trans khác phải chuyển sang block kế tiếp
- new_transaction: Creates a new transaction to go into the next mined Block
- hash:  Creates a SHA-256 hash of a Block
- proof_of_work: Simple Proof of Work Algorithm:
	+ Find a number p' such that hash(pp') contains leading 4 zeroes
	+ Where p is the previous proof, and p' is the new proof
- valid_proof: Validates the Proof
- A timestamp is accepted as valid if it is greater than the median timestamp of previous 11 blocks, and less than the network-adjusted time + 2 hours. "Network-adjusted time" is the median of the timestamps returned by all nodes connected to you."
- Mỗi block sẽ được tạo ra trong 1 khoảng thời gian, phụ thuộc vào độ khó khi giải PoW, và khi 1 block dc giải xong, nó sẽ cần truyền cho tất cả các node còn lại
- Phần thưởng cho miner đào dc 200.000 block đầu tiên là 50 coin, sau đó tiếp theo cứ 200.000 thì phần thưởng lại dc giảm đi 1 nữa
- Parameters in hash function: https://en.bitcoin.it/wiki/Block_hashing_algorithm
- But for now, we use hash(pre_hash, time, pre_nonce, merkel_root )
- Merkel Tree: https://bitcoin.stackexchange.com/questions/10479/what-is-the-merkle-root

- Talk more about timestamp: https://en.bitcoin.it/wiki/Block_timestamp
	 + Each block contains a Unix time timestamp. In addition to serving as a source of variation for the block hash, they also make it more difficult for an adversary to manipulate the block chain.
	 + A timestamp is accepted as valid if it is greater than the median timestamp of previous 11 blocks, and less than the network-adjusted time + 2 hours. 
	 + "Network-adjusted time" is the median of the timestamps returned by all nodes connected to you. As a result, block timestamps are not exactly accurate, and they do not even need to be in order. Block times are accurate only to within an hour or two.
	 + Bitcoin uses an unsigned integer for the timestamp, so the year 2038 problem is delayed for another 68 years. Đơn giản là tràn số =.= https://en.wikipedia.org/wiki/Year_2038_problem
- Proof of Work:
	+ The Proof-of-work puzzle is to find a block hash, that has a specific number of zeros prefixing it. The difficulty property defines how many prefixing zeros the block hash must have, in order for the block to be valid. The prefixing zeros are checked from the binary format of the hash.
- How difficulty is calculated: https://bitcoin.stackexchange.com/questions/5838/how-is-difficulty-calculated

https://bitcoin.stackexchange.com/questions/23912/how-is-the-target-section-of-a-block-header-calculated
https://bitcoin.stackexchange.com/questions/1781/nonce-size-will-it-always-be-big-enough
*/

//SHA-256

#ifndef SHA256_H
#define SHA256_H
#include <string>

class SHA256
{
protected:
	typedef unsigned char uint8;
	typedef unsigned int uint32;
	typedef unsigned long long uint64;

	const static uint32 sha256_k[];
	static const unsigned int SHA224_256_BLOCK_SIZE = (512 / 8);
public:
	void init();
	void update(const unsigned char *message, unsigned int len);
	void final(unsigned char *digest);
	static const unsigned int DIGEST_SIZE = (256 / 8);

protected:
	void transform(const unsigned char *message, unsigned int block_nb);
	unsigned int m_tot_len;
	unsigned int m_len;
	unsigned char m_block[2 * SHA224_256_BLOCK_SIZE];
	uint32 m_h[8];
};

std::string sha256(std::string input);

#define SHA2_SHFR(x, n)    (x >> n)
#define SHA2_ROTR(x, n)   ((x >> n) | (x << ((sizeof(x) << 3) - n)))
#define SHA2_ROTL(x, n)   ((x << n) | (x >> ((sizeof(x) << 3) - n)))
#define SHA2_CH(x, y, z)  ((x & y) ^ (~x & z))
#define SHA2_MAJ(x, y, z) ((x & y) ^ (x & z) ^ (y & z))
#define SHA256_F1(x) (SHA2_ROTR(x,  2) ^ SHA2_ROTR(x, 13) ^ SHA2_ROTR(x, 22))
#define SHA256_F2(x) (SHA2_ROTR(x,  6) ^ SHA2_ROTR(x, 11) ^ SHA2_ROTR(x, 25))
#define SHA256_F3(x) (SHA2_ROTR(x,  7) ^ SHA2_ROTR(x, 18) ^ SHA2_SHFR(x,  3))
#define SHA256_F4(x) (SHA2_ROTR(x, 17) ^ SHA2_ROTR(x, 19) ^ SHA2_SHFR(x, 10))
#define SHA2_UNPACK32(x, str)                 \
{                                             \
    *((str) + 3) = (uint8) ((x)      );       \
    *((str) + 2) = (uint8) ((x) >>  8);       \
    *((str) + 1) = (uint8) ((x) >> 16);       \
    *((str) + 0) = (uint8) ((x) >> 24);       \
}
#define SHA2_PACK32(str, x)                   \
{                                             \
    *(x) =   ((uint32) *((str) + 3)      )    \
           | ((uint32) *((str) + 2) <<  8)    \
           | ((uint32) *((str) + 1) << 16)    \
           | ((uint32) *((str) + 0) << 24);   \
}
#endif

//End SHA-256

class block
{
private:
	long long index;
	std::vector<int> transactions;
	std::string hash;
	std::string previous_hash;
	std::string timestamp;
	long long difficulty;
	long long nonce;
	double duration;

public:
	block(long long index, std::vector<int> transactions, std::string previous_hash, std::string timestamp, long long difficulty, long long nonce);
	~block();

	void set_nonce(long long n);
	void set_duration(double d);
	long long get_index();
	std::string get_hash();
	long long get_nonce();
	long long get_difficulty();
	double get_duration();
	std::string Hash_A_Block(int index, std::vector<int> transactions, std::string previous_hash, std::string timestamp, long long difficulty, long long nonce);
};