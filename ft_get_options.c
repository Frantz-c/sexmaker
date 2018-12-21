/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_get_options.c                                 .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: fcordon <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/10/24 18:10:43 by fcordon      #+#   ##    ##    #+#       */
/*   Updated: 2018/12/21 21:07:58 by fcordon     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "sexmaker.h"

/*
** get arguments from the cli
**
** format :
**	-[a-zA-Z](=.+)?
**	--[a-zA-Z][a-zA-Z_-](=.+)?
*/

int		ft_is_set_option(const char *s, t_options *o)
{
	while (o)
	{
		if (o->name && strcmp(o->name, s) == 0)
			return (1);
		o = o->next;
	}
	return (0);
}

char	*ft_get_option_value(const char *s, t_options *o)
{
	while (o)
	{
		if (o->name && strcmp(o->name, s) == 0)
			return (o->value);
		o = o->next;
	}
	return (NULL);
}

static void		add_opt(t_options **opt, char *a1, char *a2)
{
	t_options	*new;
	t_options	*tmp;

	if (!(new = (t_options *)malloc(sizeof(t_options))))
		return ;
	new->name = a1;
	new->value = a2;
	new->next = NULL;
	if (!*opt)
	{
		*opt = new;
		return ;
	}
	tmp = *opt;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

static void		add_long_opt(t_options **opt, char *ptr, char *value)
{
	if (value)
		add_opt(opt, strndup(ptr, value - ptr), strdup(value + 1));
	else
		add_opt(opt, strdup(ptr), NULL);
}

static void		add_short_opt(t_options **opt, char *ptr, char *value)
{
	if (value)
	{
		if (!isalpha(*ptr) || (value - ptr) > 1)
			add_opt(opt, NULL, strdup(ptr));
		else
			add_opt(opt, strndup(ptr, 1), strdup(value + 1));
		return ;
	}
	while (*ptr)
	{
		if (isalpha(*ptr))
			add_opt(opt, strndup(ptr, 1), NULL);
		ptr++;
	}
}

t_options		*ft_get_options(int ac, char **av)
{
	t_options	*opt;
	char		*ptr;
	char		*value;

	opt = NULL;
	while (ac--)
	{
		if (**av == '-')
		{
			ptr = *av + 1;
			value = strchr(ptr, '=');
			if (*ptr == '-')
			{
				ptr++;
				add_long_opt(&opt, ptr, value);
			}
			else
				add_short_opt(&opt, ptr, value);
		}
		else
			add_opt(&opt, NULL, strdup(*av));
		av++;
	}
	return (opt);
}
