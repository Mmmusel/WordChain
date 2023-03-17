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
        //两个多节点的scc由其他边连接
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


		// -c
		TEST_METHOD(example_c_1) {
			char* words[] = { "algebra", "apple", "zoo", "elephant", "under", "fox", "dog", "moon", "leaf", "trick", "pseudopseudohypoparathyroidism" };
			char* ans[] = { "pseudopseudohypoparathyroidism", "moon" };
			test_arg_c(words, 11, ans, 2, 0, 0, 0, false);
		}

		// -c -r 
		TEST_METHOD(example_c_r_1) {
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

		//无环
		TEST_METHOD(example_c_2) {
			char* words[] = { "ab", "ac", "ad", "bc", "bd", "cd" };
			char* ans[] = { "ab", "bc", "cd" };
			test_arg_c(words, 6, ans, 3, 0, 0, 0, false);
		}

        //自环
		TEST_METHOD(example_c_3) {
			char* words[] = { "aa", "ab", "bb", "bc", "cc", "de", "ef", "fg", "gh" };
			char* ans[] = { "aa", "ab", "bb", "bc", "cc" };
			test_arg_c(words, 9, ans, 5, 0, 0, 0, false);
		}

		//-r

		//尾部自环
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

        //test input
        TEST_METHOD(example_main_1) {
            char* args[] = { "", "-w", "D:\\PROJECTS\\lj\\input.txt" };
            int ret = parseCmd(3, args);
            Assert::AreEqual(0, ret);
        }

        //异常测试
        TEST_METHOD(exception_1_cmd_format_error) {
            try {
                char* args[] = { "WordChain.exe", "-", "D:\PROJECTS\WordChain\input.txt" };
                parseCmd(3, args);
            }
            catch (InPutException const& e) {
                Assert::AreEqual(0, strcmp("Cmd Input Error: cmd format error", e.GetInfo().data()));
                return;
            }
        }

        TEST_METHOD(exception_2_too_many_cmd) {
            try {
                char* args[] = { "WordChain.exe", "-w", "-c", "D:\PROJECTS\WordChain\input.txt" };
                parseCmd(4, args);
            }
            catch (InPutException const& e) {
                Assert::AreEqual(0, strcmp("Cmd Input Error: to many cmd. -n -w -c can only choose one and use once", e.GetInfo().data()));
                return;
            }
        }

        TEST_METHOD(exception_3_missing_char) {
            try {
                char* args[] = { "WordChain.exe", "-j", "ttt", "D:\PROJECTS\WordChain\input.txt" };
                parseCmd(4, args);
            }
            catch (InPutException const& e) {
                Assert::AreEqual(0, strcmp("Cmd Input Error: -j must followed by one char", e.GetInfo().data()));
                return;
            }
        }

        TEST_METHOD(exception_4_redeclaration) {
            try {
                char* args[] = { "WordChain.exe", "-w", "-h", "c", "-h", "c", "D:\PROJECTS\WordChain\input.txt" };
                parseCmd(7, args);
            }
            catch (InPutException const& e) {
                Assert::AreEqual(0, strcmp( "Cmd Input Error: redeclaration of-h\0", e.GetInfo().data()));
                return;
            }
        }

        TEST_METHOD(exception_5_error_char) {
            try {
                char* args[] = { "WordChain.exe", "-j", ".", "D:\PROJECTS\WordChain\input.txt" };
                parseCmd(4, args);
            }
            catch (InPutException const& e) {
                Assert::AreEqual(0, strcmp("Cmd Input Error: -j must followed by a char", e.GetInfo().data()));
                return;
            }
        }

        TEST_METHOD(exception_6_missing_char) {
            try {
                char* args[] = { "WordChain.exe", "-w", "-j", "D:\PROJECTS\WordChain\input.txt" };
                parseCmd(4, args);
            }
            catch (InPutException const& e) {
                Assert::AreEqual(0, strcmp("Cmd Input Error: -j must followed by one char", e.GetInfo().data()));
                return;
            }
        }

        TEST_METHOD(exception_7_undefined_cmd) {
            try {
                char* args[] = { "WordChain.exe", "-q", "D:\PROJECTS\WordChain\input.txt" };
                parseCmd(3, args);
            }
            catch (InPutException const& e) {
                Assert::AreEqual(0, strcmp("Cmd Input Error: undefined cmd.", e.GetInfo().data()));
                return;
            }
        }

        TEST_METHOD(exception_8_txt) {
            try {
                char* args[] = { "WordChain.exe", "-n", "D:\PROJECTS\WordChain\input" };
                parseCmd(3, args);
            }
            catch (InPutException const& e) {
                Assert::AreEqual(0, strcmp("File Input Error: filename must be *.txt", e.GetInfo().data()));
                return;
            }
        }

        TEST_METHOD(exception_9_cmd_format_error) {
            try {
                char* args[] = { "WordChain.exe", "n", "D:\PROJECTS\WordChain\input.txt" };
                parseCmd(3, args);
            }
            catch (InPutException const& e) {
                Assert::AreEqual(0, strcmp("Cmd Input Error: cmd format error", e.GetInfo().data()));
                return;
            }
        }

        TEST_METHOD(exception_10_missing_input_file) {
            try {
                char* args[] = { "WordChain.exe", "-n" };
                parseCmd(2, args);
            }
            catch (InPutException const& e) {
                Assert::AreEqual(0, strcmp("File Input Error: please enter a filename", e.GetInfo().data()));
                return;
            }
        }

        TEST_METHOD(exception_11_too_many_files) {
            try {
                char* args[] = { "WordChain.exe", "-n", "D:\PROJECTS\WordChain\input.txt","D:\PROJECTS\WordChain\input.txt" };
                parseCmd(4, args);
            }
            catch (InPutException const& e) {
                Assert::AreEqual(0, strcmp("File Input Error: you can only input one file", e.GetInfo().data()));
                return;
            }
        }

        TEST_METHOD(exception_12_cmd_format_error) {
            try {
                char* args[] = { "WordChain.exe", "-n", "-r", "D:\PROJECTS\WordChain\input.txt" };
                parseCmd(4, args);
            }
            catch (InPutException const& e) {
                Assert::AreEqual(0, strcmp("Cmd Input Error: -n should use seperately without -r", e.GetInfo().data()));
                return;
            }
        }

        TEST_METHOD(exception_13_cmd_format_error) {
            try {
                char* args[] = { "WordChain.exe", "-n", "-h", "a", "D:\PROJECTS\WordChain\input.txt" };
                parseCmd(5, args);
            }
            catch (InPutException const& e) {
                Assert::AreEqual(0, strcmp("Cmd Input Error: -n should use seperately without -h -t -j", e.GetInfo().data()));
                return;
            }
        }

        TEST_METHOD(exception_14_file_not_exist) {
            try {
                char* args[] = { "WordChain.exe", "-n", "D:\PROJECTS\WordChain\input_not_exist.txt"};
                parseCmd(3, args);
            }
            catch (InPutException const& e) {
                Assert::AreEqual(0, strcmp("File Input Error: file not exist", e.GetInfo().data()));
                return;
            }
        }

        TEST_METHOD(core_exception_1_input_char_error) {
            try {
                char* words[] = { "ab", "bc", "cd" };
                char* ans[] = { "" };
                test_arg_w(words, 3, ans, 0, '4', 0, 0, false);
            }
            catch (CoreException const& e) {
                Assert::AreEqual(0, strcmp("CoreError: -h -j -t must be a char", e.GetInfo().data()));
                return;
            }
        }

        TEST_METHOD(core_exception_2_no_solution) {
            try {
                char* words[] = { "ab", "cd", "ef"};
                char* ans[] = { "" };
                test_arg_w(words, 3, ans, 0, 0, 0, 0, false);
            }
            catch (CoreException const& e) {
                Assert::AreEqual(0, strcmp("CoreError: No Chain", e.GetInfo().data()));
                return;
            }
        }

        TEST_METHOD(core_exception_3_loop) {
            try {
                char* words[] = { "ab", "bb", "bcb" };
                char* ans[] = { "" };
                test_arg_w(words, 3, ans, 0, 0, 0, 0, false);
            }
            catch (CoreException const& e) {
                Assert::AreEqual(0, strcmp("CoreError: LOOP!", e.GetInfo().data()));
                return;
            }
        }

	};
}
