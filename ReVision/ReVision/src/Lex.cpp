#include "Lex.hpp"

namespace ReVision
{
	const char *typedef_keyword;
	const char *enum_keyword;
	const char *struct_keyword;
	const char *union_keyword;
	const char *auto_keyword;
	const char *const_keyword;
	const char *func_keyword;
	const char *sizeof_keyword;
	const char *alignof_keyword;
	const char *typeof_keyword;
	const char *offsetof_keyword;
	const char *break_keyword;
	const char *continue_keyword;
	const char *return_keyword;
	const char *if_keyword;
	const char *else_keyword;
	const char *while_keyword;
	const char *do_keyword;
	const char *for_keyword;
	const char *switch_keyword;
	const char *case_keyword;
	const char *default_keyword;
	const char *import_keyword;
	const char *jmp_keyword; // goto

	// const char *asm_keyword; // inline assembly
	// const char *jmpif_keyword; // conditional goto
	// const char *use_keyword; // reduces scope to capture list
	// const char *class_keyword; // No inheritance
	// const char *namespace_keyword;
	// const char *new_keyword;
	// const char *delete_keyword;
	
	/*
	const char *bool_keyword; // true or false
	const char *int_keyword; // either 32 or 64 bits
	const char *uint_keyword; // same as int
	const char *uintptr_keyword; // a unsigned int large enough to store all memory addresses and sizes
	const char *int8_keyword; // signed 8-bit integer (-128 to 127)
	const char *int16_keyword; // signed 16-bit integer (-32768 to 32767)
	const char *int32_keyword; // signed 32-bit integer (-2147483648 to 2147483647)
	const char *int64_keyword; // signed 64-bit integer (-9223372036854775808 to 9223372036854775807)
	const char *uint8_keyword; // unsigned 8-bit integer (0 to 255)
	const char *uint16_keyword; // unsigned 16-bit integer (0 to 65535)
	const char *uint32_keyword; // unsigned 32-bit integer (0 to 4294967295)
	const char *uint64_keyword; // unsigned 64-bit integer (0 to 18446744073709551615)
	const char *float32_keyword; // IEEE 32-bit floating-point number
	const char *float64_keyword; // IEEE 64-bit floating-point number
	const char *complex32_keyword; // complex number with float32 real and imaginary parts.
	const char *complex64_keyword; // complex number with float64 real and imaginary parts.
	const char *char_keyword; // Unicode character type
	const char *const_keyword;
	const char *auto_keyword;
	const char *string_keyword; // string with char as character type
	*/
	const char *first_keyword;
	const char *last_keyword;
	std::vector<const char *> keywords;

	#define KEYWORD(name) name##_keyword = str_intern(#name); keywords.push_back(name##_keyword)
	void init_keywords()
	{
		static bool inited;
		if (inited)
		{
			return;
		}
		char *arena_end = str_arena.end;
		KEYWORD(typedef);
		KEYWORD(enum);
		KEYWORD(struct);
		KEYWORD(union);
		KEYWORD(auto);
		KEYWORD(const);
		KEYWORD(func);
		KEYWORD(sizeof);
		KEYWORD(alignof);
		KEYWORD(typeof);
		KEYWORD(offsetof);
		KEYWORD(break);
		KEYWORD(continue);
		KEYWORD(return);
		KEYWORD(if);
		KEYWORD(else);
		KEYWORD(while);
		KEYWORD(do);
		KEYWORD(for);
		KEYWORD(switch);
		KEYWORD(case);
		KEYWORD(default);
		KEYWORD(import);
		KEYWORD(jmp); // goto

		// KEYWORD(asm); // inline assembly
		// KEYWORD(jmpif); // conditional goto
		// KEYWORD(use); // reduces scope to capture list
		// KEYWORD(class); // No inheritance
		// KEYWORD(namespace);
		// KEYWORD(new);
		// KEYWORD(delete);

		assert(str_arena.end == arena_end);
		first_keyword = typedef_keyword;
		last_keyword = jmp_keyword;
		inited = true;
	}
	#undef KEYWORD
	
	bool is_keyword_name(const char *name)
	{
		return first_keyword <= name && name <= last_keyword;
	}

	SrcPos pos_builtin{ "<builtin>", 0 };
	Token token;
	const char *stream;
	const char *line_start;

