#include "Ast.hpp"

namespace ReVision
{
	static Arena ast_arena;

	void *ast_alloc(size_t size)
	{
		assert(size != 0);
		void *ptr{ arena_alloc(&ast_arena, size) };
		memset(ptr, 0, size);
		return ptr;
	}
	void *ast_dup(const void *src, size_t size)
	{
		if (size == 0) { return nullptr; }
		void *ptr{ arena_alloc(&ast_arena, size) };
		memcpy(ptr, src, size);
		return ptr;
	}
	Typespec *typespec_new(Typespec::Kind kind)
	{
		Typespec *type{ static_cast<Typespec *>(ast_alloc(sizeof(Typespec))) };
		type->kind = kind;
		return type;
	}
	Typespec *typespec_name(const char *name)
	{
		Typespec *type{ typespec_new(Typespec::NAME) };
		type->name = name;
		return type;
	}
	Typespec *typespec_ptr(Typespec *elem)
	{
		Typespec *type{ typespec_new(Typespec::PTR) };
		type->ptr.elem = elem;
		return type;
	}
	Typespec *typespec_array(Typespec *elem, Expr *size)
	{
		Typespec *type{ typespec_new(Typespec::ARRAY) };
		type->array.elem = elem;
		type->array.size = size;
		return type;
	}
	Typespec *typespec_func(std::vector<Typespec *> args, Typespec *ret)
	{
		Typespec *type{ typespec_new(Typespec::FUNC) };
		type->func.args = args;
		type->func.ret = ret;
		return type;
	}

	Decl *decl_new(Decl::Kind kind, const char *name)
	{
		Decl *d{ static_cast<Decl *>(ast_alloc(sizeof(Decl))) };
		d->kind = kind;
		d->name = name;
		return d;
	}
	Decl *decl_enum(const char *name, std::vector<EnumItem> items)
	{
		Decl *d{ decl_new(Decl::ENUM, name) };
		d->enum_decl.items = items;
		return d;
	}
	Decl *decl_aggregate(Decl::Kind kind, const char *name, std::vector<AggregateItem> items)
	{
		assert(kind == Decl::STRUCT || kind == Decl::UNION);
		Decl *d{ decl_new(kind, name) };
		d->aggregate.items = items;
		return d;
	}
	Decl *decl_union(const char *name, std::vector<AggregateItem> items)
	{
		Decl *d{ decl_new(Decl::UNION, name) };
		d->aggregate.items = items;
		return d;
	}
	Decl *decl_var(const char *name, Typespec *type, Expr *expr)
	{
		Decl *d{ decl_new(Decl::VAR, name) };
		d->var.type = type;
		d->var.expr = expr;
		return d;
	}
	Decl *decl_func(const char *name, std::vector<FuncParam> params, Typespec *ret_type, StmtBlock block)
	{
		Decl *d{ decl_new(Decl::FUNC, name) };
		d->func.params = params;
		d->func.ret_type = ret_type;
		d->func.block = block;
		return d;
	}
	Decl *decl_const(const char *name, Expr *expr)
	{
		Decl *d{ decl_new(Decl::CONST, name) };
		d->const_decl.expr = expr;
		return d;
	}
	Decl *decl_typedef(const char *name, Typespec *type)
	{
		Decl *d{ decl_new(Decl::TYPEDEF, name) };
		d->typedef_decl.type = type;
		return d;
	}
	Expr *expr_new(Expr::Kind kind)
	{
		Expr *e{ static_cast<Expr *>(ast_alloc(sizeof(Expr))) };
		e->kind = kind;
		return e;
	}
	Expr *expr_sizeof(Expr *expr)
	{
		Expr *e{ expr_new(Expr::SIZEOF_EXPR) };
		e->sizeof_expr = expr;
		return e;
	}
	Expr *expr_sizeof(Typespec *type)
	{
		Expr *e{ expr_new(Expr::SIZEOF_TYPE) };
		e->sizeof_type = type;
		return e;
	}
	Expr *expr_int(uint64_t val)
	{
		Expr *e{ expr_new(Expr::INT) };
		e->int_val = val;
		return e;
	}
	Expr *expr_float(double val)
	{
		Expr *e{ expr_new(Expr::FLOAT) };
		e->float_val = val;
		return e;
	}
	Expr *expr_str(const char *str)
	{
		Expr *e{ expr_new(Expr::STR) };
		e->str_val = str;
		return e;
	}
	Expr *expr_name(const char *name)
	{
		Expr *e{ expr_new(Expr::NAME) };
		e->name = name;
		return e;
	}
	Expr *expr_compound(Typespec *type, std::vector<Expr *> args)
	{
		Expr *e = expr_new(Expr::COMPOUND);
		e->compound.type = type;
		e->compound.args = args;
		return e;
	}
	Expr *expr_cast(Typespec *type, Expr *expr)
	{
		Expr *e{ expr_new(Expr::CAST) };
		e->cast.type = type;
		e->cast.expr = expr;
		return e;
	}
	Expr *expr_call(Expr *expr, std::vector<Expr *> args)
	{
		Expr *e{ expr_new(Expr::CALL) };
		e->call.expr = expr;
		e->call.args = args;
		return e;
	}
	Expr *expr_index(Expr *expr, Expr *index)
	{
		Expr *e{ expr_new(Expr::INDEX) };
		e->index.expr = expr;
		e->index.index = index;
		return e;
	}
	Expr *expr_field(Expr *expr, const char *name)
	{
		Expr *e{ expr_new(Expr::FIELD) };
		e->field.expr = expr;
		e->field.name = name;
		return e;
	}
	Expr *expr_unary(Token::Kind op, Expr *expr)
	{
		Expr *e{ expr_new(Expr::UNARY) };
		e->unary.op = op;
		e->unary.expr = expr;
		return e;
	}
	Expr *expr_binary(Token::Kind op, Expr *left, Expr *right)
	{
		Expr *e{ expr_new(Expr::BINARY) };
		e->binary.op = op;
		e->binary.left = left;
		e->binary.right = right;
		return e;
	}
	Expr *expr_ternary(Expr *cond, Expr *then_expr, Expr *else_expr)
	{
		Expr *e{ expr_new(Expr::TERNARY) };
		e->ternary.cond = cond;
		e->ternary.then_expr = then_expr;
		e->ternary.else_expr = else_expr;
		return e;
	}

