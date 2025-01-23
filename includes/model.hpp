#ifndef MODEL_HPP
# define MODEL_HPP

#include <iostream>

class Model
{
	public:
		Model();
		Model(const Model &);
		~Model();
		Model				& operator=(const Model &);
	private:
};

std::ostream & operator<<(const std::ostream &, const Model &);

#endif
