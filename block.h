#pragma once

#include "stdafx.h"
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <ctime>
#include <chrono>
#include <stdint.h>
#include <conio.h>

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
- Transactions
https://bitcoin.stackexchange.com/questions/30837/how-did-satoshi-manage-to-mine-so-many-bitcoins-being-the-first-user-of-the-syst
https://blog.cex.io/bitcoin-dictionary/coinbase-transaction-12088
https://www.quora.com/What-was-the-first-Bitcoin-transaction
https://en.bitcoin.it/wiki/Transaction_expiration
https://en.bitcoin.it/wiki/Transaction
- Unspent transaction outputs: là kết quả của các giao dịch (transaction) trước, kiểu như theo dấu dòng tiền có nguồn từ đâu
- Node: https://hackernoon.com/lets-talk-about-bitcoin-nodes-e9502193198c
	+ https://bitcoin.stackexchange.com/questions/18736/what-is-a-bitcoin-node
	+ https://en.bitcoin.it/wiki/Full_node
- Transaction relaying:  In bitcoin, these transaction are also known as “unconfirmed transactions”.
	+ Typically, when someone wants to include a transaction to the blockchain (= send coins to some address ) he broadcasts the transaction to the network and hopefully some node will mine the transaction to the blockchain.
	+ This feature is very important for a working cryptocurrency, since it means you don’t need to mine a block yourself, in order to include a transaction to the blockchain.
- As a consequence, the nodes will now share two types of data when they communicate with each other:
	+ the state of the blockchain ( =the blocks and transactions that are included to the blockchain)
	+ unconfirmed transactions ( =the transactions that are not yet included in the blockchain)
- Consensus problem: https://bitcoin.stackexchange.com/questions/10343/why-is-mining-necessary-for-the-bitcoin-network-system
	+ https://bitcoin.stackexchange.com/questions/67097/so-i-mined-a-block-but-why-would-other-nodes-accept-my-mined-block
- Nonce bị tràn số thì làm sao? extraNonce là 1 value dc đặt cạnh head của block, bởi vì nếu hash k ra kết quả (do nonce kìa tràn số, nonce kia ở coinbase) thì extraNonce dc tăng thêm
	+ giá trị tràn số: 4,294,967,296
	+ https://bitcointalk.org/index.php?topic=1040859.0
	+ https://www.reddit.com/r/Bitcoin/comments/3mjgu9/can_somebody_illustrate_precisely_why_a_block_is/
-  Có thể ăn cắp nonce dc k? it is extremely unlikely for two people to have the same Merkle root because the first transaction in your block is a generation "sent" to one of your unique Bitcoin addresses.
	+ https://bitcoin.stackexchange.com/questions/51300/how-to-prevent-a-miner-from-stealing-another-miners-block?rq=1
-  how this transaction grouping is done by mining nodes?
	+ because most pools run completely custom software, they have their own criteria and their own optimizations which they perform.
	+ https://bitcoin.stackexchange.com/questions/40398/when-does-a-miner-decide-to-stop-collecting-transactions-and-start-calculating-h?noredirect=1&lq=1
	+ https://bitcoin.stackexchange.com/questions/49178/how-does-a-node-find-a-transaction-in-the-blockchain?rq=1
- The blockchain is always in sync and are to be validated by majority of network,
	+ https://bitcoin.stackexchange.com/questions/43990/how-does-each-miner-know-when-to-create-a-block?rq=1
	https://bitcoin.stackexchange.com/questions/13731/does-a-bitcoin-node-need-to-scan-through-the-whole-blockchain-in-order-to-be-100?rq=1
- SVP: simplifying payment verification
	+ https://bitcoin.stackexchange.com/questions/57434/understanding-spv-pruning-and-its-downsides-as-well?rq=1
	+ https://bitcoin.stackexchange.com/questions/35997/how-does-an-spv-node-verify-an-unconfirmed-transaction?rq=1
- What is pruning node? A pruning node will trim the number of transactions you need to store, The minimum you can keep with pruning is a couple of days worth of blocks, not a couple of blocks
	+ https://bitcoin.stackexchange.com/questions/42287/what-do-pruning-nodes-contribute-to-the-network?rq=1 final answer
- Prvent double-spend:
	+ https://bitcoin.stackexchange.com/questions/53217/how-does-spv-prevent-double-spending-of-utxo?rq=1
	+ https://bitcoin.stackexchange.com/questions/54376/why-does-only-the-earliest-transaction-matter-for-double-spending?rq=1
	+ https://bitcoin.stackexchange.com/questions/7259/how-does-the-bitcoin-client-check-for-double-spend?noredirect=1&lq=1
	+ https://bitcoin.stackexchange.com/questions/54376/why-does-only-the-earliest-transaction-matter-for-double-spending?rq=1
	+ https://bitcoin.stackexchange.com/questions/20929/how-do-people-actually-try-double-spends?rq=1
- Miners are incentivised to include as many transactions as they can, in order from highest to lowest fee paid. So they connect to a large number of nodes to make sure they have the most complete information they can reasonably get in order to fo this.
- A miner begins creating a new block as soon as it receives a valid block from another miner. It then begins to construct and hash the next block on top of the block it just received.
	+ https://bitcoin.stackexchange.com/questions/43990/how-does-each-miner-know-when-to-create-a-block?rq=1
- How tranx added to mining pool?
	+ https://bitcoin.stackexchange.com/questions/20257/how-transactions-get-added-to-a-ledger?rq=1
- Timewrap attack:
	+ https://bitcoin.stackexchange.com/questions/48503/how-does-a-time-warp-attack-work
	+ https://bitcoin.stackexchange.com/questions/35321/how-could-a-time-warp-attack-be-prevented?rq=1
