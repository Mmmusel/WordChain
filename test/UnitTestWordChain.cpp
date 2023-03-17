#include "pch.h"
#include "CppUnitTest.h"
#include "../src/main.h"
#include "../src/core/Edge.h"
#include "../src/core/Graph.h"
#include "../src/core/charCountMax.cpp"
#include "../src/core/wordCountMax.cpp"
#include "../src/core/Graph.cpp"
#include "../src/input.h"
#include "../src/input.cpp"
#include "../src/main.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestWordChain
{
	void test_arg_w(char* words[], int len, char* ans[], int ans_len, char head, char tail, char reject, bool enable_loop) {
		char** result = (char**)malloc(10000);
		int out_len = gen_chain_word(words, len, result, head, tail, reject, enable_loop);
		// Assert::AreEqual(ans_len, out_len);
		Assert::AreEqual(ans_len, out_len);
		for (int i = 0; i < ans_len; i++) {
			if (result != nullptr) Assert::AreEqual(strcmp(ans[i], result[i]), 0);
		}
	}

	void test_arg_c(char* words[], int len, char* ans[], int ans_len, char head, char tail, char reject, bool enable_loop) {
		char** result = (char**)malloc(10000);
		int out_len = gen_chain_char(words, len, result, head, tail, reject, enable_loop);
		Assert::AreEqual(ans_len, out_len);
		for (int i = 0; i < ans_len; i++) {
			if (result != nullptr) Assert::AreEqual(strcmp(ans[i], result[i]), 0);
		}
	}

	void test_arg_n(char* words[], int len, char* ans[], int ans_len) {
		char** result = (char**)malloc(10000);
		int out_len = gen_chains_all(words, len, result);
		Assert::AreEqual(ans_len, out_len);
		for (int i = 0; i < ans_len; i++) {
			if (result != nullptr) Assert::AreEqual(strcmp(ans[i], result[i]), 0);
		}
	}

	void test_vuetify_api(const char* input, char* ans, int type, char head, char tail, char reject, bool weighted) {
		const char*  result = vuetifyAPI(input, type, head, tail, reject, weighted);
		Assert::AreEqual(strcmp(ans, result), 0);
	}

	TEST_CLASS(UnitTestWordChain)
	{
	public:

		// -w-r
		TEST_METHOD(example_w_r_1) {
			char* words[] = { "element", "heaven", "table", "teach", "talk" };
			char* ans[] = { "table", "element", "teach", "heaven" };
			test_arg_w(words, 5, ans, 4, 0, 0, 0, true);
		}

		// -w-r
		TEST_METHOD(example_w_r_2) {
			char* words[] = { "arhdbva", "azzhjjjb", "f", "ff", "fff", "ffff", "bnfda",
				"ctx", "adac", "cnnb", "bd", "cec", "ce", "ef", "fg", "ge", "cff",
				"fgg", "gee", "dddddddddddddddddddddd" };
			char* ans[] = { "cec","cnnb","bnfda","arhdbva","adac","cff","ffff","fff","ff","f","fgg","gee","ef","fg","ge" };
			test_arg_w(words, 20, ans, 15, 0, 0, 0, true);
		}

		// -w
		TEST_METHOD(example_w_1) {
			char* words[] = { "algebra", "apple", "zoo", "elephant", "under", "fox", "dog", "moon", "leaf", "trick", "pseudopseudohypoparathyroidism" };
			char* ans[] = { "algebra", "apple", "elephant", "trick" };
			test_arg_w(words, 11, ans, 4, 0, 0, 0, false);
		}

		// -w -h -t
		TEST_METHOD(example_w_h_t) {
			char* words[] = { "algebra", "apple", "zoo", "elephant", "under", "fox", "dog", "moon", "leaf", "trick", "pseudopseudohypoparathyroidism" };
			char* ans[] = { "algebra", "apple", "elephant" };
			test_arg_w(words, 11, ans, 3, 'a', 't', 0, false);
		}

		// -w -j
		TEST_METHOD(example_w_j) {
			char* words[] = { "algebra", "apple", "zoo", "elephant", "under", "fox", "dog", "moon", "leaf", "trick", "pseudopseudohypoparathyroidism" };
			char* ans[] = { "algebra", "apple" };
			test_arg_w(words, 11, ans, 2, 0, 0, 'e', false);
		}

		// -w
		TEST_METHOD(exapmle_w_2) {
			char* words[] = { "ab", "ac", "ad", "bc", "bd", "cd" };
			char* ans[] = { "ab", "bc", "cd" };
			test_arg_w(words, 6, ans, 3, 0, 0, 0, false);
		}
		// -w (self-loop)
		TEST_METHOD(example_w_self_loop) {
			char* words[] = { "aa", "ab", "bb", "bc", "cc", "de", "ef", "fg", "gh" };
			char* ans[] = { "aa", "ab", "bb", "bc", "cc" };
			test_arg_w(words, 9, ans, 5, 0, 0, 0, false);
		}

		//-r

		// -w -r
		TEST_METHOD(example_w_r_3) {
			char* words[] = { "aaaaaaaaaaaaaaa", "ba", "cb" };
			char* ans[] = { "cb", "ba", "aaaaaaaaaaaaaaa" };
			test_arg_w(words, 3, ans, 3, 0, 0, 0, true);
		}

		//������ڵ��scc������������
		// -c
		TEST_METHOD(example_c_1) {
			char* words[] = { "algebra", "apple", "zoo", "elephant", "under", "fox", "dog", "moon", "leaf", "trick", "pseudopseudohypoparathyroidism" };
			char* ans[] = { "pseudopseudohypoparathyroidism", "moon" };
			test_arg_c(words, 11, ans, 2, 0, 0, 0, false);
		}

		// -c -r 
		TEST_METHOD(example_c_r_1) {
			/* TODO : test_char(words, , , , true) */
			char* words[] = { "append", "deny", "yahoo", "oops", "strange", "eat", "tuna", "banana", "pig", "graph", "news", "silence" };
			char* ans[] = { "news", "silence", "eat", "tuna", "append", "deny", "yahoo", "oops", "strange" };
			test_arg_c(words, 12, ans, 9, 0, 0, 0, true);
		}

		// -c -h -t
		TEST_METHOD(example_c_h_t_1) {
			char* words[] = { "algebra", "apple", "zoo", "elephant", "under", "fox", "dog", "moon", "leaf", "trick", "pseudopseudohypoparathyroidism" };
			char* ans[] = { "algebra", "apple", "elephant" };
			test_arg_c(words, 11, ans, 3, 'a', 't', 0, false);
		}

		// -w -j
		TEST_METHOD(example_c_j_1) {
			char* words[] = { "algebra", "apple", "zoo", "elephant", "under", "fox", "dog", "moon", "leaf", "trick", "pseudopseudohypoparathyroidism" };
			char* ans[] = { "pseudopseudohypoparathyroidism", "moon" };
			test_arg_c(words, 11, ans, 2, 0, 0, 'e', false);
		}

		//�޻�
		TEST_METHOD(example_c_2) {
			char* words[] = { "ab", "ac", "ad", "bc", "bd", "cd" };
			char* ans[] = { "ab", "bc", "cd" };
			test_arg_c(words, 6, ans, 3, 0, 0, 0, false);
		}
		//�޻� ���Ի�
		TEST_METHOD(example_c_3) {
			char* words[] = { "aa", "ab", "bb", "bc", "cc", "de", "ef", "fg", "gh" };
			char* ans[] = { "aa", "ab", "bb", "bc", "cc" };
			test_arg_c(words, 9, ans, 5, 0, 0, 0, false);
		}

		//-r

		//β���Ի�
		TEST_METHOD(example_c_r_2) {
			char* words[] = { "aaaaaaaaaaaaaaa", "ba", "cb" };
			char* ans[] = { "cb", "ba", "aaaaaaaaaaaaaaa" };
			test_arg_c(words, 3, ans, 3, 0, 0, 0, true);
		}

		//api -w
		TEST_METHOD(example_api_1) {
			const char* input = "ab, bc, cd";
			char* ans = "ab\nbc\ncd\n";
			test_vuetify_api(input, ans, 1, 0, 0, 0, true);
		}

		//api -c
		TEST_METHOD(example_api_2) {
			const char* input = "de,ef,eff";
			char* ans = "de\neff\n";
			test_vuetify_api(input, ans, 1, 0, 0, 0, false);
		}

		//api -n
		TEST_METHOD(example_api_3) {
			const char* input = "ee,er";
			char* ans = "1\nee er \n";
			test_vuetify_api(input, ans, 0, 0, 0, 0, false);
		}

	};
}
