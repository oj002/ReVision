#pragma once
#include "Common.hpp"

namespace ReVision
{
	// Control Flow
	static const char *asm_keyword; // inline assembly
	static const char *do_keyword;
	static const char *switch_keyword;
	static const char *while_keyword;
	static const char *if_keyword;
	static const char *else_keyword;
	static const char *for_keyword;
	static const char *jmp_keyword; // goto
	static const char *jmpif_keyword; // conditional goto

	static const char *break_keyword;
	static const char *continue_keyword;
	static const char *return_keyword;
	static const char *case_keyword;
	static const char *default_keyword;


	// Scope
	static const char *use_keyword; // reduces scope to capture list
	static const char *namespace_keyword;


	// Composite Types
	static const char *class_keyword; // No inheritance
	static const char *enum_keyword;
	static const char *struct_keyword;
	static const char *union_keyword;


	// Primitive Types
	static const char *bool_keyword; // true or false
	static const char *int_keyword; // either 32 or 64 bits
	static const char *uint_keyword; // same as int
	static const char *uintptr_keyword; // a unsigned int large enough to store all memory addresses and sizes

	static const char *int8_keyword; // signed 8-bit integer (-128 to 127)
	static const char *int16_keyword; // signed 16-bit integer (-32768 to 32767)
	static const char *int32_keyword; // signed 32-bit integer (-2147483648 to 2147483647)
	static const char *int64_keyword; // signed 64-bit integer (-9223372036854775808 to 9223372036854775807)

	static const char *uint8_keyword; // unsigned 8-bit integer (0 to 255)
	static const char *uint16_keyword; // unsigned 16-bit integer (0 to 65535)
	static const char *uint32_keyword; // unsigned 32-bit integer (0 to 4294967295)
	static const char *uint64_keyword; // unsigned 64-bit integer (0 to 18446744073709551615)

	static const char *float32_keyword; // IEEE 32-bit floating-point number
	static const char *float64_keyword; // IEEE 64-bit floating-point number

	static const char *complex32_keyword; // complex number with float32 real and imaginary parts.
	static const char *complex64_keyword; // complex number with float64 real and imaginary parts.

	static const char *char_keyword; // alias for uint8 (utf-8 type)
	static const char *wchar_keyword; // alias for int32 (utf-32 type)

	static const char *const_keyword;
	static const char *auto_keyword;


	// Abstract types
	static const char *string_keyword; // string with char as character type
	static const char *wstring_keyword; // string with wchar as character type


	// Operators
	static const char *new_keyword;
	static const char *delete_keyword;
	static const char *sizeof_keyword;



	static const char *first_keyword;
	static const char *last_keyword;
	static std::vector<const char *> keywords;

	void init_keywords();

	bool is_keyword_str(const char *str);

	struct Token
	{
		enum Kind
		{
			END_OF_FILE,
			LPAREN,
			RPAREN,
			LBRACE, // '{'
			RBRACE, // '}'
			LBRACKET, // '['
			RBRACKET, // ']'
			COMMA,
			DOT,
			QUESTION,
			SEMICOLON,
			KEYWORD,
			INT,
			FLOAT,
			STR,
			NAME,
			// Multiplicative precedence
			MUL,
			FIRST_MUL = MUL,
			DIV,
			MOD,
			AND,
			LSHIFT,
			RSHIFT,
			LAST_MUL = RSHIFT,
			// Additive precedence
			ADD,
			FIRST_ADD = ADD,
			SUB,
			XOR,
			OR,
			LAST_ADD = OR,
			// Comparative precedence
			EQ,
			FIRST_CMP = EQ,
			NOTEQ,
			LT,
			GT,
			LTEQ,
			GTEQ,
			LAST_CMP = GTEQ,
			AND_AND,
			OR_OR,
			// Assignment operators
			ASSIGN,
			FIRST_ASSIGN = ASSIGN,
			ADD_ASSIGN,
			SUB_ASSIGN,
			OR_ASSIGN,
			AND_ASSIGN,
			XOR_ASSIGN,
			LSHIFT_ASSIGN,
			RSHIFT_ASSIGN,
			MUL_ASSIGN,
			DIV_ASSIGN,
			MOD_ASSIGN,
			LAST_ASSIGN = MOD_ASSIGN,
			INC,
			DEC,
			
		} kind;

		enum class Mod
		{
			NONE,
			HEX,
			BIN,
			OCT,
			CHAR
		} mod;

		const char *start;
		const char *end;

		union
		{
			uint64_t int_val;
			double float_val;
			const char *str_val;
			const char *name;
		};
	};

	static Token token;
	static const char *stream;

	template<typename T>
	static inline Token::Kind token_cast(T kind)
	{
		return static_cast<Token::Kind>(kind);
	}

	const char *token_kind_name(Token::Kind kind);
	const char *token_info();

	uint8_t char_to_digit(char c);
	void scan_int();
	void scan_float();
	char escape_to_char(char c);
	void scan_char();
	void scan_str();

	void next_token();

	void init_stream(const char *str);

	bool is_token(Token::Kind kind);
	bool is_token_name(const char *name);
	bool is_keyword(const char *name);
	bool match_keyword(const char *name);
	bool match_token(Token::Kind kind);
	bool expect_token(Token::Kind kind);

	void keyword_test();
	void lex_test();
}