#include"vbc_v1.h"

node *new_node(node n)
{
	node *ret = calloc(1, sizeof(node));
	if(!ret)
		return(NULL);
	*ret = n;
	return(ret);
}

void destroy_tree(node* n)
{
	if(!n)
		return;
	if(n->type != VAL)
	{
		destroy_tree(n->l);
		destroy_tree(n->r);
	}
	free(n);
}

int unexpected(char c)
{
	if(c)
		printf("Unexpected token '%c'\n", c);
	else
		printf("Unexpected end of input\n");
	return(0);
}

int accept(char** s, char c)
{
	if(**s == c)
	{
		(*s)++;
		return(1);
	}
	return(0);
}

int expect(char **s, char c)
{
	if(accept(s, c))
		return(1);
	unexpected(**s);
	return(0);
}

node* parse_val(char** s)
{
	node n;
	if(isdigit(**s))
	{
		n.val = **s - 48;
		n.type = VAL;
		(*s)++;
		return (new_node(n));
	}
	if(accept(s, '('))
	{
		node* pn = parse_expr(s);
		expect(s, ')');
		return(pn);
	}
	unexpected(**s);
	return(NULL);
}

node* parse_multi(char** s)
{
	node *l = parse_val(s);
	while(accept(s, '*'))
	{
		node *r = parse_val(s);
		node n ;
		n.type = MULTI;
		n.l = l;
		n.r = r;
		l = new_node(n);
	}
	return(l);
}

node* parse_expr(char **s)
{
//	printf("parse_expr %s\n", *s);

	node* l = parse_multi(s);

	while(accept(s, '+'))
	{
		node *r = parse_multi(s);
		node n ;
		n.type = ADD;
		n.l = l;
		n.r = r;
		l = new_node(n);
	}

//	printf("s = %s val = %d type = %d\n", s, ret->val, ret->type);


//	if(**s)
//	{
//		//destroy_tree(l);
//		printf("s = %s val = %d type = %d\n", *s, l->val, l->type);
//		return(NULL);
//	}
	return(l);
}

int eval_tree(node *tree)
{
	switch(tree->type)
	{
		case ADD :
			return(eval_tree(tree->l) + eval_tree(tree->r));
		case MULTI :
			return(eval_tree(tree->l) * eval_tree(tree->r));
		case VAL :
			return(tree->val);
	}
	return(0);
}

int main(int argc, char** argv)
{
	if(argc != 2)
	{
		perror("argv KO");
		return(1);
	}

	node* tree = parse_expr(&argv[1]);

	if(!tree)
		return(1);

	printf("%d\n", eval_tree(tree));
	destroy_tree(tree);
	return(0);
}

