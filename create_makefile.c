/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   create_makefile.c                                .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: fcordon <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/11/15 11:19:39 by fcordon      #+#   ##    ##    #+#       */
/*   Updated: 2018/12/21 21:07:40 by fcordon     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include "sexmaker.h"

#define MAKEFILE_MAXSIZ	5120


void	write_in_file(const char *filename, const char *content)
{
	FILE *f = fopen(filename, "w+");

	if (!f) {
		fprintf(stderr, "Impossible d'ouvrir le fichier %s !\n", filename);
		exit(EXIT_FAILURE);
	}
	printf(
			"\e[0;37m%zu caracteres ecrits dans "
			"le fichier \"Makefile\" !\e[0m\n",
			fwrite(content, sizeof(char), strlen(content), f)
	);
	fclose(f);
}

int	not_omit(char **omit, char *name)
{
	if (omit)
	{
		while (*omit)
		{
			if (strcmp(*omit, name) == 0)
				return (0);
			omit++;
		}
	}
	return (1);
}

void		get_makefile(t_arg *o, char *makefile)
{
	DIR			*rep;
	dirent_t	*filelist;
	char		*mkfptr;
	int			true_filecount = 0;
	int			filecount = 0;
	int			length;

	mkfptr = makefile
			+ sprintf(makefile, "NAME = %s\nCC = %s %s %s\nFILES = ",
						o->exe, o->compilo, o->flags, o->h_path);

	if (!(rep = opendir("."))) {
		fprintf(stderr, "> ERREUR: impossible d'ouvrir le dossier courant\n");
		exit(EXIT_FAILURE);
	}


	while ((filelist = readdir(rep)) != NULL)
	{
		filecount++;
		length = strlen(filelist->d_name);

		if (filelist->d_name[length - 2] == '.'
				&& filelist->d_name[length - 1] == 'c'
				&& not_omit(o->omit, filelist->d_name))
		{
			if (!o->silent) {
				true_filecount++;
				printf("\e[0;37mfile %d = \e[0;32m'%s'\e[0m\n", 
						true_filecount, filelist->d_name);
			}
			filelist->d_name[length - 1] = 'o';
			mkfptr += sprintf(mkfptr, "%s\\\n", filelist->d_name);
		}
	}

	mkfptr[-2] = '\n';
	if (!o->silent)
		printf("\e[0;37m%d/%d \e[0;37mfichiers valides.\e[0m\n",
				true_filecount, filecount);
	closedir(rep);


	mkfptr += sprintf(mkfptr, "all: $(NAME)\n\n$(NAME): $(FILES)\n\t");
	if (!o->islib)
		mkfptr += sprintf(mkfptr, 
						"@$(CC) -o $@ $^ %s\n\n.c.o:\n\t@$(CC) -c $<\n\n",
						o->lib);
	else
		mkfptr += sprintf(mkfptr, 
						"@ar rc $@ $^ %s\n\t@ranlib $@\n\n"
						".c.o:\n\t@$(CC) -c $<\n\n", o->lib);
	sprintf(mkfptr, "clean:\n\t@/bin/rm -f $(FILES)\n\nfclean: clean"
					"\n\t@/bin/rm -f $(NAME)\n\nre: fclean all"
					"\n\nallc: all clean\n");
}

void	init_arg(t_arg *arg)
{
	arg->exe = NULL;
	arg->h_path = NULL;
	arg->flags = NULL;
	arg->lib = NULL;
	arg->compilo = NULL;
	arg->omit = NULL;
	arg->islib = 0;
	arg->silent = 0;
}

int		man(char *s)
{
	printf
		(
		 "MAN                  %s                   MAN\n"
		 "\n"
		 "-s        : (Bool) Mode silencieux\n"
		 "-h        : Repertoire des .h (facultif)\n"
		 "-c        : Repertoire des .c (facultif)\n"
		 "--flag    : Liste de flags (defaut: -Wall -Wextra -Werror)\n"
		 "--exe     : Nom de l'executable/lib (defaut: sexmaker.exe)\n"
		 "--lib     : Nom des lib a inclure (fichier .a)\n"
		 "--makelib : (Bool) Pour creer une lib (defaut: 0)\n"
		 "--omit    : Nom des fichiers a omettre\n"
		 "--compilo : Nom du compilateur (defaut: gcc)\n\n",
		 s
		);
	return 0;
}

int		main(int argc, char **argv)
{
	t_arg		arg;
	t_options	*o;
	char		*val;
	char		makefile[MAKEFILE_MAXSIZ] = {0};

	if (argc > 10 || argc < 2)
		return (man(*argv));

	/*
	** Recuperation des options de la ligne de commande
	*/
	argv++;
	argc--;
	if ((o = ft_get_options(argc, argv)) == NULL)
		return (-1);

	/*
	** Reglages des options (valeur par defaut si non precise)
	*/
	init_arg(&arg);
	if ((val = ft_get_option_value("h", o)) == NULL)
		arg.h_path = strdup("");
	else
	{
		arg.h_path = malloc(sizeof(val) + 4);
		sprintf(arg.h_path, "-I %s", val);
	}
	if ((arg.flags = ft_get_option_value("flag", o)) == NULL)
		arg.flags = strdup("-Wall -Werror -Wextra");
	if ((arg.exe = ft_get_option_value("exe", o)) == NULL)
		arg.exe = strdup("sexmaker.exe");
	if ((arg.lib = ft_get_option_value("lib", o)) == NULL)
		arg.lib = strdup("");
	if (ft_is_set_option("makelib", o) == 1)
		arg.islib = 1;
	if ((val = ft_get_option_value("omit", o)) != NULL)
		arg.omit = ft_strsplit(val, ',');
	if ((arg.compilo = ft_get_option_value("compilo", o)) == NULL)
		arg.compilo = strdup("gcc");
	if (ft_is_set_option("s", o) == 1)
		arg.silent = 1;


	get_makefile(&arg, makefile);
	write_in_file("Makefile", makefile);
	return (0);
}