	const char *token_kind_name(Token::Kind kind)
	{
		switch (kind)
		{
			case Token::END_OF_FILE: return "EOF";
			case Token::COLON: return ":";
			case Token::LPAREN: return "(";
			case Token::RPAREN: return ")";
			case Token::LBRACE: return "{";
			case Token::RBRACE: return "}";
			case Token::LBRACKET: return "[";
			case Token::RBRACKET: return "]";
			case Token::COMMA: return ",";
			case Token::DOT: return ".";
			case Token::AT: return "@";
			case Token::POUND: return "#";
			case Token::ELLIPSIS: return "...";
			case Token::QUESTION: return "?";
			case Token::SEMICOLON: return ";";
			case Token::KEYWORD: return "keyword";
			case Token::INT: return "int";
			case Token::FLOAT: return "float";
			case Token::STR: return "string";
			case Token::NAME: return "name";
			case Token::NEG: return "~";
			case Token::NOT: return "!";
			case Token::MUL: return "*";
			case Token::DIV: return "/";
			case Token::MOD: return "%";
			case Token::AND: return "&";
			case Token::LSHIFT: return "<<";
			case Token::RSHIFT: return ">>";
			case Token::ADD: return "+";
			case Token::SUB: return "-";
			case Token::OR: return "|";
			case Token::XOR: return "^";
			case Token::EQ: return "==";
			case Token::NOTEQ: return "!=";
			case Token::LT: return "<";
			case Token::GT: return ">";
			case Token::LTEQ: return "<=";
			case Token::GTEQ: return ">=";
			case Token::AND_AND: return "&&";
			case Token::OR_OR: return "||";
			case Token::ASSIGN: return "=";
			case Token::ADD_ASSIGN: return "+=";
			case Token::SUB_ASSIGN: return "-=";
			case Token::OR_ASSIGN: return "|=";
			case Token::AND_ASSIGN: return "&=";
			case Token::XOR_ASSIGN: return "^=";
			case Token::MUL_ASSIGN: return "*=";
			case Token::DIV_ASSIGN: return "/=";
			case Token::MOD_ASSIGN: return "%=";
			case Token::LSHIFT_ASSIGN: return "<<=";
			case Token::RSHIFT_ASSIGN: return ">>=";
			case Token::INC: return "++";
			case Token::DEC: return "--";
		}
		return "<unknown>";
	};
	const char *token_info()
	{
		if (token.kind == Token::NAME || token.kind == Token::KEYWORD) { return token.name; }
		else { return token_kind_name(token.kind); }
	}

	void warning(SrcPos pos, const char *fmt, ...)
	{
		if (pos.name == NULL)
		{
			pos = pos_builtin;
		}
		va_list args;
		va_start(args, fmt);
		printf("%s(%d): warning: ", pos.name, pos.line);
		vprintf(fmt, args);
		printf("\n");
		va_end(args);
	}
	void error(SrcPos pos, const char *fmt, ...)
	{
		if (pos.name == NULL)
		{
			pos = pos_builtin;
		}
		va_list args;
		va_start(args, fmt);
		printf("%s(%d): error: ", pos.name, pos.line);
		vprintf(fmt, args);
		printf("\n");
		va_end(args);
	}

	#define fatal_error(...) (error(__VA_ARGS__), exit(1))
	#define error_here(...) (error(token.pos, __VA_ARGS__))
	#define warning_here(...) (error(token.pos, __VA_ARGS__))
	#define fatal_error_here(...) (error_here(__VA_ARGS__), exit(1))

