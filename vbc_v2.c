#include"vbc_v2.h"


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
	return(1);
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
		n.type = VAL;
		n.val = **s - 48;
		node* ret = new_node(n);
		(*s)++;
		return(ret);
	}

	if(accept(s, '('))
	{
		node* ret = parse_add(s);
		if(expect(s,')'))
			return(ret);
		destroy_tree(ret);
		return(NULL);
	}
	unexpected(**s);
	return(NULL);
}

node* parse_multi(char** s)
{
	node* l = parse_val(s);
	if(!l)
		return(NULL);

	while(accept(s, '*'))
	{
		node* r = parse_val(s);
		if(!r)
		{
			destroy_tree(l);
			return(NULL);
		}
		node n;
		n.type = MULTI;
		n.l = l;
		n.r = r;
		l = new_node(n);
	}

	return(l);
}

node* parse_add(char** s)
{
	node* l = parse_multi(s);
	if(!l)
		return(NULL);

	while(accept(s, '+'))
	{
		node* r = parse_multi(s);
		if(!r)
		{
			destroy_tree(l);
			return(NULL);
		}
		node n;
		n.type = ADD;
		n.l = l;
		n.r = r;
		l = new_node(n);
	}

	return(l);
}

node* parse_expr(char *s)
{
	node* l = parse_add(&s);
	if(!l)
		return(NULL);

	if(*s)
	{
		destroy_tree(l);
		return(NULL);
	}
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
		default :
			perror("EVAL_TREE");
	}
}

int main(int argc, char** argv)
{
	if(argc != 2)
		return(1);
	node* tree = parse_expr(argv[1]);
	if(!tree)
		return(1);
	printf("%d\n", eval_tree(tree));
	destroy_tree(tree);
}

//cc vbc_V2.c  -o vbc -Wall -Wextra -Werror
//./vbc "(2+(3+2)+6*2+2)*2"
