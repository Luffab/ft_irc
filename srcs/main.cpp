/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpuccion <dpuccion@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/09 01:54:14 by fatilly           #+#    #+#             */
/*   Updated: 2022/08/23 21:21:26 by dpuccion         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int	main(int ac, char **av)
{
	uint16_t	port;
	std::string	password;

	if (ac != 3)
	{
		std::cerr << "Wrong arguments count" << std::endl;
		return (1);
	}
	port = (uint16_t)atoi(av[1]);
	password = av[2];
	Server server(port, password);
	if (!server.OpenServer())
		return (1);
	return (0);
}