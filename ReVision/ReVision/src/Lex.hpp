#pragma once
#include "Common.hpp"

namespace ReVision
{
	extern const char *typedef_keyword;
	extern const char *enum_keyword;
	extern const char *struct_keyword;
	extern const char *union_keyword;
	extern const char *const_keyword;
	extern const char *func_keyword;
	extern const char *sizeof_keyword;
	extern const char *alignof_keyword;
	extern const char *typeof_keyword;
	extern const char *offsetof_keyword;
	extern const char *break_keyword;
	extern const char *continue_keyword;
	extern const char *return_keyword;
	extern const char *if_keyword;
	extern const char *else_keyword;
	extern const char *while_keyword;
	extern const char *do_keyword;
	extern const char *for_keyword;
	extern const char *switch_keyword;
	extern const char *case_keyword;
	extern const char *default_keyword;
	extern const char *import_keyword;
	extern const char *jmp_keyword; // goto

	// extern const char *asm_keyword; // inline assembly
	// extern const char *jmpif_keyword; // conditional goto
	// extern const char *use_keyword; // reduces scope to capture list
	// extern const char *class_keyword; // No inheritance
	// extern const char *namespace_keyword;
	// extern const char *new_keyword;
	// extern const char *delete_keyword;


	extern const char *first_keyword;
	extern const char *last_keyword;
	extern std::vector<const char *> keywords;

	extern const char *always_name;
	extern const char *foreign_name;
	extern const char *complete_name;
	extern const char *assert_name;
	extern const char *declare_note_name;
	extern const char *static_assert_name;

	void init_keywords();

	bool is_keyword_name(const char *name);

	struct SrcPos
	{
		const char *name;
		int line;
	};

	struct Token
	{
		enum Kind
		{
			END_OF_FILE,
			COLON,
			LPAREN,
			RPAREN,
			LBRACE, // '{'
			RBRACE, // '}'
			LBRACKET, // '['
			RBRACKET, // ']'
			COMMA,
			DOT,
			AT,
			POUND,
			ELLIPSIS,
			QUESTION,
			SEMICOLON,
			KEYWORD,
			INT,
			FLOAT,
			STR,
			NAME,
			NEG,
			NOT,
			// Multiplicative precedence
			FIRST_MUL,
			MUL = FIRST_MUL,
			DIV,
			MOD,
			AND,
			LSHIFT,
			RSHIFT,
			LAST_MUL = RSHIFT,
			// Additive precedence
			FIRST_ADD,
			ADD = FIRST_ADD,
			SUB,
			XOR,
			OR,
			LAST_ADD = OR,
			// Comparative precedence
			FIRST_CMP,
			EQ = FIRST_CMP,
			NOTEQ,
			LT,
			GT,
			LTEQ,
			GTEQ,
			LAST_CMP = GTEQ,
			AND_AND,
			OR_OR,
			// Assignment operators
			FIRST_ASSIGN,
			ASSIGN = FIRST_ASSIGN,
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
			COLON_ASSIGN,
			NUM_TOKEN_KINDS,
		} kind;

		enum class Mod
		{
			NONE,
			HEX,
			BIN,
			OCT,
			CHAR
		} mod;

		SrcPos pos;
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

	extern SrcPos pos_builtin;
	extern Token token;
	extern const char *stream;
	extern const char *line_start;

	template<typename T>
	static inline Token::Kind token_cast(T kind)
	{
		return static_cast<Token::Kind>(kind);
	}

	const char *token_kind_name(Token::Kind kind);
	const char *token_info();

	void warning(SrcPos pos, const char *fmt, ...);
	void error(SrcPos pos, const char *fmt, ...);

	uint8_t char_to_digit(char c);
	void scan_int();
	void scan_float();
	char escape_to_char(char c);
	void scan_char();
	void scan_str();

	void next_token();

	void init_stream(const char *name, const char *str);

	bool is_token(Token::Kind kind);
	bool is_token_name(const char *name);
	bool is_keyword(const char *name);
	bool match_keyword(const char *name);
	bool match_token(Token::Kind kind);
	bool expect_token(Token::Kind kind);
}