- Bug overflow on bitcoin: https://www.coindesk.com/9-biggest-screwups-bitcoin-history/
- Compare 2 chains to choose in node
	+ https://bitcoin.stackexchange.com/questions/44470/how-does-a-node-compare-two-blockchains?rq=1
- Cumulative difficulty:
	+ Which blockchain has the most work?
	+ Which one was received first? (This can be different for different clients, which is why the previous rule is applied first.)
	+ Which one has a larger pointer address? (This is largely random, and different for different clients.)
	+ https://bitcoin.stackexchange.com/questions/37273/how-is-a-blockchain-split-resolved?noredirect=1&lq=1
	+ https://bitcoin.stackexchange.com/questions/35635/can-a-shorter-chain-overcome-a-longer-one?noredirect=1&lq=1
	+ https://bitcoin.stackexchange.com/questions/936/how-does-a-client-decide-which-is-the-longest-block-chain-if-there-is-a-fork
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

// ECDSA https://github.com/esxgx/easy-ecc

#ifndef _EASY_ECC_H_
#define _EASY_ECC_H_

/* Curve selection options. */
#define secp128r1 16
#define secp192r1 24
#define secp256r1 32
#define secp384r1 48
#ifndef ECC_CURVE
#define ECC_CURVE secp256r1
#endif

#if (ECC_CURVE != secp128r1 && ECC_CURVE != secp192r1 && ECC_CURVE != secp256r1 && ECC_CURVE != secp384r1)
#error "Must define ECC_CURVE to one of the available curves"
#endif

#define ECC_BYTES ECC_CURVE

#ifdef __cplusplus
extern "C"
{
#endif

	/* ecc_make_key() function.
	Create a public/private key pair.

	Outputs:
	p_publicKey  - Will be filled in with the public key.
	p_privateKey - Will be filled in with the private key.

	Returns 1 if the key pair was generated successfully, 0 if an error occurred.
	*/
	int ecc_make_key(uint8_t p_publicKey[ECC_BYTES + 1], uint8_t p_privateKey[ECC_BYTES]);

	/* ecdh_shared_secret() function.
	Compute a shared secret given your secret key and someone else's public key.
	Note: It is recommended that you hash the result of ecdh_shared_secret before using it for symmetric encryption or HMAC.

	Inputs:
	p_publicKey  - The public key of the remote party.
	p_privateKey - Your private key.

	Outputs:
	p_secret - Will be filled in with the shared secret value.

	Returns 1 if the shared secret was generated successfully, 0 if an error occurred.
	*/
	int ecdh_shared_secret(const uint8_t p_publicKey[ECC_BYTES + 1], const uint8_t p_privateKey[ECC_BYTES], uint8_t p_secret[ECC_BYTES]);

	/* ecdsa_sign() function.
	Generate an ECDSA signature for a given hash value.

	Usage: Compute a hash of the data you wish to sign (SHA-2 is recommended) and pass it in to
	this function along with your private key.

	Inputs:
	p_privateKey - Your private key.
	p_hash       - The message hash to sign.

	Outputs:
	p_signature  - Will be filled in with the signature value.

	Returns 1 if the signature generated successfully, 0 if an error occurred.
	*/
	int ecdsa_sign(const uint8_t p_privateKey[ECC_BYTES], const uint8_t p_hash[ECC_BYTES], uint8_t p_signature[ECC_BYTES * 2]);

	/* ecdsa_verify() function.
	Verify an ECDSA signature.

	Usage: Compute the hash of the signed data using the same hash as the signer and
	pass it to this function along with the signer's public key and the signature values (r and s).

	Inputs:
	p_publicKey - The signer's public key
	p_hash      - The hash of the signed data.
	p_signature - The signature value.

	Returns 1 if the signature is valid, 0 if it is invalid.
	*/
	int ecdsa_verify(const uint8_t p_publicKey[ECC_BYTES + 1], const uint8_t p_hash[ECC_BYTES], const uint8_t p_signature[ECC_BYTES * 2]);

#ifdef __cplusplus
} /* end of extern "C" */
#endif

#endif /* _EASY_ECC_H_ */
//End ECDSA

class Node
{
private:
	uint8_t private_key;
	uint8_t public_key; // lấy cái này để so sánh giữa 2 node có khác nhau k
	string balance; // tiền trong wallet
	//vector<block *> BLOCK_CHAIN;
public:
	//Node(uint8_t private_key, uint8_t public_key, string balance, vector<block *> BLOCK_CHAIN);
	//~Node();

	uint8_t get_pubkey();
};

// Transaction outputs (txOut) consists of an address and an amount of coins.
struct coin_out
{
	uint8_t address;
	double amount;
};

struct coin_in
{
	uint8_t pre_out_address; // track địa chỉ của lần giao dịch trước
	int index_output; // vị trí của output trong transaction trc, vì 1 transaction có thể có nhiều output
	uint8_t signature;
};

class tranx {
private:
	std::string id; // điều kiện để các transaction khác nhau
	coin_in input;
	coin_out output;
public:
	tranx();
	~tranx();
};

class block
{
private:
	long long index;
	std::vector<tranx> transactions;
	std::string hash;
	std::string previous_hash;
	std::string timestamp;
	long long difficulty;
	long long nonce;
	double duration;
public:
	block(long long index, std::vector<tranx> transactions, std::string previous_hash, std::string timestamp, long long difficulty, long long nonce);
	~block();

	void set_nonce(long long n);
	void set_duration(double d);
	long long get_index();
	std::string get_hash();
	long long get_nonce();
	long long get_difficulty();
	double get_duration();
	std::string Hash_A_Block(int index, std::vector<tranx> transactions, std::string previous_hash, std::string timestamp, long long difficulty, long long nonce);
};

// Other functions
void menu();