	uint8_t char_to_digit(char c)
	{
		switch (c)
		{
		case '0': return 0;		case '1': return 1;		case '2': return 2;
		case '3': return 3;		case '4': return 4;		case '5': return 5;
		case '6': return 6;		case '7': return 7;		case '8': return 8;
		case '9': return 9;
		case 'a': case 'A': return 10;		case 'b': case 'B': return 11;
		case 'c': case 'C': return 12;		case 'd': case 'D': return 13;
		case 'e': case 'E': return 14;		case 'f': case 'F': return 15;
		}
		return 0;
	}
	void scan_int()
	{
		uint64_t base{ 10 };
		if (*stream == '0')
		{
			++stream;
			if (std::tolower(*stream) == 'x')
			{
				++stream;
				token.mod = Token::Mod::HEX;
				base = 16;
			}
			else if (std::tolower(*stream) == 'b')
			{
				++stream;
				token.mod = Token::Mod::BIN;
				base = 2;
			}
			else if (std::isdigit(*stream) != 0)
			{
				token.mod = Token::Mod::OCT;
				base = 8;
			}
		}

		uint64_t val{ 0 };
		while (true)
		{
			uint64_t digit{ char_to_digit(*stream) };

			if (digit == 0 && *stream != '0') { break; }
			if (digit >= base)
			{
				error_here("Digit '%c' out of range for base %" PRIu64, *stream, base);
				digit = 0;
			}
			if (val > (std::numeric_limits<uint64_t>::max() - digit) / base)
			{
				error_here("Integer literal overflow");
				while (std::isdigit(*stream)) { ++stream; }
				val = 0; break;
			}
			val = val * base + digit;
			++stream;
		}
		token.kind = Token::INT;
		token.int_val = val;
	}
	void scan_float()
	{
		const char *start{ stream };
		while (std::isdigit(*stream)) { ++stream; }
		if (*stream == '.') { ++stream; }
		while (std::isdigit(*stream)) { ++stream; }

		if (tolower(*stream) == 'e')
		{
			++stream;
			if (*stream == '+' || *stream == '-') { ++stream; }
			if (!std::isdigit(*stream))
			{
				error_here("Expected digit after float literal exponent, found '%c'.", *stream);
			}
			while (std::isdigit(*stream)) { ++stream; }
		}
		const double val{ std::strtod(start, nullptr) };
		if (val == HUGE_VAL)
		{
			error_here("Float literal overflow");
		}
		token.kind = Token::FLOAT;
		token.float_val = val;
	}
	char escape_to_char(char c)
	{
		switch (c)
		{
		case 'n': return '\n';
		case 'r': return '\r';
		case 't': return '\t';
		case 'v': return '\v';
		case 'b': return '\b';
		case 'a': return '\a';
		case '0': return 0;
		}
		return 0;
	}
	void scan_char()
	{
		assert(*stream == '\''); ++stream;

		char val{ 0 };
		if (*stream == '\'')
		{
			error_here("Char literal cannot be empty"); ++stream;
		}
		else if (*stream == '\n')
		{
			error_here("Char literal cannot contain newline");
		}
		else if (*stream == '\\')
		{
			++stream;
			val = escape_to_char(*stream);
			if (val == 0 && *stream != '0')
			{
				error_here("Invalid char literal escape '\\%c'", *stream);
			}
			++stream;
		}
		else { val = *stream;  ++stream; }

		if (*stream != '\'')
		{
			error_here("Expected closing char quote, got '%c'", *stream);
		}
		else { ++stream; }

		token.kind = Token::INT;
		token.int_val = val;
		token.mod = Token::Mod::CHAR;
	}
	void scan_str()
	{
		assert(*stream == '"'); ++stream;
		std::string str;
		while ((*stream != 0) && *stream != '"')
		{
			char val{ *stream };
			if (val == '\n')
			{
				error_here("String literal cannot contain newline"); break;
			}
			else if (val == '\\')
			{
				++stream;
				val = escape_to_char(*stream);
				if (val == 0 && *stream != '0')
				{
					error_here("Invalid string literal escape '\\%c'", *stream);
				}
			}
			str += val; ++stream;
		}
		if (*stream) { assert(*stream == '"'); ++stream; }
		else { error_here("Unexpected end of file within string literal"); }
		token.kind = Token::STR;
		token.str_val = str_intern(str.c_str());
	}

	#define CASE1(c1, k1) \
	case c1: token.kind = Token::k1; ++stream; break;

	#define CASE2(c1, k1, c2, k2) \
	case c1: \
		token.kind = Token::k1; ++stream; \
		if(*stream == c2) { token.kind = Token::k2; ++stream; } \
		break;

	#define CASE3(c1, k1, c2, k2, c3, k3) \
	case c1: \
		token.kind = Token::k1; stream++; \
		if (*stream == c2) { token.kind = Token::k2; stream++; } \
		else if (*stream == c3) { token.kind = Token::k3; stream++; } \
		break;

