/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ARequest.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fberthou <fberthou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 14:49:59 by fberthou          #+#    #+#             */
/*   Updated: 2025/01/14 15:55:30 by fberthou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AREQUEST_HPP
# define AREQUEST_HPP

	#include <string>
	enum e_methods {
		GET,
		POST,
		DELETE
	};

	class ARequest
	{
		public:
			ARequest();
			~ARequest();

			virtual void		execute() = 0;
			static e_methods	findMethod();

		protected:
			std::string	_url;

	};

#endif
