#include "Test.hpp"

namespace ReVision
{
	void str_intern_test()
	{
		char a[] = "hello";
		assert(strcmp(a, str_intern(a)) == 0);
		assert(str_intern(a) == str_intern(a));
		assert(str_intern(str_intern(a)) == str_intern(a));
		char b[] = "hello";
		assert(a != b);
		assert(str_intern(a) == str_intern(b));
		char c[] = "hello!";
		assert(str_intern(a) != str_intern(c));
		char d[] = "hell";
		assert(str_intern(a) != str_intern(d));
	}

	void keyword_test()
	{
		init_keywords();
		assert(is_keyword_name(first_keyword));
		assert(is_keyword_name(last_keyword));
		for (const char *str : keywords)
		{
			assert(is_keyword_name(str));
		}
		assert(!is_keyword_name(str_intern("foo")));
	}

	#define assert_token(x) assert(match_token(x))
	#define assert_token_name(x) assert(token.name == str_intern(x) && match_token(Token::NAME))
	#define assert_token_int(x) assert(token.int_val == (x) && match_token(Token::INT))
	#define assert_token_float(x) assert(token.float_val == (x) && match_token(Token::FLOAT))
	#define assert_token_str(x) assert(strcmp(token.str_val, (x)) == 0 && match_token(Token::STR))
	#define assert_token_eof() assert(is_token(Token::END_OF_FILE))
	void lex_test()
	{
		// Integer literal tests
		init_stream(nullptr, "0 18446744073709551615 0xffffffffffffffff 042 0b1111");
		assert_token_int(0);
		assert_token_int(18446744073709551615ull);
		assert(token.mod == Token::Mod::HEX);
		assert_token_int(0xffffffffffffffffull);
		assert(token.mod == Token::Mod::OCT);
		assert_token_int(042);
		assert(token.mod == Token::Mod::BIN);
		assert_token_int(0xF);
		assert_token_eof();

		// Float literal tests
		init_stream(nullptr, "3.14 .123 42. 3e10");
		assert_token_float(3.14);
		assert_token_float(.123);
		assert_token_float(42.);
		assert_token_float(3e10);
		assert_token_eof();

		// Char literal tests
		init_stream(nullptr, "'a' '\\n'");
		assert_token_int('a');
		assert_token_int('\n');
		assert_token_eof();

		// String literal tests
		init_stream(nullptr, "\"foo\" \"a\\nb\"");
		assert_token_str("foo");
		assert_token_str("a\nb");
		assert_token_eof();

		// Operator tests
		init_stream(nullptr,
			": ( ) { } [ ] , . @ # ... ? ; \n"
			"123 0.02 \"test\" \n"
			"name ~ ! * / % & << >> \n"
			"+ - | ^ "
			"== != < > <= >= && || \n"
			"= += -= |= &= ^= *= /= %= <<= >>= ++ -- \n"

			"// This is a single line comment \n"

			"/* This is a multi line comment \n"
			"/* This is a multi line comment*/ \n"
			" This is a multi line comment*/ \n"

			"typedef enum struct union auto const func sizeof alignof typeof offsetof \n"
			"break continue return if else while do for switch case default import jmp\n");

		assert_token(Token::COLON); // ":"
		assert_token(Token::LPAREN); // "("
		assert_token(Token::RPAREN); // ")"
		assert_token(Token::LBRACE); // "{"
		assert_token(Token::RBRACE); // "}"
		assert_token(Token::LBRACKET); // "["
		assert_token(Token::RBRACKET); // "]"
		assert_token(Token::COMMA); // ","
		assert_token(Token::DOT); // "."
		assert_token(Token::AT); // "@"
		assert_token(Token::POUND); // "#"
		assert_token(Token::ELLIPSIS); // "..."
		assert_token(Token::QUESTION); // "?"
		assert_token(Token::SEMICOLON); // ";"
		assert_token(Token::INT); // "int"
		assert_token(Token::FLOAT); // "float"
		assert_token(Token::STR); // "string"
		assert_token(Token::NAME); // "name"
		assert_token(Token::NEG); // "~"
		assert_token(Token::NOT); // "!"
		assert_token(Token::MUL); // "*"
		assert_token(Token::DIV); // "/"
		assert_token(Token::MOD); // "%"
		assert_token(Token::AND); // "&"
		assert_token(Token::LSHIFT); // "<<"
		assert_token(Token::RSHIFT); // ">>"
		assert_token(Token::ADD); // "+"
		assert_token(Token::SUB); // "-"
		assert_token(Token::OR); // "|"
		assert_token(Token::XOR); // "^"
		assert_token(Token::EQ); // "=="
		assert_token(Token::NOTEQ); // "!="
		assert_token(Token::LT); // "<"
		assert_token(Token::GT); // ">"
		assert_token(Token::LTEQ); // "<="
		assert_token(Token::GTEQ); // ">="
		assert_token(Token::AND_AND); // "&&"
		assert_token(Token::OR_OR); // "||"
		assert_token(Token::ASSIGN); // "="
		assert_token(Token::ADD_ASSIGN); // "+="
		assert_token(Token::SUB_ASSIGN); // "-="
		assert_token(Token::OR_ASSIGN); // "|="
		assert_token(Token::AND_ASSIGN); // "&="
		assert_token(Token::XOR_ASSIGN); // "^="
		assert_token(Token::MUL_ASSIGN); // "*="
		assert_token(Token::DIV_ASSIGN); // "/="
		assert_token(Token::MOD_ASSIGN); // "%="
		assert_token(Token::LSHIFT_ASSIGN); // "<<="
		assert_token(Token::RSHIFT_ASSIGN); // ">>="
		assert_token(Token::INC); // "++"
		assert_token(Token::DEC); // "--"

		for (const char *k : keywords)
		{
			assert_token(Token::KEYWORD);
		}
		assert_token_eof();

		// Misc tests
		init_stream(nullptr, "XY+(XY)_HELLO1,234+994");
		assert_token_name("XY");
		assert_token(Token::ADD);
		assert_token(Token::LPAREN);
		assert_token_name("XY");
		assert_token(Token::RPAREN);
		assert_token_name("_HELLO1");
		assert_token(Token::COMMA);
		assert_token_int(234);
		assert_token(Token::ADD);
		assert_token_int(994);
		assert_token_eof();
	}
	#undef assert_token
	#undef assert_token_name
	#undef assert_token_int
	#undef assert_token_float
	#undef assert_token_str
	#undef assert_token_eof

	void run_all_tests()
	{
		str_intern_test();
		keyword_test();
		lex_test();
	}
}