	void next_token()
	{
	repeat:
		token.start = stream;
		token.mod = Token::Mod::NONE;
		switch (*stream)
		{
		case ' ': case '\t': case '\n': case '\r': case '\v':
		{
			while (isspace(*stream))
			{
				if (*stream++ == '\n')
				{
					line_start = stream;
					token.pos.line++;
				}
			}
			goto repeat;
		}
		case '\'': { scan_char(); break; }
		case '"': { scan_str(); break; }
		case '.':
			if (isdigit(stream[1]))
			{
				scan_float();
			}
			else if (stream[1] == '.' && stream[2] == '.')
			{
				token.kind = Token::ELLIPSIS;
				stream += 3;
			}
			else
			{
				token.kind = Token::DOT;
				++stream;
			}
			break;
		case '0':  case '1': case '2': case'3': case '4': case '5': case '6': case '7': case '8': case '9':
		{
			while (std::isdigit(*stream)) { stream++; }

			const char c = *stream;
			stream = token.start;
			if (c == '.' || std::tolower(c) == 'e') { scan_float(); }
			else { scan_int(); }
			break;
		}
		case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j':
		case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't':
		case 'u': case 'v': case 'w': case 'x': case 'y': case 'z': case 'A': case 'B': case 'C': case 'D':
		case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N':
		case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X':
		case 'Y': case 'Z': case '_':
		{
			while (std::isalnum(*stream) || *stream == '_')
			{
				++stream;
			}
			token.name = str_intern_range(token.start, stream);
			token.kind = (is_keyword_name(token.name) ? Token::KEYWORD : Token::NAME);
			break;
		}
		case '<':
		{
			token.kind = Token::LT; ++stream;
			if (*stream == '<')
			{
				token.kind = Token::LSHIFT; ++stream;
				if (*stream == '=')
				{
					token.kind = Token::LSHIFT_ASSIGN; ++stream;
				}
			}
			else if (*stream == '=')
			{
				token.kind = Token::LTEQ; ++stream;
			}
			break;
		}
		case '>':
		{
			token.kind = Token::GT; ++stream;
			if (*stream == '>')
			{
				token.kind = Token::RSHIFT; ++stream;
				if (*stream == '=')
				{
					token.kind = Token::RSHIFT_ASSIGN; ++stream;
				}
			}
			else if (*stream == '=')
			{
				token.kind = Token::GTEQ;
				stream++;
			}
			break;
		}
		case '/':
		{
			token.kind = Token::DIV; ++stream;
			if (*stream == '=')
			{
				token.kind = Token::DIV_ASSIGN; ++stream;
			}
			else if (*stream == '/') {
				++stream;
				while (*stream && *stream != '\n')
				{
					++stream;
				}
				goto repeat;
			}
			else if (*stream == '*')
			{
				++stream;
				int level{ 1 };
				while (*stream && level > 0)
				{
					if (stream[0] == '/' && stream[1] == '*')
					{
						++level; stream += 2;
					}
					else if (stream[0] == '*' && stream[1] == '/')
					{
						--level; stream += 2;
					}
					else
					{
						++stream;
					}
				}
				goto repeat;
			}
			break;
		}
		CASE1('\0', Token::END_OF_FILE)
		CASE1('(', Token::LPAREN)
		CASE1(')', Token::RPAREN)
		CASE1('{', Token::LBRACE)
		CASE1('}', Token::RBRACE)
		CASE1('[', Token::LBRACKET)
		CASE1(']', Token::RBRACKET)
		CASE1(',', Token::COMMA)
		CASE1('@', Token::AT)
		CASE1('#', Token::POUND)
		CASE1('?', Token::QUESTION)
		CASE1(';', Token::SEMICOLON)
		CASE1('~', Token::NEG)
		CASE1(':', Token::COLON)
		CASE2('!', Token::NOT, '=', Token::NOTEQ)
		CASE2('=', Token::ASSIGN, '=', Token::EQ)
		CASE2('^', Token::XOR, '=', Token::XOR_ASSIGN)
		CASE2('*', Token::MUL, '=', Token::MUL_ASSIGN)
		CASE2('%', Token::MOD, '=', Token::MOD_ASSIGN)
		CASE3('+', Token::ADD, '=', Token::ADD_ASSIGN, '+', Token::INC)
		CASE3('-', Token::SUB, '=', Token::SUB_ASSIGN, '-', Token::DEC)
		CASE3('&', Token::AND, '=', Token::AND_ASSIGN, '&', Token::AND_AND)
		CASE3('|', Token::OR, '=', Token::OR_ASSIGN, '|', Token::OR_OR)
		default:
		{
			syntax_error("Invalid '%c' token, skipping", *stream);
			stream++;
			goto repeat;
		}
		}
		token.end = stream;
	}

	#undef CASE1
	#undef CASE2
	#undef CASE3

	void init_stream(const char *name, const char *str)
	{
		stream = str;
		line_start = stream;
		token.pos.name = name ? name : "<string>";
		token.pos.line = 1;
		next_token();
	}

	bool is_token(Token::Kind kind) { return token.kind == kind; }
	bool is_token_name(const char *name) { return token.kind == Token::NAME && token.name == name; }
	bool is_keyword(const char *name) { return is_token(Token::KEYWORD) && token.name == name; }
	bool match_keyword(const char *name)
	{
		if (is_keyword(name))
		{
			next_token(); return true;
		}
		else { return false; }
	}
	bool match_token(Token::Kind kind)
	{
		if (is_token(kind))
		{
			next_token();
			return true;
		}
		return false;
	}
	bool expect_token(Token::Kind kind)
	{
		if (is_token(kind))
		{
			next_token();
			return true;
		}
		else
		{
			fatal_error_here("expected token %s, got %s", token_kind_name(token_cast(kind)), token_info());
			return false;
		}
	}
}