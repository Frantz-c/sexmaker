/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   sexmaker.h                                       .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: fcordon <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/12/21 21:06:17 by fcordon      #+#   ##    ##    #+#       */
/*   Updated: 2018/12/21 21:08:53 by fcordon     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#ifndef SEXMAKER_H
# define SEXMAKER_H

typedef struct dirent dirent_t;

typedef struct	s_arg
{
	char	*lib;
	char	*exe;
	char	*h_path;
	char	*flags;
	char	*compilo;
	char	**omit;
	int		silent;
	int		islib;
}				t_arg;

typedef struct		s_options
{
	char				*name;
	char				*value;
	struct s_options	*next;
}					t_options;

t_options		*ft_get_options(int ac, char **av);
int				ft_is_set_option(const char *s, t_options *o);
char			*ft_get_option_value(const char *s, t_options *o);
char			**ft_strsplit(const char *s, char c);

#endif
