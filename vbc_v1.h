#ifndef VBC_V1_H
# define VBC_V1_H

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<ctype.h>

typedef struct s_node
{
	enum{ADD, MULTI, VAL} type;
	int val;
	struct s_node *l;
	struct s_node *r;
} node;

node*	new_node(node n);
void	destroy_tree(node* n);
int		unexpected(char c);
int		accept(char** s, char c);
int		expect(char **s, char c);
node*	parse_val(char** s);
node*	parse_multi(char** s);
node*	parse_expr(char **s);
int		eval_tree(node *tree);

#endif
