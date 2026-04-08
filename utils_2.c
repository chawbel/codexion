/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbahry <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 02:38:55 by cbahry            #+#    #+#             */
/*   Updated: 2026/04/03 02:38:55 by cbahry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	ft_to_lower(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] >= 'A' && str[i] <= 'Z')
			str[i] = str[i] + 32;
		i++;
	}
}

int	print_error(char *error_msg)
{
	fprintf(stderr, "%s\n", error_msg);
	exit(1);
}

int	is_number(const char *str)
{
	if (!str || !*str)
		return (0);
	if (*str == '-' || *str == '+')
		str++;
	if (!*str)
		return (0);
	while (*str)
	{
		if (*str < 48 || *str > 57)
			return (0);
		str++;
	}
	return (1);
}

char	*ft_strdup(const char *s)
{
	size_t	i;
	char	*dup;
	size_t	size;

	size = strlen(s);
	dup = malloc((size + 1) * sizeof(char));
	if (dup == NULL)
		return (NULL);
	i = 0;
	while (s[i] != '\0')
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

void	swap(t_heap_node *node_1, t_heap_node *node_2)
{
	t_heap_node	temp;

	temp = *node_1;
	*node_1 = *node_2;
	*node_2 = temp;
}