	Stmt *stmt_new(Stmt::Kind kind)
	{
		Stmt *s{ static_cast<Stmt *>(ast_alloc(sizeof(Stmt))) };
		s->kind = kind;
		return s;
	}
	Stmt *stmt_return(Expr *expr)
	{
		Stmt *s{ stmt_new(Stmt::RETURN) };
		s->return_stmt.expr = expr;
		return s;
	}
	Stmt *stmt_break() { return stmt_new(Stmt::BREAK); }
	Stmt *stmt_continue() { return stmt_new(Stmt::CONTINUE); }
	Stmt *stmt_block(StmtBlock block)
	{ 
		Stmt *s{ stmt_new(Stmt::BLOCK) };
		s->block = block;
		return s;
	}
	Stmt *stmt_if(Expr *cond, StmtBlock then_block, std::vector<ElseIf> elseifs, StmtBlock else_block)
	{
		Stmt *s{ stmt_new(Stmt::IF) };
		s->if_stmt.cond = cond;
		s->if_stmt.then_block = then_block;
		s->if_stmt.elseifs = elseifs;
		s->if_stmt.else_block = else_block;
		return s;
	}
	Stmt *stmt_while(Expr *cond, StmtBlock block)
	{
		Stmt *s{ stmt_new(Stmt::WHILE) };
		s->while_stmt.cond = cond;
		s->while_stmt.block = block;
		return s;
	}
	Stmt *stmt_do_while(Expr *cond, StmtBlock block)
	{
		Stmt *s{ stmt_new(Stmt::DO_WHILE) };
		s->while_stmt.cond = cond;
		s->while_stmt.block = block;
		return s;
	}
	Stmt *stmt_for(Stmt *init, Expr *cond, Stmt *next, StmtBlock block)
	{
		Stmt *s{ stmt_new(Stmt::FOR) };
		s->for_stmt.init = init;
		s->for_stmt.cond = cond;
		s->for_stmt.next = next;
		s->for_stmt.block = block;
		return s;
	}
	Stmt *stmt_switch(Expr *expr, std::vector<SwitchCase> cases)
	{
		Stmt *s{ stmt_new(Stmt::SWITCH) };
		s->switch_stmt.expr = expr;
		s->switch_stmt.cases = cases;
		return s;
	}
	Stmt *stmt_assign(Token::Kind op, Expr *left, Expr *right)
	{
		Stmt *s{ stmt_new(Stmt::ASSIGN) };
		s->assign.op = op;
		s->assign.left = left;
		s->assign.right = right;
		return s;
	}
	template<typename T>
	Stmt *stmt_assign(T op, Expr *left, Expr *right) { return stmt_assign(token_cast(op), left, right); }
	Stmt *stmt_init(const char *name, Expr *expr)
	{
		Stmt *s{ stmt_new(Stmt::INIT) };
		s->init.name = name;
		s->init.expr = expr;
		return s;
	}
	Stmt *stmt_expr(Expr *expr)
	{
		Stmt *s{ stmt_new(Stmt::EXPR) };
		s->expr = expr;
		return s;
	}
}