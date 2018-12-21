/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strsplit.c                                    .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: fcordon <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/10/22 18:25:43 by fcordon      #+#   ##    ##    #+#       */
/*   Updated: 2018/11/14 20:48:00 by fcordon     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>

static const char	*delete_left_spaces(const char *s, char c)
{
	while (*s && *s == c)
		s++;
	return (s);
}

static size_t		str_no_end_spaces_length(const char *s, char c)
{
	const char	*start = s;

	s += strlen(s) - 1;
	while (*s == c)
		s--;
	return ((size_t)(s - start));
}

char				*ft_strctrim(const char *s, char c)
{
	size_t	length;

	if (!s)
		return (NULL);
	s = delete_left_spaces(s, c);
	if (!*s)
		return ((char *)calloc(1, 1));
	length = str_no_end_spaces_length(s, c);
	return (strndup(s, length + 1));
}
static int			count_words(const char *s, char c)
{
	int count;

	count = 1;
	while (*s)
	{
		if (*s && *s == c)
		{
			count++;
			while (*s && *s == c)
				s++;
			continue;
		}
		s++;
	}
	return (count);
}

static char			*get_wordcpy(const char *s, char c)
{
	int		length;

	length = 0;
	while (s[length] && s[length] != c)
		length++;
	return (strndup(s, length));
}

static void			copy_words(char **split, const char *trim, char c)
{
	*(split++) = get_wordcpy(trim, c);
	while (*trim)
	{
		if (*trim == c)
		{
			while (*trim == c)
				trim++;
			*(split++) = get_wordcpy(trim, c);
			continue ;
		}
		trim++;
	}
	*split = NULL;
}

char				**ft_strsplit(const char *s, char c)
{
	char	**split;
	char	*trim;
	int		words_count;

	trim = ft_strctrim(s, c);
	if (!s || !trim)
		return (NULL);
	if (*trim == '\0')
	{
		if (!(split = (char**)malloc(sizeof(char*))))
			return (NULL);
		*split = NULL;
		free(trim);
		return (split);
	}
	words_count = count_words(trim, c);
	if ((split = (char**)malloc(sizeof(char*) * (words_count + 1))) == NULL)
	{
		free(trim);
		return (NULL);
	}
	copy_words(split, trim, c);
	free(trim);
	return (split);
}
