// BlockChainMain.cpp : Defines the entry point for the console application.
//
#include "stdafx.h" // má, để cái này củ chuối này lên đầu mới k bị lỗi =.=
#include "block.h"

using namespace std;

int main()
{
	// User_1
	cout << "User 1: ";
	// https://www.badprog.com/c-type-what-are-uint8-t-uint16-t-uint32-t-and-uint64-t: giải thích kiểu dữ liệu lạ :/
	uint8_t p_publicKey[ECC_BYTES + 1];
	uint8_t p_privateKey[ECC_BYTES];
	if (ecc_make_key(p_publicKey, p_privateKey) == 1) {
		//string s;
		//s.assign(p_publicKey, p_publicKey + sizeof(p_publicKey));
		//cout << hex << string(p_publicKey, p_publicKey + sizeof(p_publicKey)) <<  endl;
		//cout << hex << string(p_privateKey, p_privateKey + sizeof(p_privateKey)) << endl;
		//https://stackoverflow.com/questions/29320434/convert-array-of-uint8-t-to-string-in-c
		//https://stackoverflow.com/questions/7664529/converting-a-string-to-uint8-t-array-in-c
		ostringstream convert;
		for (int a = 0; a < ECC_BYTES + 1; a++) {
			convert << (int)p_publicKey[a];
		}
		string key_string = convert.str();
		cout << key_string << endl;
	}

	// User_2
	cout << "User 2: ";
	uint8_t p_publicKey_2[ECC_BYTES + 1];
	uint8_t p_privateKey_2[ECC_BYTES];
	
	if (ecc_make_key(p_publicKey, p_privateKey) == 1) {
		ostringstream convert;
		for (int a = 0; a < ECC_BYTES + 1; a++) {
			convert << (int)p_publicKey[a];
		}
		string key_string = convert.str();
		cout << key_string << endl;
	}

	// Sign
	cout << "Sigature: ";
	uint8_t p_signature[ECC_BYTES * 2];
	string mess = "this is a mess";
	const uint8_t * cbuff = reinterpret_cast<const uint8_t*>(mess.c_str());
	if (ecdsa_sign(p_privateKey, cbuff, p_signature)) {
		ostringstream convert;
		for (int a = 0; a < ECC_BYTES + 1; a++) {
			convert << (int)p_signature[a];
		}
		string key_string = convert.str();
		cout << key_string << endl;
	}

	// Verify
	cout << "Verify: ";
	if (ecdsa_verify(p_publicKey, cbuff, p_signature)) {
		cout << "Verified!!!\n";
	}

	
	std::string output1 = sha256("anh duc");
	cout << output1;
	vector<Node> NODE;
	vector<block *> BLOCK_CHAIN;

	// Khởi tạo block đầu tiên, còn gọi là Genesis block
	time_t result = std::time(nullptr); // get UNIX time from Epoch: 01-Jan-1970
	//cout << std::asctime(std::localtime(&result)) << result << " seconds since the Epoch\n";
	stringstream ss;
	ss << result;
	string ts = ss.str();
	block *genesis = new block(0, vector<tranx>(), "GENESIS", ts, 1, 0); // truyền vector NULL
	BLOCK_CHAIN.push_back(genesis);
	cout << "\t\t\t\t~(o.0)~ Alpha Zeta!!!! ~(0.o)~\n";
	// READY GO TO UNIVERSE , ah, actually I use multi-thread to play this, lul


	menu();
	
	const double BLOCK_GENERATION_TIME_INTERVAL = 0.002; // Khoảng thời gian tạo 1 block trong 0.002

	int press_button;
	while (true)
	{
		cin >> press_button;

		if (press_button == 1) {
			// thêm 1 node

		}
		else if (press_button == 2) {
			time_t result_next_block = std::time(nullptr);
			stringstream ss_next_block;
			ss_next_block << result_next_block;
			string ts_next_block = ss_next_block.str();

			// Lấy ra vài thông tin từ previous block
			block *pre_block = BLOCK_CHAIN.back();
			long long pre_block_index = pre_block->get_index();
			string pre_block_hash = pre_block->get_hash();
			long long pre_block_difficulty = pre_block->get_difficulty();
			long long pre_block_nonce = pre_block->get_nonce();
			// Xong

			// Điều chỉnh difficulty
			if (pre_block_difficulty > 1) {
				if (pre_block->get_duration() > BLOCK_GENERATION_TIME_INTERVAL) {
					pre_block_difficulty -= 1;
				}
				else if (pre_block->get_duration() < BLOCK_GENERATION_TIME_INTERVAL)
				{
					pre_block_difficulty += 1;
				}
			}

			block *next_block = new block(pre_block_index + 1, vector<tranx>(), pre_block_hash, ts_next_block, pre_block_difficulty, pre_block_nonce); // truyền vector NULL

			BLOCK_CHAIN.push_back(next_block);
		}
		else if (press_button == 3) {
			cout << "OMG!!! World is going to be exploded! Get away!!!!\n";
			break;
		}
	}

	for (int i = 0; i < BLOCK_CHAIN.size(); i++) {
		cout << "Index: " << BLOCK_CHAIN[i]->get_index() << endl;
		cout << "Hash: " << BLOCK_CHAIN[i]->get_hash() << endl;
		cout << "Nonce: " << BLOCK_CHAIN[i]->get_nonce() <<endl;
		cout << "Duration: " << BLOCK_CHAIN[i]->get_duration() << endl;
		cout << "Difficulty: " << BLOCK_CHAIN[i]->get_difficulty() << endl;
		cout << "---------------------------------\n";
	}


	// Ez =.=
	/*for (auto i : BLOCK_CHAIN)
	{
		delete i;
	}*/

	for (int i = 0; i < BLOCK_CHAIN.size(); i++) {
		delete BLOCK_CHAIN[i];
	}
    return 0;
}

