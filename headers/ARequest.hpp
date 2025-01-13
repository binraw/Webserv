/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ARequest.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fberthou <fberthou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 14:49:59 by fberthou          #+#    #+#             */
/*   Updated: 2025/01/13 15:20:26 by fberthou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AREQUEST_HPP
# define AREQUEST_HPP

	#include <string>

	class ARequest
	{
		public:
			ARequest();
			~ARequest();

			virtual void	execute() = 0;
			static void		findMethod();
		private:
			std::string	_url;
	};

#endif